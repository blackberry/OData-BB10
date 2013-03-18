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

private:


public:

    FilterQueryController();
    virtual ~FilterQueryController();

    Q_INVOKABLE QString testFilter();

};

#endif /* FILTERQUERYCONTROLLER_H_ */
