odatabb10
=========

Open Data Protocol (OData version 4.0) library for BlackBerry 10 (BB10) Cascades apps.
This library makes it easy to build native BB10 apps that consume OData services.  
For information about OData, refer to http://www.odata.org/

Features
--------------------

+ Create/read/update/delete entities
+ Bind operation
+ Create/read/update/delete navigation links
+ Format switching(JSON, ATOM)
+ Filtering support for System Query Options:  $filter, $count, $orderby, $skip, $top and $search.
  Just use the filterModel() function with a map of key : value pairs for each query option.

Building this Sample
--------------------

Import both OData and TestHarness projects into your NDK workspace. The OData project is the library itself and TestHarness is a simple application that shows how you might use the OData library.

Requirements

+ BlackBerry Cascades Native SDK + QDE
+ A BlackBerry 10 Device or Simulator

**Build the Application**

1. In order to utilize the writeback functionality of OData you need a writeable source. Visit this link ( http://services.odata.org/V4/(S(readwrite))/OData/OData.svc/ ) to generate a private sandbox to edit. Copy the generated url in the address bar and then place it in the marked location in TestHarness/src/applicationui.cpp. It should look like this http://services.odata.org/V4/(S(xxxxxxxxxxxxxxxxxxxxxxxx))/OData/OData.svc/ where the x's are a set of random letters and numbers.

2. From the QDE build the application for the target you need.

3. Deploy to the target.

How to use For Your Own Application
-----------------------------------

To integrate into your app:

+ Add this to your .pro file

```
CONFIG += odata
```

+ If your project is not in the same folder as the odata library, edit your Makefile

```
QMAKEFEATURES = <path to odata library project>
```

+ Option 2. You can simply copy the src/ directory from the OData project into a folder in your project and directly build this library into your project. You may need to add the QtNetwork library to your includes/LIBs if you are not using it already.
