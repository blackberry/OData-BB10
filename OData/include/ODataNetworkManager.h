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

#ifndef ODATANETWORKMANAGER_H_
#define ODATANETWORKMANAGER_H_

#include <qobject.h>
#include <qvariant.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <bb/data/DataAccessError>

#include "ODataService.h"

static const QByteArray TYPE_ATOM = "application/atom+xml";
static const QByteArray TYPE_JSON = "application/json";
static const QByteArray TYPE_XML = "application/xml";

class ODataNetworkManager : public QObject {
    Q_OBJECT

public:
    ODataNetworkManager(QObject* parent = 0);
    virtual ~ODataNetworkManager();

    void setFormat(ODataService::Format format);

    void read(const QString& url);
    void create(const QString& url, const QByteArray& dataModel);
    void update(const QString& url, const QByteArray& dataModel, const QString& type = "PATCH");
    void del(const QString& url); // wanted to call delete but it's a protected keyword

private:
    QNetworkAccessManager* mNetAccessManager;
    ODataService::Format mFormat;

    void handleError(QNetworkReply* reply);
    void handleErrorNoReply();
    void handleDataAccessError(const bb::data::DataAccessError& error);
    void handleResponseStatus(int code, const QString& url, const QString& message);

signals:
    void jsonReady(const QVariant& response);
    void atomReady(const QVariant& response);
    void xmlReady(const QVariant& response);

    void createJsonReady(const QVariant& response);
    void createAtomReady(const QVariant& response);
    void createXmlReady(const QVariant& response);

    void createSuccessful();
    void updateSuccessful();
    void deleteSuccessful();

    void responseStatus(int code, const QString& url, const QString& message);
public slots:
    void onReadReply();
    void onCreateReply();
    void onUpdateReply();
    void onDeleteReply();
};

#endif /* ODATANETWORKMANAGER_H_ */
