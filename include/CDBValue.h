//
// Created by nechi on 07.03.2022.
// upd by dan on 12.03.2022
//

#ifndef CDBVALUE_H
#define CDBVALUE_H

#undef TYPES_H

#include "functions.h"

static const char *types_name[] {
    "Целочисленное",
    "Вещественное",
    "Строчное",
    "Булево",
    "Не назначено",
    "Что-то странное"
};

/**@brief Типы значений полей */
enum EValueTypes {
    EVT_SMALL,
    EVT_USMALL,
    EVT_INT,
    EVT_UINT,
    EVT_LONG_I,
    EVT_ULONG_I,
    EVT_TIME_T,
    EVT_FLOAT,
    EVT_DOUBLE,
    EVT_LONG_DOUBLE,
    EVT_STRING,
    EVT_SINGLE_CHAR,
    EVT_BYTE,
    EVT_NO,  //!< если без типа вдруг нужно
    EVT_COUNT
};

/** @brief структура для хранения значения, имени переменной и его типа.
    структура для формирования условия */
union u_value {
    long double data_ld;
    double      data_d;
    float       data_f;
    int         data_i;
    long int    data_li;
    short       data_si;
    unsigned    data_ui;
    unsigned short data_us;
    unsigned long  data_ul;
    time_t      data_tt;
    char        data_c;
    unsigned char data_uc;
    char      * data_s{nullptr};
};

struct SDBValue
{
    u_value     value;
    EValueTypes type {EVT_NO};
    Tstring name;

    SDBValue()= default;
    SDBValue(const SDBValue& other);
    SDBValue(SDBValue&& other) noexcept;
    SDBValue& operator=(const SDBValue& other);
    SDBValue& operator=(SDBValue&& other) noexcept;

    ~SDBValue();
    friend bool operator== (const SDBValue &val1, const SDBValue &val2);
    friend bool operator!= (const SDBValue &val1, const SDBValue &val2);
private:
private:
    void copyFrom(const SDBValue& other);
    void moveFrom(SDBValue&& other) noexcept;
    void clear();
};


/** @brief обёртка вокруг struct SDBValue
Описание:
    Содержит поля для хранения значения структуры и функции для обработки их
    Конструктор по-умолчанию отключен, чтобы всегда задавали имя поля и значение нужного типа (на базе этого будет заполняться поле type структуры SDBValue
    Каждый конструктор инициализации для своего типа, соответственно
    Далее геттеры и сеттеры. */

class CDBValue {
public:
    CDBValue();
    explicit CDBValue(const Tstring & name);
    CDBValue(const Tstring & name, const short &value);
    CDBValue(const Tstring & name, const unsigned short &value);
    CDBValue(const Tstring & name, const int & value);
    CDBValue(const Tstring & name, const unsigned int & value);
#ifdef Windows
    CDBValue(const Tstring & name, const long & value);
    CDBValue(const Tstring & name, const unsigned long & value);
#endif
    CDBValue(const Tstring & name, const Tsize & value);
    CDBValue(const Tstring & name, const time_t & value);
    CDBValue(const Tstring & name, const float & value);
    CDBValue(const Tstring & name, const double & value);
    CDBValue(const Tstring & name, const long double & value);
    CDBValue(const Tstring & name, const Tstring & value);
    CDBValue(const Tstring & name, const char * value);
    CDBValue(const Tstring & name, const char & value);
    CDBValue(const Tstring & name, const unsigned char & value);
    CDBValue(const CDBValue &src);
    ~CDBValue();


    [[nodiscard]] SDBValue value() const;
    [[nodiscard]] EValueTypes type() const;
    [[nodiscard]] Tstring name() const;

    void setValue(const short & value);
    void setValue(const unsigned short & value);
    void setValue(const int & value);
    void setValue(const unsigned int & value);
    void setValue(const long & value);
    void setValue(const unsigned long & value);
    void setValue(const float & value);
    void setValue(const double & value);
    void setValue(const long double & value);
    void setValue(const char & value);
    void setValue(const unsigned char & value);
    void setValue(const Tstring & value);


    [[nodiscard]] Tstring getValueAsString(Tsize DoublePrecision = 2,
                             bool isDoublePoint = true,
                             EBoolTypes BoolType = EBoolTypes::EBT_UPSTR) const;

    CDBValue& operator=(const CDBValue &rhs);


    [[nodiscard]] int         integer() const;
    [[nodiscard]] Tstring     string() const;
    long double               decimal() const;

private:
    SDBValue * m_value;

    void    set_string_data(const Tstring &value);
    void    del_str();
};


#endif //CDBVALUE_H
