import bb.cascades 1.0

ODataList {
    onCreationCompleted: {
        _datasource.fetchData("http://odata.netflix.com/Catalog/Titles?$filter=ReleaseYear%20le%201989%20and%20ReleaseYear%20ge%201980%20and%20AverageRating%20gt%204&$expand=Awards&$format=json");
    }
}
