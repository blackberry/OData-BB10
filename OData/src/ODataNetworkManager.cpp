/* Copyright (c) 2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ODataNetworkManager.h"
#include <qnetworkrequest.h>
#include <bb/data/JsonDataAccess>
#include <bb/data/XmlDataAccess>
#include <bb/cascades/QmlDocument>
#include <qbuffer.h>
#include <QDebug>
#include <bb/data/DataAccessErrorType>
#include "ODataService.h"

/*
 * CONSTRUCTORS / DESTRUCTORS
 */

ODataNetworkManager::ODataNetworkManager(QObject* parent)
: QObject(parent), mNetAccessManager(NULL), mFormat(ODataService::JSON)
{
    mNetAccessManager = bb::cascades::QmlDocument::defaultDeclarativeEngine()->networkAccessManager();
    //new QNetworkAccessManager(this);
}

ODataNetworkManager::~ODataNetworkManager() {
    //delete mNetAccessManager;
}

/*
 * PUBLIC FUNCTIONS
 */

void ODataNetworkManager::read(const QString& url) {
    QUrl qurl(url);

    QNetworkRequest req(qurl);

    if(mFormat == ODataService::ATOM) {
        req.setRawHeader("Content-Type", "application/atom+xml");
    }

    if(mFormat == ODataService::JSON) {
        req.setRawHeader("Content-Type", "application/json;odata=minimalmetadata");
    }

    if(mFormat == ODataService::XML) {
        req.setRawHeader("Accept", "application/xml");
    }

    QNetworkReply* reply = mNetAccessManager->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(onReadReply()));
}

void ODataNetworkManager::create(const QString& url, const QByteArray& dataModel) {
    QUrl qurl;

    qurl.setEncodedUrl(url.toAscii());
    QNetworkRequest req(qurl);

    if(mFormat == ODataService::ATOM) {
        req.setRawHeader("Content-Type", "application/xml");
    }

    if(mFormat == ODataService::JSON) {
        req.setRawHeader("Content-Type", "application/json;odata=minimalmetadata");
    }

    QNetworkReply* reply = mNetAccessManager->post(req, dataModel);
    connect(reply, SIGNAL(finished()), this, SLOT(onCreateReply()));
}

void ODataNetworkManager::update(const QString& url, const QByteArray& dataModel, const QString& type) {
    QUrl qurl;

    qurl.setEncodedUrl(url.toAscii());
    QNetworkRequest req(qurl);

    if(mFormat == ODataService::ATOM) {
        req.setRawHeader("Content-Type", "application/xml");
    }

    if(mFormat == ODataService::JSON) {
        req.setRawHeader("Content-Type", "application/json;odata=minimalmetadata");
    }

    QBuffer* data = new QBuffer();
    data->setData(dataModel);

    QNetworkReply* reply = mNetAccessManager->sendCustomRequest(req, type.toAscii(), data);
    connect(reply, SIGNAL(finished()), this, SLOT(onUpdateReply()));

}

void ODataNetworkManager::del(const QString& url) {
    QUrl qurl(url);

    QNetworkRequest req(qurl);
    QNetworkReply* reply = mNetAccessManager->deleteResource(req);
    connect(reply, SIGNAL(finished()), this, SLOT(onDeleteReply()));
}

/*
 * SLOTS
 */

void ODataNetworkManager::onReadReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());


    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response;

            response = reply->readAll();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QString url = reply->url().toString();
            bb::data::DataAccessError error;

            QByteArray contentType = reply->rawHeader("Content-Type");
            // determine what the response is encoded in
            if (contentType.contains(TYPE_ATOM)) {
                bb::data::XmlDataAccess xda;
                error = xda.error();
                emit atomReady(xda.loadFromBuffer(response));
            }

            if (contentType.contains(TYPE_JSON)) {
                bb::data::JsonDataAccess jda;
                QVariant data = jda.loadFromBuffer(response);
                error = jda.error();
                emit jsonReady(data);
            }

            if (contentType.contains(TYPE_XML)) {
                bb::data::XmlDataAccess xda;
                error = xda.error();
                emit xmlReady(xda.loadFromBuffer(response));
            }

            handleDataAccessError(error);
            handleResponseStatus(statusCode, url, response);
        }
        else {
            handleError(reply);
        }
    }
    else {
        handleErrorNoReply();
    }

    reply->deleteLater();
}

