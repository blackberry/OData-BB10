import bb.cascades 1.0
import odata 1.0
import "controls" 1.0

Page {
    property variant updateModel
    property bool create: true
    property variant modelStructure
    
    attachedObjects: [
        ODataObjectModel {
            id: objectModel
        }
    ]
    
    actions: [
        ActionItem {
            title: qsTr("OK")
            imageSource: "asset:///icons/ic_done.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                toastMsg.body = "Not implemented";
                toastMsg.show();
            }
        }
    ]

    ScrollView {
	    Container {
	        layout: StackLayout {
	            orientation: LayoutOrientation.TopToBottom
	        }
	
	        Label {
	            text: create ? qsTr("Create Product") : qsTr("Edit Product")
	            textStyle.base: SystemDefaults.TextStyles.BigText
	            verticalAlignment: VerticalAlignment.Center
	            horizontalAlignment: HorizontalAlignment.Center
	        }
	        ItemInputRow {
	            id: productRow
	            label: qsTr("Product")
	            data: updateModel.title[".data"]
	        }
	        ItemInputRow {
	            id: descriptionRow
	            label: qsTr("Description")
	            data: updateModel.summary[".data"]
	        }
	        ItemInputRow {
	            id: costRow
	            label: qsTr("Cost")
	            data: updateModel.content["m:properties"]["d:Price"][".data"]
	        }
	        ItemInputRow {
	            id: ratingRow
	            label: qsTr("Rating")
	            data: updateModel.content["m:properties"]["d:Rating"][".data"]
	        }
	        ItemInputRow {
	            id: releasedRow
	            label: qsTr("Released")
	            data: updateModel.content["m:properties"]["d:ReleaseDate"][".data"]
	        }
	        ItemInputRow {
	            id: discontinuedRow
	            label: qsTr("Discontinued")
	            data: updateModel.content["m:properties"]["d:DiscontinuedDate"][".data"]
	        }
	    }
	}    

    onCreationCompleted: {
        modelStructure = objectModel.getModelStructure("Product", dataService.getMetadata());
    }
}
