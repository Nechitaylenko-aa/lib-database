//
// Created by nechi on 07.03.2022.
// upd by dan on 05.04.2022
//


#include "../include/CDBConditionList.h"


/*********************************************************************
 * Реализация методов класса CDBConditionNode
 *
 *
 *******************************************************************
*/


/**
 * @brief CDBConditionNode::CDBConditionNode - конструктор присваивания
 * @param val - указатель на значение, добавляемое в список
 */

CDBConditionNode::CDBConditionNode(SDBCondition * val){
    value = val;
    next = nullptr;
    is_own = false;
}

CDBConditionNode::CDBConditionNode(const SDBCondition &val)
{
    value = new SDBCondition(val);
    is_own = true;
    next = nullptr;
}

CDBConditionNode::~CDBConditionNode()
{
    if (is_own)
        delete value;
}

CDBConditionNode * CDBConditionNode::getNext()
{
    return this->next;
}

void CDBConditionNode::setNext(CDBConditionNode * pvalue)
{
    next = pvalue;
}


SDBCondition * CDBConditionNode::getValue()
{
    return this->value;
}



/*********************************************************************
 * Реализация методов класса CDBConditionList
 *
 *
 *******************************************************************
*/


CDBConditionList::CDBConditionList() {
}

CDBConditionList::CDBConditionList(const SDBCondition &value)
{
    this->push_back(value);
}


CDBConditionList::~CDBConditionList() {
    this->clear();
}

CDBConditionNode * CDBConditionList::init(SDBCondition * value) {

    CDBConditionNode * res = new CDBConditionNode(value);
    this->start = res;
    this->end = res;
    ++this->count;
    return res;
}

CDBConditionNode *CDBConditionList::init(const SDBCondition &value)
{
    CDBConditionNode * res = new CDBConditionNode(value);
    this->start = res;
    this->end = res;
    ++this->count;
    return res;
}

CDBConditionNode * CDBConditionList::push_back(SDBCondition * value) {

    if (this->isEmpty())
    {
        return this->init(value);
    }
    else
    {
        auto * res = new CDBConditionNode(value);
        this->end->setNext(res);
        this->end = res;
        ++this->count;
        return res;
    }
}

CDBConditionNode *CDBConditionList::push_back(const SDBCondition &value)
{
    if (this->isEmpty())
    {
        return this->init(value);
    }
    else
    {
        auto * res = new CDBConditionNode(value);
        this->end->setNext(res);
        this->end = res;
        ++this->count;
        return res;
    }
}

bool CDBConditionList::isEmpty() const {
    return this->start == nullptr;
}


SDBCondition * CDBConditionList::getValue(const Tsize & index) {
    if (this->isEmpty() || index >= this->Length())   return nullptr;

    Tsize current_index = 0;
    CDBConditionNode * current;

    // find by index
    for (current = this->start; index != current_index; current = current->getNext(), ++current_index)
        ; // stop when index arrived.

    return current->getValue();

}

CDBConditionNode *CDBConditionList::getFirst()
{
    return this->start;
}

CDBConditionNode *CDBConditionList::getLast()
{
    return this->end;
}



void CDBConditionList::clear() {

    if (this->isEmpty())  return;

    CDBConditionNode * node = this->start;

    while (node){
        CDBConditionNode * current = node;
        node = node->getNext();
        delete current;
        --this->count;
    }
    this->start = nullptr;
    this->end = nullptr;
    this->count = 0;
}

Tsize CDBConditionList::Length() const
{
    return this->count;
}

CDBConditionList &operator<<(CDBConditionList &list, const SDBCondition &condition) {
    list.push_back(condition);
    return list;
}
