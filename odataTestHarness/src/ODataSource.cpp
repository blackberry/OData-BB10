/*
 * ODataSource.cpp
 *
 *  Created on: Mar 4, 2013
 *      Author: martin
 *
 *
 *  Uses data sources from:
 *      http://developer.netflix.com/docs/oData_Catalog
 */

#include "ODataSource.h"
#include <bb/data/JsonDataAccess>
#include <bb/system/SystemToast>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include "LOGGER.h"

using namespace bb::cascades;
using namespace bb::data;
using bb::system::SystemToast;

const int ITEMS_PER_PAGE = 12;  // MUST be more than what fits in a page else onAtEndChanged is not triggered

// TODO: Support read of ATOM sources as: http://services.odata.org/OData/OData.svc/
// For JSON: http://services.odata.org/OData/OData.svc/?$format=json
// TODO: Move to a odata library
// TODO: Make the class generic so any odata service may be read
// TODO: Implement sorting using $orderby desc/asc
// TODO: Implement filters
// TODO: Should $format=json be a parameter?
ODataSource::ODataSource(QObject *parent) :
                QObject(parent),
                m_nPage(0),
                m_bEndReached(false) {
    m_oDataModel = new ArrayDataModel(this);
    m_netManager = new QNetworkAccessManager();
}

ODataSource::~ODataSource() {
}

void ODataSource::fetchData(const QString& requestURL) {

    m_nPage = 0;
    m_bEndReached = false;
    m_oDataModel->clear();
    m_requestURL = requestURL;

    QNetworkRequest request;

    // TODO: If there is no param in requestURL add ? instead of &
    QString pagedRequestURL = requestURL + QString("&$top=%1").arg(ITEMS_PER_PAGE);
    LOGGER::log("ODataSource::fetchData - pagedRequestURL:", pagedRequestURL);
    request.setUrl(pagedRequestURL);

    QNetworkReply* reply = m_netManager->get(request);

    if (reply) {
        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onODataReveived()));
        if (!res) {
            LOGGER::log("ODataSource::fetchData - Connect failed - finished");
        }
    }
}

void ODataSource::loadMoreItems() {
    if (m_bEndReached) {
        LOGGER::log("ODataSource::loadMoreItems - End reached ignore, count:", m_oDataModel->childCount(QVariantList()));
        return;
    }

    if (m_oDataModel->childCount(QVariantList()) > 0) {
        addLoadItem(m_oDataModel); // Display loading indicator if not the initial query
    }

    m_nPage++;
    QString requestURL = m_requestURL + QString("&$top=%1&$skip=%2").arg(ITEMS_PER_PAGE).arg(ITEMS_PER_PAGE*m_nPage);
    LOGGER::log("ODataSource::loadMoreItems - requestURL:", requestURL);

    QNetworkRequest request;
    request.setUrl(requestURL);
    QNetworkReply* reply = m_netManager->get(request);
    if (reply) {
        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onODataReveived()));
        if (!res) {
            LOGGER::log("ODataSource::loadMoreDrilldownList - Connect failed - finished");
        }
    }
}

DataModel* ODataSource::oDataModel() const {
    return m_oDataModel;
}

void ODataSource::onODataReveived() {
    removeLoadItem(m_oDataModel); // Remove loading indicator

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int countAdded = onDataListReceived(reply, m_oDataModel);
    m_bEndReached = countAdded <= 0;
    emit oDataListLoaded();

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

    int countAdded = fillDataModelItems(result, dataModel);
    return countAdded;
}

int ODataSource::fillDataModelItems(QByteArray result, ArrayDataModel* dataModel) {
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
    QString message;
    QVariantMap root = jsonContent.value<QVariantMap>();

    return parseItemList(root, dataModel);
}

int ODataSource::parseItemList(QVariantMap& root, ArrayDataModel* dataModel) {
    // TODO: Is this generic to all odata services?
    QVariantMap rootVal = root["d"].value<QVariantMap>();
    QVariant resultVal = rootVal["results"]; // TODO: This seems to be feed specific
    QVariantList list = resultVal.value<QVariantList>();

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