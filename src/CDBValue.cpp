//
// Created by nechi on 07.03.2022.
// upd by dan on 12.03.2022
//

#include "../include/CDBValue.h"
#include <cstring>


bool operator== (const SDBValue &val1, const SDBValue &val2)
{
    if (val1.type != val2.type)
        return false;

    if (val1.name != val2.name)
        return false;

    switch(val1.type)
    {
        case EVT_SMALL:
            return val1.value.data_si == val2.value.data_si;
        case EVT_USMALL:
            return val1.value.data_us == val2.value.data_us;
        case EVT_INT:
            return val1.value.data_i == val2.value.data_i;
        case EVT_UINT:
            return val1.value.data_ui == val2.value.data_ui;
        case EVT_LONG_I:
            return val1.value.data_li == val2.value.data_li;
        case EVT_ULONG_I:
            return val1.value.data_ul == val2.value.data_ul;
        case EVT_FLOAT:
            return val1.value.data_f == val2.value.data_f;
        case EVT_DOUBLE:
            return val1.value.data_d == val2.value.data_d;
        case EVT_LONG_DOUBLE:
            return val1.value.data_ld == val2.value.data_ld;
        case EVT_SINGLE_CHAR:
            return val1.value.data_c == val2.value.data_c;
        case EVT_STRING:
            return strcmp(val1.value.data_s, val2.value.data_s) == 0;
        case EVT_BYTE:
            return val1.value.data_uc == val2.value.data_uc;
        case EVT_NO:
        case EVT_COUNT:
        case EVT_TIME_T:
            return true;
    }
    return false;
}

bool operator!= (const SDBValue &val1, const SDBValue &val2)
{
    return !(val1 == val2);
}

SDBValue::~SDBValue()
{
    if (type ==EVT_STRING)
    {
        delete [] value.data_s;
    }
}

SDBValue::SDBValue(const SDBValue &other)
{
    copyFrom(other);
}

SDBValue::SDBValue(SDBValue &&other) noexcept
{
    moveFrom(std::move(other));
}

SDBValue &SDBValue::operator=(const SDBValue &other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }
    return *this;
}

SDBValue &SDBValue::operator=(SDBValue &&other) noexcept
{
    if (this != &other)
    {
        clear();
        moveFrom(std::move(other));
    }
    return *this;
}

void SDBValue::copyFrom(const SDBValue &other)
{
    type = other.type;
    name = other.name;

    if (type == EVT_STRING && other.value.data_s != nullptr)
    {
        size_t len = strlen(other.value.data_s);
        value.data_s = new char[len + 1];
        strcpy(value.data_s, other.value.data_s);
    }
    else
    {
        value = other.value;
    }

}

void SDBValue::moveFrom(SDBValue &&other) noexcept
{
    type = other.type;
    name = std::move(other.name);
    value = other.value;

    other.type = EVT_NO;
    other.value.data_s = nullptr;
}

void SDBValue::clear()
{
    if (type == EVT_STRING && value.data_s != nullptr)
    {
        delete[] value.data_s;
        value.data_s = nullptr;
    }
    type = EVT_NO;
}


CDBValue::CDBValue()
{
    m_value = new SDBValue();
}

CDBValue::~CDBValue()
{
    delete m_value;
}

CDBValue::CDBValue(const Tstring &name)
{
    m_value = new SDBValue();
    m_value->name = name;
}

CDBValue::CDBValue(const Tstring &name, const short &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_si = value;
    m_value->type = EVT_SMALL;
}

CDBValue::CDBValue(const Tstring &name, const unsigned short &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_us = value;
    m_value->type = EVT_USMALL;
}

CDBValue::CDBValue(const Tstring &name, const int &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_i = value;
    m_value->type = EVT_INT;
}

CDBValue::CDBValue(const Tstring &name, const unsigned int &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_ui = value;
    m_value->type = EVT_UINT;
}
#ifdef Windows
CDBValue::CDBValue(const Tstring &name, const long &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_li = value;
    m_value->type = EVT_LONG_I;
}

CDBValue::CDBValue(const Tstring &name, const unsigned long &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_ul = value;
    m_value->type = EVT_ULONG_I;
}
#endif

CDBValue::CDBValue(const Tstring &name, const Tsize & value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_ul = value;
    m_value->type = EVT_ULONG_I;
}

CDBValue::CDBValue(const Tstring &name, const time_t &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_tt = value;
    m_value->type = EVT_TIME_T;
}

