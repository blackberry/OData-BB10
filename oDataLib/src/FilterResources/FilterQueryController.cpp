/*
 * FilterQueryTestObject.cpp
 *
 *  Created on: 15/03/2013
 *      Author: adrian
 */

#include "FilterQueryController.h"

#include "../LOGGER.h"

FilterQueryController::FilterQueryController() {
}

FilterQueryController::~FilterQueryController() {
}

QString FilterQueryController::testFilter() {
    FilterQueryObject aObject ("Price",LT,9);
    FilterQueryObject bObject ("Name",EQ,"Milk");
    FilterQueryObject cObject ("Price",SUB,9,LT,10);

    LOGGER::log((aObject || bObject || cObject).getQueryText());

    return (aObject || bObject || cObject).getQueryText();
}

QString FilterQueryController::getQueryText() const {
    return queryObject.getQueryText();
}

//TODO: Change int op to LogicalOperation in methods interface

void FilterQueryController::addQuery(const QString &field, int arithmeticOp, float arithmeticValue, int logicalOp, float logicValue) {
    queryObject = FilterQueryObject(field, (ArithmeticOperation)arithmeticOp, arithmeticValue ,(LogicalOperation)logicalOp, logicValue);
}

void FilterQueryController::addQuery(const QString &field, int op, const QString &value) {
    queryObject = FilterQueryObject(field, (LogicalOperation)op, value);
}

void FilterQueryController::addQuery(const QString &field, int op, float value) {
    queryObject = FilterQueryObject(field, (LogicalOperation)op, value);
}

void FilterQueryController::andQuery(const QString &field, int op, const QString &value) {
    queryObject = queryObject && FilterQueryObject(field, (LogicalOperation)op, value);
}

void FilterQueryController::andQuery(const QString &field, int op, float value) {
    queryObject = queryObject && FilterQueryObject(field, (LogicalOperation)op, value);
}

void FilterQueryController::orQuery(const QString &field, int op, const QString &value) {
    queryObject = queryObject || FilterQueryObject(field, (LogicalOperation)op, value);
}

void FilterQueryController::orQuery(const QString &field, int op, float value) {
    queryObject = queryObject || FilterQueryObject(field, (LogicalOperation)op, value);
}



