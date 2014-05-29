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

#ifndef ODATALISTMODEL_H_
#define ODATALISTMODEL_H_

#include <qobject.h>
#include <qvariant.h>
#include <bb/cascades/DataModel>

#include "ODataService.h"

class ODataNetworkManager;

class ODataListModel : public bb::cascades::DataModel {
    Q_OBJECT

    Q_PROPERTY(int pageSize READ getPageSize WRITE setPageSize NOTIFY pageSizeChanged)
    Q_PROPERTY( ODataService* service READ service WRITE setService )

public:
    ODataListModel(QObject* parent = 0);
    virtual ~ODataListModel();

    int getPageSize();
    void setPageSize(int newSize);

    ODataService* service() const;
    void setService(ODataService* service);

    Q_INVOKABLE QVariant data(const QVariantList& indexPath);
    Q_INVOKABLE int childCount(const QVariantList& indexPath);
    Q_INVOKABLE bool hasChildren(const QVariantList& indexPath);
    Q_INVOKABLE void readModel(const QString& resourcePath);
    Q_INVOKABLE void filterModel(const QString& resourcePath, const QVariantMap& filters);

signals:
    void sourceChanged();
    void pageSizeChanged();
    void modelError(int code, const QString& message);

public slots:
    void jsonCompleted(const QVariant& response);
    void atomCompleted(const QVariant& response);

    void responseStatus(int code, const QString& url, const QString& message);

private:
    ODataNetworkManager* mNetworkManager;
    QVariantList mDataList;
    QString mSource;
    int mPageSize;
    ODataService* mService;
};

#endif /* ODATALISTMODEL_H_ */
