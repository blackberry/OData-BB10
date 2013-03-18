/*
 * OrderByQueryCollection.cpp
 *
 *  Created on: 13/03/2013
 *      Author: adrian
 */

#include "OrderByQueryController.h"

OrderByQueryController::OrderByQueryController(QObject *parent):
    QObject(parent),
    queryObjects (QList<OrderByQueryObject>())
{
}

OrderByQueryController::~OrderByQueryController() {
}

QString OrderByQueryController::getQueryText() const {

    QString queryRequest = QString();
    int size = queryObjects.count();

    int i = 0;

    while (i < size) {
        queryRequest.append(queryObjects.at(i).getQueryText());
        if (i < size -1) queryRequest.append(",");
        i++;
    }

    return queryRequest;
}

void OrderByQueryController::addQuery (const QString& field, bool ascending) {
    queryObjects.append(OrderByQueryObject(field, ascending));
}
