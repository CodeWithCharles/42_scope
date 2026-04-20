# ============================================================================
# Makefile.msg
# ----------------------------------------------------------------------------
# FR: Messages centralises pour garder le Makefile principal lisible.
# EN: Centralized messages to keep the main Makefile readable.
# ============================================================================

MSG_RUN         = $(TERM_CYAN)Running %s from %s$(TERM_RESET)
MSG_TEST_RUN    = $(TERM_CYAN)Running smoke test for %s$(TERM_RESET)
MSG_TEST_OK     = $(TERM_GREEN)SCOP build OK$(TERM_RESET)
MSG_TEST_ERR    = $(TERM_RED)SCOP build ERROR (exit code %s)$(TERM_RESET)
MSG_TEST_SKIP   = $(TERM_YELLOW)No runnable binary found at %s; smoke test skipped.$(TERM_RESET)
MSG_LIBS        = $(TERM_CYAN)Building local third-party libraries inside %s$(TERM_RESET)
MSG_FETCH_GLFW  = $(TERM_CYAN)Fetching GLFW into %s$(TERM_RESET)
MSG_CFG_GLFW    = $(TERM_CYAN)Configuring GLFW (%s) in %s$(TERM_RESET)
MSG_BUILD_GLFW  = $(TERM_CYAN)Building and installing GLFW locally into %s$(TERM_RESET)
MSG_CHECK_GLAD  = $(TERM_CYAN)Checking GLAD files inside %s$(TERM_RESET)
MSG_GLAD_ERR    = $(TERM_RED)Missing GLAD files in %s. Expected glad.c and generated headers under include/ and src/.$(TERM_RESET)
MSG_DOCTOR      = $(TERM_CYAN)Running project diagnostics...$(TERM_RESET)
MSG_DOCTOR_OK   = $(TERM_GREEN)[OK] %s$(TERM_RESET)
MSG_DOCTOR_WARN = $(TERM_YELLOW)[WARN] %s$(TERM_RESET)
MSG_DOCTOR_ERR  = $(TERM_RED)[KO] %s$(TERM_RESET)
MSG_DOCTOR_DONE = $(TERM_GREEN)Doctor finished: environment looks good.$(TERM_RESET)
MSG_DOCTOR_FAIL = $(TERM_RED)Doctor finished: at least one check failed.$(TERM_RESET)
MSG_DEPS        = $(TERM_CYAN)Refreshing dependency files for %s build...$(TERM_RESET)
MSG_CLEAN       = $(TERM_YELLOW)Cleaning objects, dependencies, binaries and copied assets...$(TERM_RESET)
MSG_DISTCLEAN   = $(TERM_YELLOW)Removing build directory and local dependency artefacts...$(TERM_RESET)
MSG_FORMAT      = $(TERM_CYAN)Formatting sources with clang-format...$(TERM_RESET)
MSG_FORMAT_EMPTY = $(TERM_YELLOW)No source/header files found to format.$(TERM_RESET)
MSG_FORMAT_SKIP = $(TERM_YELLOW)clang-format not found; skipping format target.$(TERM_RESET)
MSG_INSTALL     = $(TERM_CYAN)Installing %s into %s$(TERM_RESET)
MSG_NO_SOURCES  = $(TERM_RED)Error: no C/C++ sources found under %s$(TERM_RESET)
MSG_COPY_ASSETS = $(TERM_CYAN)Copying assets from %s to %s$(TERM_RESET)
MSG_NO_ASSETS   = $(TERM_YELLOW)No asset directory found at %s; skipping asset copy.$(TERM_RESET)
MSG_LINK        = $(TERM_MAGENTA)Linking %s$(TERM_RESET)
MSG_CXX         = $(TERM_BLUE)CXX %s$(TERM_RESET)
MSG_CC          = $(TERM_BLUE)CC  %s$(TERM_RESET)

HELP_LINES = \
'Usage / Utilisation:' \
'  make                 Build default profile (debug) / compile le profil debug' \
'  make doctor          Check toolchain and local deps / verifie l environnement' \
'  make libs            Build external deps in deps/ / compile les deps en local' \
'  make run             Build then run / compile puis lance le programme' \
'  make debug           Force debug flags (-g -O0) / force les flags debug' \
'  make release         Force release flags (-O3 -DNDEBUG) / force le release' \
'  make BUILD_TYPE=release run      Run the release build / lance le build release' \
'  make clean           Remove objects, binary and copied assets / supprime les artefacts' \
'  make distclean       Also remove dependency files / supprime aussi le dossier build' \
'  make format          Run clang-format if available / lance clang-format si dispo' \
'  make install         Install current BUILD_TYPE / installe le BUILD_TYPE courant' \
'  make test            Run a minimal smoke test / lance un test minimal' \
'' \
'Configurable variables / Variables configurables:' \
'  CC, CXX, CFLAGS, CXXFLAGS, LDFLAGS, INCLUDES, LIBS' \
'  BUILD_DIR, SRC_DIR, INC_DIR, ASSET_DIR, SHADER_DIR, OBJ_DIR, BIN_NAME' \
'  DEPS_DIR, LOCAL_DIR, GLFW_SRC_DIR, GLFW_BUILD_DIR, GLFW_GIT, GLAD_DIR' \
'  PREFIX (default: project-local), DESTDIR, VERBOSE=1 (or V=1)'
