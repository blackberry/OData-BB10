/*
 * OrderByQueryObject.cpp
 *
 *  Created on: 12/03/2013
 *      Author: adrian
 */

#include "OrderByQueryObject.h"

OrderByQueryObject::OrderByQueryObject():
    field (" "),
    ascending (true) {
}

OrderByQueryObject::OrderByQueryObject(const QString &field, bool ascending) :
    field (field),
    ascending (ascending) {
}

OrderByQueryObject::~OrderByQueryObject() {
}

/*OrderByQueryObject::OrderByQueryObject(const OrderByQueryObject & other) :
    field (other.field),
    ascending (other.ascending) {
}

OrderByQueryObject& OrderByQueryObject::operator=(const OrderByQueryObject &other) {
    this->field = other.field;
    this->ascending = other.ascending;
    return *this;
}*/

bool OrderByQueryObject::getAscending() const {
    return ascending;
}

void OrderByQueryObject::setAscending(bool ascending) {
    this->ascending = ascending;
}

QString OrderByQueryObject::getField() const {
    return field;
}

void OrderByQueryObject::setField(const QString & field) {
    this->field = field;
}

QString OrderByQueryObject::getQueryText() const {
    QString queryText = QString(this->field);
    (this->ascending) ? queryText.append(" asc") :  queryText.append(" desc");
    return queryText;
}
