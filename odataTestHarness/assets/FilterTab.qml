import bb.cascades 1.0
import ODataLib 1.0 

NavigationPane {
    id: navBrowse
    Page {
        id: filterTab
        
        Container {
            id: rootContainer
            
            attachedObjects: [
                ArrayDataModel {
                    id: array;
                },
                
                ComponentDefinition { 
                    id: logicalFilter
                    source: "LogicalFilter.qml"
                },
                ComponentDefinition { 
                    id: arithmeticFilter
                    source: "ArithmeticFilter.qml"
                },
                ComponentDefinition { 
                    id: customFilter
                    source: "CustomFilter.qml"
                }
            ]
            
            Container {
                ListView {
                    dataModel: array;
                    
                    onTriggered: {
                        var newPage;
                        switch( array.data(indexPath)) {
                            case "LogicalFilter":
                                newPage = logicalFilter.createObject();
                                break;
                            case "ArithmeticFilter":
                                newPage = arithmeticFilter.createObject();
                                break;
                            case "CustomFilter":
                                newPage = customFilter.createObject();
                                break;
                            default:
                                break
                        }
                        navBrowse.push(newPage);
                    }
                
                }
            }
            
            onCreationCompleted: {
                array.append("LogicalFilter");
                array.append("ArithmeticFilter");
                array.append("CustomFilter");
            }
        
        
        }
    } //page
}