CDBValue::CDBValue(const Tstring &name, const float &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_f = value;
    m_value->type = EVT_FLOAT;
}

CDBValue::CDBValue(const Tstring &name, const double &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_d = value;
    m_value->type = EVT_DOUBLE;
}

CDBValue::CDBValue(const Tstring &name, const long double &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_ld = value;
    m_value->type = EVT_LONG_DOUBLE;
}

CDBValue::CDBValue(const Tstring &name, const Tstring &value)
{
    m_value = new SDBValue();
    set_string_data(value);
    m_value->name = name;
    m_value->type = EValueTypes::EVT_STRING;
}

void CDBValue::set_string_data(const Tstring &value)
{
    if (value.empty())
    {
        return;
    }

    if (m_value->type == EVT_STRING)
    {
        delete [] m_value->value.data_s;
    }

    auto len = value.length();

    m_value->value.data_s = new char [len+1];
    memcpy(m_value->value.data_s, value.c_str(), len);
    m_value->value.data_s[len] = 0;

    m_value->type = EVT_STRING;
}

void CDBValue::del_str()
{
    if (m_value->type == EVT_STRING)
    {
        delete [] m_value->value.data_s;
        m_value->value.data_s = nullptr;
    }
}

CDBValue::CDBValue(const Tstring &name, const char *value)
{
    m_value = new SDBValue();
    m_value->name = name;

    if (!value)
    {
        return;
    }

    auto len = strlen(value);

    m_value->value.data_s = new char[len+1];

    memcpy(m_value->value.data_s, value, len);
    m_value->value.data_s[len] = 0;
}

CDBValue::CDBValue(const Tstring &name, const char &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_c = value;
    m_value->type = EVT_SINGLE_CHAR;
}

CDBValue::CDBValue(const CDBValue &src)
{
    m_value = new SDBValue();

    m_value->type = src.m_value->type;

    switch (m_value->type)
    {
        case EVT_STRING:
            set_string_data(src.m_value->value.data_s);
            break;
        default:
            m_value->value = src.m_value->value;
    }
}

SDBValue CDBValue::value() const
{
    SDBValue val = *m_value;
    return val;
}

EValueTypes CDBValue::type() const
{
    return m_value->type;
}

Tstring CDBValue::name() const
{
    return m_value->name;
}

CDBValue::CDBValue(const Tstring &name, const unsigned char &value)
{
    m_value = new SDBValue();
    m_value->name = name;
    m_value->value.data_uc = value;
    m_value->type = EVT_BYTE;
}

void CDBValue::setValue(const short &value)
{
    del_str();
    m_value->value.data_si = value;
    m_value->type = EVT_SMALL;
}

void CDBValue::setValue(const unsigned short &value)
{
    del_str();
    m_value->value.data_us = value;
    m_value->type = EVT_USMALL;
}

void CDBValue::setValue(const int &value)
{
    del_str();
    m_value->value.data_i = value;
    m_value->type = EVT_INT;
}

void CDBValue::setValue(const unsigned int &value)
{
    del_str();
    m_value->value.data_ui = value;
    m_value->type = EVT_UINT;
}

void CDBValue::setValue(const long &value)
{
    del_str();
    m_value->value.data_li = value;
    m_value->type = EVT_LONG_I;
}

void CDBValue::setValue(const unsigned long &value)
{
    del_str();
    m_value->value.data_ul = value;
    m_value->type = EVT_ULONG_I;
}

void CDBValue::setValue(const float &value)
{
    del_str();
    m_value->value.data_f = value;
    m_value->type = EVT_FLOAT;
}

void CDBValue::setValue(const double &value)
{
    del_str();
    m_value->value.data_d = value;
    m_value->type = EVT_DOUBLE;
}

void CDBValue::setValue(const long double &value)
{
    del_str();
    m_value->value.data_ld = value;
    m_value->type = EVT_LONG_DOUBLE;
}

void CDBValue::setValue(const char &value)
{
    del_str();
    m_value->value.data_c = value;
    m_value->type = EVT_SINGLE_CHAR;
}

void CDBValue::setValue(const unsigned char &value)
{
    del_str();
    m_value->value.data_uc = value;
    m_value->type = EVT_BYTE;
}

