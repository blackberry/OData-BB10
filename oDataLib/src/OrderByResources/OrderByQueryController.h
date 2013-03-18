/*
 * OrderByQueryCollection.h
 *
 *  Created on: 13/03/2013
 *      Author: adrian
 */

#ifndef ORDERBYQUERYCONTROLLER_H_
#define ORDERBYQUERYCONTROLLER_H_

#include <QObject>

#include "OrderByQueryObject.h"

class OrderByQueryController: public QObject {

    Q_OBJECT

    private:
        QList<OrderByQueryObject> queryObjects;

    public:
        OrderByQueryController(QObject *parent=0);
        virtual ~OrderByQueryController();

        Q_INVOKABLE void addQuery (const QString& field, bool ascending) ;

        Q_INVOKABLE QString getQueryText() const;
};

#endif /* ORDERBYQUERYCONTROLLER_H_ */
