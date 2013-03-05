odatabb10
=========

Open Data Protocol (OData) library for BlackBerry 10 (BB10) Cascades apps.  
This library makes it easy to build native BB10 apps that consume OData services.  
For information about OData, refer to http://www.odata.org/

How to use
----------
To integrate into your app:

1. Add this to your .pro file

        LIBS   += -lbbsystem
        LIBS   += -lbbdata

2. In your main class, after:

        QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
Add:

        m_datasource = new ODataSource(this);
        qml->setContextProperty("_datasource", m_datasource);

