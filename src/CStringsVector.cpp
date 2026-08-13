//
// Created by artem on 08.10.23.
//

#include "../include/CStringsVector.h"
#include <cstring>

CStringsVector::CStringsVector()
= default;

CStringsVector::CStringsVector(const CStringsVector &src)
{
    *this = src;
}

CStringsVector::~CStringsVector()
{
    clear();
}

CStringsVector &CStringsVector::operator=(const CStringsVector &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    clear();

    if (rhs.empty())
    {
        return *this;
    }

    resize(rhs.m_count);

    for (int i = 0; i < rhs.size(); i++)
    {
        this->push_back(rhs.at(i));
    }

    return *this;
}

void CStringsVector::clear()
{
    if (empty())
    {
        return;
    }

    for (int i = 0; i < m_count; i++)
    {
        free(m_items[i]);
    }

    free(m_items);

    m_count = 0;
    m_capacity = 0;
    m_items = nullptr;
}

bool CStringsVector::empty() const
{
    return m_count == 0;
}

Tsize CStringsVector::size() const
{
    return m_count;
}

Tstring CStringsVector::front() const
{
    if (empty())
    {
        return "";
    }

    Tstring r = m_items[0];

    return r;
}

Tstring CStringsVector::back() const
{
    if (empty())
    {
        return "";
    }

    return m_items[m_count - 1];
}

Tstring CStringsVector::pop_back()
{
    if (empty())
    {
        return "";
    }

    //Tstring b = back();

    remove_at_index(m_count - 1);

    return {};
}

Tstring CStringsVector::pop_front()
{
    if (empty())
    {
        return {};
    }

    Tstring f = m_items[0];

    remove_at_index(0);

    return f;
}

void CStringsVector::push_back(const Tstring &value)
{
    if (m_count == m_capacity)
    {
        resize(m_capacity * 2 + V_RESERVE);
    }

    Tsize len = value.length();

    if (len == 0)
    {
        return;
    }

    Tsize n_size = (len + 1) * sizeof(char);

    m_items[m_count] = (char*) malloc ( n_size );
    memcpy(m_items[m_count], value.c_str(), len);
    m_items[m_count][len] = 0;

    m_count++;
}

void CStringsVector::push_back(const char *value)
{
    Tstring val = value;
    if (val.empty())
    {
        return;
    }

    push_back(val);
}

Tstring CStringsVector::at(const Tsize &index) const
{
    if (index >= m_count)
    {
        return {};
    }

    return m_items[index];
}

void CStringsVector::resize(const Tsize &size)
{
    if (size <= m_count)
    {
        return;
    }

    Tsize n_size = size * sizeof(char*);

    char ** tmp = (char**) realloc(m_items, n_size);

    m_capacity = size;

    m_items = tmp;
}

void CStringsVector::remove_at_index(const Tsize &index)
{
    if (index >= m_count)
    {
        return;
    }

    if (m_count == 1)
    {
        clear();
        return;
    }

    free(m_items[index]);

    Tsize next = index + 1;
    Tsize len = (m_count - next) * sizeof(char*);

    if (next < m_count)
    {
        memmove(m_items + index, m_items + next, len);
    }

    double n_size = m_count;
    if ((double)(m_capacity - m_count) > n_size / 2.0)
    {
        n_size = (double)m_count * 1.2;
        resize((Tsize)(n_size));
    }

    m_count--;
}

CStringsVector &CStringsVector::operator<<(const Tstring &value)
{
    push_back(value);
    return *this;
}
