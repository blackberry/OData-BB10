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

#include "ODataObjectModel.h"
#include "ODataConstants.h"

#include "ODataNetworkManager.h"

#include "stdio.h"
#include <bb/data/JsonDataAccess>
#include <QStringList>

using namespace OData;
using namespace bb::data;

/*
 * CONSTRUCTORS / DESCTRUCTORS
 */

ODataObjectModel::ODataObjectModel(QObject* parent)
        : QObject(parent),
          mService(NULL) {
    mNetworkManager = new ODataNetworkManager(this);
    mNetworkManager->setFormat(ODataService::JSON);

    connect(mNetworkManager, SIGNAL(jsonReady(const QVariant&)), this, SLOT(jsonReadComplete(const QVariant&)));
    connect(mNetworkManager, SIGNAL(atomReady(const QVariant&)), this, SLOT(atomReadComplete(const QVariant&)));
    connect(mNetworkManager, SIGNAL(responseStatus(int, const QString&, const QString&)), this,
            SLOT(responseStatus(int, const QString&, const QString&)));
    connect(mNetworkManager, SIGNAL(deleteSuccessful()), this, SLOT(deleteComplete()));
    connect(mNetworkManager, SIGNAL(createSuccessful()), this, SLOT(createComplete()));
    connect(mNetworkManager, SIGNAL(updateSuccessful()), this, SLOT(updateComplete()));
}

ODataObjectModel::~ODataObjectModel() {
}

/*
 * PROPERTIES
 */

QVariant ODataObjectModel::getModel() {
    return mModel;
}

void ODataObjectModel::setModel(const QVariant& newModel) {
    mModel = newModel;

    emit modelChanged();
}

/*
 * FUNCTIONS
 */

void ODataObjectModel::deleteModel(const QString& resourcePath) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QStringList urlTokens;
    urlTokens << mService->getSource() << resourcePath;
    QString url = urlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->del(url);
        mRequestUrls.push_back(url);
    }
}

QVariantList ODataObjectModel::getModelStructure(const QString& modelKey, const QVariant& metadata) {
    mStructure = QVariantList();

    QVariantMap metaEntity;

    // Grab the entities from the Meta Data
    QVariantList entities = metadata.toMap()[ENTITY_TYPE].toList();

    // toList won't list things that only occur once so we handle this case here
    if (entities.count() == 0) {
        entities.append(metadata.toMap()[ENTITY_TYPE]);
    }

    // Get the complex data types (if any)
    QVariantList complexTypes = metadata.toMap()[COMPLEX_TYPE].toList();

    if (complexTypes.count() == 0) {
        complexTypes.append(metadata.toMap()[COMPLEX_TYPE]);
    }

    // find the correct entity
    for (int i = 0; i < entities.count(); i++) {
        if (entities[i].toMap()[NAME].toString().compare(modelKey) == 0) {
            metaEntity = entities[i].toMap();
            break;
        }
    }

    // parse out the correct properties and add them to the mStructure Variant
    if (metaEntity.count() >= 1) {
        /*
         * Handle the fields.
         */
        entities = metaEntity[PROPERTY].toList();

        if (entities.count() == 0) {
            entities.append(metaEntity[PROPERTY]);
        }

        for (int i = 0; i < entities.count(); i++) {
            QVariantMap entityData;

            entityData[NAME] = entities[i].toMap()[NAME];
            entityData[TYPE] = entities[i].toMap()[TYPE];
            entityData[NULLABLE] = entities[i].toMap()[NULLABLE];
            entityData[DATA] = "";

            mStructure.append(entityData);

            // handle complex data types, ie not Edm.<type>
            if (entityData[TYPE].toString().indexOf(EDM) != 0) {
                QVariantMap complexType;

                for (int j = 0; j < complexTypes.count(); j++) {
                    if (entityData[TYPE].toString().contains(complexTypes[j].toMap()[NAME].toString())) {
                        complexType = complexTypes[j].toMap();
                        break;
                    }
                }

                if (!complexType.isEmpty()) {
                    QVariantList typeFields = complexType[PROPERTY].toList();

                    if (typeFields.count() == 0) {
                        typeFields.append(complexType[PROPERTY]);
                    }

                    for (int j = 0; j < typeFields.count(); j++) {
                        entityData[NAME] = typeFields[j].toMap()[NAME];
                        entityData[TYPE] = typeFields[j].toMap()[TYPE];
                        entityData[NULLABLE] = typeFields[j].toMap()[NULLABLE];
                        entityData[DATA] = "";

                        mStructure.append(entityData);
                    }

                    // add an end tag so we know we got to the end
                    entityData[NAME] = entities[i].toMap()[NAME];
                    entityData[TYPE] = END_COMPLEX_TYPE;
                    entityData[NULLABLE] = false;
                    entityData[DATA] = "";

                    mStructure.append(entityData);
                }
            }
        }

        /*
         * Handle the links to other items
         */
        entities = metaEntity[NAVIGATION_PROPERTY].toList();

        if (entities.count() == 0) {
            entities.append(metaEntity[NAVIGATION_PROPERTY]);
        }

        for (int i = 0; i < entities.count(); i++) {
            QVariantMap entityData;

            entityData[NAME] = entities[i].toMap()[NAME];
            entityData[TYPE] = NAVIGATION_PROPERTY;
            entityData[DATA] = "";

            mStructure.append(entityData);
        }
    }

    return mStructure;
}

