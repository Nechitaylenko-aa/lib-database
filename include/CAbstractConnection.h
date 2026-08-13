#ifndef CABSTRACTCONNECTION_H
#define CABSTRACTCONNECTION_H

#include <thread>
#include <mutex>
#include <atomic>
#include "../include/db-types.h"


class CAbstractConnection
{
public:
    static  CAbstractConnection* createDatabaseInstance(const E_DB_TYPE &db_type, SDBConnection *connection);

    virtual void Delete() = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual SDBConnection *     get_connecting_params() = 0;

    virtual ~CAbstractConnection();

    [[nodiscard]] bool is_opened() const;
    [[nodiscard]] E_DB_TYPE     database_kind() const;

    [[nodiscard]] bool    is_idle_enabled() const;
    void    enable_idle(const bool &yes_no);

protected:

    static CAbstractConnection*   createMySQLInstance(SDBConnection *connection_params);
    static CAbstractConnection*   createPostgreInstance(SDBConnection *connection_params);

    SSslConf ssl_conf;

    std::string host;
    std::string dbname;
    std::string user;
    std::string pass;
    int port;
    bool    m_idle_enabled{false};
    E_DB_TYPE     m_database_kind;
    Tsize   m_idle_secs{60 * 5}; // default connection idle time is 5 min


    bool isOpened {false};

    void   stop_idle_thread();
    void   start_idle_thread();

private:
    std::thread     m_idle_thread;
    std::mutex      m_inner_lock;
    std::mutex      m_outer_lock;
    time_t          m_start;
    std::atomic<bool>  m_thr_flag{false};


    void   thread_func();

};




#endif // CABSTRACTCONNECTION_H
