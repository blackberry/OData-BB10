/*
 * FilterQueryObject.cpp
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#include "FilterQueryObject.h"

FilterQueryObject::FilterQueryObject(const QString& field, const QString& op, const QString& value) :
    queryText ("(" + field + " " + op + " '" + value + "')")
{
}

FilterQueryObject::FilterQueryObject(const QString& field, const QString& op, float value) :
    queryText (QString("(" + field + " " + op + " %1)").arg(value))
{
}

FilterQueryObject::FilterQueryObject(const FilterQueryObject& anotherFilter, const QString& op, const QString& value) :
    queryText ("(" + anotherFilter.getQueryText() + " " + op + " '" + value + "')")
{
}

FilterQueryObject::FilterQueryObject(const FilterQueryObject& anotherFilter, const QString& op, float value) :
    queryText (QString("(" + anotherFilter.getQueryText() + " " + op + " %1)").arg(value))
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

    FilterQueryObject returnValue (fullText);
    return returnValue;
}

const FilterQueryObject FilterQueryObject::andOperator(const FilterQueryObject &anotherObject) const {
    return this->customOperator("and", anotherObject);
}

const FilterQueryObject FilterQueryObject::orOperator(const FilterQueryObject &anotherObject) const {
    return this->customOperator("or", anotherObject);
}

const FilterQueryObject FilterQueryObject::operator+ (const FilterQueryObject &anotherObject) const {
    return this->orOperator(anotherObject);
}

const FilterQueryObject FilterQueryObject::operator* (const FilterQueryObject &anotherObject) const {
    return this->andOperator(anotherObject);
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
