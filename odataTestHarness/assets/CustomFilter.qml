import bb.cascades 1.0
import ODataLib 1.0 

Page {
    id: customFilterTab
    Container {
        
        attachedObjects: [
            ODataQuery {
                id: oDataQuery; 
            }
        ]
        
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
            
            TextField {
                id: queryField;
            }
            Button {
                text: "Filter"
                onClicked: {
                    oDataQuery.setUri("http://services.odata.org/OData/OData.svc/" + dropDown.selectedOption.text).format("json").filter(queryField.text);
                    _datasourceFilter.fetchData(oDataQuery.getQueryText());
                    
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

