/*
 * ODataSource.cpp
 *
 *  Created on: Mar 4, 2013
 *      Author: Martin Larochelle
 *
 *
 *  Uses data sources from:
 *      http://developer.netflix.com/docs/oData_Catalog
 */

#include "ODataSource.h"
#include <bb/data/JsonDataAccess>
//#include <bb/data/XmlDataAccess>
#include <bb/system/SystemToast>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include "cODataLib.h"
#include "Atom_constants.h"
#include "cAtomManager.h"

#include "LOGGER.h"

#include "OrderByResources/OrderByQueryObject.h"


using namespace bb::cascades;
using namespace bb::data;
using bb::system::SystemToast;

const int ITEMS_PER_PAGE = 12; // MUST be more than what fits in a page else onAtEndChanged is not triggered

// TODO: Support read of ATOM sources as: http://services.odata.org/OData/OData.svc/
// For JSON: http://services.odata.org/OData/OData.svc/?$format=json
// TODO: Make the class generic so any odata service may be read
// TODO: Implement filters
// TODO: Should $format=json be a parameter?
// TODO: Should we support jsonverbose? http://www.odata.org/media/30002/OData%20JSON%20Verbose%20Format.html
ODataSource::ODataSource(QObject *parent) :
                QObject(parent),
                m_nPage(0),
                m_bEndReached(false),
                m_pagingEnabled(false),
                m_JSONEnabled(false) {
    m_oDataModel = new ArrayDataModel(this);
    m_netManager = new QNetworkAccessManager();
}

ODataSource::~ODataSource() {
}

//////////// For testing purposes only
QVariant ODataSource::byIntegerCallBogus(QVariant q, int iMethod)
{
	QVariant vt;
	int toSwapInDebugTime = iMethod;

	QString test = cAtomManager::Instance().getAtomXMLUpdatedDateTime();

	LOGGER::log("date time is = ",  test);

	// Testing writing to database with a some simplified atom/xml string using "PUT"
	if (toSwapInDebugTime == 1)
	{
		vt = cAtomManager::Instance().createHTTP_request_test("PUT");
		QByteArray arrB = vt.value<QByteArray>();

		// Info
		QString toXchge = QString(arrB);
	    LOGGER::log("ODataSource::arrB == ",  toXchge);

	    // Nota bene: ... This link was generated manually.
	    // Use any browser for generating it from OData.org / see sample services Read/Write database
	    QString url("http://services.odata.org/(S(0aatlc1jdpzfp1x241vfduge))/OData/OData.svc/Categories(0)");

	    // should be changed to update data
	    updateData(url, arrB);
	}
	// Testing writing to database with a some simplified atom/xml string using "POST"
	// this is intended for adding an entry in a table
	// ... IDEALLY, read the $metadata; build in GUI with a list of data input box for each of the items described in the metadata;
	// Allow the end-user to enter data; then send an entry with new data to be entered in database
	else if (toSwapInDebugTime == 2)
	{
		vt = cAtomManager::Instance().createHTTP_request_test("POST");
		QByteArray arrB = vt.value<QByteArray>();

		// Info
		QString toXchge = QString(arrB);
	    LOGGER::log("ODataSource::arrB == ",  toXchge);

	    // Nota bene: ... This link was generated manually.
	    // Use any browser for generating it from OData.org / see sample services Read/Write database
	    QString url("http://services.odata.org/(S(0aatlc1jdpzfp1x241vfduge))/OData/OData.svc/Categories(0)");

	    // should be changed to update data
	    //addData(url, arrB);
	}

	return vt;
}

/* Filter querys */

void ODataSource::filter(const QString& requestURL, const QString& filterQuery ,bool paging) {
    QString queryRequest(requestURL);
    (queryRequest.contains("?")) ? queryRequest.append("&") : queryRequest.append("?");
    queryRequest.append("$filter=");
    queryRequest.append(filterQuery);
    this->fetchData(queryRequest, paging);
}

/* Order By querys */

// fieldAndOrientation querys options separated by comma ',', for example 'ReleaseDate asc, Rating desc'
void ODataSource::orderBy(const QString& requestURL, const QString& fieldAndOrientation ,bool paging)  {
    QString queryRequest(requestURL);
    (queryRequest.contains("?")) ? queryRequest.append("&") : queryRequest.append("?");
    queryRequest.append("$orderby=");
    queryRequest.append(fieldAndOrientation);
    this->fetchData(queryRequest, paging);
}

