#ifndef CDATABASEMODEL_H
#define CDATABASEMODEL_H


#include "../include/CDBValue.h"
#include "../include/CDBConditionList.h"
#include "../include/CStringsVector.h"

enum EDBErrors{
    EDBE_NO_ERRORS, // нет ошибок
    EDBE_CONNECTION_ERROR, // ошибка подключения (функция open)
    EDBE_GET_RESULT_ERROR, // ошибка получения результата (result = NULL)
    EDBE_RESULT_COUNT_ERROR, // ошибка в количестве полученных данных (нет данных)
    EDBE_READ_QUERY_ERROR, // ошибка в составлении запроса на чтение
    EDBE_WRITE_QUERY_ERROR, // ошибка в составлении запроса на запись
    EDBE_ADD_QUERY_ERROR, // ошибка в составлении запроса на добавление
    EDBE_REMOVE_QUERY_ERROR, // ошибка в составлении запроса на удаление
    EDBE_COUNT
};


static const char* EDBError_text[EDBErrors::EDBE_COUNT] {
    "Нет ошибок",
    "Ошибка подключения (функция open)",
    "Ошибка получения результата (result = NULL)",
    "Ошибка в количестве полученных данных (нет данных)",
    "Ошибка в составлении запроса на чтение",
    "Ошибка в составлении запроса на запись",
    "Ошибка в составлении запроса на добавление",
    "Ошибка в составлении запроса на удаление"
};

enum db_type
{
    dt_mysql,
    dt_postgres,
    dt_undef
};

struct SDBRequest
{
    std::vector<CDBValue*>  *    values;
    CDBConditionList  *  conditions;
    CStringsVector  *    other;

    explicit SDBRequest() :
            values(new std::vector<CDBValue*>()), conditions(new CDBConditionList()), other(new CStringsVector()){};

    explicit SDBRequest(const std::vector<CDBValue*> & v, const CDBConditionList & c, const CStringsVector& o);

    ~SDBRequest();
};


/** @brief Все остальные модели будут наследоваться от него и переопределять конструктор по-умолчанию (формируя в нём нужный список полей)
    Если все модели грамотно написать, то для смены СУБД нужно будет переписать только функции этого класса, а именно:
        - функции подключения и отключения БД
        - функции запросов данных
        - функции вставки и обновления данных
        - дополнительные функции, если понадобятся */


class CDataBaseModel
{

public:

    virtual ~CDataBaseModel();

    virtual Tuint64 read(const bool &isAll = false, const Tstring & sql = "") = 0;    // читаем данные в соответствии с m_request и возвращаем количество найденных данных (values - поля, conditions - условия выборки)
    virtual bool    write() = 0;  // записываем данные в соответствии с m_request (values - поля и значения, conditions - условия)
    virtual bool    add(const bool & isSimple = true) = 0;   // вставляем данные в соответствии с m_request (values - значения)
    virtual bool    remove() = 0;   // удаляем данные в соответствии с m_request (conditions - условия)
    virtual void    escape_string(Tstring &str) = 0;
    virtual Tsize get_last_id() = 0;
    virtual Tuint64 execSQL_read(const Tstring &sql) = 0;

    std::vector<std::vector<CDBValue*>*>* get_answer();
    void    clear_answer();
    SDBRequest  * request();
    void    clear_all();
    std::vector<Tstring>    * fields();

protected:
    explicit CDataBaseModel(const Tstring  &name, const bool &is_encrypt = false);

    // virtual void set_name(const Tstring &Name) = 0;

    SDBRequest   * m_request; // поля и значения для запроса, а так же условия и дополнительные параметры
    Tuint8 lastError{};
    Tstring  name;
    std::vector<Tstring>  *m_fields;                  // имена полей, которые есть в таблице (заполняется в конструкторе нужной таблицы)
    std::vector<std::vector<CDBValue*>*>  *answer;  // ответ пишется сюда

    Tstring getConditionsString();
    Tstring getValuesString(const bool & isNames = false);
    Tstring getNamesString();

    Tstring getSelectReq(const bool & isAll = false);
    Tstring getUpdateReq();
    Tstring getInsertReq(const bool & isSimple = true);
    Tstring getDeleteReq();

    bool    m_is_encrypt{false};
    db_type m_db_type{db_type::dt_undef};


private:


};




#endif // CDATABASEMODEL_H
