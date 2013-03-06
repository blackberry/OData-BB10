import bb.cascades 1.0

/**
 * Example of how to use ODataList to display Catalog content from NetFlix
 */
ODataList {

    odatasource: _datasourceNetflix
    listItemComponents: [
        ListItemComponent {
            type: "loadItem"
            LoadingListItem {
            }
        },
        ListItemComponent {
            StandardListItem {
                title: ListItemData.Name
                description: ListItemData.ShortSynopsis
                status: ListItemData.Rating
            }
        }
    ]
    onCreationCompleted: {
        _datasourceNetflix.fetchData("http://odata.netflix.com/Catalog/Titles?$filter=ReleaseYear%20le%201989%20and%20ReleaseYear%20ge%201980%20and%20AverageRating%20gt%204&$expand=Awards&$format=json", true);
    }
}