void ODataObjectModel::readModel(const QString& resourcePath) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    mModel.clear();

    QStringList urlTokens;
    urlTokens << mService->getSource() << resourcePath;
    QString url = urlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->read(url);
        mRequestUrls.push_back(url);
    }
}

void ODataObjectModel::createModel(QString resourcePath, QString category, QVariant content, QByteArray links) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QByteArray model;

    ODataService::Format format = mService->getFormat();

    if (format == ODataService::JSON) {
        model = buildJsonData(category, content, links);
    }
    if (format == ODataService::ATOM) {
        model = buildAtomData(category, content, links);
    }

    QStringList urlTokens;
    urlTokens << mService->getSource() << resourcePath;
    QString url = urlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->create(url, model);
        mRequestUrls.push_back(url);
    }
}

void ODataObjectModel::updateModel(QString resourcePath, QString category, QVariant content, QByteArray links) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QByteArray model;

    ODataService::Format format = mService->getFormat();

    format = mService->getFormat();

    if (format == ODataService::JSON) {
        model = buildJsonData(category, content, links);
    }
    if (format == ODataService::ATOM) {
        model = buildAtomData(category, content, links);
    }

    QStringList urlTokens;
    urlTokens << mService->getSource() << resourcePath;
    QString url = urlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->update(url, model);
        mRequestUrls.push_back(url);
    }
}

void ODataObjectModel::filterModel(const QString& resourcePath, const QVariantMap& filters) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    mModel.clear();

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

    if (mNetworkManager) {
        mNetworkManager->read(url.toString());
        mRequestUrls.push_back(url.toString());
    }
}

QByteArray ODataObjectModel::parseContentAtom(const QVariant& content) {
    QByteArray parsedContent;

    QVariantList contentList = content.toList();

    for (int i = 0; i < content.toList().count(); i++) {
        QVariantMap dataMap = contentList[i].toMap();

        // now we ignore the types of links and preprocess everything else
        if (!dataMap[TYPE].toString().contains(NAVIGATION_PROPERTY)) {
            if (dataMap[TYPE].toString().indexOf(EDM) != 0) {
                if (dataMap[TYPE].toString().compare(END_COMPLEX_TYPE) == 0) { // close tag
                    parsedContent.append(createCloseTag(dataMap[NAME].toString()));
                }
                else { // open tag
                    parsedContent.append(
                            createOpenTag(
                                    dataMap[NAME].toString().append(QString("m:type='")).append(
                                            dataMap[TYPE].toString()).append(QString("'"))));
                }
            }
            else {
                if (dataMap[DATA].toString().isEmpty()) {
                    parsedContent.append(createNullTag(dataMap[NAME].toString()));
                }
                else {
                    parsedContent.append(createOpenTag(dataMap[NAME].toString()));
                    parsedContent.append(dataMap[DATA].toString());
                    parsedContent.append(createCloseTag(dataMap[NAME].toString()));
                }
            }
        }
    }

    return parsedContent;
}

