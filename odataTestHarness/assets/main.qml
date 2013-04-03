// Tabbed Pane project template
import bb.cascades 1.0

TabbedPane {
    showTabsOnActionBar: false
    Tab {
        title: qsTr("Netflix")
        OdataViewerTab {
        }
    }
    Tab {
        title: qsTr("OData")
        ODataServiceTab {
        }
    }
    Tab {
        title: qsTr("OrderBy")
        OrderByTab {
        }
    }
    Tab {
        title: qsTr("Filter")
        FilterTab {  
        }
    }
   Tab {
       objectName: "testPostTab"
        title: qsTr("TestPost")
        TestPost {  
        }
    }
    Tab {
        title: qsTr("Tab 6")
        Page {
            id: tab6
            Container {
                // define tab content here
                Label {
                    text: qsTr("Tab 6 title")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                Container {
                    layout: DockLayout {
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Label {
                        text: qsTr("Tab 6 content")
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                        }
                    }
                }
            }
        }
    }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("TabbedPane - onCreationCompleted()")
        
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
