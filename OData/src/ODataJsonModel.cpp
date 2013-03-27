/*
 * ODataJsonModel.cpp
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#include "ODataJsonModel.h"
#include "ODataNetworkManager.h"
#include <bb/cascades/DataModelChangeType>

/*
 * CONSTRUCTORS / DESCTRUCTORS
 */

ODataJsonModel::ODataJsonModel() {
}

ODataJsonModel::~ODataJsonModel() {
}

/*
 * PROPERTIES
 */

QString ODataJsonModel::getSource(){
    return mSource;
}
void ODataJsonModel::setSource(QString newSource){
    mSource = newSource;
    emit sourceChanged();
}

int ODataJsonModel::getPageSize(){
    return mPageSize;
}
void ODataJsonModel::setPageSize(int newSize){
    mPageSize = newSize;
    emit pageSizeChanged();
}

/*
 * FUNCTIONS
 */

QString ODataJsonModel::itemType(const QVariantList &indexPath){
    return "item";
}

QVariant ODataJsonModel::data(const QVariantList& indexPath){
    return mDataList[indexPath[0].toInt()];
}

int ODataJsonModel::childCount(const QVariantList& indexPath){
    // the empty list is the root.
    // when it requests the root we should go get the data
    if (indexPath.count() == 0 && mDataList.count() == 0) {
        loadData();
    }

    // we are a single level data source so we can just return the amount of items we own
    return mDataList.count();
}

bool ODataJsonModel::hasChildren(const QVariantList& indexPath){
    // we aren't multilevel so we are only concerned with the root having children
    return indexPath.count() == 0 && !mDataList.isEmpty();
}

void ODataJsonModel::loadData(){
    ODataNetworkManager* manager = new ODataNetworkManager();
    manager->read(mSource);

    connect(manager, SIGNAL(readReady(QVariant)), this, SLOT(readCompleted(QVariant)));
}

/*
 * SLOTS
 */

void ODataJsonModel::readCompleted(QVariant response) {
    mDataList = response.toMap()["d"].toList();

    for (int i = 0; i < mDataList.count(); i++) {
        fprintf(stderr, mDataList[i].toMap()["Name"].toString().toUtf8().constData());
    }

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
}
