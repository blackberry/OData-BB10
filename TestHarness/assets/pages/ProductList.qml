import bb.cascades 1.0
import odata 1.0
    
Page {

    property string title
    property string dataSource
    
    actions: [
        ActionItem {
            title: qsTr("Create")
            imageSource: ""
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                toastMsg.body = "Not implemented";
                toastMsg.show();
            }
        }
    ]

    Container {
        layout: StackLayout {
        }
        Label {
            text: title
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        ListView {
            dataModel: ODataListModel {
                source: dataSource
            }
            onTriggered: {
                var selectedItem = dataModel.data(indexPath); // get the selected item
                var activeTab = tabPane.activeTab;

                var productDetails = productDetailsPage.createObject();
                productDetails.dataSource = selectedItem.id;
                activeTab.content.push(productDetails);

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
