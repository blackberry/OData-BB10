/*
 * ODataSource.h
 *
 *  Created on: Mar 4, 2013
 *      Author: martin
 */

#ifndef ODATASOURCE_H_
#define ODATASOURCE_H_

#include <QObject>
#include <bb/cascades/ArrayDataModel>

#include "OrderByQueryObject.h"
#include "OrderByQueryCollection.h"

/**
 * Class that provides generic access to the odata service
 * It implements infinite scrolling using $skip and $top, so those parameters must not be
 * used in requestURL.
 */
class ODataSource: public QObject {

	Q_OBJECT

public:
    ODataSource(QObject *parent = 0);
    virtual ~ODataSource();

    // copy require for QSharedDataPointer
 //   ODataSource(const ODataSource &other);
 //   ODataSource& operator=(const ODataSource &other);

    // Access methods
public:
    //bb::cascades::ArrayDataModel getArrayDataModel() const;

    bb::cascades::DataModel* getDataModel() const;

    void orderByCollection(const QString& requestURL, int nIndex, int nSize,  OrderByQueryCollection* queryObject ,bool paging = false);
    void orderBy(const QString& requestURL, int nIndex, int nSize, const QString& fieldAndOrientation ,bool paging = false);

    // Fetch the data from the network
    void fetchData(const QString& requestURL, int nIndex, int nSize, bool paging = false);

    // Load more items in the data model
    // Used for infinite scrolling
    void loadMoreItems();

    void onODataReceived();

public Q_SLOTS:
	Q_INVOKABLE
    void onODataReceived_Slot();

//signals:
    // Triggered when the data is received to allow the QML to stop its activity indicator
//    void oDataListLoaded();

private:
    int onDataListReceived(QNetworkReply* reply, bb::cascades::ArrayDataModel* dataModel);
    int fillDataModelItems(QByteArray result, bb::cascades::ArrayDataModel* dataModel);
    int parseItemList(QVariantMap& root, bb::cascades::ArrayDataModel* dataModel);
    void addLoadItem(bb::cascades::ArrayDataModel* dataModel);
    void removeLoadItem(bb::cascades::ArrayDataModel* dataModel);

private:
//    bb::cascades::DataModel* oDataModel();
    bb::cascades::ArrayDataModel* m_oDataModel;  // data model for passing entries to QML
    QNetworkAccessManager* m_netManager;

    int m_nPage;          // The last requested page
    bool m_bEndReached;   // True if the end of the data is reached
    QString m_requestURL; // Current request URL
    bool m_pagingEnabled; // True if paging mode is enabled
};

#endif /* ODATASOURCE_H_ */
