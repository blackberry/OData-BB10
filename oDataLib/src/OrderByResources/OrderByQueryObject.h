/*
 * OrderByQueryObject.h
 *
 *  Created on: 12/03/2013
 *      Author: adrian
 */

#ifndef ORDERBYQUERYOBJECT_H_
#define ORDERBYQUERYOBJECT_H_

#include <QString>

class OrderByQueryObject {

private:
    QString field;
    bool ascending;

public:
    OrderByQueryObject();

    OrderByQueryObject(const QString &field, bool ascending);

    virtual ~OrderByQueryObject();

    bool getAscending() const;
    void setAscending(bool ascending);

    QString getField() const;
    void setField(const QString &);

    QString getQueryText() const;
};

#endif /* ORDERBYQUERYOBJECT_H_ */
