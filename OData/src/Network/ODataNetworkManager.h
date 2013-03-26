/*
 * ODataNetworkManager.h
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#ifndef ODATANETWORKMANAGER_H_
#define ODATANETWORKMANAGER_H_

#include <qobject.h>
#include <qnetworkaccessmanager.h>

class ODataNetworkManager : public QObject {
    Q_OBJECT

public:
    ODataNetworkManager();
    virtual ~ODataNetworkManager();

    void read(QString url);

    enum MessageType {
            GET,
            PUT,
            POST,
            DELETE
    };

private:
    QByteArray generateHeader(MessageType type);

    QNetworkAccessManager* mNetAccessManager;
signals:
    void jsonReady(const QByteArray& response);
    void atomReady(const QByteArray& response);

protected slots:
    void reply();
};

#endif /* ODATANETWORKMANAGER_H_ */
