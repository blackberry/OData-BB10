/*
 * OrderByQueryCollection.h
 *
 *  Created on: 13/03/2013
 *      Author: adrian
 */

#ifndef ORDERBYQUERYCOLLECTION_H_
#define ORDERBYQUERYCOLLECTION_H_

#include <QObject>

#include "OrderByQueryObject.h"

//TODO: Extract Model information and leave these class as a Controller

class OrderByQueryCollection: public QObject {

    Q_OBJECT

    private:
        QList<OrderByQueryObject> queryObjects;

    public:
        OrderByQueryCollection(QObject *parent=0);
        virtual ~OrderByQueryCollection();

        Q_INVOKABLE void addQuery (const QString& field, bool ascending) ;

        QString getQueryText() const;
};

#endif /* ORDERBYQUERYCOLLECTION_H_ */