void ODataSource::updateData(const QString& requestURL, const QByteArray& body) {
	QNetworkRequest request;
    QString addRequestURL = requestURL;

    // Not planning to support this yet
    // m_JSONEnabled = addRequestURL.contains(QString("$format=json"));

    LOGGER::log("ODataSource::addData - addRequestURL:", addRequestURL);
    request.setUrl(addRequestURL);

    // set the raw headers
    // QT has a definition for Content-Type... but not for the others...
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/atom+xml"); // ATOM_Resource_Content_Type);  ///);

    request.setRawHeader(HttpRequestHeader_Accept, "application/atom+xml"); // should it be ==> ATOM_Resource_Accept); //);
    request.setRawHeader(HttpRequestHeader_UserAgent, "BB10 ODataLib v0.1");
    request.setRawHeader(HttpRequestHeader_DataService, Resource_DataServiceVersion_1);
    request.setRawHeader(HttpRequestHeader_MaxDataService, Resource_MaxDataServiceVersion);
    request.setRawHeader(HttpRequestHeader_Host, "services.odata.org");
    QByteArray postDataSize = QByteArray::number(body.size());
    request.setRawHeader(HttpRequestHeader_ContentLength, postDataSize);

    QNetworkReply* reply = m_netManager->put(request, body);
    if (reply) {
    	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onUpdatingDataError_Slot(QNetworkReply::NetworkError)));

        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onfinishedUpdatingData_Slot()));

        if (!res) {
            LOGGER::log("ODataSource::addData - Connect failed - finished");
        }
    }
}

void ODataSource::fetchData(const QString& requestURL, bool paging) {

    m_nPage = 0;
    m_bEndReached = false;
    m_oDataModel->clear();
    m_requestURL = requestURL;
    m_pagingEnabled = paging;

    QNetworkRequest request;

    QString pagedRequestURL = requestURL;

    m_JSONEnabled = pagedRequestURL.contains(QString("$format=json"));

    if (m_pagingEnabled) {
        // TODO: If there is no param in requestURL add ? instead of &
        pagedRequestURL = requestURL;
        pagedRequestURL.contains("?") ? pagedRequestURL.append("&") : pagedRequestURL.append("?");
        pagedRequestURL = pagedRequestURL.append(QString("$top=%1").arg(ITEMS_PER_PAGE));
    }
    else {
        m_bEndReached = true;
    }

    LOGGER::log("ODataSource::fetchData - pagedRequestURL:", pagedRequestURL);
    request.setUrl(pagedRequestURL);

    QNetworkReply* reply = m_netManager->get(request);

    if (reply) {
        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onODataReceived_Slot()));
        if (!res) {
            LOGGER::log("ODataSource::fetchData - Connect failed - finished");
        }
    }
}

void ODataSource::loadMoreItems() {
    if (m_bEndReached) {
        LOGGER::log("ODataSource::loadMoreItems - End reached ignore, count:",
                m_oDataModel->childCount(QVariantList()));
        return;
    }

    if (m_oDataModel->childCount(QVariantList()) > 0) {
        addLoadItem(m_oDataModel); // Display loading indicator if not the initial query
    }

    m_nPage++;
    QString requestURL = m_requestURL;
    if (m_pagingEnabled) {
        requestURL = m_requestURL + QString("&$top=%1&$skip=%2").arg(ITEMS_PER_PAGE).arg(ITEMS_PER_PAGE * m_nPage);
    }
    LOGGER::log("ODataSource::loadMoreItems - requestURL:", requestURL);

    QNetworkRequest request;
    request.setUrl(requestURL);
    QNetworkReply* reply = m_netManager->get(request);
    if (reply) {
        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onODataReceived_Slot()));
        if (!res) {
            LOGGER::log("ODataSource::loadMoreDrilldownList - Connect failed - finished");
        }
    }
}

DataModel* ODataSource::getDataModel() const {
    return m_oDataModel;
}

void ODataSource::onODataReceived_Slot() {
	onODataReceived();
}

// We could remove this method and keep onODataReceived_Slot... may be
void ODataSource::onfinishedUpdatingData_Slot() {

	// TO DO - should we review the response ???
	LOGGER::log("ODataSource::onfinishedUpdatingData_Slot called");

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    // To implement
    // ((cODataLib*)parent())->triggerDataEdited();

    // Memory management
    reply->deleteLater();
}

