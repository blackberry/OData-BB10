// Tabbed pane project template
#include "OdataTestHarness.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include "../../ODataLib/src/ODataLib.h"

using namespace bb::cascades;


// Temporarly globals
cODataLib *objToDataLib = NULL;
cODataLib *objToDataLibSvcs = NULL;
cODataLib *objToDataLibDDown = NULL;
cODataLib *objToDataLibOrderBy = NULL;

OdataTestHarness::OdataTestHarness(bb::cascades::Application *app) :
                QObject(app) {
	// You must add this for class registration for use in the qml View and/or Container
    qmlRegisterType<cODataLib>("ODataLib", 1, 0, "cODataLib");
    qmlRegisterType<OrderByQueryCollection>("ODataLib", 1, 0, "OrderByQueryCollection");
    qmlRegisterType<FilterQueryTestObject>("ODataLib", 1, 0, "FilterQueryTestObject");

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Set the Data source object so it can be accessed from QML
    objToDataLib = new cODataLib(this);
    qml->setContextProperty("_datasourceNetflix", objToDataLib);
    // objToDataLib->fetchData("http://odata.netflix.com/Catalog/Titles?$filter=ReleaseYear%20le%201989%20and%20ReleaseYear%20ge%201980%20and%20AverageRating%20gt%204&$expand=Awards&$format=json", 0, 20, true);
    objToDataLibSvcs = new cODataLib(this);
    qml->setContextProperty("_datasourceODataServices", objToDataLibSvcs);
    objToDataLibDDown = new cODataLib(this);
    qml->setContextProperty("_datasourceDrilldown", objToDataLibDDown);

    objToDataLibOrderBy = new cODataLib(this);
    qml->setContextProperty("_datasourceOrderBy", objToDataLibOrderBy);


    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

OdataTestHarness::~OdataTestHarness()
{
	if (objToDataLib != NULL)
		delete objToDataLib;
	if (objToDataLibSvcs != NULL)
		delete objToDataLibSvcs;
	if (objToDataLibDDown != NULL)
		delete objToDataLibDDown;
}
