//
// Created by artem on 04.10.23.
//

#include <algorithm>
#include <cstring>
#include "CMySQLModel.h"
#include "octo-encryption-cpp/base64.hpp"

CMySQLModel::CMySQLModel(const Tstring  &name, CAbstractConnection * connection) : CDataBaseModel(name)
{
    this->connection = dynamic_cast<CMySqlConnection*>(connection);
    m_is_encrypt = connection->get_connecting_params()->is_data_encrypt_enabled;
    m_db_type = dt_mysql;
}

CMySQLModel::~CMySQLModel()
= default;

CDBValue *CMySQLModel::getValueInRow(const Tstring &Name)
{
    if (!answer)
        return nullptr;

    std::vector<CDBValue*> * value = answer->at(0);
    return find_by_name(Name, value);
}

Tint32 CMySQLModel::getIntValueInRow(const Tstring &Name, const Tint32 defaultValue)
{
    CDBValue * value = getValueInRow(Name);

    if (value && value->type() == EVT_INT)
        return value->integer();
    else
        return defaultValue;

}

Tstring CMySQLModel::getStringValueInRow(const Tstring &Name, const Tstring& defaultValue)
{
    CDBValue * value = getValueInRow(Name);

    if (value && value->type() == EVT_STRING)
        return value->string();
    else
        return defaultValue;
}

CDBValue * CMySQLModel::getValueById(const Tuint16 &id, const Tstring &Name)
{

    for (auto &row : *answer)
    {
        CDBValue *item = find_by_name("id", row);

        if (!item)
        {
            continue;
        }

        if (item->type() == EVT_INT && item->integer() == id)
        {
            return item;
        }
    }

    return nullptr;
}

Tint32 CMySQLModel::getIntValueById(const Tuint16 &id, const Tstring &name, const Tint32 defaultValue)
{
    CDBValue * value = getValueById(id, name);

    if (value && value->type() == EVT_INT)
        return value->integer();
    else
        return defaultValue;

}

Tstring CMySQLModel::getStringValueById(const Tuint16 &id, const Tstring &Name, const Tstring& defaultValue)
{
    CDBValue * value = getValueById(id, Name);

    if (value && value->type() == EVT_STRING)
        return value->string();
    else
        return defaultValue;
}


