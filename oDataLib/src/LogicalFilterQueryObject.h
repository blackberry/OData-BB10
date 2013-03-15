/*
 * LogicalFilterQueryObject.h
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#ifndef LOGICALFILTERQUERYOBJECT_H_
#define LOGICALFILTERQUERYOBJECT_H_

#include <QObject>

class LogicalFilterQueryObject  {

private:

    QString queryText;

    LogicalFilterQueryObject(const QString &);
    LogicalFilterQueryObject  customOperator(QString, const LogicalFilterQueryObject &);

public:
    LogicalFilterQueryObject(const QString &, const QString &, const QString &);
    LogicalFilterQueryObject(const QString &, const QString &, int);

    // Used when applying Arithmetic Operators, for example: /Products?$filter=Price add 5 gt 10
    LogicalFilterQueryObject(const LogicalFilterQueryObject&, const QString&, const QString&);
    LogicalFilterQueryObject(const LogicalFilterQueryObject&, const QString&, int);

    virtual ~LogicalFilterQueryObject();

    QString getQueryText() const;

    LogicalFilterQueryObject  andOperator(const LogicalFilterQueryObject &);
    LogicalFilterQueryObject  orOperator(const LogicalFilterQueryObject &);
    LogicalFilterQueryObject  notOperator();

     LogicalFilterQueryObject operator+(const LogicalFilterQueryObject &) ;
     LogicalFilterQueryObject operator*(const LogicalFilterQueryObject &) ;

};

#endif /* LOGICALFILTERQUERYOBJECT_H_ */
