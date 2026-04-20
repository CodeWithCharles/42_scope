# ============================================================================
# SCOP / GLFW / GLAD / OpenGL 4.1 - GNU Makefile
# ----------------------------------------------------------------------------
# Aide rapide / Quick help:
#   make          : compile le profil par defaut (debug) / build default profile
#   make run      : compile puis lance l'executable / build then run the binary
#   make debug    : force un build debug (-g, -O0) / force debug flags
#   make release  : force un build release (-O3, strip si disponible)
#   make clean    : supprime objets + binaire + assets copies / remove artefacts
#
# FR: Makefile principal leger, inspire du split de minishell.
# EN: Lightweight top-level Makefile, inspired by the minishell split layout.
# ============================================================================

include Makefile.colors
include Makefile.vars
include Files.mk
include Makefile.msg

.DEFAULT_GOAL := all
.SUFFIXES:
.DELETE_ON_ERROR:

all: libs check-sources $(TARGET) copy-assets

libs: check-glad $(LOCAL_LIBS)
	$(Q)printf '$(MSG_LIBS)\n' "$(LOCAL_DIR)"

debug:
	$(Q)$(MAKE) --no-print-directory BUILD_TYPE=debug VERBOSE="$(VERBOSE)" V="$(V)" all

release:
	$(Q)$(MAKE) --no-print-directory BUILD_TYPE=release VERBOSE="$(VERBOSE)" V="$(V)" all

doctor:
	$(Q)set -eu; \
	status=0; \
	printf '$(MSG_DOCTOR)\n'; \
	check_cmd() { \
		label="$$1"; \
		value="$$2"; \
		set -- $$value; \
		cmd="$$1"; \
		if command -v "$$cmd" >/dev/null 2>&1; then \
			printf '$(MSG_DOCTOR_OK)\n' "$$label: $$cmd"; \
		else \
			printf '$(MSG_DOCTOR_ERR)\n' "$$label missing: $$cmd" >&2; \
			status=1; \
		fi; \
	}; \
	check_file() { \
		label="$$1"; \
		path="$$2"; \
		if [ -e "$$path" ]; then \
			printf '$(MSG_DOCTOR_OK)\n' "$$label: $$path"; \
		else \
			printf '$(MSG_DOCTOR_ERR)\n' "$$label missing: $$path" >&2; \
			status=1; \
		fi; \
	}; \
	check_cmd "C compiler" "$(CC)"; \
	check_cmd "C++ compiler" "$(CXX)"; \
	check_cmd "CMake" "$(CMAKE)"; \
	check_cmd "Git" "$(GIT)"; \
	check_file "GLAD source" "$(GLAD_SOURCE)"; \
	check_file "GLAD header" "$(GLAD_HEADER)"; \
	check_file "KHR header" "$(GLAD_KHR_HEADER)"; \
	check_file "GLFW local library" "$(GLFW_LOCAL_LIB)"; \
	check_file "GLFW local header" "$(LOCAL_INCLUDE_DIR)/GLFW/glfw3.h"; \
	check_file "GLFW source tree" "$(GLFW_SRC_DIR)/CMakeLists.txt"; \
	mkdir -p "$(DOCTOR_DIR)"; \
	printf '%s\n' 'int main(void) { return 0; }' > "$(DOCTOR_DIR)/link_test.c"; \
	if [ -n "$(strip $(DOCTOR_SYS_LIBS))" ]; then \
		if $(CC) $(CPPFLAGS) $(CFLAGS) "$(DOCTOR_DIR)/link_test.c" $(DOCTOR_SYS_LIBS) -o "$(DOCTOR_DIR)/link_test" >/dev/null 2>&1; then \
			printf '$(MSG_DOCTOR_OK)\n' "system OpenGL/windowing link test"; \
		else \
			printf '$(MSG_DOCTOR_ERR)\n' "system OpenGL/windowing link test failed" >&2; \
			status=1; \
		fi; \
	else \
		printf '$(MSG_DOCTOR_WARN)\n' "no platform-specific link test configured for $(UNAME_S)"; \
	fi; \
	rm -f "$(DOCTOR_DIR)/link_test.c" "$(DOCTOR_DIR)/link_test"; \
	if [ $$status -eq 0 ]; then \
		printf '$(MSG_DOCTOR_DONE)\n'; \
	else \
		printf '$(MSG_DOCTOR_FAIL)\n' >&2; \
		exit $$status; \
	fi