Tuint64 CMySQLModel::read(const bool &isAll, const Tstring & stmt)
{
    Tstring Tsql = stmt;

    if (stmt.empty())
    {
        if (this->m_request->values->empty() && !isAll)
        {
            this->lastError = EDBErrors::EDBE_READ_QUERY_ERROR;
            return 0;
        }


        if (!this->connection->is_opened())
        {
            connection->open();
            if (!connection->is_opened())
            {
                this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
                return 0;
            }
        }


        // сделать запрос
        Tsql = this->getSelectReq(isAll);
    }

    const char * sql = Tsql.c_str();

    int query_res = mysql_real_query(this->connection->get_connector(), sql, strlen(sql));
    if (query_res > 0) {
        this->lastError = query_res;
        return 0;
    }


    MYSQL_RES * results = mysql_store_result(this->connection->get_connector()); // читаем результат
    Tsize rows_count = mysql_num_rows(results);  //количество строк
    int32_t iVal;
    Tstring sVal;
    Tstring Name;

    if (!this->answer)
        return 0;

    //this->answer->clear(); // очистим ответ
    clear_answer();

    for(Tsize i = 0; i < rows_count; ++i){     // побежали по строкам
        MYSQL_ROW row = mysql_fetch_row(results); // читаем конкретную строку
        auto * list = new std::vector<CDBValue*>(); // создадим список данных для одной строки

        for (Tsize j = 0; j < results->field_count; ++j){ // побежали по значениям в строке
            MYSQL_FIELD * field =  mysql_fetch_field_direct(results, j); // возьмём данные столбца

            Name = field->name;                     // и его название

            switch (field->type){                           //.. и его тип, чтобы преобразовать (ниже) - в зависимости от типа вызываем нужную функцию преобразования
                case enum_field_types::MYSQL_TYPE_BIT :
                    list->push_back(new CDBValue(Name, CFunctions::ToBool(row[j])));
                    break;
                case enum_field_types::MYSQL_TYPE_DOUBLE :
                case enum_field_types::MYSQL_TYPE_FLOAT :
                    list->push_back(new CDBValue(Name, CFunctions::ToDouble(row[j])));

                    break;
                case enum_field_types::MYSQL_TYPE_TINY :
                case enum_field_types::MYSQL_TYPE_SHORT :
                case enum_field_types::MYSQL_TYPE_LONG :
                case enum_field_types::MYSQL_TYPE_LONGLONG :
                    iVal = row[j] ? CFunctions::ToInt32(row[j]) : -1;
                    list->push_back(new CDBValue(Name, iVal));
                    break;

                case enum_field_types::MYSQL_TYPE_VAR_STRING:
                    sVal = row[j] ? row[j] : "-";

                    if (m_is_encrypt)
                    {
                        sVal = octo::encryption::Base64::base64_decode(sVal);
                    }

                    list->push_back(new CDBValue(Name, sVal));
                    break;
                case enum_field_types::MYSQL_TYPE_BLOB:
                    {
                        unsigned long *lengths = mysql_fetch_lengths(results);

                        if (row[j] && lengths && lengths[j] > 0)
                        {
                            // Проверяем, текстовые ли данные (нет нулевых байт в середине)
                            if (strlen(row[j]) == lengths[j])
                            {
                                // Чистый текст, используем обычное преобразование
                                sVal = row[j];
                            } else
                            {
                                // Бинарные данные, берём с длиной
                                sVal.assign(row[j], lengths[j]);
                            }

                            if (m_is_encrypt)
                            {
                                sVal = octo::encryption::Base64::base64_decode(sVal);
                            }
                        } else
                        {
                            sVal = "";
                        }
                    }

                    list->push_back(new CDBValue(Name, sVal));
                    break;
                default:
                    sVal = row[j] ? row[j] : "nullptr";
                    //list->push_back(new CDBValue(Name, sVal)); // sure?
                    continue;
            }
        }
        this->answer->push_back(list);  // ну и записать заполненный список в строку ответа
    }

    // очистить результат запроса
    mysql_free_result(results);

    return this->answer->size();

}

void CMySQLModel::readRow(const Tint32 &id)
{
    this->m_request->values->clear();
    this->m_request->conditions->clear();
    this->m_request->conditions->push_back(SDBCondition("id", EConditionOpers::ECO_EQ, CFunctions::ToString(id), ELogicOpers::ELO_NO));

    this->read(true);
}

[[maybe_unused]] void CMySQLModel::readFieldByName(const Tint32 &id, const Tstring &Name)
{
    this->m_request->values->clear();
    this->m_request->values->push_back(new CDBValue(Name));

    this->m_request->conditions->clear();
    this->m_request->conditions->push_back(SDBCondition("id", EConditionOpers::ECO_EQ, CFunctions::ToString(id), ELogicOpers::ELO_NO));

    this->read();
}

void CMySQLModel::escape_string(Tstring &value)
{
    char buff[1024];
    bzero(buff, 1024);

    auto size = mysql_real_escape_string(this->connection->get_connector(), buff, value.c_str(), value.length());

    if (size > 1023)
    {
        size = 1023;
    }

    buff[size] = 0;

    value = buff;
}

bool CMySQLModel::write()
{

    if (!this->connection || !this->connection->is_opened())
    {
        this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
        return false;
    }



    Tstring sql = this->getUpdateReq();

    this->lastError = mysql_real_query(this->connection->get_connector(), sql.c_str(), sql.length());

    return this->lastError == 0;
}

