/*
 * ODataNetworkManager.cpp
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#include "ODataNetworkManager.h"
#include <qnetworkrequest.h>
#include <qnetworkreply.h>
#include <JsonDataAccess.hpp>
#include <XmlDataAccess.hpp>

/*
 * CONSTRUCTORS / DESTRUCTORS
 */

ODataNetworkManager::ODataNetworkManager()
: mNetAccessManager(NULL)
{
    mNetAccessManager = new QNetworkAccessManager(this);
}

ODataNetworkManager::~ODataNetworkManager() {
    delete mNetAccessManager;
}

/*
 * PUBLIC FUNCTIONS
 */

void ODataNetworkManager::read(QString url) {
    QUrl qurl(url);

    QNetworkRequest req(qurl);
    QNetworkReply *reply = mNetAccessManager->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(onReply()));
}

/*
 * SLOTS
 */

void ODataNetworkManager::onReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());


    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response;

            response = reply->readAll();

            QByteArray contentType = reply->rawHeader("Content-Type");

            // determine what the response is encoded in
            if (contentType.contains(TYPE_ATOM)) {
                bb::data::XmlDataAccess xda;
                emit atomReady(xda.loadFromBuffer(response));
            }
            else if (contentType.contains(TYPE_JSON)) {
                bb::data::JsonDataAccess jda;
                emit jsonReady(jda.loadFromBuffer(response));
            }
            else if (contentType.contains(TYPE_XML)) {
                bb::data::XmlDataAccess xda;
                emit xmlReady(xda.loadFromBuffer(response));
            }
        }
        else {
            // TODO: handle the error

        }
    }
    else {
        // TODO: handle the error

    }

}
