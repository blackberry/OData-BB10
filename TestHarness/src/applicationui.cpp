/* Copyright (c) 2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <ODataListModel.h>
#include <ODataObjectModel.h>
#include <ODataConstants.h>
#include <ODataService.h>
#include <bb/ApplicationInfo>

using namespace bb;
using namespace bb::cascades;
using namespace OData;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
{
    qmlRegisterType<ODataListModel>("odata", 1, 0, "ODataListModel");
    qmlRegisterType<ODataObjectModel>("odata", 1, 0, "ODataObjectModel");
    qmlRegisterType<ODataService>("odata", 1, 0, "ODataService");
    qmlRegisterType<ApplicationInfo>("odata", 1, 0, "ApplicationInfo");
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);

    /*******************************************************************
     * Place your read/write url here in place of the read only one
     *
     * Access the following url to get a read/write sandbox and copy it in below
     *
     * http://services.odata.org/(S(readwrite))/OData/OData.svc/
     *
     *                                    VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
     *******************************************************************/

    _dataService = new ODataService("http://services.odata.org/V4/(S(ugj4qzwzxt5zukh2k2b32tx3))/OData/OData.svc/", ODataService::JSON);
    qml->setContextProperty("dataService", _dataService);

    // create root object for the UI
    AbstractPane* root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}
