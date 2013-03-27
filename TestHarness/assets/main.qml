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
            dataModel: ODataJsonModel {
                source: "http://services.odata.org/OData/OData.svc/Products?$format=json"
            }
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        layout: StackLayout {
                            
                        }
                        Label {
                            text: ListItemData.Name
                            textStyle.color: Color.White
                        }
                        Label {
                            text: ListItemData.Description
                            textStyle.color: Color.White
                        }
                        Label {
                            text: ListItemData.Price
                            textStyle.color: Color.White
                        }
                    }
                }
            ]
        }
    }
}

