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

void cODataLib::filter(const QString& requestURL, int nIndex, int nSize, const QString& filterQuery ,bool paging) {
    ((ODataSource*)m_datasource)->filter(requestURL, nIndex, nSize, filterQuery , paging);
}


void cODataLib::orderByCollection(const QString& requestURL, int nIndex, int nSize, QObject *  queryObject ,bool paging ) {
    ((ODataSource*)m_datasource)->orderByCollection(requestURL, nIndex, nSize, (OrderByQueryCollection *) queryObject , paging);
}

void cODataLib::orderBy(const QString& requestURL, int nIndex, int nSize, const QString& fieldAndOrientation ,bool paging)  {
    ((ODataSource*)m_datasource)->orderBy(requestURL, nIndex, nSize, fieldAndOrientation, paging);
}

void cODataLib::fetchData(const QString& requestURL, int nIndex, int nSize, bool paging)
{
	((ODataSource*)m_datasource)->fetchData(requestURL, nIndex, nSize, paging);
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

