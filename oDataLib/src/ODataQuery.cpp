/*
 * ODataQuery.cpp
 *
 *  Created on: 20/03/2013
 *      Author: adrian
 */

#include "ODataQuery.h"

#include "LOGGER.h"

ODataQuery::ODataQuery(QObject *parent) :
    QObject(parent) {
}

ODataQuery::~ODataQuery() {
}

QString ODataQuery::getQueryText() {
    return requestUri;
}

ODataQuery* ODataQuery::setUri(const QString &baseUri) {
    requestUri = baseUri;
    return this;
}

QString ODataQuery::uri( ) const {
    return requestUri;
}

ODataQuery* ODataQuery::addQuery(const QString& queryField, const QString& queryValue) {
    LOGGER::log(queryField);

    QString queryRequest(requestUri);
    (queryRequest.contains("?")) ? queryRequest.append("&$") : queryRequest.append("?$");
    queryRequest.append(queryField);
    queryRequest.append("=");
    queryRequest.append(queryValue);
    requestUri = queryRequest;
    return this;

}

ODataQuery* ODataQuery::filter(const QString& filterQuery) {
    return this->addQuery("filter",filterQuery);
}

ODataQuery* ODataQuery::orderBy(const QString& fieldAndOrientation)  {
    return this->addQuery("orderby",fieldAndOrientation);
}

ODataQuery* ODataQuery::format(const QString& formatValue) {
    return this->addQuery("format",formatValue);
}
