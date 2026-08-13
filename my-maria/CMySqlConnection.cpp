#include "CMySqlConnection.h"
#include <stdexcept>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/file.h>

CAbstractConnection * CAbstractConnection::createMySQLInstance(SDBConnection *connection_params)
{
    return dynamic_cast<CAbstractConnection*>(new CMySqlConnection(connection_params));
}

CMySqlConnection::CMySqlConnection(SDBConnection *connection_params) : CAbstractConnection()
{
    if (!connection_params)
    {
        throw std::runtime_error("Can't accept nullptr as parameter in 'CMySqlConnection::CMySqlConnection'");
    }

    host   = connection_params->host;
    dbname = connection_params->dbname;
    user   = connection_params->user;
    pass   = connection_params->pass;
    port   = connection_params->port;
    m_database_kind = E_DB_TYPE::EDT_MYSQL;

    m_connecting_param = new SDBConnection();

    *m_connecting_param = *connection_params;

    ssl_conf = connection_params->ssl_config;
}

MYSQL *CMySqlConnection::get_connector() const
{
    return connector;
}

bool isReachable(const std::string& ip) {
    std::string cmd = "ping -c 1 -W 1 " + ip + " > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}

bool CMySqlConnection::open()
{
    /*if (!isReachable(this->host))
    {
        return false;
    }*/

    if (this->isOpened)
    {
        return true;
    }


    connector = mysql_init(nullptr);

    if (this->ssl_conf.is_enabled) // ssl enable
    {
        int timeout = 5; // seconds
        mysql_options(connector, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
        mysql_options(connector, MYSQL_OPT_SSL_KEY,    ssl_conf.key_file.c_str());
        mysql_options(connector, MYSQL_OPT_SSL_CERT,   ssl_conf.cert_file.c_str());
        mysql_options(connector, MYSQL_OPT_SSL_CA,     ssl_conf.ca_file.c_str());
        // mysql_options(mysql, MYSQL_OPT_SSL_CAPATH, capath);
        //mysql_options(mysql, MYSQL_OPT_SSL_CIPHER, cipher);
        /*mysql_ssl_set(this->connector,
                      this->ssl_conf.key_file.c_str(),
                      this->ssl_conf.cert_file.c_str(),
                      this->ssl_conf.ca_file.c_str(),
                      nullptr,
                      nullptr);*/
    }

    // connect to the defined database
    if (!mysql_real_connect(this->connector, this->host.c_str(),
                            this->user.c_str(), this->pass.c_str(),
                            this->dbname.c_str(), this->port, nullptr, 0))
    {
        fprintf(stderr, "mysql_real_connect error '%s'\n", mysql_error(connector));
        return false;
    }

    mysql_set_character_set(this->connector, "utf8");

    if (m_idle_enabled)
    {
        stop_idle_thread();
        start_idle_thread();
    }

    this->isOpened = true;

    return true;
}

void CMySqlConnection::close()
{
    if (this->isOpened && this->connector)
    {
        mysql_close(this->connector);
        this->isOpened = false;
    }
}

CMySqlConnection::~CMySqlConnection()
{
    delete m_connecting_param;
    if (isOpened)
    {
        do_staff();
    }
}

void CMySqlConnection::do_staff()
{
    close();
}

void CMySqlConnection::Delete()
{
    delete this;
}

SDBConnection *CMySqlConnection::get_connecting_params()
{
    return m_connecting_param;
}

bool CMySqlConnection::test_accessible()
{
    return false;
}
