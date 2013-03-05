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

/**
 * Class that provides generic access to the odata service
 */
class ODataSource: public QObject {
Q_OBJECT

Q_PROPERTY(bb::cascades::DataModel* oDataModel READ oDataModel CONSTANT)

public:
    ODataSource(QObject *parent);
    virtual ~ODataSource();

    bb::cascades::DataModel* oDataModel() const;

public Q_SLOTS:
    // Fetch the data from the network
    Q_INVOKABLE
    void fetchData(const QString& requestURL);

    Q_INVOKABLE
    void onODataReveived();

signals:
    // Triggered when the data is received to allow the QML to stop its activity indicator
    void oDataListLoaded();

private:
    int onDataListReceived(QNetworkReply* reply, bb::cascades::ArrayDataModel* dataModel);
    int fillDataModelItems(QByteArray result, bb::cascades::ArrayDataModel* dataModel);
    int parseItemList(QVariantMap& root, bb::cascades::ArrayDataModel* dataModel);

private:
    bb::cascades::ArrayDataModel* m_oDataModel;  // data model for passing entries to QML
    QNetworkAccessManager* m_netManager;
};

#endif /* ODATASOURCE_H_ */