QByteArray ODataObjectModel::buildJsonData(const QString& category, const QVariant& content, const QByteArray& links) {
    QVariantMap data;
    QVariantList contentList = content.toList();
    QVariant item;
    QVariantMap itemMap;

    Q_FOREACH(item, contentList)
    {
        itemMap = item.toMap();
        if (!itemMap[TYPE].toString().contains(NAVIGATION_PROPERTY)) {
            if (itemMap[DATA].toString().isEmpty())
                data[itemMap[NAME].toString()] = QVariant();
            else
                data[itemMap[NAME].toString()] = itemMap[DATA];
        }
    }

    data[ODATA_TYPE] = category;

    JsonDataAccess jda;
    QByteArray model;
    jda.saveToBuffer(data, &model);
    model.insert(model.size() - 1, links);

    return model;
}

QByteArray ODataObjectModel::buildAtomData(const QString& category, const QVariant& content, const QByteArray& links) {
    QByteArray model;

    model.append(XML_TAG);
    model.append(ENTRY_OPEN);

    model.append(CATEGORY_OPEN);
    model.append(category);
    model.append(CATEGORY_CLOSE);

    model.append(links);

    model.append(CONTENT_OPEN);
    model.append(parseContentAtom(content));
    model.append(CONTENT_CLOSE);

    model.append(ENTRY_CLOSE);

    return model;
}

QString ODataObjectModel::createOpenTag(QString tag) {
    return tag.prepend("<d:").append(">");
}

QString ODataObjectModel::createCloseTag(QString tag) {
    return tag.prepend("</d:").append(">");
}

QString ODataObjectModel::createNullTag(QString tag) {
    return tag.prepend("<d:").append(" m:null='true' />");
}

/*
 * SLOTS
 */

void ODataObjectModel::jsonReadComplete(const QVariant& response) {
    mModel = response.toMap();

    emit modelReady();
}

void ODataObjectModel::atomReadComplete(const QVariant& response) {
    mModel = response;

    emit modelReady();
}

void ODataObjectModel::deleteComplete() {
    emit modelDeleted();
}

void ODataObjectModel::createComplete() {
    emit modelCreated();
}

void ODataObjectModel::updateComplete() {
    emit modelUpdated();
}

void ODataObjectModel::responseStatus(int code, const QString& url, const QString& message) {
    if (mRequestUrls.contains(url, Qt::CaseInsensitive)) {
        if (code == 204) {
            mModel.clear();
            emit modelReady();
        }

        if (code >= 300)
            emit modelError(code, message);
        mRequestUrls.removeAll(url);
    }
}

void ODataObjectModel::createLink(const QString& from, const QString& fromID, const QString& to, const QString& toID,
        const QString& toPluralName) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    ODataService::Format format = mService->getFormat();

    if (format == ODataService::JSON) {
        createLinkJSON(from, fromID, to, toID, toPluralName);
    }

    if (format == ODataService::ATOM) {
        createLinkATOM(from, fromID, to, toID, toPluralName);
    }
}

void ODataObjectModel::updateLink(const QString& from, const QString& fromID, const QString& to, const QString& toID,
        const QString& toNamePlural) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    ODataService::Format format = mService->getFormat();

    if (format == ODataService::JSON) {
        updateLinkJSON(from, fromID, to, toID, toNamePlural);
    }

    if (format == ODataService::ATOM) {
        updateLinkATOM(from, fromID, to, toID, toNamePlural);
    }
}

void ODataObjectModel::deleteLink(const QString& from, const QString& fromID, const QString& to, const QString& toID) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QString fromEntity = from + "(" + fromID + ")";
    QString toEntitiy = toID.isEmpty() ? to : to + "(" + toID + ")";
    QStringList urlTokens;
    QString baseUrl = mService->getSource();
    urlTokens << baseUrl << fromEntity << to;

    if (!toID.isEmpty())
        urlTokens << QString(REF) + QString("?") + QString(ID) + QString("=") + toEntitiy;
    else
        urlTokens << REF;

    QString url = urlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->del(url);
        mRequestUrls.push_back(url);
    }
}