bool CMySQLModel::add(const bool & isSimple)
{
    this->lastError = 0;

    if (this->m_request->values->empty())
    {
        return false;
    }

    if (!this->connection || !this->connection->is_opened())
    {
        this->lastError = EDBErrors::EDBE_CONNECTION_ERROR;
        return false;
    }


    // сделать запрос
    Tstring sql = this->getInsertReq(isSimple);
    this->lastError = mysql_real_query(this->connection->get_connector(), sql.c_str(), sql.length());

    if (lastError > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(connection->get_connector()));
    }

    return this->lastError == 0;
}

bool CMySQLModel::remove()
{

    if (!this->connection || !this->connection->is_opened()){
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
    int res = mysql_real_query(this->connection->get_connector(), sql.c_str(), sql.length());
    if (res)
    {
        print_mysql_err(res);
        return false;
    }

    return true;
}

void CMySQLModel::clearConditions()
{
    if (!this->m_request || !m_request->conditions)
    {
        return;
    }

    m_request->conditions->clear();
}

void CMySQLModel::clearValues()
{
    if (!this->m_request || !m_request->values || m_request->values->empty())
    {
        return;
    }

    m_request->values->clear();

}

void CMySQLModel::addCondition(const SDBCondition &condition)
{
    if (!this->m_request || !m_request->conditions)
    {
        return;
    }
    this->m_request->conditions->push_back(new SDBCondition(condition));
}


void CMySQLModel::addValue(const CDBValue & value)
{
    if (!this->m_request || !m_request->values)
    {
        return;
    }

    auto nVal = new CDBValue(value);
    Tstring tmp;

    if (value.type() == EVT_STRING)
    {
        tmp = nVal->string();
        escape_string(tmp);
        nVal->setValue(tmp);
    }

    this->m_request->values->push_back(nVal);
}


Tsize CMySQLModel::get_last_id()
{
    Tstring sql = "select MAX(`id`) from "+this->name;
    if (this->connection && this->connection->is_opened())
    {
        int res = mysql_real_query(this->connection->get_connector(), sql.c_str(), sql.length());
        if (res)
        {
            print_mysql_err(res);
            return 0;
        }

        MYSQL_RES * results = mysql_store_result(this->connection->get_connector()); // читаем результат
        MYSQL_ROW row = mysql_fetch_row(results);
        int id = std::stoi(row[0]);
        mysql_free_result(results);

        return id;
    }
    else
        return 0;

}

void CMySQLModel::print_mysql_err(const int &code)
{
    switch (code) {
        case CR_COMMANDS_OUT_OF_SYNC:
            fprintf(stderr, "Commands were executed in an improper order.\n");
            break;
        case CR_SERVER_GONE_ERROR:
            fprintf(stderr, "The MySQL server has gone away.\n");
            break;
        case CR_SERVER_LOST:
            fprintf(stderr, "The m_connection to the server was lost during the query.\n");
            break;
        case CR_UNKNOWN_ERROR:
            fprintf(stderr, "An unknown error occurred.\n");
            break;
        default:
            fprintf(stderr, "mysql_real_query returned: %d\n", code);
            break;
    }
}

Tuint64 CMySQLModel::get_last_error() const
{
    return this->lastError;
}

std::vector<CDBValue*> *CMySQLModel::field_set()
{
    return m_request->values;
}

CDBConditionList *CMySQLModel::condition_set()
{
    return m_request->conditions;
}

Tstring CMySQLModel::get_name() const
{
    return this->name;
}

CDBValue *CMySQLModel::find_by_name(const Tstring &name, std::vector<CDBValue *> *array)
{
    auto res = std::find_if(array->begin(),
                            array->end(),
                            [=](const CDBValue *val){return val->name() == name ? val : nullptr;});

    return *res;
}

bool CMySQLModel::exec_stored_proc_no_res(const Tstring &storedProcName)
{
    Tstring query = "CALL " + storedProcName + "();";
    int query_res = mysql_real_query(this->connection->get_connector(), query.c_str(), query.length());
    if (query_res > 0) {
        this->lastError = query_res;
        return false;
    }
    return true;
}

Tuint64 CMySQLModel::execSQL_read(const Tstring &sql)
{
    return read(false, sql);
}
