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

#ifndef ODATASERVICE_H_
#define ODATASERVICE_H_

#include <qobject.h>
#include <qvariant.h>

class ODataService : public QObject {
    Q_OBJECT

    Q_ENUMS(Format)
    Q_PROPERTY(QString source READ getSource WRITE setSource NOTIFY sourceChanged)

public:
     enum Format{
         ATOM,
         JSON,
         XML
     };

    ODataService(QObject* parent = 0);
    ODataService(QString source, Format f, QObject* parent = 0);
    virtual ~ODataService();

    QString getSource();
    void setSource(const QString& newSource);

    Format getFormat();
    void setFormat(Format f);


    Q_INVOKABLE QVariant getServiceDefinition();
    Q_INVOKABLE QVariant getMetadata();

signals:
    void sourceChanged();
    void definitionReady();
    void metadataReady();

public slots:
    void jsonDefinitionReadComplete(const QVariant& response);
    void xmlDefinitionReadComplete(const QVariant& response);
    void metadataReadComplete(const QVariant& response);

private:
    QString mSource;
    QVariant mServiceDefinition;
    QVariant mMetadata;
    Format mFormat;

    void loadData();
};

#endif /* ODATASERVICE_H_ */
