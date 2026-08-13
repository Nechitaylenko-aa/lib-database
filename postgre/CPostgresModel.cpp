//
// Created by artem on 04.10.23.
//

#include "CPostgresModel.h"
#include <algorithm>
#include <stdexcept>
#include <strings.h>
#include "octo-encryption-cpp/base64.hpp"

CPostgresModel::CPostgresModel(const Tstring  &name, CAbstractConnection *conn) : CDataBaseModel(name)
{
    if (!conn)
    {
        throw std::runtime_error("can't use conn as nullptr in 'CPostgresModel::CPostgresModel'");
    }

    m_connection = dynamic_cast<CPostgreConnection*>(conn);

    auto oids = m_connection->get_oids_data();

    if (oids->int_oids == nullptr)
    {
        init_postgres_oids();
    }

    m_is_encrypt = m_connection->get_connecting_params()->is_data_encrypt_enabled;
    m_db_type = dt_postgres;
}

CPostgresModel::~CPostgresModel()
= default;

CDBValue *CPostgresModel::get_value_in_row(const Tstring &name)
{
    if (answer->empty())
    {
        return nullptr;
    }

    std::vector<CDBValue*> * values = answer->at(0);
    return find_by_name(name, values);
}

Tint32 CPostgresModel::get_int_value_in_row(const Tstring &name, const Tint32 &defaultValue)
{
    CDBValue * value = get_value_in_row(name);

    return (value && value->type() == EVT_INT) ? value->integer() : defaultValue;
}

Tstring CPostgresModel::get_string_value_in_row(const Tstring &name, const Tstring &defaultValue)
{
    CDBValue * value = get_value_in_row(name);

    return (value && value->type() == EVT_STRING) ? value->string() : defaultValue;
}

CDBValue *CPostgresModel::get_value_by_id(const Tuint16 &id, const Tstring &name)
{
    if (!answer || answer->empty())
    {
        return nullptr;
    }

    std::vector<CDBValue*> * value;
    Tint32 fid;

    //for (CDBValueNode2D * node = answer->getFirst(); node != nullptr; node = node->getNext())

    for (auto &row : *answer)
    {
        auto val = find_by_name("id", row);
        if (!val)
        {
            continue;
        }

        if (val->type() == EVT_INT && val->integer() == id)
        {
            return val;
        }
    }

    return nullptr;
}

Tint32 CPostgresModel::get_int_value_by_id(const Tuint16 &id, const Tstring &name, const Tint32 &defaultValue)
{
    CDBValue * value = get_value_by_id(id, name);

    return (value && value->type() == EVT_INT) ? value->integer() : defaultValue;
}

Tstring CPostgresModel::get_string_value_by_id(const Tuint16 &id, const Tstring &name, const Tstring &defaultValue)
{
    CDBValue * value = get_value_by_id(id, name);

    return (value && value->type() == EVT_STRING) ? value->string() : defaultValue;
}

Tuint64 CPostgresModel::read(const bool &isAll, const Tstring & stmt)
{
    Tstring Tsql = stmt;

    if (stmt.empty())
    {
        if (!this->m_request || !this->m_request->values)
        {
            this->lastError = EDBErrors::EDBE_READ_QUERY_ERROR;
            return 0;
        }


        if (this->m_request->values->empty() && !isAll)
        {
            this->lastError = EDBErrors::EDBE_READ_QUERY_ERROR;
            return 0;
        }


        if (!this->m_connection->is_opened())
        {
            if (!m_connection->open())
            {
                lastError = EDBErrors::EDBE_CONNECTION_ERROR;
                return 0;
            }
        }

        // сделать запрос
        Tsql = this->getSelectReq(isAll);
    }

    Tuint64 rows = 0, cols = 0;

    std::vector<EValueTypes> fields_types;

    PQclear(m_query_result);

    m_query_result = PQexec(m_connection->get_connector(), Tsql.c_str());

    if (PQresultStatus(m_query_result) == PGRES_TUPLES_OK)
    {
        answer->clear();

        rows = PQntuples(m_query_result);
        cols = PQnfields(m_query_result);

        analyze_row(fields_types);

        Tstring dec_value;

        for (int r = 0; r < rows; r++)
        {
            auto * values_row = new std::vector<CDBValue*>();

            for (int c = 0; c < cols; c++)
            {
                dec_value = PQgetvalue(m_query_result, r, c);
                auto f_name = PQfname(m_query_result, c);

                CDBValue value;

                switch (fields_types.at(c))
                {
                    case EVT_INT:
                        value = CDBValue(f_name, CFunctions::ToInt32(dec_value));
                        break;
                    case EVT_FLOAT:
                        value = CDBValue(f_name, CFunctions::ToDouble(dec_value));
                        break;
                    case EVT_STRING:
                        if (m_is_encrypt)
                        {
                            dec_value = octo::encryption::Base64::base64_decode(dec_value);
                        }
                        value = CDBValue(f_name, dec_value);
                        break;
                    default:
                        value = {};
                        break;
                }
                values_row->push_back(new CDBValue(value));
            }

            answer->push_back(values_row);
        }
    }


    return rows;
}


bool CPostgresModel::add(const bool &isSimple)
{
    this->lastError = 0;

    if (this->m_request->values->empty())
    {
        return false;
    }

    if (!this->m_connection || !this->m_connection->is_opened())
    {
        this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
        return false;
    }


    // сделать запрос

    Tstring sql = this->getInsertReq(isSimple);
    PGresult * res = local_query(sql);

    PQclear(res);

    if (res == nullptr)
    {
        return false;
    }

    return true;
}

