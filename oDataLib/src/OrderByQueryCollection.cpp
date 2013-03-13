/*
 * OrderByQueryCollection.cpp
 *
 *  Created on: 13/03/2013
 *      Author: adrian
 */

#include "OrderByQueryCollection.h"

#include "LOGGER.h"

OrderByQueryCollection::OrderByQueryCollection(QObject *parent):
    QObject(parent),
    queryObjects (QList<OrderByQueryObject>())
{
}

OrderByQueryCollection::~OrderByQueryCollection() {
}

QString OrderByQueryCollection::getQueryText() const {

    QString queryRequest = QString();
    int size = queryObjects.count();

    int i = 0;

    while (i < size) {
        queryRequest.append(queryObjects.at(i).getQueryText());
        if (i < size -1) queryRequest.append(",");
        i++;
    }

    LOGGER::log("Query collection request: ",queryRequest);

    return queryRequest;
}

void OrderByQueryCollection::addQuery (const QString& field, bool ascending) {
    queryObjects.append(OrderByQueryObject(field, ascending));
}
