/* Copyright (c) 2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import bb.cascades 1.0
import odata 1.0
import "controls" 1.0

Page {
    id: productDetails
    property string dataSource
    property variant dataModel
    property variant supplierModel
    property variant categoriesModel
    
    actions: [
        ActionItem {
            title: qsTr("Update")
            imageSource: "asset:///icons/ic_edit.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                var activeTab = tabPane.activeTab;

                var createEditPage = createEditProductPage.createObject();
                createEditPage.updateModel = dataModel;
                createEditPage.create = false;
                createEditPage.updateCategory = productDetails.categoriesModel;
                if(supplierModel)
                    createEditPage.supplierID = supplierModel.ID === undefined ? -1 : supplierModel.ID;
                activeTab.content.push(createEditPage);
            }
        },
        ActionItem {
            title: qsTr("Refresh")
            imageSource: "asset:///icons/ic_rotate.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                toastMsg.body = "Refreshing";
                toastMsg.show();

                odataModel.readModel(dataSource);
            }
        },
        ActionItem {
            title: qsTr("Delete")
            imageSource: "asset:///icons/ic_delete.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
            	odataModel.deleteModel(dataSource);
            	toastMsg.body = "Deleting";
            	toastMsg.show();
            }
        }
    ]

	attachedObjects: [
	    ODataObjectModel{
	        id: odataModel
            service: dataService
	    },
        ODataObjectModel {
            id: supplierODataModel
            service: dataService
        },
        ODataListModel {
            id: categoryODataModel
            service: dataService
        }
    ]

    content: Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        Label {
            text: "Product Details"
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        ScrollView {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                DetailsItemRow {
                    id: name
                    label: qsTr("Product")
                }
                DetailsItemRow {
                    id: description
                    label: qsTr("")
                }
                DetailsItemRow {
                    id: cost
                    label: qsTr("Cost")
                }
                DetailsItemRow {
                    id: rating
                    label: qsTr("Rating")
                }
                DetailsItemRow {
                    id: released
                    label: qsTr("Released")
                }
                DetailsItemRow {
                    id: discontinued
                    label: qsTr("Discontinued")
                }

                // Supplier Linked Row
                Container {
                    topMargin: 30
                    gestureHandlers: [
                        TapHandler {
                            onTapped: {
                                activeTab = tabPane.activeTab;

                                var supplierDetails = supplierDetailsPage.createObject();
                                supplierDetails.dataSource += "Suppliers(" + supplierModel.ID + ")";
                                activeTab.content.push(supplierDetails);
                            }
                        }
                    ]

                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill

                    Label {
                        text: qsTr("Supplier")
                        textStyle.fontSize: FontSize.Medium
                        textStyle.fontWeight: FontWeight.W500
                        verticalAlignment: VerticalAlignment.Top

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                    }
                    Label {
                        text: supplierModel ? supplierModel.Name : ""
                        textStyle.fontSize: FontSize.Medium
                        textStyle.color: Color.create("#FF8EC1DA")
                        verticalAlignment: VerticalAlignment.Top
                        multiline: true

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 5
                        }
                    }
                }

                // Category Linked Row
                Container {
                    topMargin: 30

                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill

                    Label {
                        text: qsTr("Categories")
                        textStyle.fontSize: FontSize.Medium
                        textStyle.fontWeight: FontWeight.W500
                        verticalAlignment: VerticalAlignment.Top

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                    }
                    Label {
                        id: categories
                        textStyle.fontSize: FontSize.Medium
                        textStyle.color: Color.create("#FF8EC1DA")
                        verticalAlignment: VerticalAlignment.Top
                        multiline: true

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 5
                        }
                    }
                }
            }
        }
    }

    onDataSourceChanged: {
        if(dataSource) {
            odataModel.readModel(dataSource);
        }
    }

    onCreationCompleted: {
        odataModel.modelReady.connect(bindToDataModel);
        supplierODataModel.modelReady.connect(bindSupplier);
        categoryODataModel.itemsChanged.connect(setCategories);
        categoryODataModel.childCount([]);

        odataModel.modelDeleted.connect(deleteSuccess);
    }
    
    function setCategories() {
        var categoriesStr;
        var categoryNameList = [];
        var categoryIDList = [];
        for(var i = 0; i < categoryODataModel.childCount([]); ++i) {
            categoryIDList[i] = categoryODataModel.data([i]).ID;
            categoryNameList[i] = categoryODataModel.data([i]).Name;
        }
        if(categoryIDList)
            categoriesModel = categoryIDList;
        if(categoryNameList)
            categories.text = categoryNameList.join(",");
    }

    onDataModelChanged: {
        if(dataModel) {
            name.data = dataModel.Name || "";
            description.data = dataModel.Description || "";
            cost.data = dataModel.Price || "";
            rating.data = dataModel.Rating || "";
            released.data = parseISO8601(dataModel.ReleaseDate) || "";
            discontinued.data = parseISO8601(dataModel.DiscontinuedDate) || "";
        }
    }

    function bindToDataModel() {
        dataModel = odataModel.model;

        supplierODataModel.readModel(dataSource + "/Supplier");
        categoryODataModel.readModel(dataSource + "/Categories");
    }

    function bindSupplier() {
        supplierModel = supplierODataModel.model;
    }

    function deleteSuccess() {
        toastMsg.body = "Success";
        toastMsg.show();
        
        var activeTab = tabPane.activeTab;
        activeTab.content.pop();
    }
}