void ODataSource::onUpdatingDataError_Slot(QNetworkReply::NetworkError errNet) {
	 LOGGER::log("ODataSource::onUpdatingDataError_Slot - error", errNet);
}

void ODataSource::onODataReceived() {
    removeLoadItem(m_oDataModel); // Remove loading indicator

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int countAdded = onDataListReceived(reply, m_oDataModel);
    if (m_pagingEnabled) {
        m_bEndReached = countAdded <= 0;
    }

    // try to remove signals in this class and move to the generic class cODataLib...
    // emit oDataListLoaded();
    ((cODataLib*)parent())->triggerDataFetched();

    // Memory management
    reply->deleteLater();
}

int ODataSource::onDataListReceived(QNetworkReply* reply, ArrayDataModel* dataModel) {
    QNetworkReply::NetworkError errorCode = reply->error();
    if (errorCode != QNetworkReply::NoError) {

        if (errorCode == QNetworkReply::AuthenticationRequiredError) {
            LOGGER::log("ODataSource::onDataListReceived - Authentication Failed");

            SystemToast *toast = new SystemToast(this);
            toast->setBody(tr("Failed to get featured list\nAuthentication needed."));
            toast->show();
        }
        else if (errorCode == QNetworkReply::HostNotFoundError) {
            LOGGER::log("ODataSource::onDataListReceived - HostNotFound");
            SystemToast *toast = new SystemToast(this);
            toast->setBody(tr("Failed to get featured list.\nCheck your network connection"));
            toast->show();
        }
        else {
            LOGGER::log("ODataSource::onDataListReceived errorCode:", errorCode);

            SystemToast *toast = new SystemToast(this);
            toast->setBody(tr("Failed to get featured list."));
            toast->show();
        }

        return 0;
    }

    QByteArray result;
    result = reply->readAll();

    int countAdded = 0;
    if  (m_JSONEnabled)	{
    	countAdded = fillDataModelItems(result, dataModel);
    }
    else {
    	// Atom XML parsing cases...
    	countAdded = cAtomManager::Instance().fillDataModelItems(result, *dataModel);
    }

    return countAdded;
}

int ODataSource::fillDataModelItems(QByteArray result, ArrayDataModel* dataModel) {

	QVariantMap root;
	// load the JSON data
	JsonDataAccess jda;
	QVariant jsonContent = jda.loadFromBuffer(result);
	if (jda.hasError()) {
		DataAccessError error = jda.error();
		LOGGER::log("failed to load featured list JSON: ", error.errorType());
		LOGGER::log("failed to load featured list JSON: ", error.errorMessage());
		return 0;
	}
	// Retrieve list
	root = jsonContent.value<QVariantMap>();

	return parseItemList(root, dataModel);
}

int ODataSource::parseItemList(QVariantMap& root, ArrayDataModel* dataModel) {
    // TODO: Is this generic to all odata services?
    QVariantList list;

	QVariantMap rootVal = root["d"].value<QVariantMap>();
	if (rootVal.contains("EntitySets")) {
		QVariant resultVal = rootVal["EntitySets"];
		list = resultVal.value<QVariantList>();
	}
	else if (rootVal.contains("results")) {
		QVariant resultVal = rootVal["results"]; // TODO: This seems to be feed specific
		list = resultVal.value<QVariantList>();
	} else {
		list = root["d"].value<QVariantList>();
	}

    // add the data to the model
    dataModel->append(list);
    return list.size();
}

void ODataSource::addLoadItem(bb::cascades::ArrayDataModel* dataModel) {
    int index = dataModel->childCount(QVariantList()) - 1;
    QVariantMap lastItem = dataModel->value(index).toMap();

    if (!lastItem.contains("loadItem")) {
        QVariantMap newLastItem;
        newLastItem["loadItem"] = QVariant(true);
        dataModel->append(newLastItem);
    }
}

void ODataSource::removeLoadItem(bb::cascades::ArrayDataModel* dataModel) {
    int index = dataModel->childCount(QVariantList()) - 1;
    QVariantMap lastItem = dataModel->value(index).toMap();

    if (lastItem.contains("loadItem")) {
        dataModel->removeAt(index);
    }
}
