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

#include "ODataListModel.h"
#include "ODataNetworkManager.h"
#include <bb/cascades/DataModelChangeType>

/*
 * CONSTRUCTORS / DESCTRUCTORS
 */

ODataListModel::ODataListModel(QObject* parent)
        : DataModel(parent),
          mPageSize(0),
          mService(NULL) {
    mNetworkManager = new ODataNetworkManager();
    mNetworkManager->setFormat(ODataService::JSON);

    connect(mNetworkManager, SIGNAL(jsonReady(const QVariant&)), this, SLOT(jsonCompleted(const QVariant&)));
    connect(mNetworkManager, SIGNAL(atomReady(const QVariant&)), this, SLOT(atomCompleted(const QVariant&)));
    connect(mNetworkManager, SIGNAL(responseStatus(int, const QString&, const QString&)), this,
            SLOT(responseStatus(int, const QString&, const QString&)));
}

ODataListModel::~ODataListModel() {
}

/*
 * PROPERTIES
 */
int ODataListModel::getPageSize() {
    return mPageSize;
}
void ODataListModel::setPageSize(int newSize) {
    mPageSize = newSize;
    emit pageSizeChanged();
}

/*
 * FUNCTIONS
 */

QVariant ODataListModel::data(const QVariantList& indexPath) {
    QVariantMap item = mDataList[indexPath[0].toInt()].toMap();

    return item;
}

int ODataListModel::childCount(const QVariantList& indexPath) {
    Q_UNUSED(indexPath);

    // we are a single level data source so we can just return the amount of items we own
    return mDataList.count();
}

bool ODataListModel::hasChildren(const QVariantList& indexPath) {
    // we aren't multilevel so we are only concerned with the root having children
    return indexPath.count() == 0 && !mDataList.isEmpty();
}

void ODataListModel::readModel(const QString& resourcePath) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QStringList urlTokens;
    urlTokens << mService->getSource() << resourcePath;

    QString url = urlTokens.join("/");

    mDataList.clear();
    emit itemsChanged(bb::cascades::DataModelChangeType::Init);

    if (mNetworkManager)
        mNetworkManager->read(url);
}

void ODataListModel::filterModel(const QString& resourcePath, const QVariantMap& filters) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QUrl url = mService->getSource() + "/" + resourcePath;

    for (QVariantMap::const_iterator i = filters.constBegin(); i != filters.constEnd(); i++) {
        if (i.key() == "filter")
            url.addQueryItem("$filter", i.value().toString());
        else if (i.key() == "orderby")
            url.addQueryItem("$orderby", i.value().toString());
        else if (i.key() == "count")
            url.addQueryItem("$count", i.value().toString());
        else if (i.key() == "skip")
            url.addQueryItem("$skip", QString::number(i.value().toInt()));
        else if (i.key() == "top")
            url.addQueryItem("$top", QString::number(i.value().toInt()));
        else if (i.key() == "search")
            url.addQueryItem("$search", i.value().toString());
    }

    mDataList.clear();
    emit itemsChanged(bb::cascades::DataModelChangeType::Init);

    if (mNetworkManager)
        mNetworkManager->read(url.toString());
}
/*
 * SLOTS
 */

void ODataListModel::jsonCompleted(const QVariant& response) {
    mDataList = response.toMap()["value"].toList();

// handle the case where there was only 1 item in the list
    if (mDataList.count() == 0) {
        mDataList.append(response.toMap()["value"]);
    }

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
}

void ODataListModel::atomCompleted(const QVariant& response) {
    mDataList = response.toMap()["entry"].toList();

// handle the case where there was only 1 item in the list
    if (mDataList.count() == 0) {
        mDataList.append(response.toMap()["entry"]);
    }

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
}

void ODataListModel::responseStatus(int code, const QString& url, const QString& message) {
    if (code >= 300)
        emit modelError(code, message);
}

ODataService* ODataListModel::service() const {
    return mService;
}

void ODataListModel::setService(ODataService* service) {
    if (service != mService) {
        mService = service;
        mNetworkManager->setFormat(mService->getFormat());
    }
}
