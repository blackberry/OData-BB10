/*
 * LogicalFilterQueryObject.cpp
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#include "LogicalFilterQueryObject.h"

#include "LOGGER.h"

LogicalFilterQueryObject::LogicalFilterQueryObject(const QString& field, const QString& op, const QString& value) :
    queryText ("(" + field + " " + op + " '" + value + "')")
{
}

LogicalFilterQueryObject::LogicalFilterQueryObject(const QString& field, const QString& op, int value) :
    queryText (QString("(" + field + " " + op + " %1)").arg(value))
{
}

LogicalFilterQueryObject::LogicalFilterQueryObject(const LogicalFilterQueryObject& anotherFilter, const QString& op, const QString& value) :
    queryText ("(" + anotherFilter.getQueryText() + " " + op + " '" + value + "')")
{
}

LogicalFilterQueryObject::LogicalFilterQueryObject(const LogicalFilterQueryObject& anotherFilter, const QString& op, int value) :
    queryText (QString("(" + anotherFilter.getQueryText() + " " + op + " %1)").arg(value))
{
}

LogicalFilterQueryObject::LogicalFilterQueryObject(const QString &anotherQuery) :
    queryText (anotherQuery)
{
}


LogicalFilterQueryObject::~LogicalFilterQueryObject() {
}

QString LogicalFilterQueryObject::getQueryText() const {
    return queryText;
}

LogicalFilterQueryObject LogicalFilterQueryObject::notOperator() {
    QString fullText = "not (";
    fullText.append(queryText);
    fullText.append(")");

    LogicalFilterQueryObject returnValue (fullText);
    return returnValue;
}

LogicalFilterQueryObject LogicalFilterQueryObject::customOperator(QString op, const LogicalFilterQueryObject &anotherObject) {
    QString fullText = "(";
    fullText.append(queryText);
    fullText.append(" ");
    fullText.append(op);
    fullText.append(" ");
    fullText.append(anotherObject.getQueryText());
    fullText.append(")");

    LogicalFilterQueryObject returnValue (fullText);
    return returnValue;
}

LogicalFilterQueryObject LogicalFilterQueryObject::andOperator(const LogicalFilterQueryObject &anotherObject) {
    return this->customOperator("and", anotherObject);
}

LogicalFilterQueryObject LogicalFilterQueryObject::orOperator(const LogicalFilterQueryObject &anotherObject) {
    return this->customOperator("or", anotherObject);
}

LogicalFilterQueryObject LogicalFilterQueryObject::operator+(const LogicalFilterQueryObject &anotherObject) {
    return this->orOperator(anotherObject);
}

LogicalFilterQueryObject LogicalFilterQueryObject::operator*(const LogicalFilterQueryObject &anotherObject) {
    return this->andOperator(anotherObject);
}
