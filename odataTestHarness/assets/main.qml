// Tabbed Pane project template
import bb.cascades 1.0

TabbedPane {
    showTabsOnActionBar: true
    Tab {
        title: qsTr("Tab 1")
        OdataViewerTab {
        }
    }
    Tab {
        title: qsTr("Tab 2")
        Page {
            id: tab2
            Container {
                // define tab content here
                Label {
                    text: qsTr("Tab 2 title")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("Tab 3")
        Page {
            id: tab3
            Container {
                // define tab content here
                Label {
                    text: qsTr("Tab 3 title")
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
                        text: qsTr("Tab 3 content")
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
