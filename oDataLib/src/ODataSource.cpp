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
#include "cProperty.h"

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
                m_JSONEnabled(false),
                m_MetaTestEnabled(false) {
    m_oDataModel = new ArrayDataModel(this);
    m_netManager = new QNetworkAccessManager();

}

ODataSource::~ODataSource() {
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

void ODataSource::postData(const QString& toURL, const QVariant& qvariantListProperties, const QString& sCollectionName, const QString& sNamespace, const QString& sEntityName) {
	QByteArray body;
	QNetworkRequest request;

	// TODO: should validate if we need to do this before sCollectionName: + "/" +
    QString toURLPost = toURL + sCollectionName;

    LOGGER::log("ODataSource::post - url: ", toURLPost);

    request.setUrl(toURLPost);

    cAtomManager::Instance().setHeaders(request, body);


    QVariantList list = qvariantListProperties.value<QVariantList>();
    QByteArray xmlPropBody = cAtomManager::Instance().buildXMLfromPropertyList(list, sCollectionName, sNamespace, sEntityName);

    if (xmlPropBody.size() == 0) {
         LOGGER::log("ODataSource::post - build XML body failed");
         return;
    }
    QByteArray postDataSize = QByteArray::number(xmlPropBody.size());
    request.setRawHeader(HttpRequestHeader_ContentLength, postDataSize);

    body.append(xmlPropBody);

    LOGGER::log("ODataSource::postData xml: ", QString(body));

    QNetworkReply* reply = m_netManager->post(request, body);
    if (reply) {
    	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onPostingDataError_Slot(QNetworkReply::NetworkError)));

        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onfinishedPostingData_Slot()));

        if (!res) {
            LOGGER::log("ODataSource::post - Connect failed");
        }
    }
}

void ODataSource::fetchMetadata(const QString& requestURL) {
	QString queryRequest(requestURL);
    QNetworkRequest request;
    request.setUrl(queryRequest);

    QNetworkReply* reply = m_netManager->get(request);

    if (reply) {
        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onODataMetadataReceived_Slot()));
        if (!res) {
            LOGGER::log("ODataSource::fetchMetadata - Connect failed - finished");
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

QByteArray ODataSource::getMetadata() const {
    return m_metadata;
}

void ODataSource::onODataReceived_Slot() {
	onODataReceived();
}

void ODataSource::onODataMetadataReceived_Slot() {
	onODataMetadataReceived();
}

// We could remove this method and keep onODataReceived_Slot with switch case and derive a class for json vs xml
void ODataSource::onfinishedPostingData_Slot() {

	// TODO - should we review the response ???
	LOGGER::log("ODataSource::onfinishedPostingData_Slot called");

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    // To implement
    // ((cODataLib*)parent())->triggerDataPosted();

    // Memory management
    reply->deleteLater();
}

void ODataSource::onPostingDataError_Slot(QNetworkReply::NetworkError errNet) {
	 LOGGER::log("ODataSource::onUpdatingDataError_Slot - error", errNet);
}

void ODataSource::onODataMetadataReceived() {

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray result;
	if (!readReply(reply, result)) {
		LOGGER::log("ODataSource::onODataMetadataReceived - reading metadata failed");
	}
	m_metadata = result;

    ((cODataLib*)parent())->triggerMetadataFetched();

    reply->deleteLater();
}

void ODataSource::onODataReceived() {
    removeLoadItem(m_oDataModel); // Remove loading indicator

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int countAdded = onDataListReceived(reply, m_oDataModel);
    if (m_pagingEnabled) {
        m_bEndReached = countAdded <= 0;
    }

    ((cODataLib*)parent())->triggerDataFetched();

    // Memory management
    reply->deleteLater();
}

bool ODataSource::readReply(QNetworkReply* reply, QByteArray& result) {
    QNetworkReply::NetworkError errorCode = reply->error();
    if (errorCode != QNetworkReply::NoError) {

        if (errorCode == QNetworkReply::AuthenticationRequiredError) {
            LOGGER::log("ODataSource::readReply - Authentication Failed");

            SystemToast *toast = new SystemToast(this);
            toast->setBody(tr("Failed to get featured list\nAuthentication needed."));
            toast->show();
        }
        else if (errorCode == QNetworkReply::HostNotFoundError) {
            LOGGER::log("ODataSource::readReply - HostNotFound");
            SystemToast *toast = new SystemToast(this);
            toast->setBody(tr("Failed to get featured list.\nCheck your network connection"));
            toast->show();
        }
        else {
            LOGGER::log("ODataSource::readReply errorCode:", errorCode);

            SystemToast *toast = new SystemToast(this);
            toast->setBody(tr("Failed to get featured list."));
            toast->show();
        }

        return false;
    }

    // QByteArray result;
    result = reply->readAll();

    return true;
}

int ODataSource::onDataListReceived(QNetworkReply* reply, ArrayDataModel* dataModel) {

    QByteArray result;
	if (!readReply(reply, result))
		return 0;

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

	// "d" refers to a prefix commonly used for namespacing in OData v3.
	// It is defined in the Atom_constants as OData_ns_dataservies_Prefix
	// See documentation
	// http://www.odata.org/documentation/atom-format
	// 2.3. Representing Collections of Entries
	QVariantMap rootVal = root[OData_ns_dataservices_Prefix].value<QVariantMap>();
	if (rootVal.contains("EntitySets")) {
		QVariant resultVal = rootVal["EntitySets"];
		list = resultVal.value<QVariantList>();
	}
	else if (rootVal.contains("results")) {
		QVariant resultVal = rootVal["results"]; // TODO: This seems to be feed specific
		list = resultVal.value<QVariantList>();
	} else {
		list = root[OData_ns_dataservices_Prefix].value<QVariantList>();
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
