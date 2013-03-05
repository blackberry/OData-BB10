// Base control for OData lists
import bb.cascades 1.0

Container {
    layout: DockLayout {
    }
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    ListView {
        id: odataList
        dataModel: _datasource.oDataModel
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
                type: "loadItem"
                LoadingListItem {
                }
            },
            ListItemComponent {
                StandardListItem { // TODO: Allow user to specify custom layout
                    title: ListItemData.Name
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
    }
    function onListLoaded() {
        loadingIndicator.stop();
        loadingIndicator.running = false;
        loadingIndicator.visible = false;
    }
}
