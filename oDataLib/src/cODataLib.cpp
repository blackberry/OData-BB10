/*
 * cODataSource.cpp
 *
 *  Created on: 2013-03-07
 *      Author: cmartin
 */

#include "ODataSource.h"
#include "cODataLib.h"
#include "LOGGER.h"

using namespace bb::cascades;

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

void cODataLib::fetchData(const QString& requestURL, bool paging)
{
	((ODataSource*)m_datasource)->fetchData(requestURL, paging);
}

void cODataLib::loadMoreItems()
{
	((ODataSource*)m_datasource)->loadMoreItems();
}

void cODataLib::onODataReceived()
{
	((ODataSource*)m_datasource)->onODataReceived();
}

bb::cascades::DataModel* cODataLib::oDataModel() const
{
	return ((ODataSource*)m_datasource)->getDataModel();
}

void cODataLib::triggerDataFetched()
{
	emit dataFetched();
}

QVariant cODataLib::byIntegerCallBogus(QVariant q, int iMethod)
{
	return ((ODataSource*)m_datasource)->byIntegerCallBogus(q, iMethod);
}


