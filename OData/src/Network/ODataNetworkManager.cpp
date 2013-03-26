/*
 * ODataNetworkManager.cpp
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#include "ODataNetworkManager.h"
#include <qnetworkrequest.h>
#include <qnetworkreply.h>

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

    QUrl qurl;
    qurl.setUrl(url);

    QNetworkRequest req(qurl);
    QNetworkReply *reply = mNetAccessManager->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

/*
 * PRIVATE FUNCTIONS
 */

QByteArray ODataNetworkManager::generateHeader(MessageType type) {

}

/*
 * SLOTS
 */

void ODataNetworkManager::reply() {

}
