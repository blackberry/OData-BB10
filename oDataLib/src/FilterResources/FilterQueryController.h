/*
 * FilterQueryTestObject.h
 *
 *  Created on: 15/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYCONTROLLER_H_
#define FILTERQUERYCONTROLLER_H_

#include <QObject>

#include "FilterQueryObject.h"

class FilterQueryController : public QObject {

    Q_OBJECT

private:
    FilterQueryObject queryObject;

public:

    FilterQueryController();
    virtual ~FilterQueryController();

    Q_INVOKABLE QString testFilter();

    Q_INVOKABLE void addQuery(const QString &field, int arithmeticOp, float arithmeticValue, int logicalOp, float logicValue);

    Q_INVOKABLE void addQuery(const QString &field, int op, float value);
    Q_INVOKABLE void addQuery(const QString &field, int op, const QString &value);

    Q_INVOKABLE void andQuery(const QString &field, int op, const QString &value);
    Q_INVOKABLE void andQuery(const QString &field, int op, float value);

    Q_INVOKABLE void orQuery(const QString &field, int op, const QString &value);
    Q_INVOKABLE void orQuery(const QString &field, int op, float value);

    Q_INVOKABLE QString getQueryText() const;
};

#endif /* FILTERQUERYCONTROLLER_H_ */