run: all
	$(Q)printf '$(MSG_RUN)\n' "$(BIN_NAME)" "$(BUILD_MODE_DIR)"
	$(Q)cd "$(BUILD_MODE_DIR)" && "./$(BIN_NAME)"

test: all
	$(Q)if [ -x "$(TARGET)" ]; then \
		printf '$(MSG_TEST_RUN)\n' "$(BIN_NAME)"; \
		cd "$(BUILD_MODE_DIR)" && "./$(BIN_NAME)"; \
		status=$$?; \
		if [ $$status -eq 0 ]; then \
			printf '$(MSG_TEST_OK)\n'; \
		else \
			printf '$(MSG_TEST_ERR)\n' "$$status" >&2; \
			exit $$status; \
		fi; \
	else \
		printf '$(MSG_TEST_SKIP)\n' "$(TARGET)"; \
	fi

deps: check-sources
	$(Q)printf '$(MSG_DEPS)\n' "$(BUILD_TYPE)"
	$(Q)$(MAKE) --no-print-directory -B BUILD_TYPE="$(BUILD_TYPE)" VERBOSE="$(VERBOSE)" V="$(V)" $(OBJECTS)

rebuild: clean all

clean:
	$(Q)printf '$(MSG_CLEAN)\n'
	$(Q)rm -rf "$(BUILD_DIR)/debug/obj" "$(BUILD_DIR)/debug/dep" "$(BUILD_DIR)/debug/$(BIN_NAME)" "$(BUILD_DIR)/debug/assets"
	$(Q)rm -rf "$(BUILD_DIR)/release/obj" "$(BUILD_DIR)/release/dep" "$(BUILD_DIR)/release/$(BIN_NAME)" "$(BUILD_DIR)/release/assets"

distclean: clean
	$(Q)printf '$(MSG_DISTCLEAN)\n'
	$(Q)rm -rf "$(BUILD_DIR)" "$(LOCAL_DIR)" "$(DEPS_DIR)/build"

format:
	$(Q)if command -v clang-format >/dev/null 2>&1; then \
		files="$$(find "$(SRC_DIR)" "$(INC_DIR)" -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) 2>/dev/null)"; \
		if [ -n "$$files" ]; then \
			printf '$(MSG_FORMAT)\n'; \
			clang-format -i $$files; \
		else \
			printf '$(MSG_FORMAT_EMPTY)\n'; \
		fi; \
	else \
		printf '$(MSG_FORMAT_SKIP)\n'; \
	fi

install: all
	$(Q)printf '$(MSG_INSTALL)\n' "$(BIN_NAME)" "$(DESTDIR)$(PREFIX)"
	$(Q)$(INSTALL) -d "$(INSTALL_BIN_DIR)" "$(INSTALL_SHARE_DIR)"
	$(Q)$(INSTALL) -m 0755 "$(TARGET)" "$(INSTALL_BIN_DIR)/$(BIN_NAME)"
	$(Q)if [ -d "$(ASSET_BUILD_DIR)" ]; then \
		mkdir -p "$(INSTALL_SHARE_DIR)/assets"; \
		cp -R "$(ASSET_BUILD_DIR)/." "$(INSTALL_SHARE_DIR)/assets/"; \
	fi

help:
	@printf '%s\n' $(HELP_LINES)

check-sources:
	$(Q)if [ -z "$(strip $(SOURCES))" ]; then \
		printf '$(MSG_NO_SOURCES)\n' "$(SRC_DIR)" >&2; \
		exit 1; \
	fi

check-glad:
	$(Q)printf '$(MSG_CHECK_GLAD)\n' "$(GLAD_DIR)"
	$(Q)if [ ! -f "$(GLAD_SOURCE)" ] || [ ! -f "$(GLAD_HEADER)" ] || [ ! -f "$(GLAD_KHR_HEADER)" ]; then \
		printf '$(MSG_GLAD_ERR)\n' "$(GLAD_DIR)" >&2; \
		exit 1; \
	fi

