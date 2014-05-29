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
    property string dataSource
    property variant dataModel

    attachedObjects: [
        ODataObjectModel {
            id: odataModel
            service: dataService
        }
    ]

    content: Container {
        layout: StackLayout {

        }
        Label {
            text: "Supplier Details"
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        DetailsItemRow {
            label: qsTr("Name")
            data: dataModel.Name
        }
        DetailsItemRow {
            label: qsTr("Address")
            data: dataModel.Address.Street
        }
        DetailsItemRow {
            label: qsTr("")
            data: dataModel.Address.City
        }
        DetailsItemRow {
            label: qsTr("")
            data: dataModel.Address.State
        }
        DetailsItemRow {
            label: qsTr("")
            data: dataModel.Address.ZipCode
        }
        DetailsItemRow {
            label: qsTr("")
            data: dataModel.Address.Country
        }
        Button {
            topMargin: 30
            text: qsTr("View Products")
            onClicked: {
                var activeTab = tabPane.activeTab;

                var productList = productListPage.createObject();
                productList.title = dataModel.Name + " - " + qsTr("Products");
                productList.dataSource = "/Suppliers(" + dataModel.ID + ")/Products";
                activeTab.content.push(productList);
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
    }

    onCreationCompleted: {
        odataModel.modelReady.connect(bindToDataModel);
    }

    onDataSourceChanged: {
        if(dataSource) {
            odataModel.readModel(dataSource);
        }
    }

    function bindToDataModel() {
        dataModel = odataModel.model;
    }
}