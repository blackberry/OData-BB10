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

#include "ODataService.h"
#include "ODataConstants.h"

#include "ODataNetworkManager.h"

/*
 * CONSTRUCTORS / DESCTRUCTORS
 */

ODataService::ODataService(QObject* parent) : QObject(parent)
{
}

ODataService::ODataService(QString source, Format f, QObject* parent) : QObject(parent)
{
    setFormat(f);
    setSource(source);
}

ODataService::~ODataService() {
}

/*
 * PROPERTIES
 */

QString ODataService::getSource(){
    return mSource;
}
void ODataService::setSource(const QString& newSource){
    mSource = newSource;

    mServiceDefinition.clear();
    mMetadata.clear();
    loadData();

    emit sourceChanged();
}

ODataService::Format ODataService::getFormat() {
    return mFormat;
}

void ODataService::setFormat(Format f) {
    mFormat = f;
}


/*
 * FUNCTIONS
 */

void ODataService::loadData(){
    ODataNetworkManager* manager = new ODataNetworkManager();
    manager->setFormat(mFormat);
    manager->read(mSource);

    connect(manager, SIGNAL(jsonReady(const QVariant&)), this, SLOT(jsonDefinitionReadComplete(const QVariant&)));
    connect(manager, SIGNAL(atomReady(const QVariant&)), this, SLOT(xmlDefinitionReadComplete(const QVariant&)));
    connect(manager, SIGNAL(xmlReady(const QVariant&)), this, SLOT(xmlDefinitionReadComplete(const QVariant&)));

    // we want to connect this to a different SLOT so we need a new one
    ODataNetworkManager* metaManager = new ODataNetworkManager();
    metaManager->setFormat(ODataService::XML);
    metaManager->read(mSource + "/" + OData::METADATA);

    connect(metaManager, SIGNAL(atomReady(const QVariant&)), this, SLOT(metadataReadComplete(const QVariant&)));
    connect(metaManager, SIGNAL(xmlReady(const QVariant&)), this, SLOT(metadataReadComplete(const QVariant&)));
}

QVariant ODataService::getServiceDefinition() {
    return mServiceDefinition;
}

QVariant ODataService::getMetadata() {
    return mMetadata;
}

/*
 * SLOTS
 */

// for now lets just store the values we get back

void ODataService::jsonDefinitionReadComplete(const QVariant& response){
    mServiceDefinition = response.toMap()["value"];
    emit definitionReady();
}

void ODataService::xmlDefinitionReadComplete(const QVariant& response){
    mServiceDefinition = response.toMap()[OData::WORKSPACE];
    emit definitionReady();
}

void ODataService::metadataReadComplete(const QVariant& response){
    mMetadata = response.toMap()[OData::EDMX_DATASERVICES].toMap()[OData::SCHEMA];

    emit metadataReady();
}
