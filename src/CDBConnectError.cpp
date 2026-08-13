//
// Created by artem on 04.10.23.
//

#include "../include/CDBConnectError.h"

CDBConnectError::CDBConnectError(CAbstractConnection *connection, const int &err_code)
{

}

CDBConnectError::~CDBConnectError()
= default;

const char *CDBConnectError::what()
{
    return nullptr;
}
