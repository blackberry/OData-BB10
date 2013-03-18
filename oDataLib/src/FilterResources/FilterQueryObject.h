/*
 * FilterQueryObject.h
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#ifndef FILTERQUERYOBJECT_H_
#define FILTERQUERYOBJECT_H_

#include <QObject>

class FilterQueryObject {

private:

    QString queryText;

    const FilterQueryObject customOperator(QString, const FilterQueryObject &) const;

public:

    FilterQueryObject(const QString &);

    FilterQueryObject(const QString &field, const QString &op, const QString &value);
    FilterQueryObject(const QString &field, const QString &op, float value);

    // Used when applying Arithmetic Operators, for example: /Products?$filter=Price add 5 gt 10
    FilterQueryObject(const FilterQueryObject &arithmeticObject, const QString &op, const QString &value);
    FilterQueryObject(const FilterQueryObject &arithmeticObject, const QString&op, float value);

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