bool CPostgresModel::write()
{
    if (!this->m_connection || !this->m_connection->is_opened())
    {
        this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
        return false;
    }

    Tstring sql = this->getUpdateReq();

    PGresult * res = local_query(sql);

    if (!res)
        return false;

    PQclear(res);

    return true;
}

bool CPostgresModel::remove()
{
    if (!this->m_connection || !this->m_connection->is_opened()){
        this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
        return false;
    }

    if (!this->m_request || !m_request->conditions){
        this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
        return false;
    }

    if (this->m_request->conditions->isEmpty())
    {
        return false;
    }

    Tstring sql = this->getDeleteReq();
    PGresult * res = local_query(sql);

    if (!res)
        return false;

    PQclear(res);

    return true;
}

void CPostgresModel::init_postgres_oids()
{

    PGconn * pg_conn = m_connection->get_connector();
    s_oids_data * oids = m_connection->get_oids_data();

    oids->int_oids = new  std::vector<int>();
    oids->float_oids = new  std::vector<int>();
    oids->string_oids = new  std::vector<int>();


    PGresult * res = local_query("select typname, oid from pg_type where typname like '%float%';");

    if (res == nullptr)
    {
        PQclear(res);
        return;
    }

    for (int r = 0; r < PQntuples(res); r++)
    {
        auto *field = PQgetvalue(res, r, 1);
        oids->float_oids->push_back(std::stoi(field));
    }

    PQclear(res);
    while ( PQflush( pg_conn ) ) ;

    res = local_query("select typname, oid from pg_type where typname like '%int_' AND typname not like '%ints';");
    if (res == nullptr)
    {
        PQclear(res);
        return;
    }

    for (int r = 0; r < PQntuples(res); r++)
    {
        auto *field = PQgetvalue(res, r, 1);
        oids->int_oids->push_back(std::stoi(field));
    }

    PQclear(res);
    while ( PQflush( pg_conn ) ) ;

    res = local_query("select typname, oid from pg_type where typname like '%bool';");
    if (res == nullptr)
    {
        PQclear(res);
        return;
    }

    PQclear(res);
    while ( PQflush( pg_conn ) ) ;

    res = local_query("select typname, oid from pg_type where typname like '%char';");
    if (res == nullptr)
    {
        PQclear(res);
        return;
    }

    for (int r = 0; r < PQntuples(res); r++)
    {
        auto *field = PQgetvalue(res, r, 1);
        oids->string_oids->push_back(std::stoi(field));
    }

    PQclear(res);
    while ( PQflush( pg_conn ) ) ;
}

Tsize CPostgresModel::get_last_id()
{
    // Tstring sec_name = dimention_name + "_id_seq";

    Tstring sql = "SELECT currval(pg_get_serial_sequence('"+name+"','id'));";

    PGresult * res = local_query(sql);

    if (!res)
    {
        return 0;
    }

    auto f_value = PQgetvalue(res, 0, 0);

    unsigned long  val = CFunctions::ToInt32(f_value);

    PQclear(res);

    return val;
}

PGresult* CPostgresModel::local_query(const Tstring &sql)
{
    PGresult *result = nullptr;
    PQclear( result );

    auto * pg_conn = dynamic_cast<CPostgreConnection*>(m_connection);

    result = PQexec( pg_conn->get_connector(), sql.c_str() );
    ExecStatusType statusPGexec;

    if( ( statusPGexec = PQresultStatus( result ) ) != PGRES_COMMAND_OK &&
        statusPGexec != PGRES_TUPLES_OK )
    {
        char *msg = PQresultErrorMessage ( result );

        fprintf ( stderr, "PQexecParams(): %s\n", msg );
        fflush ( stderr);

        return nullptr;
    }

    //while ( PQflush ( pg_conn->get_connector() ) ) ;

    return result;
}

void CPostgresModel::analyze_row(std::vector<EValueTypes> &types)
{
    int cols = PQnfields(m_query_result);
    auto oids = m_connection->get_oids_data();

    for (int i = 0; i < cols; i++)
    {
        Tsize oid = PQftype(m_query_result, i);

        if (is_right(oid, oids->int_oids))
        {
            types.push_back(EValueTypes::EVT_INT);
            continue;
        }

        if (is_right(oid, oids->float_oids))
        {
            types.push_back(EVT_FLOAT);
            continue;
        }

        if (is_right(oid, oids->string_oids))
        {
            types.push_back(EVT_STRING);
            continue;
        }

        char buff[30];
        bzero(buff, 30);
        sprintf(buff, "unknown oid %lu", oid);
        throw std::runtime_error(buff);
    }
}

bool CPostgresModel::is_right(const Tsize &oid, std::vector<int> *array)
{
    return std::any_of(array->begin(), array->end(), [=](const int &m_oid){ return oid == m_oid;});
}

CDBValue *CPostgresModel::find_by_name(const Tstring &name, std::vector<CDBValue *> *array)
{
    auto res = std::find_if(array->begin(),
                            array->end(),
                            [=](const CDBValue *val){return val->name() == name ? val : nullptr;});

    return *res;
}

void CPostgresModel::escape_string(Tstring &value)
{
    ;// I have not found an analog of mysql_real_escape_string
}

bool CPostgresModel::exec_stored_proc_no_res(const Tstring &storedProcName)
{
    Tstring query = "CALL " + storedProcName + "();";

    PGresult *result = local_query(query);

    if (!result) {
        return false;
    }

    ExecStatusType status = PQresultStatus(result);
    if (status != PGRES_COMMAND_OK)
    {
        char *msg = PQresultErrorMessage(result);
        fprintf(stderr, "Procedure execution failed: %s\n", msg);
        PQclear(result);
        return false;
    }

    PQclear(result);
    return true;
}

Tuint64 CPostgresModel::execSQL_read(const Tstring &sql)
{
    return read(false, sql);
}
