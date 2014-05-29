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

NavigationPane {
    property string dataSource

    Page {

        Container {
            layout: StackLayout {
            }
            Label {
                text: qsTr("Suppliers")
                textStyle.base: SystemDefaults.TextStyles.BigText
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
            ListView {
                dataModel: ODataListModel {
                    id: model
                    service: dataService
                }
                onTriggered: {
                    var selectedItem = dataModel.data(indexPath); // get the selected item
                    var activeTab = tabPane.activeTab;

                    var supplierDetails = supplierDetailsPage.createObject();
                    supplierDetails.dataSource = dataSource + "(" + selectedItem.ID + ")";
                    activeTab.content.push(supplierDetails);
                }
                listItemComponents: [
                    ListItemComponent {
                        StandardListItem {
                            title: ListItemData.Name
                        }
                    }
                ]
                onCreationCompleted: {
                    if(dataSource) {
                        model.readModel(dataSource);
                    }
                }
            }
        }
    }

    onPopTransitionEnded: {
        page.destroy();
    }
}
