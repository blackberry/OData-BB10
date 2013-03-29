// Default empty project template
import bb.cascades 1.0
import odata 1.0

TabbedPane {
    showTabsOnActionBar: false
    activeTab: atomTab
	Tab {
	    id: atomTab
	    title: qsTr("atom")
	    
	    content: Page {
		    Container {
		        layout: StackLayout {}
		        Label {
		            text: qsTr("Atom")
		            textStyle.base: SystemDefaults.TextStyles.BigText
		            verticalAlignment: VerticalAlignment.Center
		            horizontalAlignment: HorizontalAlignment.Center
		        }
		        ListView {
		            dataModel: ODataListModel {
		                source: "http://services.odata.org/OData/OData.svc/Products"
		            }
		            listItemComponents: [
		                ListItemComponent {
		                    StandardListItem {
		                        title: ListItemData.title[".data"]
		                        description: ListItemData.summary[".data"] + " - Cost: " + ListItemData.content["m:properties"]["d:Price"][".data"]
		                    }
		                }
		            ]
		        }
		    }
	    }
	}
	Tab {
        id: jsonTab
        title: qsTr("json")

        content: Page {
            Container {
                layout: StackLayout {
                }
                Label {
                    text: qsTr("JSON")
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
                ListView {
                    dataModel: ODataListModel {
                        source: "http://services.odata.org/OData/OData.svc/Products?$format=json"
                    }
                    listItemComponents: [
                        ListItemComponent {
                            StandardListItem {
                                title: ListItemData.Name
                                description: ListItemData.Description + " - Cost: " + ListItemData.Price
                            }
                        }
                    ]
                }
            }
        }
    }
}