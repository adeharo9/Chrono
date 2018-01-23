# DIRECTORIES
SRC_DIR = ./src
TST_DIR = ./test
TST_CLASS_DIR = $(TST_DIR)/test-classes

# EXTENSIONS
HDR_EXT = .h
SRC_EXT = .cpp
OBJ_EXT = .o
EXE_EXT = .exe

# EXECUTABLES
CP = g++
RM = rm

# FLAGS
CP_OPTIONS = -o
CP_FLAGS = --std=c++11

RM_FLAGS = -rf

# FILE PATHS
EXE_FILE = TestMain$(EXE_EXT)

HEADER_FILES = \
	$(SRC_DIR)/Chrono$(HDR_EXT)

TEST_HEADER_FILES = \
	$(TST_CLASS_DIR)/Test$(HDR_EXT) \
	$(TST_CLASS_DIR)/TestState$(HDR_EXT) \
	$(TST_CLASS_DIR)/ModeTest$(HDR_EXT)

TEST_SOURCE_FILES = \
	$(TST_DIR)/TestMain$(SRC_EXT) \
	$(TST_CLASS_DIR)/ModeTest$(SRC_EXT)

ALL_FILES = \
	$(HEADER_FILES) \
	$(TEST_HEADER_FILES) \
	$(TEST_SOURCE_FILES)

# COMMANDS
default: $(EXE_FILE)

all: $(EXE_FILE)

$(EXE_FILE): $(ALL_FILES)
	$(CP) $(CP_OPTIONS) $(EXE_FILE) $(ALL_FILES) $(CP_FLAGS)

tests: $(EXE_FILE)

clean:
	$(RM) $(RM_FLAGS) *$(OBJ_EXT)
	$(RM) $(RM_FLAGS) $(SRC_DIR)/*$(OBJ_EXT)
	$(RM) $(RM_FLAGS) $(TST_DIR)/*$(OBJ_EXT)
	$(RM) $(RM_FLAGS) $(TST_CLASS_DIR)/*$(OBJ_EXT)
	$(RM) $(RM_FLAGS) *$(EXE_EXT)

.PHONY: clean