copy-assets:
	$(Q)mkdir -p "$(BUILD_MODE_DIR)"
	$(Q)if [ -d "$(ASSET_DIR)" ]; then \
		printf '$(MSG_COPY_ASSETS)\n' "$(ASSET_DIR)" "$(ASSET_BUILD_DIR)"; \
		rm -rf "$(ASSET_BUILD_DIR)"; \
		mkdir -p "$(ASSET_BUILD_DIR)"; \
		cp -R "$(ASSET_DIR)/." "$(ASSET_BUILD_DIR)/"; \
	else \
		printf '$(MSG_NO_ASSETS)\n' "$(ASSET_DIR)"; \
	fi

$(GLFW_SRC_DIR)/CMakeLists.txt:
	$(Q)printf '$(MSG_FETCH_GLFW)\n' "$(GLFW_SRC_DIR)"
	$(Q)mkdir -p "$(DEPS_DIR)"
	$(Q)if [ ! -d "$(GLFW_SRC_DIR)/.git" ] && [ ! -f "$(GLFW_SRC_DIR)/CMakeLists.txt" ]; then \
		$(GIT) clone --depth 1 "$(GLFW_GIT)" "$(GLFW_SRC_DIR)"; \
	fi

$(GLFW_BUILD_DIR)/CMakeCache.txt: $(GLFW_SRC_DIR)/CMakeLists.txt
	$(Q)printf '$(MSG_CFG_GLFW)\n' "$(GLFW_CMAKE_BUILD_TYPE)" "$(GLFW_BUILD_DIR)"
	$(Q)mkdir -p "$(GLFW_BUILD_DIR)" "$(LOCAL_DIR)"
	$(Q)$(CMAKE) -S "$(GLFW_SRC_DIR)" -B "$(GLFW_BUILD_DIR)" \
		-DCMAKE_BUILD_TYPE="$(GLFW_CMAKE_BUILD_TYPE)" \
		-DCMAKE_INSTALL_PREFIX="$(LOCAL_DIR)" \
		-DCMAKE_INSTALL_INCLUDEDIR=include \
		-DCMAKE_INSTALL_LIBDIR=lib \
		-DGLFW_BUILD_DOCS=OFF \
		-DGLFW_BUILD_TESTS=OFF \
		-DGLFW_BUILD_EXAMPLES=OFF \
		-DBUILD_SHARED_LIBS=OFF

$(GLFW_LOCAL_LIB): $(GLFW_BUILD_DIR)/CMakeCache.txt
	$(Q)printf '$(MSG_BUILD_GLFW)\n' "$(LOCAL_DIR)"
	$(Q)$(CMAKE) --build "$(GLFW_BUILD_DIR)" --parallel
	$(Q)$(CMAKE) --install "$(GLFW_BUILD_DIR)"

$(TARGET): $(OBJECTS) $(LOCAL_LIBS)
	$(Q)printf '$(MSG_LINK)\n' "$@"
	$(Q)mkdir -p "$(BUILD_MODE_DIR)"
	$(Q)$(CXX) $(OBJECTS) $(LDFLAGS) $(BUILD_LDFLAGS) $(LIBS) -o "$@"
	$(Q)if [ "$(BUILD_TYPE)" = "release" ] && command -v strip >/dev/null 2>&1; then \
		strip "$@" || true; \
	fi

$(GLAD_OBJECT): $(GLAD_SOURCE) $(GLAD_HEADER) $(GLAD_KHR_HEADER)
	$(Q)printf '$(MSG_CC)\n' "$<"
	$(Q)mkdir -p "$(dir $@)" "$(dir $(call depfile,$@))"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) $(BUILD_CFLAGS) -MMD -MP -MF "$(call depfile,$@)" -c "$<" -o "$@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(Q)printf '$(MSG_CXX)\n' "$<"
	$(Q)mkdir -p "$(dir $@)" "$(dir $(call depfile,$@))"
	$(Q)$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(BUILD_CXXFLAGS) -MMD -MP -MF "$(call depfile,$@)" -c "$<" -o "$@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(Q)printf '$(MSG_CC)\n' "$<"
	$(Q)mkdir -p "$(dir $@)" "$(dir $(call depfile,$@))"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) $(BUILD_CFLAGS) -MMD -MP -MF "$(call depfile,$@)" -c "$<" -o "$@"

-include $(DEPS)

.PHONY: all libs debug release doctor run test deps rebuild clean distclean format install help check-sources check-glad copy-assets
