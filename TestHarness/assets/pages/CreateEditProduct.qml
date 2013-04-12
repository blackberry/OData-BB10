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
        },
        ODataListModel {
            id: suppliersList
            source: dataService.source + "/Suppliers"
        },
        ODataListModel {
            id: categoriesList
            source: dataService.source + "/Categories"
        },
        ComponentDefinition {
            id: option

            content: Option {
            }
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
        ScrollView {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
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
                
                // Supplier List
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill

                    bottomMargin: 10

                    Label {
                        text: qsTr("Supplier")
                        textStyle.fontSize: FontSize.Medium
                        textStyle.fontWeight: FontWeight.W500
                        verticalAlignment: VerticalAlignment.Center

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                    }
                    DropDown {
                        id: supplierRow
                        verticalAlignment: VerticalAlignment.Bottom

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 5
                        }
                    }
                }
                
                // Category list
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill

                    bottomMargin: 10

                    Label {
                        text: qsTr("Category")
                        textStyle.fontSize: FontSize.Medium
                        textStyle.fontWeight: FontWeight.W500
                        verticalAlignment: VerticalAlignment.Center

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                    }
                    DropDown {
                    	id: categoryRow
                        verticalAlignment: VerticalAlignment.Bottom
                        
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 5
                        }
                    }
                }
            }
        }
    }

    onCreationCompleted: {
        modelStructure = objectModel.getModelStructure("Product", dataService.getMetadata());
        
        // Get the lists for the drop downs
        suppliersList.itemsChanged.connect(bindSuppliersDropDown);
        suppliersList.childCount([]); // force the data hit
        categoriesList.itemsChanged.connect(bindCategoriesDropDown);
        categoriesList.childCount([]); // force the data hit
    }
    
    function bindSuppliersDropDown() {
        supplierRow.removeAll();

        for (var i = 0; i < suppliersList.childCount([]); i++) {
            var opt = option.createObject();
            opt.text = suppliersList.data([i]).title[".data"];
            supplierRow.add(opt);
        }
    } 
    
    function bindCategoriesDropDown() {
        categoryRow.removeAll();

        for (var i = 0; i < categoriesList.childCount([]); i ++) {
            var opt = option.createObject();
            opt.text = categoriesList.data([ i ]).title[".data"];
            categoryRow.add(opt);
        }
    }
}
