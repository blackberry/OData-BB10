import bb.cascades 1.0
import ODataLib 1.0 



Page {
    id: drilldownPage
    property string title: ""
    
   attachedObjects: [
       ODataQuery {
           id: oDataQuery; 
       }
   ]
    
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
    
    
    onTitleChanged: {
        // title is not set at creation time, so wait until it is set before we fetch the list
        
       _datasourceDrilldown.fetchData("http://services.odata.org/OData/OData.svc/" + drilldownPage.title + "?$format=json");
       
       //Example:
       //oDataQuery.setUri("http://services.odata.org/OData/OData.svc/" + drilldownPage.title).format("json").orderBy("Name asc").filter("Name ne 'Milk'");
       //_datasourceDrilldown.fetchData(oDataQuery.getQueryText());  
    }
}
