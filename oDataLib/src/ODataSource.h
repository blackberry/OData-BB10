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

#include "OrderByResources/OrderByQueryController.h"

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

    // Access methods
public:
    //bb::cascades::ArrayDataModel getArrayDataModel() const;

    bb::cascades::DataModel* getDataModel() const;
    QByteArray getMetadata() const;

    void filter(const QString& requestURL, const QString& filterQuery, bool paging = false);
    void orderBy(const QString& requestURL, const QString& fieldAndOrientation, bool paging = false);

    // Fetch the data from the network
    void fetchData(const QString& requestURL, bool paging = false);

    // Use for fetching Atom XML OData definition of a given service
    void fetchMetadata(const QString& requestURL);

    // Add data through OData service
    void postData(const QString& toURL, const QVariant& qvariantListProperties, const QString& sCollectionName, const QString& sNamespace, const QString& sEntityName);

    // Load more items in the data model
    // Used for infinite scrolling
    void loadMoreItems();

    void onODataReceived();
    void onODataMetadataReceived();

public Q_SLOTS:
	Q_INVOKABLE
    void onODataReceived_Slot();

	Q_INVOKABLE
	void onfinishedPostingData_Slot();

	Q_INVOKABLE
	void onODataMetadataReceived_Slot();

	Q_INVOKABLE
	void onPostingDataError_Slot(QNetworkReply::NetworkError errNet);

	// QVariant byIntegerCallBogus(QVariant q, int iMethod);

//signals:
    // Triggered when the data is received to allow the QML to stop its activity indicator
//    void oDataListLoaded();

private:
    bool readReply(QNetworkReply* reply, QByteArray& result);

	int onDataListReceived(QNetworkReply* reply, bb::cascades::ArrayDataModel* dataModel);
    int fillDataModelItems(QByteArray result, bb::cascades::ArrayDataModel* dataModel);
    int parseItemList(QVariantMap& root, bb::cascades::ArrayDataModel* dataModel);
    void addLoadItem(bb::cascades::ArrayDataModel* dataModel);
    void removeLoadItem(bb::cascades::ArrayDataModel* dataModel);

private:
//    bb::cascades::DataModel* oDataModel();
    bb::cascades::ArrayDataModel* m_oDataModel;  // data model for passing entries to QML
    QByteArray m_metadata;  //

    QNetworkAccessManager* m_netManager;

    int m_nPage;          // The last requested page
    bool m_bEndReached;   // True if the end of the data is reached
    QString m_requestURL; // Current request URL
    bool m_pagingEnabled; // True if paging mode is enabled
    bool m_JSONEnabled;
    bool m_MetaTestEnabled;
};

#endif /* ODATASOURCE_H_ */
