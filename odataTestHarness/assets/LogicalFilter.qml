import bb.cascades 1.0
import ODataLib 1.0 

Page {
    
    id: logicalFilterTab
    
    Container {
        
        layout:StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        Container {
            DropDown {  
                id: dropDown
                horizontalAlignment: HorizontalAlignment.Left
                Option {
                    selected: true;
                    text: "Products"   
                }
                Option {
                    text: "Categories"    
                }
                Option {
                    text: "Suppliers"
                }
            }
        }
        
        Container {
            layout:StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            Container {
                layout:StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                TextField {
                    id: queryField;
                }
            }
            Button {
                text: "Filter"
                onClicked: {
                    _datasourceFilter.filter("http://services.odata.org/OData/OData.svc/" + dropDown.selectedOption.text + "?$format=json", queryField.text);
                }
            }
        }
        
        Container {
            ODataList {
                odatasource: _datasourceFilter
                listItemComponents: [
                    ListItemComponent {
                        type: "loadItem"
                        LoadingListItem {
                        }
                    },
                    ListItemComponent {
                        StandardListItem {
                            title: ListItemData.Name
                            description: typeof ListItemData.Description!= "undefined"?ListItemData.Description:""
                            status: typeof ListItemData.Price!= "undefined"?ListItemData.Price+"$":""
                        }
                    }
                ]
            }
        }
    }
} //page

