QMAKE_TARGET  = TestHarness
PROJECT_DIR	  := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
I18N_DIR	  := $(PROJECT_DIR)/translations

QMAKEFEATURES = $(CURDIR)/../OData
export QMAKEFEATURES

include mk/cs-base.mk

