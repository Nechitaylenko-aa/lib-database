//
// Created by artem on 04.10.23.
//

#ifndef REMINDER_CDBCONNECTERROR_H
#define REMINDER_CDBCONNECTERROR_H

#include "CAbstractConnection.h"

/** @brief database m_connection exception */
class CDBConnectError
{
public:
    CDBConnectError() = delete;
    CDBConnectError(const CDBConnectError &) = delete;
    CDBConnectError(CAbstractConnection *connection, const int &err_code);
    ~CDBConnectError();

    const char* what();

protected:

private:


};


#endif //REMINDER_CDBCONNECTERROR_H
