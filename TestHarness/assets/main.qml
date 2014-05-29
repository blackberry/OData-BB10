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
import bb.system 1.0
import "tabs" 1.0
import "pages" 1.0

TabbedPane {
    id: tabPane
    showTabsOnActionBar: false
    
    attachedObjects: [
        ComponentDefinition {
            id: productDetailsPage
            source: "asset:///pages/ProductDetails.qml"
        },
        ComponentDefinition {
            id: productListPage
            source: "asset:///pages/ProductList.qml"
        },
        ComponentDefinition {
            id: supplierDetailsPage
            source: "asset:///pages/SupplierDetails.qml"
        },
        ComponentDefinition {
            id: createEditProductPage
            source: "asset:///pages/CreateEditProduct.qml"
        },
        ComponentDefinition {
            id: aboutPageDef
            source: "asset:///pages/AboutPage.qml"
        },
        SystemToast {
            id: toastMsg
        }
    ]

	Tab {
	    title: qsTr("Products")
	    content: ProductsListTab {
	        title: qsTr("Products")
	        dataSource: "/Products"
        }
	    imageSource: "asset:///icons/ic_view_list.png"
	}
    Tab {
        title: qsTr("Categories")
        content: CategoriesListTab {
            dataSource: "/Categories"
        }
        imageSource: "asset:///icons/ic_info.png"
    }
    Tab {
        title: qsTr("Suppliers")
        content: SuppliersListTab {
            dataSource: "/Suppliers"
        }
        imageSource: "asset:///icons/ic_map.png"
    }

    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///icons/ic_about.png"
                onTriggered: {
                    var sheet  = aboutPageDef.createObject();
                    sheet.open();
                }
            }
        ]
    }

    function parseISO8601(str) {
        try {
            var _date = new Date();
            var parts = str.split('T'),
            dateParts = parts[0].split('-'),
            timeParts = parts[1].split('-'),
            timeSubParts = timeParts[0].split(':');

            _date.setFullYear(Number(dateParts[0]));
            _date.setMonth(Number(dateParts[1])-1);
            _date.setDate(Number(dateParts[2]));
            _date.setHours(Number(timeSubParts[0]));
            _date.setMinutes(Number(timeSubParts[1]));

            return Qt.formatDateTime(_date, "dd/MM/yyyy");
        }
        catch (error) {
            return null;
        }
    }
}