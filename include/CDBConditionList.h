//
// Created by nechi on 07.03.2022.
// upd by dan on 12.03.2022
//

#ifndef CDBCONDITIONLIST_H
#define CDBCONDITIONLIST_H

#include "../include/db-types.h"

#undef TYPES_H





/// @brief logic result
enum EConditionOpers : uint8_t {
    ECO_LT,     //!< less than
    ECO_LE,     //!< less or equal
    ECO_GT,     //!< grater than
    ECO_GE,     //!< grater or equal
    ECO_EQ,     //!< equal
    ECO_NE,     //!< not equal
    ECO_COUNT
};


static Tstring CondtionOpersStrings[EConditionOpers::ECO_COUNT] = {"<", "<=", ">", ">=", "=", "<>"};
static Tstring ConditionIconsSrc[EConditionOpers::ECO_COUNT] {
    ":/24/images/24/is-less-than.png",
    ":/24/images/24/is-less-than-or-equal-to.png",
    ":/24/images/24/is-greater-than.png",
    ":/24/images/24/more-than-or-equal.png",
    ":/24/images/24/equal.png",
    ":/24/images/24/is-not-equal-to.png"
};


/** @brief logic operations */
enum ELogicOpers : uint8_t {
    ELO_AND,
    ELO_OR,
    ELO_AND_NOT,
    ELO_OR_NOT,
    ELO_COUNT,
    ELO_NO

};


static Tstring LogicOpersStrings[ELogicOpers::ELO_COUNT] = {"AND", "OR", "AND NOT", "OR NOT"};

/** @brief */
struct SDBCondition {
    Tstring name;
    EConditionOpers oper{ECO_COUNT};
    Tstring value_s;
    ELogicOpers logic{ELO_NO};
    bool isString {false};

    SDBCondition()
        : name(""), oper(EConditionOpers::ECO_EQ), value_s(""), logic(ELogicOpers::ELO_NO), isString(false) {}

    SDBCondition(const Tstring &name, const EConditionOpers &oper, const Tstring &value, const ELogicOpers &logic, const bool isString = false)
        : name(name), oper(oper), value_s(value), logic(logic), isString(isString) {}

    SDBCondition(const SDBCondition & cond )
        : name(cond.name), oper(cond.oper), value_s(cond.value_s), logic(cond.logic), isString(cond.isString) {}

    SDBCondition(const Tstring &name, const Tstring &val, const EConditionOpers oper = ECO_EQ)
        : name(name), value_s(val), oper(oper) { this->isString = false;}

    //SDBCondition(Tstring n, Tstring v, )

};

class CDBConditionNode {

private:
    CDBConditionNode * next {nullptr};
    SDBCondition * value {nullptr};
    bool is_own{false};


public:
    explicit CDBConditionNode(SDBCondition * val);
    explicit CDBConditionNode(const SDBCondition & val);
    ~CDBConditionNode();

    CDBConditionNode * getNext();
    void setNext(CDBConditionNode * pvalue);
    SDBCondition * getValue();
};



class CDBConditionList {
private:
    CDBConditionNode * start {nullptr};
    CDBConditionNode * end {nullptr};
    Tsize count {0};

    CDBConditionNode * init(SDBCondition * value);
    CDBConditionNode * init(const SDBCondition & value);

public:
    CDBConditionList();
    explicit CDBConditionList(const SDBCondition & value);
    ~CDBConditionList();
    CDBConditionNode * push_back(SDBCondition * value);
    CDBConditionNode * push_back(const SDBCondition & value);
    bool isEmpty() const;
    SDBCondition * getValue(const Tsize & index);
    CDBConditionNode * getFirst();
    CDBConditionNode * getLast();
    void clear();
    Tsize Length() const;

    friend CDBConditionList& operator << (CDBConditionList &list, const SDBCondition &condition);
};



#endif //CDBCONDITIONLIST_H
