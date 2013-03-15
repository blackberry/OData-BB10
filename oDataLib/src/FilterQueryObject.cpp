/*
 * FilterQueryObject.cpp
 *
 *  Created on: 14/03/2013
 *      Author: adrian
 */

#include "FilterQueryObject.h"

#include "LogicalFilterQueryObject.h"

#include "LOGGER.h"

FilterQueryObject::FilterQueryObject() {
    // TODO Auto-generated constructor stub

}

FilterQueryObject::~FilterQueryObject() {
    // TODO Auto-generated destructor stub
}

QString FilterQueryObject::testLogicalFilterObject() {

    //TODO: Make real test, these ones are unfinished and they are only to see de coherence of the '()'
    LOGGER::log("Testing");

    LogicalFilterQueryObject aObject("Name", "eq", "Milk");
    LogicalFilterQueryObject bObject("Price", "sub", 5);
    LogicalFilterQueryObject cObject(bObject, "gt", 14);
    LogicalFilterQueryObject dObject("Name", "eq", "Bread");

    LOGGER::log("A: "+ aObject.getQueryText());
    LOGGER::log("B: "+ bObject.getQueryText());
    LOGGER::log("C: "+ cObject.getQueryText());
    LOGGER::log("D: "+ dObject.getQueryText());

    LogicalFilterQueryObject test = dObject.orOperator( (bObject.andOperator(cObject.andOperator(aObject))));
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

    LOGGER::log("End Testing");

    return test.getQueryText();
}
