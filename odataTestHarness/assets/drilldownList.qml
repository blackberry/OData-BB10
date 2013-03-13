import bb.cascades 1.0
import ODataLib 1.0 

Page {
    id: drilldownPage
    property string title: ""
    titleBar: TitleBar {
        title: drilldownPage.title
    }
    ODataList {
        odatasource: _datasourceDrilldown
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
    
    attachedObjects: [
        OrderByQueryCollection {
            id: collection;
        }
    ]
    
    onTitleChanged: {
        // title is not set at creation time, so wait until it is set before we fetch the list

       _datasourceDrilldown.fetchData("http://services.odata.org/OData/OData.svc/" + drilldownPage.title + "?$format=json", 20, 10);
       
       // This examples works only for Products tab
       
       // Example using a collection
       /*collection.addQuery("Rating",false);
       collection.addQuery("Name",true);
       _datasourceDrilldown.orderByCollection("http://services.odata.org/OData/OData.svc/" + drilldownPage.title + "?$format=json", 20, 10, collection);*/

       // Example using a query string
       //_datasourceDrilldown.orderBy("http://services.odata.org/OData/OData.svc/" + drilldownPage.title + "?$format=json", 20, 10, "Rating asc, Name desc");

    
    }
}
