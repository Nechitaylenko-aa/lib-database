//
// Created by artem on 04.10.23.
//

#ifndef REM_DB_CPOSTGRESMODEL_H
#define REM_DB_CPOSTGRESMODEL_H

#include "../models/CDataBaseModel.h"
#include "CPostgreConnection.h"
#include "../include/CAbstractConnection.h"


class CPostgresModel : public CDataBaseModel
{
public:
    CPostgresModel() = delete;
    CPostgresModel(const CPostgresModel &) = delete;
    explicit CPostgresModel(const Tstring  &name, CAbstractConnection *connection);
    ~CPostgresModel() override;

    Tuint64 read(const bool &isAll = false, const Tstring &sql = "") override;
    bool    write() override;
    bool    add(const bool & isSimple = true) override;
    bool    remove() override;
    Tsize get_last_id()  override;
    void    escape_string(Tstring &value) override;
    bool    exec_stored_proc_no_res(const Tstring &procName);
    Tuint64 execSQL_read(const Tstring & sql) override;

protected:



    CPostgreConnection  * m_connection;
    PGresult            * m_query_result{nullptr};

    // если прочитали одну строку
    CDBValue *  get_value_in_row(const Tstring & name);
    Tint32      get_int_value_in_row(const Tstring & name, const Tint32 &defaultValue = 0);
    Tstring     get_string_value_in_row(const Tstring & name, const Tstring& defaultValue = "-");

    // если прочитали не одну строку
    CDBValue *  get_value_by_id(const Tuint16 & id, const Tstring & name);
    Tint32      get_int_value_by_id(const Tuint16 & id, const Tstring & name, const Tint32 &defaultValue = 0);
    Tstring     get_string_value_by_id(const Tuint16 & id, const Tstring & name, const Tstring& defaultValue = "-");

private:
    void init_postgres_oids();  //!< считывает oid для int/float/string... этой подлости от сервера БД я не ожидал
    PGresult* local_query(const Tstring &sql);

    void    analyze_row(std::vector<EValueTypes> &types);
    bool    is_right(const Tsize &oid, std::vector<int> *array);

    CDBValue* find_by_name(const Tstring &name, std::vector<CDBValue*> *array);

};


#endif //REM_DB_CPOSTGRESMODEL_H