void ODataObjectModel::createLinkJSON(const QString& from, const QString& fromID, const QString& to,
        const QString& toID, const QString& toPluralName) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    JsonDataAccess jda;
    QByteArray model;

    QString fromEntity = from + "(" + fromID + ")";
    QStringList requestUrlTokens;
    QString baseUrl = mService->getSource();
    requestUrlTokens << baseUrl << fromEntity << to << REF;

    QString toEntity = toPluralName.isEmpty() ? to + "(" + toID + ")" : toPluralName + "(" + toID + ")";
    QStringList relationUrlTokens;
    relationUrlTokens << baseUrl << toEntity;

    QVariantMap data;
    data[ODATA_ID] = relationUrlTokens.join("/");
    jda.saveToBuffer(data, &model);
    QString url = requestUrlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->create(url, model);
        mRequestUrls.push_back(url);
    }

}

void ODataObjectModel::updateLinkJSON(const QString& from, const QString& fromID, const QString& to,
        const QString& toID, const QString& toNamePlural) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QString fromEntity = from + "(" + fromID + ")";
    QString toEntity = toNamePlural + "(" + toID + ")";
    QString baseUrl = mService->getSource();

    QStringList urlTokens;
    urlTokens << baseUrl << fromEntity << to << REF;
    QString url = urlTokens.join("/");

    QStringList relationUrlTokens;
    relationUrlTokens << baseUrl << toEntity;

    QVariantMap data;
    QByteArray body;
    JsonDataAccess jda;
    data[ODATA_ID] = relationUrlTokens.join("/");
    jda.saveToBuffer(data, &body);

    if (mNetworkManager) {
        mNetworkManager->update(url, body, "PUT");
        mRequestUrls.push_back(url);
    }
}

void ODataObjectModel::createLinkATOM(const QString& from, const QString& fromID, const QString& to,
        const QString& toID, const QString& toPluralName) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QByteArray body;

    QString fromEntity = from + "(" + fromID + ")";
    QStringList requestUrlTokens;
    QString baseUrl = mService->getSource();
    requestUrlTokens << baseUrl << fromEntity << to << REF;

    QString toEntity = toPluralName.isEmpty() ? to + "(" + toID + ")" : toPluralName + "(" + toID + ")";
    QStringList relationUrlTokens;
    relationUrlTokens << baseUrl << toEntity;

    QString entityReference = QString(ENTITY_REFERENCE_ATOM);
    entityReference.replace("{{HOST_SERVICE}}", baseUrl);
    entityReference.replace("{{ID}}", relationUrlTokens.join("/"));

    body = entityReference.toAscii();
    QString url = requestUrlTokens.join("/");

    if (mNetworkManager) {
        mNetworkManager->create(url, body);
        mRequestUrls.push_back(url);
    }
}

void ODataObjectModel::updateLinkATOM(const QString& from, const QString& fromID, const QString& to,
        const QString& toID, const QString& toNamePlural) {
    if (!mService) {
        qDebug() << "Data service isn't set";
        return;
    }

    QByteArray body;

    QString fromEntity = from + "(" + fromID + ")";
    QString toEntity = toNamePlural + "(" + toID + ")";
    QString baseUrl = mService->getSource();

    QStringList urlTokens;
    urlTokens << baseUrl << fromEntity << to << REF;
    QString url = urlTokens.join("/");

    QStringList relationUrlTokens;
    relationUrlTokens << baseUrl << toEntity;

    QString entityReference = QString(ENTITY_REFERENCE_ATOM);
    entityReference.replace("{{HOST_SERVICE}}", baseUrl);
    entityReference.replace("{{ID}}", relationUrlTokens.join("/"));

    body = entityReference.toAscii();

    if (mNetworkManager) {
        mNetworkManager->update(url, body, "PUT");
        mRequestUrls.push_back(url);
    }

}

ODataService* ODataObjectModel::service() const {
    return mService;
}

void ODataObjectModel::setService(ODataService* service) {
    if (service != mService) {
        mService = service;
        mNetworkManager->setFormat(mService->getFormat());
    }
}
