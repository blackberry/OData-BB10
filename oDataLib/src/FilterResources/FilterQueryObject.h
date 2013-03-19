/*
 * FilterQueryObject.h
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYOBJECT_H_
#define FILTERQUERYOBJECT_H_

#include <QString>

typedef enum {
    EQ,
    NE,
    GT,
    GE,
    LT,
    LE
} LogicalOperation;

static const char * logicalOperationStrings[] = { "eq", "ne", "gt", "ge", "lt", "le" };

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD
} ArithmeticOperation;

static const char * arithmeticOperationStrings[] = { "add", "sub", "mul", "div", "mod"};

class FilterQueryObject {

private:

    QString queryText;

    FilterQueryObject(const QString &);

    const FilterQueryObject customOperator(QString, const FilterQueryObject &) const;
    const FilterQueryObject andOperator(const FilterQueryObject &) const;
    const FilterQueryObject orOperator(const FilterQueryObject &) const;
    const FilterQueryObject notOperator() const;

public:

    FilterQueryObject();

    FilterQueryObject(const QString &field, LogicalOperation op, const QString &value);
    FilterQueryObject(const QString &field, LogicalOperation op, float value);

    FilterQueryObject(const QString &field, ArithmeticOperation arithmeticOperation, float arithmeticValue, LogicalOperation logicalOperation, float value);


    virtual ~FilterQueryObject();

    QString getQueryText() const;

    const FilterQueryObject operator&&(const FilterQueryObject &) const;
    const FilterQueryObject operator||(const FilterQueryObject &) const;
    const FilterQueryObject operator!() const;

};

#endif /* FILTERQUERYOBJECT_H_ */
