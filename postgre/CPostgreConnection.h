//
// Created by artem on 02.10.23.
//

#ifndef REM_DB_CPOSTGRECONNECTION_H
#define REM_DB_CPOSTGRECONNECTION_H

#ifdef Windows
#include <libpq-fe.h>
#endif
#ifdef Linux
//#include <postgresql/libpq-fe.h>
#include <libpq-fe.h>
#endif
#ifdef FreeBSD
#include <libpq-fe.h>
#endif
#include <vector>

#include "../include/CAbstractConnection.h"

struct s_oids_data
{
    std::vector<int>    * int_oids{nullptr};
    std::vector<int>    * float_oids{nullptr};
    std::vector<int>    * string_oids{nullptr};
};

class CPostgreConnection : public CAbstractConnection
{
public:
    friend CAbstractConnection* CAbstractConnection::createPostgreInstance(SDBConnection *parameters);

    explicit CPostgreConnection(SDBConnection *connect_params);
    ~CPostgreConnection() override;

    void Delete() override;

    bool open() override;
    void close() override;
    SDBConnection *     get_connecting_params() override;

    PGconn * get_connector();

    s_oids_data * get_oids_data();

protected:

    s_oids_data     * m_oids_data;

private:
    PGconn          * m_pg_connection{nullptr};
    SDBConnection   * m_params{nullptr};

    PGconn*
    upg_connect_db (const char* dbhost, const char* dbport, const char* dbname, const char* dbuser, const char* dbupwd );

    void do_stuff();
};


#endif //REM_DB_CPOSTGRECONNECTION_H
