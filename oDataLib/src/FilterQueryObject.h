/*
 * FilterQueryObject.h
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYOBJECT_H_
#define FILTERQUERYOBJECT_H_

#include <QObject>

class FilterQueryObject  {

private:

    QString queryText;

    FilterQueryObject(const QString &);
    const FilterQueryObject customOperator(QString, const FilterQueryObject &) const;

public:
    FilterQueryObject(const QString &, const QString &, const QString &);
    FilterQueryObject(const QString &, const QString &, int);

    // Used when applying Arithmetic Operators, for example: /Products?$filter=Price add 5 gt 10
    FilterQueryObject(const FilterQueryObject&, const QString&, const QString&);
    FilterQueryObject(const FilterQueryObject&, const QString&, int);

    virtual ~FilterQueryObject();

    QString getQueryText() const;

    const FilterQueryObject andOperator(const FilterQueryObject &) const;
    const FilterQueryObject orOperator(const FilterQueryObject &) const;
    const FilterQueryObject notOperator() const;

    const FilterQueryObject operator*(const FilterQueryObject &) const;
    const FilterQueryObject operator+(const FilterQueryObject &) const;

    const FilterQueryObject operator&&(const FilterQueryObject &) const;
    const FilterQueryObject operator||(const FilterQueryObject &) const;

    const FilterQueryObject operator!() const;

};

#endif /* FILTERQUERYOBJECT_H_ */
