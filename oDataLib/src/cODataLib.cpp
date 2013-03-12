/*
 * cODataSource.cpp
 *
 *  Created on: 2013-03-07
 *      Author: cmartin
 */

#include "ODataSource.h"
#include "cODataLib.h"

using namespace bb::cascades;

cODataLib::cODataLib(QObject *parent) :
    QObject(parent)
{
	m_datasource =  new ODataSource(this);
}

cODataLib::~cODataLib()
{
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

