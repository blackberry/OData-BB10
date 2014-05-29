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
    id: createEditProductPage
    property variant updateModel
    property variant updateCategory: ""
    property int supplierID: -1
    property bool create: true
    property variant modelStructure

    attachedObjects: [
        ODataObjectModel {
            id: highestId
            service: dataService
            onModelReady: {
                idRow.data = highestId.model.value[0].ID + 1;
            }
        },
        ODataObjectModel {
            id: objectModel
            service: dataService
            onModelUpdated: {
                success();
            }
            onModelCreated: {
                createCategoryLinks();
                createSupplierLink();
                success();
            }
        },
        ODataListModel {
            id: suppliersList
            service: dataService
        },
        ODataListModel {
            id: categoriesList
            service: dataService
        },
        ComponentDefinition {
            id: option
            Option {
            }
        },
        ComponentDefinition {
            id: checkBoxDef
            CheckBox {
            }
        }
    ]

    actions: [
        ActionItem {
            title: qsTr("OK")
            imageSource: "asset:///icons/ic_done.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (create) {
                    createObject();
                } else {
                    updateObject();
                }
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
                    id: idRow
                    label: qsTr("Id")
                    visible: create
                    editable: ! create
                }
                ItemInputRow {
                    id: productRow
                    label: qsTr("Product")
                }
                ItemInputRow {
                    id: descriptionRow
                    label: qsTr("Description")
                }
                ItemInputRow {
                    id: costRow
                    label: qsTr("Cost")
                }
                ItemInputRow {
                    id: ratingRow
                    label: qsTr("Rating")
                }
                DateInputRow {
                    id: releasedRow
                    label: qsTr("Released")
                }
                DateInputRow {
                    id: discontinuedRow
                    label: qsTr("Discontinued")
                    isNullable: true
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
                    RadioGroup {
                        id: supplierRow
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 5
                        }
                    }
                }

                Divider {
                }

                // Category list
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill

                    bottomMargin: 10

                    Label {
                        text: qsTr("Categories")
                        textStyle.fontSize: FontSize.Medium
                        textStyle.fontWeight: FontWeight.W500
                        verticalAlignment: VerticalAlignment.Center

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                    }
                    Container {
                        id: categories
                    }
                }
            }
        }
    }

    onCreationCompleted: {
        modelStructure = objectModel.getModelStructure("Product", dataService.getMetadata());

        if (create) {
            highestId.filterModel("Products", {'orderby' : 'ID desc', 'top' : 1});
        }

        suppliersList.readModel("Suppliers");
        categoriesList.readModel("Categories");

        // Get the lists for the drop downs
        suppliersList.itemsChanged.connect(bindSuppliers);
        categoriesList.itemsChanged.connect(bindCategories);
    }

    onUpdateModelChanged: {
        if (updateModel) {
            idRow.data = updateModel.ID || "";
            productRow.data = updateModel.Name || "";
            descriptionRow.data = updateModel.Description || "";
            costRow.data = updateModel.Price || "";
            ratingRow.data = updateModel.Rating || "";
            releasedRow.data = updateModel.ReleaseDate || "";
            discontinuedRow.data = updateModel.DiscontinuedDate || "";
        }
    }

    function bindSuppliers() {
        supplierRow.removeAll();

        for (var i = 0; i < suppliersList.childCount([]); i ++) {
            var opt = option.createObject();
            opt.text = suppliersList.data([ i ]).Name;
            opt.value = suppliersList.data([ i ]).ID;
            supplierRow.add(opt);

            if (! create && suppliersList.data([ i ]).ID == supplierID) {
                supplierRow.setSelectedIndex(i);
            }
        }
    }

    function bindCategories() {
        categories.removeAll();
        var checkBox;
        for (var i = 0; i < categoriesList.childCount([]); ++ i) {
            checkBox = checkBoxDef.createObject();
            checkBox.text = categoriesList.data([ i ]).Name;
            for (var j = 0; j < updateCategory.length; ++ j) {
                if (updateCategory[j] == categoriesList.data([ i ]).ID) {
                    checkBox.checked = true;
                    break;
                }
            }
            categories.add(checkBox);
        }
    }

    function createObject() {
        readForm();

        objectModel.createModel("Products", "ODataDemo.Product", modelStructure, "");
        toastMsg.body = "Creating";
        toastMsg.show();
    }

    function updateObject() {
        readForm();

        objectModel.updateModel("Products(" + updateModel.ID + ")", "ODataDemo.Product", modelStructure, "");
        removeCategoryLinks();
        createCategoryLinks();
        updateSupplierLink();
        toastMsg.body = "Updating";
        toastMsg.show();
    }

    function createCategoryLinks() {
        var c = checkedCategories();
        for (var i = 0; i < c.length; ++ i) {
            if (updateCategory.indexOf(c[i]) == -1) {
                objectModel.createLink("Products", create ? idRow.getData() : updateModel.ID, "Categories", c[i]);
            }
        }
    }

    function removeCategoryLinks() {
        var c = checkedCategories();
        for (var i = 0; i < updateCategory.length; ++ i) {
            if (c.indexOf(updateCategory[i]) == -1) {
                objectModel.deleteLink("Products", updateModel.ID, "Categories", updateCategory[i]);
            }
        }
    }

    function createSupplierLink() {
        var option = supplierRow.selectedOption;
        if (option.value != supplierID) {
            if (option.value != -1) {
                objectModel.updateLink("Products", idRow.getData(), "Supplier", option.value, "Suppliers");
            }
        }
    }

    function updateSupplierLink() {
        var option = supplierRow.selectedOption;
        if (option.value != supplierID) {
            if (option.value == -1) {
                objectModel.deleteLink("Products", updateModel.ID, "Supplier");
            } else {
                objectModel.updateLink("Products", updateModel.ID, "Supplier", option.value, "Suppliers");
            }
        }
    }

    function readForm() {
        var model = modelStructure;

        for (var i = 0; i < model.length; i ++) {
            if (model[i].Name === "ID") {
                model[i].Data = idRow.getData();
            } else if (model[i].Name === "Name") {
                model[i].Data = productRow.getData();
            } else if (model[i].Name === "Description") {
                model[i].Data = descriptionRow.getData();
            } else if (model[i].Name === "ReleaseDate") {
                model[i].Data = releasedRow.getData();
            } else if (model[i].Name === "DiscontinuedDate") {
                model[i].Data = discontinuedRow.getData();
            } else if (model[i].Name === "Rating") {
                model[i].Data = ratingRow.getData();
            } else if (model[i].Name === "Price") {
                model[i].Data = costRow.getData();
            } else if (model[i].Name === "Supplier") {
                model[i].Data = supplierRow.selectedValue;
            } else if (model[i].Name === "Category") {
                model[i].Data = categoryRow.selectedValue;
            }
        }

        modelStructure = model;
    }

    function success() {
        toastMsg.body = "Success";
        toastMsg.show();

        var activeTab = tabPane.activeTab;
        if (activeTab.content.top == createEditProductPage)
            activeTab.content.pop();
    }

    function checkedCategories() {
        var list = [];
        var controls = categories.controls;
        for (var i = 0; i < controls.length; ++ i) {
            if (controls[i].checked) {
                for (var j = 0; j < categoriesList.childCount([]); ++ j) {
                    if (controls[i].text == categoriesList.data([ i ]).Name) {
                        list.push(categoriesList.data([ i ]).ID);
                        break;
                    }
                }
            }
        }
        return list;
    }
}
