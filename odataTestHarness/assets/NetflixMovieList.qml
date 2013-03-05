// Shared control for all pages that list movies
import bb.cascades 1.0

Container {
    layout: DockLayout {
    }
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    ListView {
        dataModel: _datasource.oDataModel
        layoutProperties: StackLayoutProperties {
            // Make the list view to fill all available height
            spaceQuota: 1.0
        }
        listItemComponents: [
            ListItemComponent {
                StandardListItem {
                    title: ListItemData.Name
                }
            }
        ]
    }
    ActivityIndicator {
        id: loadingIndicator
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        preferredHeight: 200
        preferredWidth: 200
    }
    onCreationCompleted: {
        loadingIndicator.start();
        loadingIndicator.running = true;
        loadingIndicator.visible = true;
        
        _datasource.oDataListLoaded.connect(onListLoaded);
        _datasource.fetchData("http://odata.netflix.com/Catalog/Titles?$filter=ReleaseYear%20le%201989%20and%20ReleaseYear%20ge%201980%20and%20AverageRating%20gt%204&$expand=Awards&$format=json");
    }
    function onListLoaded() {
        loadingIndicator.stop();
        loadingIndicator.running = false;
        loadingIndicator.visible = false;
    }
}
