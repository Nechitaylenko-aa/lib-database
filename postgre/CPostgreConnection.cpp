//
// Created by artem on 02.10.23.
//

#include "CPostgreConnection.h"
#include <vector>
#include <cstring>


CAbstractConnection * CAbstractConnection::createPostgreInstance(SDBConnection *parameters)
{
    return dynamic_cast<CAbstractConnection*>(new CPostgreConnection(parameters));
}


CPostgreConnection::CPostgreConnection(SDBConnection *connect_params)
{
    host   = connect_params->host;
    dbname = connect_params->dbname;
    user   = connect_params->user;
    pass   = connect_params->pass;
    port   = connect_params->port;
    m_params = connect_params;
    m_database_kind = E_DB_TYPE::EDT_POSTGRES;

    m_oids_data = new s_oids_data();


}

CPostgreConnection::~CPostgreConnection()
{

    delete m_oids_data->float_oids;
    delete m_oids_data->int_oids;
    delete m_oids_data->string_oids;

    delete m_oids_data;

    if (isOpened)
    {
        do_stuff();
    }
}

bool CPostgreConnection::open()
{
    char port_num[6];
    bzero(port_num, 6);
    sprintf(port_num, "%d", port);

    if (m_pg_connection == nullptr)
    {


        Tstring  port_n = std::to_string(port);

        if (!upg_connect_db(host.c_str(),
                            port_n.c_str(),
                            dbname.c_str(),
                            user.c_str(),
                            pass.c_str()))
        {
            return false;
        }
    }

    // https://postgrespro.ru/docs/postgresql/14/libpq-ssl

    if (m_idle_enabled)
    {
        stop_idle_thread();
        start_idle_thread();
    }

    this->isOpened = true;
    return true;
}

void CPostgreConnection::close()
{
    if (m_pg_connection) // && PQstatus(m_pg_connection) == CONNECTION_OK)
    {
        PQfinish(m_pg_connection);
    }

    this->isOpened = false;
    m_pg_connection = nullptr;
}

PGconn *CPostgreConnection::get_connector()
{
    if (m_pg_connection && PQstatus(m_pg_connection) == CONNECTION_OK)
    {
        return m_pg_connection;
    }

    if (!open())
    {
        return nullptr;
    }

    return m_pg_connection;
}

PGconn *
CPostgreConnection::upg_connect_db(const char *dbhost, const char *dbport, const char *dbname, const char *dbuser,
                                   const char *dbupwd)
{
    m_pg_connection = PQsetdbLogin( dbhost,
                                    dbport,
                                    nullptr,
                                    nullptr,
                                    dbname,
                                    dbuser,
                                    dbupwd);

    if ( PQstatus(m_pg_connection) != CONNECTION_OK )
    {
        char *msg = PQerrorMessage(m_pg_connection);
        fprintf(stderr,"PQsetdbLogin(): %s", (char*)msg );

        fflush(stderr);

        PQfinish(m_pg_connection);
        m_pg_connection = nullptr;

        return nullptr;
    }

    return m_pg_connection;
}

void CPostgreConnection::do_stuff()
{
    close();
}

void CPostgreConnection::Delete()
{
    delete this;
}

s_oids_data *CPostgreConnection::get_oids_data()
{
    return m_oids_data;
}

SDBConnection *CPostgreConnection::get_connecting_params()
{
    return m_params;
}
