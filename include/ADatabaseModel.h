//
// Created by artem on 12.10.23.
//

#ifndef REM_DB_ADATABASEMODEL_H
#define REM_DB_ADATABASEMODEL_H

#include "../models/CDataBaseModel.h"
#include "../my-maria/CMySQLModel.h"
#include "../postgre/CPostgresModel.h"
#include "CDBValue.h"


class ADatabaseModel
{
public:
    ADatabaseModel() = delete;
    ADatabaseModel(const ADatabaseModel &) = delete;
    explicit ADatabaseModel(const Tstring  &name, CAbstractConnection *connection);
    virtual ~ADatabaseModel() ;

    Tuint64 read(const bool &is_all = false) ;
    bool    write() ;
    bool    add(const bool & is_simple = true) ;
    bool    remove() ;
    std::vector<std::vector<CDBValue*>*>* get_answer() ;
    void    clear_answer() ;
    void    clear_all() ;
    void    escape_string(Tstring &str) ;
    unsigned long get_last_id();
    bool    exec_stored_proc_no_res(const Tstring & procName);
    Tuint64 execSQL_read(const Tstring & sql);

protected:
    CMySQLModel     * m_mysql_model{nullptr};
    CPostgresModel  * m_postgres_model{nullptr};
    CAbstractConnection * m_connection{nullptr};

    std::vector<std::vector<CDBValue*>*>  *answer;
    SDBRequest   *request;
    std::vector<Tstring>    * fields;
};


#endif //REM_DB_ADATABASEMODEL_H
