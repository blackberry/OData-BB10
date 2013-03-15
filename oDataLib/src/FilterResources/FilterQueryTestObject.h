/*
 * FilterQueryTestObject.h
 *
 *  Created on: 15/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYTESTOBJECT_H_
#define FILTERQUERYTESTOBJECT_H_

#include <QObject>

class FilterQueryTestObject : public QObject {

    Q_OBJECT

public:
    FilterQueryTestObject();
    virtual ~FilterQueryTestObject();

    Q_INVOKABLE QString testFilterObject ();
};

#endif /* FILTERQUERYTESTOBJECT_H_ */



