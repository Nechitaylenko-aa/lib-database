//
// Created by artem on 04.10.23.
//

#ifndef REM_DB_CMYSQLMODEL_H
#define REM_DB_CMYSQLMODEL_H

#include "../models/CDataBaseModel.h"
#include "CMySqlConnection.h"

class CMySQLModel: public CDataBaseModel
{

public:
    explicit CMySQLModel(const Tstring  &name, CAbstractConnection * connection);

    ~CMySQLModel() override;


    Tuint64 read(const bool &isAll = false, const Tstring &sql = "") override;                    //!< читаем данные в соответствии с m_request и возвращаем количество найденных данных (values - поля, conditions - условия выборки)
    void    readRow(const Tint32 & id);                                 //!< читаем одну строчку в таблице по id
    [[maybe_unused]] void    readFieldByName(const Tint32 & id, const Tstring & name);   //!< читаем одно значение из строки

    bool    write() override;                                           //!< записываем данные в соответствии с m_request (values - поля и значения, conditions - условия)
    bool    add(const bool & isSimple = true) override;                 //!< вставляем данные в соответствии с m_request (values - значения)
    bool    remove() override;                                          //!< удаляем данные в соответствии с m_request (conditions - условия)
    void    clearConditions();                                          //!< очищает все условия
    void    clearValues();                                              //!< очищает все поля
    void    addCondition(const SDBCondition &condition);                //!< добавляет условие в запрос
    void    addValue(const CDBValue &value);                            //!< поле и значение в запрос
    bool    exec_stored_proc_no_res(const Tstring &storedProcName);
    Tuint64 execSQL_read(const Tstring &sql) override;

    Tuint64 get_last_error() const;
    std::vector<CDBValue*>*       field_set() ;
    CDBConditionList*   condition_set();
    Tstring             get_name() const;
    Tsize get_last_id() override;

    void    escape_string(Tstring &value) override;

protected:
    // имя таблицы (заполняется в конструкторе)




private:



    void print_mysql_err(const int &code);

    CMySqlConnection * connection;

    // если прочитали одну строку
    CDBValue *  getValueInRow(const Tstring & name);
    Tint32      getIntValueInRow(const Tstring & name, const Tint32 defaultValue = 0);
    Tstring     getStringValueInRow(const Tstring & name, const Tstring& defaultValue = 0);

    // если прочитали не одну строку
    CDBValue *  getValueById(const Tuint16 & id, const Tstring & name);
    Tint32      getIntValueById(const Tuint16 & id, const Tstring & name, const Tint32 defaultValue = 0);
    Tstring     getStringValueById(const Tuint16 & id, const Tstring & name, const Tstring& defaultValue = "");

    CDBValue *  find_by_name(const Tstring &name, std::vector<CDBValue*> *array);
};



#endif //REM_DB_CMYSQLMODEL_H