void ODataNetworkManager::onCreateReply(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());


    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // further object parsing if they want the data out
            QByteArray response;

            response = reply->readAll();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QString url = reply->url().toString();
            bb::data::DataAccessError error;

            if(statusCode == 201)
                emit createSuccessful();

            QByteArray contentType = reply->rawHeader("Content-Type");
            qDebug() << "LOADING CONTENT" << contentType;

            // determine what the response is encoded in
            if (contentType.contains(TYPE_ATOM)) {
                bb::data::XmlDataAccess xda;
                error = xda.error();
                emit createAtomReady(xda.loadFromBuffer(response));
            }
            if (contentType.contains(TYPE_JSON)) {
                bb::data::JsonDataAccess jda;
                error = jda.error();
                emit createJsonReady(jda.loadFromBuffer(response));
            }
            if (contentType.contains(TYPE_XML)) {
                bb::data::XmlDataAccess xda;
                error = xda.error();
                emit createXmlReady(xda.loadFromBuffer(response));
            }

            handleDataAccessError(error);
            handleResponseStatus(statusCode, url, response);
        }
        else {
            handleError(reply);
        }
    }
    else {
        handleErrorNoReply();
    }

    reply->deleteLater();
}

void ODataNetworkManager::onUpdateReply(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());


    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {

            QByteArray response = reply->readAll();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QString url = reply->url().toString();
            handleResponseStatus(statusCode, url, response);

            emit updateSuccessful();
        }
        else {
            handleError(reply);
        }
    }
    else {
        handleErrorNoReply();
    }

    reply->deleteLater();
}

void ODataNetworkManager::onDeleteReply(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());


    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            emit deleteSuccessful();
        }
        else {
            handleError(reply);
        }
    }
    else {
        handleErrorNoReply();
    }

    reply->deleteLater();
}

void ODataNetworkManager::handleResponseStatus(int code, const QString& url, const QString& message) {

    QString status;

    switch(code) {
        case 200:
            status = "OK";
            break;
        case 201:
            status = "Created";
            break;
        case 202:
            status = "Accepted";
            break;
        case 204:
            status = "No Content";
            break;
        case 304:
            status = "Not Modified";
            break;
        case 404:
            status = "Not Found";
            break;
        case 405:
            status = "Method Not Allowed";
            break;
        case 410:
            status = "Resource is no longer available";
            break;
        case 501:
            status = "Not Implemented";
            break;
        default:
            status = "Unknown Error";
    }

    qDebug() << "Status : " << code << " " << status;
    qDebug() << "Url : " << url;
    qDebug() << "Response Message : " << message;

    emit responseStatus(code, url, message);
}

void ODataNetworkManager::handleError(QNetworkReply* reply){
    emit responseStatus(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), reply->url().toString(), reply->errorString());

    // for debugging errors just uncomment
    // LOG the error to the console
    qDebug("Error: Code %d - %s\n", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), reply->errorString().toUtf8().constData());

    QByteArray response;
    response = reply->readAll();
    qDebug("Response: %s\n", response.constData());

}

void ODataNetworkManager::handleErrorNoReply(){
    emit responseStatus(0, "", "No Reply");
}

void ODataNetworkManager::handleDataAccessError(const bb::data::DataAccessError& error) {
    // Determine type of error
    if (error.errorType() == bb::data::DataAccessErrorType::SourceNotFound)
    {
        qDebug() << "DataAccessError - Error loading: " << error.errorMessage();
    }
    else if (error.errorType() == bb::data::DataAccessErrorType::ConnectionFailure)
    {
        qDebug() << "DataAccessError - Connection failure: " << error.errorMessage();
    }
    else if (error.errorType() == bb::data::DataAccessErrorType::OperationFailure)
    {
        qDebug() << "DataAccessError - Operation failure: " << error.errorMessage();
    }
}


void ODataNetworkManager::setFormat(ODataService::Format format)
{
    if(format != mFormat) {
        mFormat = format;
    }
}
