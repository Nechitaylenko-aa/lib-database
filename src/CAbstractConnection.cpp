#include "../include/CAbstractConnection.h"

CAbstractConnection::~CAbstractConnection()
{
    if (m_idle_enabled)
    {
        stop_idle_thread();
    }
}

bool CAbstractConnection::is_opened() const
{
    return this->isOpened;
}

E_DB_TYPE CAbstractConnection::database_kind() const
{
    return m_database_kind;
}

bool CAbstractConnection::is_idle_enabled() const
{
    return m_idle_enabled;
}

void CAbstractConnection::enable_idle(const bool &yes_no)
{
    m_idle_enabled = yes_no;
}

void CAbstractConnection::thread_func()
{
    m_start = time(nullptr);
    bool inner = false;

    while ((time(nullptr) - m_start) < m_idle_secs)
    {
        m_inner_lock.lock();
        inner = m_thr_flag;
        m_inner_lock.unlock();

        if (inner)
        {
            m_inner_lock.lock();
            m_thr_flag = false;
            m_inner_lock.unlock();

            return;
        }
    }

    m_inner_lock.lock();
    {
        m_thr_flag = false;
        this->close();
    }
    m_inner_lock.unlock();
}

void CAbstractConnection::stop_idle_thread()
{
    m_outer_lock.lock();
    m_thr_flag = true;
    m_outer_lock.unlock();

    if (m_idle_thread.joinable())
    {
        m_idle_thread.join();
    }
}

void CAbstractConnection::start_idle_thread()
{
    m_idle_thread = std::thread{&CAbstractConnection::thread_func, this};
    m_idle_thread.detach();
}

CAbstractConnection *CAbstractConnection::createDatabaseInstance(const E_DB_TYPE &db_type, SDBConnection *connection)
{
    if (db_type == EDT_MYSQL)
        return createMySQLInstance(connection);
    else
        return createPostgreInstance(connection);
}
