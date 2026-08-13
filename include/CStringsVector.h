//
// Created by artem on 08.10.23.
//

#ifndef REM_DB_CSTRINGSVECTOR_H
#define REM_DB_CSTRINGSVECTOR_H


#include "../include/db-types.h"

#define V_RESERVE 10



class CStringsVector
{
public:
    CStringsVector();
    CStringsVector(const CStringsVector &src);
    ~CStringsVector();

    CStringsVector & operator=(const CStringsVector& rhs);
    CStringsVector & operator<<(const Tstring &value);

    [[nodiscard]] Tstring front() const;
    [[nodiscard]] Tstring back() const;

    Tstring pop_back();
    Tstring pop_front();

    void    push_back(const Tstring &value);
    void    push_back(const char * value);

    [[nodiscard]] Tstring at(const Tsize &index) const;

    void    resize(const Tsize &size);

    void    clear();


    [[nodiscard]] bool    empty() const;
    [[nodiscard]] Tsize   size() const;

private:
    char ** m_items{nullptr};
    Tsize   m_capacity{0};
    Tsize   m_count{0};

    void    remove_at_index(const Tsize &index);
};



#endif //REM_DB_CSTRINGSVECTOR_H
