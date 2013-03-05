import bb.cascades 1.0

Container {
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    horizontalAlignment: HorizontalAlignment.Fill
    Container {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center
        Label {
            text: qsTr("Loading...")
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            leftPadding: 0.0
            topPadding: 10.0
            bottomPadding: 10.0
            textStyle {
                textAlign: TextAlign.Center
            }
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        leftPadding: 30
        topPadding: 20
        bottomPadding: 20
        ActivityIndicator {
            id: loadingIndicator
            preferredWidth: 100
            preferredHeight: 100
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            running: true
            visible: true
        }
    }
    Container {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
    }
}
