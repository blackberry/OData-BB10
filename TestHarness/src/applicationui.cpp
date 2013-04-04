// Default empty project template
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <ODataService.h>
#include <ODataListModel.h>
#include <ODataObjectModel.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
{
    qmlRegisterType<ODataListModel>("odata", 1, 0, "ODataListModel");
    qmlRegisterType<ODataObjectModel>("odata", 1, 0, "ODataObjectModel");
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_controller", this);

    ODataService* dataService = new ODataService("http://services.odata.org/(S(3loftsrb4pdnnemtjylc0szd))/OData/OData.svc/");
    qml->setContextProperty("dataService", dataService);

    // create root object for the UI
    AbstractPane* root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

