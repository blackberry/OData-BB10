/*
 * ODataListModel.cpp
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#include "ODataListModel.h"
#include "ODataNetworkManager.h"
#include <bb/cascades/DataModelChangeType>

/*
 * CONSTRUCTORS / DESCTRUCTORS
 */

ODataListModel::ODataListModel() {
}

ODataListModel::~ODataListModel() {
}

/*
 * PROPERTIES
 */

QString ODataListModel::getSource(){
    return mSource;
}
void ODataListModel::setSource(QString newSource){
    mSource = newSource;
    emit sourceChanged();
}

int ODataListModel::getPageSize(){
    return mPageSize;
}
void ODataListModel::setPageSize(int newSize){
    mPageSize = newSize;
    emit pageSizeChanged();
}

/*
 * FUNCTIONS
 */

QVariant ODataListModel::data(const QVariantList& indexPath){
    return mDataList[indexPath[0].toInt()].toMap();
}

int ODataListModel::childCount(const QVariantList& indexPath){
    // the empty list is the root.
    // when it requests the root we should go get the data
    if (indexPath.count() == 0 && mDataList.count() == 0) {
        loadData();
    }

    // we are a single level data source so we can just return the amount of items we own
    return mDataList.count();
}

bool ODataListModel::hasChildren(const QVariantList& indexPath){
    // we aren't multilevel so we are only concerned with the root having children
    return indexPath.count() == 0 && !mDataList.isEmpty();
}

void ODataListModel::loadData(){
    ODataNetworkManager* manager = new ODataNetworkManager();
    manager->read(mSource);

    connect(manager, SIGNAL(jsonReady(QVariant)), this, SLOT(jsonCompleted(QVariant)));
    connect(manager, SIGNAL(atomReady(QVariant)), this, SLOT(atomCompleted(QVariant)));
}

/*
 * SLOTS
 */

void ODataListModel::jsonCompleted(QVariant response) {
    mDataList = response.toMap()["d"].toList();

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
}


void ODataListModel::atomCompleted(QVariant response) {
    mDataList = response.toMap()["entry"].toList();

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
}
