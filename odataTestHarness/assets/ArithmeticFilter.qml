import bb.cascades 1.0
import ODataLib 1.0 

Page {
    id: arithmeticFilterTab
    
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
                    id: dropDownArithmeticalOperation
                    Option {
                        selected: true;
                        text: "ADD"   
                        value: 0
                    }
                    Option {
                        text: "SUB"
                        value: 1
                    }
                    Option {
                        text: "MUL"
                        value: 2
                    }
                    Option {
                        text: "DIV"   
                        value: 3
                    }
                    Option {
                        text: "MOD"    
                        value: 4
                    }
                
                }
                
                TextField {
                    id: queryArithmneticalValue
                    minWidth: 350.0
                }
            }
            Container {
                layout:StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                DropDown {  
                    maxWidth: 50.0
                    id: dropDownLogicalOperation
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
                    id: queryLogicalValue
                    minWidth: 350.0
                }
            }
            Button {
                text: "Filter"
                onClicked: {

                    controller.addQuery(queryField.text, dropDownArithmeticalOperation.selectedOption.value, parseFloat(queryArithmneticalValue.text), dropDownLogicalOperation.selectedOption.value, parseFloat(queryLogicalValue.text));
                    
                    _datasourceFilter.filter("http://services.odata.org/OData/OData.svc/" + dropDown.selectedOption.text + "?$format=json", controller.getQueryText());
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

