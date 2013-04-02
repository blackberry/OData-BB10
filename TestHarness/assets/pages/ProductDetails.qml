import bb.cascades 1.0
import odata 1.0
import "controls" 1.0

Page {
    property string dataSource
    property variant dataModel
    property variant supplierModel
    property variant categoryModel
    
    actions: [
        ActionItem {
            title: qsTr("Update")
            imageSource: ""
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                toastMsg.body = "Not implemented";
                toastMsg.show();
            }
        },
        ActionItem {
            title: qsTr("Delete")
            imageSource: ""
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                toastMsg.body = "Not implemented";
                toastMsg.show();
            }
        }
    ]

	attachedObjects: [
	    ODataObjectModel{
	        id: odataModel
	        source: dataSource
	    },
        ODataObjectModel {
            id: supplierODataModel
        },
        ODataObjectModel {
            id: categoryODataModel
        }
    ]

    content: Container {
        layout: StackLayout {
            
        }
        Label {
            text: "Product Details"
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        DetailsItemRow {
            label: qsTr("Product")
            data: dataModel.title[".data"]
        }
        DetailsItemRow {
            label: qsTr("")
            data: dataModel.summary[".data"]
        }
        DetailsItemRow {
            label: qsTr("Cost")
            data: dataModel.content["m:properties"]["d:Price"][".data"]
        }
        DetailsItemRow {
            label: qsTr("Rating")
            data: dataModel.content["m:properties"]["d:Rating"][".data"]
        }
        DetailsItemRow {
            label: qsTr("Released")
            data: dataModel.content["m:properties"]["d:ReleaseDate"][".data"]
        }
        DetailsItemRow {
            label: qsTr("Discontinued")
            data: dataModel.content["m:properties"]["d:DiscontinuedDate"][".data"]
        }
        DetailsItemRow {
            label: qsTr("Updated")
            data: dataModel.updated
        }
        DetailsItemRow {
            label: qsTr("Supplier")
            data: supplierModel.title[".data"]
        }
        DetailsItemRow {
            label: qsTr("Category")
            data: categoryModel.title[".data"]
        }
    }
    
    onCreationCompleted: {
        odataModel.modelReady.connect(bindToDataModel);
        supplierODataModel.modelReady.connect(bindSupplier);
        categoryODataModel.modelReady.connect(bindCategory);
    }
    
    function bindToDataModel() {
        dataModel = odataModel.model;

        supplierODataModel.source = dataService.source + "/" + dataModel.link[2]["href"];
        categoryODataModel.source = dataService.source + "/" + dataModel.link[1]["href"]
    }
    
    function bindSupplier() {
        supplierModel = supplierODataModel.model;
    }

    function bindCategory() {
        categoryModel = categoryODataModel.model;
    }
}
