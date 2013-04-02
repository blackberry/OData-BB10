import bb.cascades 1.0
import odata 1.0

NavigationPane {
    property string dataSource

    Page {
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
                text: qsTr("Suppliers")
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

                    var supplierDetails = supplierDetailsPage.createObject();
                    supplierDetails.dataSource = selectedItem.id;
                    activeTab.content.push(supplierDetails);
                }
                listItemComponents: [
                    ListItemComponent {
                        StandardListItem {
                            title: ListItemData.title[".data"]
                        }
                    }
                ]
            }
        }
    }
    onPopTransitionEnded: {
        page.destroy();
    }
}
