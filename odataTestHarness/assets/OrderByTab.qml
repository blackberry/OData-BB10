import bb.cascades 1.0
import ODataLib 1.0 

Page {
    id: orderByTab
    
    Container {
        id: rootContainer
        
        attachedObjects: [
            OrderByQueryController {
                id: controller;
            }
        ]
        
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
            Label {
                id: fullQuery
            }
            Container {
                layout:StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                
                }
                TextField {
                    id: fieldText
                }
                
                DropDown {
                    id: dropDownOrientation
                    preferredWidth: 15.0
                    Option {
                        selected: true
                        text: "Ascending"
                        value: true
                    }
                    Option {
                        text: "Descending"
                        value: false
                    }
                }
                
                Button {
                    id: addQueryButton
                    text: qsTr("+")
                    preferredWidth: 5.0
                    onClicked: {
                        controller.addQuery(fieldText.text,dropDownOrientation.selectedValue);
                        fieldText.setText("");
                        fullQuery.setText(controller.getQueryText());
                        
                    }
                }
            }
        }
        
        Container {
            layout:StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            Button {
                id: orderByButton
                text: qsTr("OrderBy")
                onClicked: {
                    _datasourceOrderBy.orderBy("http://services.odata.org/OData/OData.svc/" + dropDown.selectedOption.text + "?$format=json", controller.getQueryText());
                }
            }
           
        }
        
        Container {
            ODataList {
                odatasource: _datasourceOrderBy
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

