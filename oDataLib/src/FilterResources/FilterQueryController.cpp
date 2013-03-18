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

QString FilterQueryController::testFilterObject() {

    //TODO: Make real test, these ones are unfinished and they are only to see the coherence of the '()'
    LOGGER::log("Testing");

    FilterQueryObject aObject("Name", "eq", "Milk");
    FilterQueryObject bObject("Price", "sub", 5);
    FilterQueryObject cObject(bObject, "gt", 14);
    FilterQueryObject dObject("Name", "eq", "Bread");

    LOGGER::log("A: "+ aObject.getQueryText());
    LOGGER::log("B: "+ bObject.getQueryText());
    LOGGER::log("C: "+ cObject.getQueryText());
    LOGGER::log("D: "+ dObject.getQueryText());

    FilterQueryObject test = dObject.orOperator( (bObject.andOperator(cObject.andOperator(aObject))));
    LOGGER::log("Test 1: "+test.getQueryText());

    test = (dObject.andOperator(aObject)).orOperator( bObject.andOperator(cObject).notOperator());
    LOGGER::log("Test 2: "+test.getQueryText());

    test = (dObject * aObject) + (bObject * cObject);
    LOGGER::log("Test 3: "+test.getQueryText());

    test = (dObject + cObject) * aObject;
    LOGGER::log("Test 4: "+test.getQueryText());

    test = (dObject || aObject) || (bObject && cObject);
    LOGGER::log("Test 5: "+test.getQueryText());

    test = (dObject || cObject) * !aObject;
    LOGGER::log("Test 6: "+test.getQueryText());

    FilterQueryObject eObject("Price", "le", 200);
    FilterQueryObject fObject("Price", "gt", 3.5);

    test = eObject && fObject;

    return test.getQueryText();
}