CDBValue &CDBValue::operator=(const CDBValue &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    del_str();

    m_value->type = rhs.m_value->type;
    m_value->name = rhs.m_value->name;

    switch (m_value->type)
    {
        case EVT_STRING:
            set_string_data(rhs.m_value->value.data_s);
            break;
        default:
            m_value->value = rhs.m_value->value;
            break;
    }

    return *this;
}

void CDBValue::setValue(const Tstring &value)
{
    del_str();
    set_string_data(value);
}

Tstring CDBValue::getValueAsString(Tsize DoublePrecision, bool isDoublePoint, EBoolTypes BoolType) const
{
    switch(m_value->type)
    {
        case EVT_SMALL:
            return CFunctions::ToString(m_value->value.data_si);
        case EVT_USMALL:
            return CFunctions::ToString(m_value->value.data_us);
        case EVT_INT:
            return CFunctions::ToString(m_value->value.data_i);
        case EVT_UINT:
            return CFunctions::ToString(m_value->value.data_ui);
        case EVT_LONG_I:
            return CFunctions::ToString(m_value->value.data_li);
        case EVT_ULONG_I:
            return CFunctions::ToString(m_value->value.data_ul);
        case EVT_FLOAT:
            return CFunctions::ToString(m_value->value.data_f, DoublePrecision, isDoublePoint);
        case EVT_DOUBLE:
            return CFunctions::ToString(m_value->value.data_d, DoublePrecision, isDoublePoint);
        case EVT_LONG_DOUBLE:
            return CFunctions::ToString(m_value->value.data_ld, DoublePrecision, isDoublePoint);
        case EVT_SINGLE_CHAR:
            return CFunctions::ToString(m_value->value.data_c);
        case EVT_STRING:
            return m_value->value.data_s;
        case EVT_BYTE:
            return CFunctions::ToString(m_value->value.data_uc);
        case EVT_TIME_T:
            return CFunctions::ToString(m_value->value.data_tt);
        case EVT_NO:
        case EVT_COUNT:
            return "-";
    }
    return "-";
}

int CDBValue::integer() const
{
    switch(m_value->type)
    {
        case EVT_SMALL:
            return m_value->value.data_si;
        case EVT_USMALL:
            return static_cast<int>(m_value->value.data_us);
        case EVT_INT:
            return static_cast<int>(m_value->value.data_i);
        case EVT_UINT:
            return static_cast<int>(m_value->value.data_ui);
        case EVT_LONG_I:
            return static_cast<int>(m_value->value.data_li);
        case EVT_ULONG_I:
            return static_cast<int>(m_value->value.data_ul);
        case EVT_FLOAT:
            return static_cast<int>(m_value->value.data_f);
        case EVT_DOUBLE:
            return static_cast<int>(m_value->value.data_d);
        case EVT_LONG_DOUBLE:
            return static_cast<int>(m_value->value.data_ld);
        case EVT_SINGLE_CHAR:
            return static_cast<int>(m_value->value.data_c);
        case EVT_BYTE:
            return static_cast<int>(m_value->value.data_uc);
        case EVT_STRING:
        default:
            //throw std::runtime_error("wrong datatype required");
            return 0;
    }
}

Tstring CDBValue::string() const
{
    return getValueAsString();
}

long double CDBValue::decimal() const
{
    switch(m_value->type)
    {
        case EVT_SMALL:
            return m_value->value.data_si;
        case EVT_USMALL:
            return static_cast<long double>(m_value->value.data_us);
        case EVT_INT:
            return static_cast<long double>(m_value->value.data_i);
        case EVT_UINT:
            return static_cast<long double>(m_value->value.data_ui);
        case EVT_LONG_I:
            return static_cast<long double>(m_value->value.data_li);
        case EVT_ULONG_I:
            return static_cast<long double>(m_value->value.data_ul);
        case EVT_FLOAT:
            return static_cast<long double>(m_value->value.data_f);
        case EVT_DOUBLE:
            return static_cast<long double>(m_value->value.data_d);
        case EVT_LONG_DOUBLE:
            return static_cast<long double>(m_value->value.data_ld);
        case EVT_SINGLE_CHAR:
            return static_cast<long double>(m_value->value.data_c);
        case EVT_BYTE:
            return static_cast<long double>(m_value->value.data_uc);
        case EVT_TIME_T:
            return static_cast<long double>(m_value->value.data_tt);
        case EVT_STRING:
        default:
            //throw std::runtime_error("wrong datatype required");
            return 0;
    }
}
