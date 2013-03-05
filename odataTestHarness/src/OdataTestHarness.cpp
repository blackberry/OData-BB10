// Tabbed pane project template
#include "OdataTestHarness.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include "ODataSource.h"

using namespace bb::cascades;

OdataTestHarness::OdataTestHarness(bb::cascades::Application *app) :
                QObject(app) {

    m_datasource = new ODataSource(this);

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Set the Data source object so it can be accessed from QML
    qml->setContextProperty("_datasource", m_datasource);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}
