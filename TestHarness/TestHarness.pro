APP_NAME = TestHarness

CONFIG += qt warn_on cascades10

INCLUDEPATH += ../../OData/Device-Debug/public

include(config.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}

LIBS += -Bstatic -L../../OData/Device-Debug/ -lOData -Bdynamic
LIBS += -lbbdata