import bb.cascades 1.0
import ODataLib 1.0 

Page {
    id: testPost
    property variant propertiesList
    
    //property bytearray metaD
    
    Container {
        id: postContainer
        objectName: "ctnPost"
        
        layout:StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        // define tab content here
        Label {
            text: qsTr("Tab Post")
            horizontalAlignment: HorizontalAlignment.Center

            topMargin: 2.0
            bottomMargin: 2.0

            textStyle {
                base: SystemDefaults.TextStyles.TitleText
            }
        }
        
        Container {
            WebView {
                id:webViewReadWriteTestService
                url: "http://services.odata.org/V3/(S(readwrite))/OData/OData.svc/"      
                visible: false           
                /*onLoadingChanged: {
                if (loadRequest.status == WebLoadStatus.Succeeded) {
                console.log("URl is now: " + url);
                }
                }*/
            }
        }
        
        ActivityIndicator {
            id: loadingIndicator
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 200
            preferredWidth: 200
        }
        
        Button {
            text: "fetch $metadata"

            topMargin: 2.0
            bottomMargin: 2.0

            onClicked: {
                // loadingIndicator.start();
                // loadingIndicator.running = true;
                // loadingIndicator.visible = true;
                
                testRef.fetchMetadata("http://services.odata.org/OData/OData.svc/$metadata");
                
                //  testRef.metadataFetched.connect(onMetadataLoaded);
            }
        }
        
        function onMetadataLoaded() {
            //loadingIndicator.stop();
            //loadingIndicator.running = false;
            //loadingIndicator.visible = false;
        }
        
        Divider {
            topMargin: 2.0
            bottomMargin: 2.0
        }
        
        Button {
            text: "get Property list"
            objectName: "btnPost"
            property int itemsCnt: 0
 
            topMargin: 2.0
            bottomMargin: 2.0
            
            onClicked: {
                setText("Started");
                
                propertiesList = testRef.buildPropertyList(testRef.Metadata, "schema1", "Category");
                
                // It appears that accessing c++ object while iterating through
                // the qvariantlist is not possible. At least, I was not able
                // to find a way to do it.
                // We would need to implement some custom collection for cProperty class
                /*for (var itm in propertiesList) {
                    console.log("Object item:", itm, "=", propertiesList[itm]);
                }*/
            }
        }
        
        Divider {
            topMargin: 2.0
            bottomMargin: 2.0
        }
        
        Container {
            id: containerForID
            layout:StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            topMargin: 2.0
            bottomMargin: 2.0

            Label {
                text: qsTr("Enter ID:")
                horizontalAlignment: HorizontalAlignment.Left
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                }
            }
            TextField {
                id: idValue
                minWidth: 350.0
                text: qsTr("98")
            }
        }
        
        Divider {
            topMargin: 2.0
            bottomMargin: 2.0
        }
        
        Container {
            id: containerForName
            
            layout:StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            topMargin: 2.0
            bottomMargin: 2.0
            
            Label {
                text: qsTr("Enter Name:")
                horizontalAlignment: HorizontalAlignment.Left
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                }
            }
            
            TextField {
                id: nameValue
                minWidth: 350.0
                text: qsTr("John Thomas")
            }
        }
        
        Divider {
            topMargin: 2.0
            bottomMargin: 2.0
        }
        
        Button {
            text: "send Post"
            objectName: "btnPost"

            topMargin: 2.0

            onClicked: {
                setText("Post sent");
                
                testRef.postData(webViewReadWriteTestService.url, testRef.Metadata, propertiesList, "Categories", "NotDefined", "Category"); 
            }
        }
    }
}