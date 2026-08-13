//
// Created by artem on 12.10.23.
//

#include "../include/ADatabaseModel.h"
#include <stdexcept>

ADatabaseModel::ADatabaseModel(const Tstring  &name, CAbstractConnection *connection)
{
    if (!connection)
    {
        fprintf(stderr, "connection could not be nullptr in 'ADatabaseModel::ADatabaseModel'");
        throw std::runtime_error("connection could not be nullptr in 'ADatabaseModel::ADatabaseModel'");
    }

    m_connection = connection;

    switch (connection->database_kind())
    {
        case EDT_MYSQL:
            m_mysql_model = new CMySQLModel(name, connection);
            answer = m_mysql_model->get_answer();
            request = m_mysql_model->request();
            fields = m_mysql_model->fields();
            break;
        case EDT_POSTGRES:
            m_postgres_model = new CPostgresModel(name, connection);
            answer = m_postgres_model->get_answer();
            request = m_postgres_model->request();
            fields = m_mysql_model->fields();
            break;
        default:
            fprintf(stderr, "can't create model unknown type\n");
            throw std::runtime_error("unknown database type in 'ADatabaseModel::ADatabaseModel'");
    }
}

ADatabaseModel::~ADatabaseModel()
{
    delete m_mysql_model;
    delete m_postgres_model;
}

Tuint64 ADatabaseModel::read(const bool &is_all)
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->read(is_all);
        case EDT_POSTGRES:
            return m_postgres_model->read(is_all);
        default:
            return 0;
    }
}

bool ADatabaseModel::write()
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->write();
        case EDT_POSTGRES:
            return m_postgres_model->write();
        default:
            return false;
    }
}

bool ADatabaseModel::add(const bool &is_simple)
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->add(is_simple);
        case EDT_POSTGRES:
            return m_postgres_model->add(is_simple);
        default:
            return false;
    }
}

bool ADatabaseModel::remove()
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->remove();
        case EDT_POSTGRES:
            return m_postgres_model->remove();
        default:
            return false;
    }
}

std::vector<std::vector<CDBValue *> *> *ADatabaseModel::get_answer()
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->get_answer();
        case EDT_POSTGRES:
            return m_postgres_model->get_answer();
        default:
            return nullptr;
    }
}

void ADatabaseModel::clear_answer()
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->clear_answer();
        case EDT_POSTGRES:
            return m_postgres_model->clear_answer();
        default:
            break;
    }
}

void ADatabaseModel::clear_all()
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->clear_all();
        case EDT_POSTGRES:
            return m_postgres_model->clear_all();
        default:
            break;
    }
}

void ADatabaseModel::escape_string(Tstring &str)
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->escape_string(str);
        case EDT_POSTGRES:
            return m_postgres_model->escape_string(str);
        default:
            break;
    }
}

unsigned long ADatabaseModel::get_last_id()
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->get_last_id();
        case EDT_POSTGRES:
            return m_postgres_model->get_last_id();
        default:
            return 0;
    }
}

bool ADatabaseModel::exec_stored_proc_no_res(const Tstring &procName)
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->exec_stored_proc_no_res(procName);
        case EDT_POSTGRES:
            return m_postgres_model->exec_stored_proc_no_res(procName);
        default:
            return false;
    }
}

Tuint64 ADatabaseModel::execSQL_read(const Tstring &sql)
{
    switch (m_connection->database_kind())
    {
        case EDT_MYSQL:
            return m_mysql_model->execSQL_read(sql);
        case EDT_POSTGRES:
            return m_postgres_model->execSQL_read(sql);
        default:
            return 0;
    }
}
