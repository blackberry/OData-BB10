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

// TODO: Support read of ATOM sources as: http://services.odata.org/OData/OData.svc/
// For JSON: http://services.odata.org/OData/OData.svc/?$format=json
// TODO: Move to a odata library
// TODO: Make the class generic so any odata service may be read
// TODO: Implement infinite scrolling using $skip and $top
// TODO: Implement sorting using $orderby desc/asc
// TODO: Implement filters
// TODO: Should $format=json be a parameter?
ODataSource::ODataSource(QObject *parent) :
                QObject(parent) {
    m_oDataModel = new ArrayDataModel(this);
    m_netManager = new QNetworkAccessManager();
}

ODataSource::~ODataSource() {
}

void ODataSource::fetchData(const QString& requestURL) {
    QNetworkRequest request;
    request.setUrl(requestURL);
    QNetworkReply* reply = m_netManager->get(request);

    if (reply) {
        bool res = connect(reply, SIGNAL(finished()), this, SLOT(onODataReveived()));
        if (!res) {
            LOGGER::log("ODataSource::fetchData - Connect failed - finished");
        }
    }
}

DataModel* ODataSource::oDataModel() const {
    return m_oDataModel;
}

void ODataSource::onODataReveived() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    onDataListReceived(reply, m_oDataModel);
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
