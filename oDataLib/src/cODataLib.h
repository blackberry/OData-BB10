/*
 * cODataLib.h
 *
 *  Created on: 2013-03-08
 *      Author: cmartin
 */

#ifndef CODATALIB_H_
#define CODATALIB_H_

#include <QtCore/QtGlobal>

/* Not necessary for static library
#if defined(ODATA_LIBRARY)
#  define ODATALIB_EXPORT Q_DECL_EXPORT
#else
#  define ODATALIB_EXPORT Q_DECL_IMPORT
#endif
*/

#include <QObject>
#include <bb/cascades/ArrayDataModel>

#include "OrderByResources/OrderByQueryCollection.h"

#include "FilterResources/FilterQueryController.h"

// class ODataSource;
class /* ODATALIB_EXPORT*/ cODataLib : public QObject /* inheritance from QObject is necessary for signal to work. */
{
	Q_OBJECT
	Q_PROPERTY(bb::cascades::DataModel* oDataModel READ oDataModel CONSTANT)

public:
	explicit cODataLib(QObject *parent = NULL);
	virtual ~cODataLib();

	// property for QML access
    bb::cascades::DataModel* oDataModel() const;

	// Access methods
    Q_INVOKABLE void filter(const QString& requestURL, const QString& filterQuery ,bool paging = false);
    Q_INVOKABLE void orderByCollection(const QString& requestURL, QObject * orderByCollectionObject ,bool paging = false);
    Q_INVOKABLE void orderBy(const QString& requestURL, const QString& fieldAndOrientation, bool paging = false);
    Q_INVOKABLE void fetchData(const QString& url, bool paged = false);
    Q_INVOKABLE void loadMoreItems();

    Q_INVOKABLE void onODataReceived();

public:
	// called after triggered signal from onODataReceived in ODataSource obj
	void triggerDataFetched();

private:
	// member variables
	QObject* m_datasource; // the developer using this library does not need to know what is the QObject.

public:
signals:
	// Triggered when the fetchData requested is completed
	void dataFetched();
};

#endif /* CODATALIB_H_ */
