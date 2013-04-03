/*
 * cODataSource.cpp
 *
 *  Created on: 2013-03-07
 *      Author: cmartin
 */

#include "ODataSource.h"
#include "cODataLib.h"
#include "cAtomManager.h"
#include "cProperty.h"

#include "LOGGER.h"

using namespace bb::cascades;

// TODO: remove this global and handle it in ODataSource
// temporary global
QVariant g_qvariantListProperties;

cODataLib::cODataLib(QObject *parent) :
    QObject(parent)
{
	m_datasource =  new ODataSource(this);
}

cODataLib::~cODataLib()
{
}

void cODataLib::filter(const QString& requestURL, const QString& filterQuery, bool paging) {
    ((ODataSource*)m_datasource)->filter(requestURL, filterQuery, paging);
}

void cODataLib::orderBy(const QString& requestURL, const QString& fieldAndOrientation, bool paging) {
    ((ODataSource*)m_datasource)->orderBy(requestURL, fieldAndOrientation, paging);
}

void cODataLib::fetchData(const QString& requestURL, bool paging) {
	((ODataSource*)m_datasource)->fetchData(requestURL, paging);
}

void cODataLib::fetchMetadata(const QString& requestURL) {
	((ODataSource*)m_datasource)->fetchMetadata(requestURL);
}

void cODataLib::loadMoreItems() {
	((ODataSource*)m_datasource)->loadMoreItems();
}

void cODataLib::onODataReceived() {
	((ODataSource*)m_datasource)->onODataReceived();
}

bb::cascades::DataModel* cODataLib::oDataModel() const {
	return ((ODataSource*)m_datasource)->getDataModel();
}

QByteArray cODataLib::Metadata() const {
	return ((ODataSource*)m_datasource)->getMetadata();
}

void cODataLib::triggerDataFetched() {
	emit dataFetched();
}

// called after triggered signal from onODataMetadataReceived in ODataSource obj
void cODataLib::triggerMetadataFetched() {
	emit metadataFetched();
}

QVariant cODataLib::buildPropertyList(const QByteArray& arrMetadata, const QString& sSchema, const QString& sEntityType) {
	// TODO: Remove this an put it in ODatasource
	g_qvariantListProperties = cAtomManager::Instance().buildPropertyList(arrMetadata, sSchema, sEntityType);
	return g_qvariantListProperties;
}

void cODataLib::postData(const QString& toURL, const QByteArray& metadata, QVariant q, const QString& sCollectionName, const QString& schema, const QString& entityName)
{
	QVariantList ql = g_qvariantListProperties.value<QVariantList>();

	QVariantList list = q.value<QVariantList>();
	// TODO: remove this hard coding and implement some iterative mean to set values for each cProperty from QML forms
	// The "Category" has only two properties
	int i = 0;
	foreach (QVariant vt, list) {
	  cProperty* objProp = (cProperty *) vt.value<void *>();
	  if (i == 0)
		  objProp->setValue(QVariant(3)); // this is ID
	  else
		  objProp->setValue(QVariant("Cars"));
	  ++i;
	}

	// TODO: remove hard coding of "ODataDemo"; would need some method in AtomManager to extract the namespace for a given schema and entityName
	// remove hard coded "Categories and find an algo to figure out the collection name
	((ODataSource*)m_datasource)->postData(toURL, g_qvariantListProperties, "Categories", "ODataDemo", entityName);
}


