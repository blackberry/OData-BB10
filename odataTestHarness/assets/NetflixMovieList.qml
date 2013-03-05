// Shared control for all pages that list movies
import bb.cascades 1.0

Container {
    layout: DockLayout {
    }
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    ListView {
        id: odataList
        dataModel: _datasource.oDataModel
        layoutProperties: StackLayoutProperties {
            // Make the list view to fill all available height
            spaceQuota: 1.0
        }
        attachedObjects: [
            ListScrollStateHandler {
                onAtEndChanged: {
                    if (atEnd) {
                        if (odataList.dataModel.childCount([]) <= 0) {
                            // This list is not loaded yet; ignore
                            return;
                        }

                        // Update the lists for infinite scroll
                        _datasource.loadMoreItems();
                        odataList.scrollToPosition(ScrollPosition.End, ScrollAnimation.Smooth);
                    }
                }
            }
        ]
        listItemComponents: [
            ListItemComponent {
                StandardListItem {
                    title: ListItemData.Name
                }
            },
            ListItemComponent {
                type: "loadItem"
                LoadingListItem {
                }
            }
        ]
        function itemType(data, indexPath) {
            return (data.loadItem == true ? 'loadItem' : '');
        }
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
