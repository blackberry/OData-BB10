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

Page {
    id: productListPage
    property string title
    property string dataSource

    actions: [
        ActionItem {
            title: qsTr("Create")
            imageSource: "asset:///icons/ic_add.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                var activeTab = tabPane.activeTab;

                var createEditPage = createEditProductPage.createObject();
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

                model.readModel(dataSource);
            }
        },
        ActionItem {
            title: qsTr("< 10")
            imageSource: "asset:///icons/ic_info.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                toastMsg.body = "Showing products that cost less than 10";
                toastMsg.show();

                model.filterModel(dataSource, {"filter" : "Price lt 10"});
            }
        }
    ]

    Container {
        layout: StackLayout {
        }
        Label {
            text: title
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        ListView {
            id: productsList

            dataModel: ODataListModel {
                id: model
                service: dataService
            }
            onTriggered: {
                var selectedItem = dataModel.data(indexPath); // get the selected item
                var activeTab = tabPane.activeTab;

                var productDetails = productDetailsPage.createObject();
                productDetails.dataSource = productListPage.dataSource + "(" + selectedItem.ID + ")";
                activeTab.content.push(productDetails);
            }

            listItemComponents: [
                ListItemComponent {
                    StandardListItem {
                        title: ListItemData.Name
                        description: ListItemData.Description + " - Cost: " + ListItemData.Price
                    }
                }
            ]
        }
    }

    onDataSourceChanged: {
        if (dataSource)
            model.readModel(dataSource);
    }
}
