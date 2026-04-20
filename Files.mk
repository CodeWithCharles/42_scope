# ============================================================================
# Files.mk
# ----------------------------------------------------------------------------
# FR: Decouverte automatique des sources et generation des listes d'objets.
# EN: Automatic source discovery and object/dependency list generation.
# ============================================================================

SOURCES := $(shell [ -d "$(SRC_DIR)" ] && find "$(SRC_DIR)" -type f \( -name "*.cpp" -o -name "*.c" \) | LC_ALL=C sort)
CPP_SOURCES := $(filter %.cpp,$(SOURCES))
C_SOURCES   := $(filter %.c,$(SOURCES))
HEADERS := $(shell [ -d "$(INC_DIR)" ] && find "$(INC_DIR)" -type f \( -name "*.h" -o -name "*.hpp" \) | LC_ALL=C sort)
GLAD_HEADERS := $(shell [ -d "$(GLAD_INC_DIR)" ] && find "$(GLAD_INC_DIR)" -type f \( -name "*.h" -o -name "*.hpp" \) | LC_ALL=C sort)
MAIN_SOURCES := $(shell [ -d "$(SRC_DIR)" ] && find "$(SRC_DIR)" -type f \( -name "main.cpp" -o -name "main.c" \) | LC_ALL=C sort)

CPP_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
C_OBJECTS   = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
GLAD_OBJECT = $(OBJ_DIR)/deps/glad/glad.o
OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS) $(if $(wildcard $(GLAD_SOURCE)),$(GLAD_OBJECT))
DEPS = $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.d,$(OBJECTS))

depfile = $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.d,$(1))
