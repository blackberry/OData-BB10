/*
 * cODataLib.h
 *
 *  Created on: 2013-03-08
 *      Author: cmartin
 */

#ifndef CODATALIB_H_
#define CODATALIB_H_

#include <QtCore/QtGlobal>

#include <QObject>
#include <bb/cascades/ArrayDataModel>

#include "OrderByResources/OrderByQueryController.h"
#include "FilterResources/FilterQueryController.h"
#include "FilterResources/FilterQueryObject.h"

#include "ODataQuery.h"

// class ODataSource;
class cODataLib : public QObject /* inheritance from QObject is necessary for signal to work. */
{
	Q_OBJECT
	Q_PROPERTY(bb::cascades::DataModel* oDataModel READ oDataModel CONSTANT)

	// TODO: Not sure: Might not need Metadata as a property
	Q_PROPERTY(QByteArray Metadata READ Metadata CONSTANT)

public:
	explicit cODataLib(QObject *parent = NULL);
	virtual ~cODataLib();

	// property for QML access
    bb::cascades::DataModel* oDataModel() const;

    QByteArray Metadata() const;

	// Access methods
    Q_INVOKABLE void filter(const QString& requestURL, const QString& filterQuery, bool paging = false);
    Q_INVOKABLE void orderBy(const QString& requestURL, const QString& fieldAndOrientation, bool paging = false);
    Q_INVOKABLE void fetchData(const QString& url, bool paged = false);
    Q_INVOKABLE void loadMoreItems();


    Q_INVOKABLE void fetchMetadata(const QString& url);
    Q_INVOKABLE QVariant buildPropertyList(const QByteArray& arrMetadata, const QString& schema, const QString& entityType);

    Q_INVOKABLE void postData(const QString& toURL, const QByteArray& metadata, QVariant q, const QString& sCollectionName, const QString& schema, const QString& entityName);

    Q_INVOKABLE void onODataReceived();

public:
	// called after triggered signal from onODataReceived in ODataSource obj
	void triggerDataFetched();

	// called after triggered signal from onODataMetadataReceived in ODataSource obj
	void triggerMetadataFetched();

private:
	// member variables
	QObject* m_datasource; // the developer using this library does not need to know what is the QObject.

public:
signals:
	// Triggered when the fetchData requested is completed
	void dataFetched();
	void metadataFetched();
};

#endif /* CODATALIB_H_ */
