/*
 * FilterQueryObject.h
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYOBJECT_H_
#define FILTERQUERYOBJECT_H_

#include <QObject>

class FilterQueryObject : public QObject {

    Q_OBJECT

public:
    FilterQueryObject();
    virtual ~FilterQueryObject();

    Q_INVOKABLE QString testLogicalFilterObject ();

};

#endif /* FILTERQUERYOBJECT_H_ */
