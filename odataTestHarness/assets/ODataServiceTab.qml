import bb.cascades 1.0

NavigationPane {
    id: navBrowse
    Page {
        ODataList {
            attachedObjects: [
                ComponentDefinition {
                    id: drilldownList
                    source: "drilldownList.qml"
                }
            ]
            odatasource: _datasourceODataServices
            listItemComponents: [
                ListItemComponent {
                    type: "loadItem"
                    LoadingListItem {
                    }
                },
                ListItemComponent {
                    StandardListItem {
                        title: ListItemData
                    }
                }
            ]
            onCreationCompleted: {
                
                // JSON 
                _datasourceODataServices.fetchData("http://services.odata.org/OData/OData.svc/?$format=json"); // 
                
                // Atom XML... be careful, the last slash is important                  -------
                //                                                                             |
                //                                                                             V  
                //_datasourceODataServices.fetchData("http://services.odata.org/OData/OData.svc/"); 
            }
            
            onTriggered: {
                var selectedItem = _datasourceODataServices.oDataModel.data(indexPath);
                var newPage = drilldownList.createObject();
                newPage.title = selectedItem;
                navBrowse.push(newPage);
            }
        }
    }
}
