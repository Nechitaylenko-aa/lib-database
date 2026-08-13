//
// Created by artem on 02.10.23.
//

#include "CTestModel.h" // for postgres
#include "CUserModelDB.h" // for mySQL

#include <iostream>

// полностью соответствует таблице в БД
struct s_links
{
    unsigned int id{0};
    unsigned int id_user{0};
    unsigned int id_node{0};  //!< parent node
    Tstring  description{};
    Tstring  content{};
};

/// выбор базы данных, что-то одно должно быть выбрано

#define MYSQL
//#define POSTGRES

int main()
{
    // this code will be a part of the CLoader - load config from config-file -----------------------
    /// создание конфига соединения с postgres
    /*
    auto db_pg_conf = new SDBConnection();
    db_pg_conf->user = "postgres";
    db_pg_conf->dbname = "test_db"; //"equipment";
    db_pg_conf->pass = "Postgres04";
    db_pg_conf->host = "192.168.1.66";
    db_pg_conf->is_data_encrypt_enabled = false;
    db_pg_conf->port = 5432;//3306;
    */
    /// создание конфига соединения с mysql
    auto db_ms_conf = new SDBConnection();
    db_ms_conf->user = "artem";
    db_ms_conf->dbname = "reminder";//"resources"; //"equipment";
    db_ms_conf->pass = "masterkey";
    db_ms_conf->host = "192.168.1.101";//"localhost";
    db_ms_conf->is_data_encrypt_enabled = false;
    db_ms_conf->port = 3306;

    auto connection = CAbstractConnection::createDatabaseInstance(E_DB_TYPE::EDT_MYSQL, db_ms_conf);

    //delete db_pg_conf;
    delete db_ms_conf;


    // при создании модели БД передаём в конструктор имя таблицы и экземпляр конфига соединения
    //auto model = new CTestModel("links", connection);
    auto model = new CUserModelDB(connection);

    std::vector<CUser*> users;
    model->get_users(true, &users);

    if (!users.empty())
    {
        for (auto &user : users)
        {
            std::cout << "User: " << user->get_login() << " id(): " << user->get_id() << std::endl;
            delete user;
        }
    }


    delete model;
    delete connection;


    return 0;

}
