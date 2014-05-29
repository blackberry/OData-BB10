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

#ifndef ODATAOBJECTMODEL_H_
#define ODATAOBJECTMODEL_H_

#include "ODataService.h"

#include <qobject.h>
#include <qvariant.h>
#include <QStringList>

class ODataNetworkManager;

class ODataObjectModel : public QObject {
    Q_OBJECT

    Q_PROPERTY( QVariant model READ getModel WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( ODataService* service READ service WRITE setService )

public:
    ODataObjectModel(QObject* parent = 0);
    virtual ~ODataObjectModel();

    QVariant getModel();
    void setModel(const QVariant& newModel);

    ODataService* service() const;
    void setService(ODataService* service);

    Q_INVOKABLE QVariantList getModelStructure(const QString& modelKey, const QVariant& metadata);

    Q_INVOKABLE void readModel(const QString& resourcePath);
    Q_INVOKABLE void createModel(QString resourcePath, QString category, QVariant content, QByteArray links);
    Q_INVOKABLE void updateModel(QString resourcePath, QString category, QVariant content, QByteArray links);
    Q_INVOKABLE void filterModel(const QString& resourcePath, const QVariantMap& filters);

    Q_INVOKABLE void createLink(const QString& from, const QString& fromID,
                                const QString& to, const QString& toID, const QString& toPluralName = "");
    Q_INVOKABLE void updateLink(const QString& from, const QString& fromID,
                                const QString& to, const QString& toID,  const QString& toNamePlural);
    Q_INVOKABLE void deleteLink(const QString& from, const QString& fromID,
                                const QString& to, const QString& toID = "");

    Q_INVOKABLE void deleteModel(const QString& resourcePath);

signals:
    void modelReady();
    void sourceChanged();
    void modelChanged();

    void modelDeleted();
    void modelCreated();
    void modelUpdated();

    void modelError(int code, const QString& message);
public slots:
    void jsonReadComplete(const QVariant& response);
    void atomReadComplete(const QVariant& response);

    void deleteComplete();
    void createComplete();
    void updateComplete();

    void responseStatus(int code, const QString& url, const QString& message);
private:
    QVariant mModel;
    ODataNetworkManager* mNetworkManager;
    ODataService* mService;
    QStringList mRequestUrls;

    QVariantList mStructure; // the data structure of the object (used for creates and updates)

    QByteArray parseContentAtom(const QVariant& content);
    QByteArray buildJsonData(const QString& category, const QVariant& content, const QByteArray& links);
    QByteArray buildAtomData(const QString& category, const QVariant& content, const QByteArray& links);
    QString createOpenTag(QString tag);
    QString createCloseTag(QString tag);
    QString createNullTag(QString tag);
    void createLinkJSON(const QString& from, const QString& fromID,
                        const QString& to, const QString& toID, const QString& toPluralName = "");
    void updateLinkJSON(const QString& from, const QString& fromID,
                        const QString& to, const QString& toID,  const QString& toNamePlural);
    void createLinkATOM(const QString& from, const QString& fromID,
                        const QString& to, const QString& toID, const QString& toPluralName = "");
    void updateLinkATOM(const QString& from, const QString& fromID,
                        const QString& to, const QString& toID,  const QString& toNamePlural);
};

#endif /* ODATAOBJECTMODEL_H_ */
