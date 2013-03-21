import bb.cascades 1.0
import ODataLib 1.0 

Page {
    
    id: logicalFilterTab
    
    attachedObjects: [
        FilterQueryController {
            id: controller;
        },
        ODataQuery {
           id: oDataQuery; 
        }
        
    ]
    
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
                    id: queryField
                    minWidth: 350.0
                }
                DropDown {  
                    id: dropDownOperation
                    horizontalAlignment: HorizontalAlignment.Left
                    Option {
                        selected: true;
                        text: "EQ"   
                        value: 0
                    }
                    Option {
                        text: "NE"
                        value: 1
                    }
                    Option {
                        text: "GT"
                        value: 2
                    }
                    Option {
                        text: "GE"   
                        value: 3
                    }
                    Option {
                        text: "LT"    
                        value: 4
                    }
                    Option {
                        text: "LE"
                        value: 5
                    }
                }
                
                TextField {
                    id: queryValue
                    minWidth: 350.0
                }
            }
            Button {
                text: "Filter"
                onClicked: {
                    if ( isNaN(parseFloat(queryValue.text) ) )
                        controller.addQuery(queryField.text, dropDownOperation.selectedOption.value, queryValue.text);
                    else
                        controller.addQuery(queryField.text, dropDownOperation.selectedOption.value, parseFloat(queryValue.text));
                    
                    oDataQuery.setUri("http://services.odata.org/OData/OData.svc/" + dropDown.selectedOption.text).format("json").filter(controller.getQueryText());
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

