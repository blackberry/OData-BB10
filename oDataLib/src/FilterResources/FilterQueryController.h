/*
 * FilterQueryTestObject.h
 *
 *  Created on: 15/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYCONTROLLER_H_
#define FILTERQUERYCONTROLLER_H_

#include <QObject>

class FilterQueryController : public QObject {

    Q_OBJECT

public:
    FilterQueryController();
    virtual ~FilterQueryController();

    Q_INVOKABLE QString testFilterObject ();

    //Q_INVOKABLE QString testFilterObject (QString query);
    //Q_INVOKABLE QString testFilterObject (const QString &field, const QString &op, const QString &value);
};

#endif /* FILTERQUERYCONTROLLER_H_ */
