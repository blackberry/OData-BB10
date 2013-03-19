/*
 * FilterQueryObject.cpp
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#include "FilterQueryObject.h"

FilterQueryObject::FilterQueryObject() :
    queryText("")
{

}

FilterQueryObject::FilterQueryObject(const QString& field, LogicalOperation op, const QString& value) :
    queryText (QString("(" + field + " %1 '" + value + "')").arg(logicalOperationStrings[op]))
{
}

FilterQueryObject::FilterQueryObject(const QString& field, LogicalOperation op, float value) :
    queryText (QString("(" + field + " %1 %2)").arg(logicalOperationStrings[op]).arg(value))
{
}

FilterQueryObject::FilterQueryObject(const QString &field, ArithmeticOperation arithmeticOperation, float arithmeticValue, LogicalOperation logicalOperation, float value) :
            queryText (QString("(" + field + " %1 %2) %3 %4").arg(arithmeticOperationStrings[arithmeticOperation]).arg(arithmeticValue).arg(logicalOperationStrings[logicalOperation]).arg(value))
{
}

FilterQueryObject::FilterQueryObject(const QString &anotherQuery) :
    queryText (anotherQuery)
{
}

FilterQueryObject::~FilterQueryObject() {
}

QString FilterQueryObject::getQueryText() const {
    return queryText;
}

const FilterQueryObject FilterQueryObject::notOperator() const {
    QString fullText = "not (";
    fullText.append(queryText);
    fullText.append(")");

    FilterQueryObject returnValue (fullText);
    return returnValue;
}

const FilterQueryObject FilterQueryObject::customOperator(QString op, const FilterQueryObject &anotherObject) const {
    QString fullText = "(";
    fullText.append(queryText);
    fullText.append(" ");
    fullText.append(op);
    fullText.append(" ");
    fullText.append(anotherObject.getQueryText());
    fullText.append(")");

    FilterQueryObject returnValue(fullText);
    return returnValue;
}

const FilterQueryObject FilterQueryObject::andOperator(const FilterQueryObject &anotherObject) const {
    return this->customOperator("and", anotherObject);
}

const FilterQueryObject FilterQueryObject::orOperator(const FilterQueryObject &anotherObject) const {
    return this->customOperator("or", anotherObject);
}

const FilterQueryObject FilterQueryObject::operator||(const FilterQueryObject &anotherObject) const {
    return this->orOperator(anotherObject);
}

const FilterQueryObject FilterQueryObject::operator&&(const FilterQueryObject &anotherObject) const {
    return this->andOperator(anotherObject);
}

const FilterQueryObject FilterQueryObject::operator! () const {
    return this->notOperator();
}
