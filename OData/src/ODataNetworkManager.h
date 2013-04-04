/*
 * ODataNetworkManager.h
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#ifndef ODATANETWORKMANAGER_H_
#define ODATANETWORKMANAGER_H_

#include <qobject.h>
#include <qvariant.h>
#include <qnetworkaccessmanager.h>

static const QByteArray TYPE_ATOM = "application/atom+xml";
static const QByteArray TYPE_JSON = "application/json";
static const QByteArray TYPE_XML = "application/xml";

class ODataNetworkManager : public QObject {
    Q_OBJECT

public:
    ODataNetworkManager();
    virtual ~ODataNetworkManager();

    void read(QString url);
    void create(QString url, QVariant dataModel);
    void update(QString url, QVariant dataModel);
    void del(QString url); // wanted delete but it's a protected keyword

private:
    QNetworkAccessManager* mNetAccessManager;

signals:
    void jsonReady(QVariant response);
    void atomReady(QVariant response);
    void xmlReady(QVariant response);

    void createJsonReady(QVariant response);
    void createAtomReady(QVariant response);
    void createXmlReady(QVariant response);

    void updateSuccessful();
    void deleteSuccessful();

public slots:
    void onReadReply();
    void onCreateReply();
    void onUpdateReply();
    void onDeleteReply();
};

#endif /* ODATANETWORKMANAGER_H_ */
