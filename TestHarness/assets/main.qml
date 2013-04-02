// Default empty project template
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
        SystemToast {
            id: toastMsg
        }
    ]

	Tab {
	    title: qsTr("Products")
	    content: ProductsListTab {
	        title: qsTr("Products")
	        dataSource: dataService.source + "/Products"
        }
	}
    Tab {
        title: qsTr("Categories")
        content: CategoriesListTab {
            dataSource: dataService.source + "/Categories"
        }
    }
    Tab {
        title: qsTr("Suppliers")
        content: SuppliersListTab {
            dataSource: dataService.source + "/Suppliers"
        }
    }
}