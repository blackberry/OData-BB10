/*
 * ODataObjectModel.cpp
 *
 *  Created on: 2013-03-28
 *      Author: Daniel Baxter
 */

#include "ODataObjectModel.h"
#include "ODataConstants.h"

#include "ODataNetworkManager.h"

/*
 * CONSTRUCTORS / DESCTRUCTORS
 */

ODataObjectModel::ODataObjectModel() {
}

ODataObjectModel::ODataObjectModel(QString source) {
    setSource(source);
}

ODataObjectModel::~ODataObjectModel() {
}

/*
 * PROPERTIES
 */

QString ODataObjectModel::getSource(){
    return mSource;
}
void ODataObjectModel::setSource(QString newSource){
    mSource = newSource;

    mModel.clear();
    loadData();

    emit sourceChanged();
}

QVariant ODataObjectModel::getModel(){
    return mModel;
}
void ODataObjectModel::setModel(QVariant newModel){
    mModel = newModel;

    emit modelChanged();
}
/*
 * FUNCTIONS
 */

void ODataObjectModel::loadData(){
    ODataNetworkManager* manager = new ODataNetworkManager();
    manager->read(mSource);

    connect(manager, SIGNAL(jsonReady(QVariant)), this, SLOT(jsonReadComplete(QVariant)));
    connect(manager, SIGNAL(atomReady(QVariant)), this, SLOT(atomReadComplete(QVariant)));
}

void ODataObjectModel::deleteModel() {
    ODataNetworkManager* manager = new ODataNetworkManager();
    manager->del(mSource);

    connect(manager, SIGNAL(deleteSuccessful()), this, SLOT(deleteComplete()));
}

/*
 * SLOTS
 */

void ODataObjectModel::jsonReadComplete(QVariant response){
    mModel = response.toMap()["d"];

    emit modelReady();
}

void ODataObjectModel::atomReadComplete(QVariant response){
    mModel = response;

    emit modelReady();
}

void ODataObjectModel::deleteComplete() {
    emit modelDeleted();
}
