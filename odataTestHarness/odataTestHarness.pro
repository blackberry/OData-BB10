TEMPLATE = app
APP_NAME = odataTestHarness
TARGET = odataTestHarness

CONFIG += qt warn_on debug_and_release cascades10

INCLUDEPATH += ../src
SOURCES += ../src/*.cpp
HEADERS += ../src/*.hpp ../src/*.h

#specific to oDataLib
						#INCLUDEPATH += $$BASEDIR/../oDataLib/src
						#SOURCES += $$BASEDIR/../oDataLib/src/*.cpp
						#HEADERS += $$BASEDIR/../oDataLib/src/*.h
DEPENDPATH += ../../oDataLib/src

device {
	CONFIG(release, debug|release) {
		LIBS += -L../../oDataLib/Device-Release
		DESTDIR = o.le-v7
	}
	CONFIG(debug, debug|release) {
		LIBS += -L../../oDataLib/Device-Debug
		DESTDIR = o.le-v7-g
	}
}

simulator {
	CONFIG(release, debug|release) {
		LIBS += -L../../oDataLib/Simulator-Release
		DESTDIR = o
	}
	CONFIG(debug, debug|release) {
		LIBS += -L../../oDataLib/Simulator-Debug
		DESTDIR = o-g
	}
}


LIBS += -Bstatic -loDataLib -Bdynamic -lbbsystem -lbbdata  \
 
OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

suredelete.target = sureclean
suredelete.commands = $(DEL_FILE) $${MOC_DIR}/*; $(DEL_FILE) $${RCC_DIR}/*; $(DEL_FILE) $${UI_DIR}/*
suredelete.depends = distclean

QMAKE_EXTRA_TARGETS += suredelete
 
include(config.pri) 
