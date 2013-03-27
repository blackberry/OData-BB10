// Default empty project template
import bb.cascades 1.0
import odata 1.0
// creates one page with a label
Page {
    Container {
        layout: StackLayout {}
        Label {
            text: qsTr("Hello World")
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
                        title: ListItemData.Name
                        description: ListItemData.Description + " - Cost: " + ListItemData.Price
                    }
                }
            ]
        }
    }
}

