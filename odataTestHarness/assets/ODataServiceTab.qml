import bb.cascades 1.0

Page {
    ODataList {
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
            _datasourceODataServices.fetchData("http://services.odata.org/OData/OData.svc/?$format=json");
        }
    }
}
