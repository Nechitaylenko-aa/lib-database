#ifndef CMYSQLCONNECTION_H
#define CMYSQLCONNECTION_H

#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include "../include/CAbstractConnection.h"



class CMySqlConnection : public CAbstractConnection
{
public:
    friend CAbstractConnection*  CAbstractConnection::createMySQLInstance(SDBConnection *connection_params);

    explicit CMySqlConnection(SDBConnection *connectionParams);
    ~CMySqlConnection() override;

    void Delete() override;

    bool open()     override;
    void close()    override;
    SDBConnection *     get_connecting_params() override;



    [[nodiscard]] MYSQL *get_connector() const;

private:
    MYSQL *connector{nullptr};
    SDBConnection * m_connecting_param;

    void do_staff();

    bool test_accessible();
};

#endif // CMYSQLCONNECTION_H
