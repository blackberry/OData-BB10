/*
 * FilterQueryTestObject.cpp
 *
 *  Created on: 15/03/2013
 *      Author: adrian
 */

#include "FilterQueryController.h"

#include "FilterQueryObject.h"
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







