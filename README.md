odatabb10
=========

Open Data Protocol (OData) library for BlackBerry 10 (BB10) Cascades apps.  
This library makes it easy to build native BB10 apps that consume OData services.  
For information about OData, refer to http://www.odata.org/

Building this Sample
--------------------

Import both OData and TestHarness projects into your NDK workspace.

**Build the Library**

1. Find the moc*.sh file and change the moc command to direct to your bbndk's moc installation.

2. Execute the moc and copy the moc_ files output to the public directory

3. Build the .a library archive.

**Build the Application**

1. In order to utilize the writeback functionality of OData you need a writeable source. Visit this link ( http://services.odata.org/(S(readwrite))/OData/OData.svc/ ) here to generate a private sandbox to edit. Copy the url in the address bar and then place it in the marked location in TestHarness/src/applicationui.cpp

2. The TestHarness application should simply build and deploy given that the library built with no issues.

How to use
----------

To integrate into your app:

1. Add this to your .pro file

```
LIBS += -lbbdata
```

2. Configure the .pro file to import the correct library artifacts from your copy of the library.

```
device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
		INCLUDEPATH += ../../OData/Device-Debug/public
		LIBS += -Bstatic -L../../OData/Device-Debug/ -lOData -Bdynamic
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
		INCLUDEPATH += ../../OData/Device-Release/public
		LIBS += -Bstatic -L../../OData/Device-Release/ -lOData -Bdynamic
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
		INCLUDEPATH += ../../OData/Simulator-Debug/public
		LIBS += -Bstatic -L../../OData/Simulator-Debug/ -lOData -Bdynamic
    }
}
```

**Alternately** you can simply copy the src/ directory into a folder in your project and directly build this library into your project.
