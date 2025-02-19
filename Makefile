## Project
COMPONENT_NAME ?= pastry-crunch
export PROJ_ROOT := $(CURDIR)
SUBDIRS = tests
SRCDIR = $(PROJ_ROOT)/src
BUILDDIR = $(PROJ_ROOT)/build
TOOLSDIR = $(PROJ_ROOT)/tools
3RDPARTYDIR = $(PROJ_ROOT)/3rd-party

## 3rd-party settings
SDL_VERSION=2.0.8
SDL_PATH = $(3RDPARTYDIR)/SDL2-$(SDL_VERSION)
SDL_IMAGE_VERSION=2.0.1
SDL_IMAGE_PATH =$(3RDPARTYDIR)/SDL2_image-$(SDL_IMAGE_VERSION)
SDL_TTF_PATH =$(3RDPARTYDIR)/SDL2_ttf-2.0.14
SDL_FREETYPE_PATH =$(3RDPARTYDIR)/freetype-2.4.10
JSON_PATH = $(3RDPARTYDIR)/nlohmann
SDL_MIXER_VERSION=2.0.2
SDL_MIXER_PATH=$(3RDPARTYDIR)/SDL2_mixer-$(SDL_MIXER_VERSION)

INCLUDE_DIRS += -I$(PROJ_ROOT)/inc \
				-I$(SDL_PATH)/include/SDL2 \
				-I$(SDL_IMAGE_PATH)/include \
				-I$(SDL_TTF_PATH)/include/SDL2 \
				-I$(SDL_FREETYPE_PATH)/include \
				-I$(JSON_PATH)/include \
				-I$(SDL_MIXER_PATH)/include/SDL2

## Libs
LIBS = -lSDL2 -lSDL2main -lSDL2_image -lsndio -pthread -lSDL2_ttf -lfreetype -lSDL2_mixer
LIBS_PATH = -L$(BUILDDIR) -L$(SDL_PATH)/lib -L$(SDL_IMAGE_PATH)/lib -L$(3RDPARTYDIR)/sndio -L$(SDL_TTF_PATH)/lib -L$(SDL_FREETYPE_PATH)/lib -L$(SDL_MIXER_PATH)/lib
export LD_LIBRARY_PATH=$(SDL_PATH)/lib:$(3RDPARTYDIR)/sndio:$(SDL_IMAGE_PATH)/lib:$(BUILDDIR):$(SDL_TTF_PATH)/lib:$(SDL_FREETYPE_PATH)/lib:$(SDL_MIXER_PATH)/lib
export LD_LIBRARY_PATH_=$(LD_LIBRARY_PATH)

## Compiler
BUILD_TYPE ?= DEBUG
CXX = g++
STATIC = lib$(COMPONENT_NAME).a
DYNAMIC = lib$(COMPONENT_NAME).so
LDFLAGS = -shared
CXXFLAGS = -Wall -Winline -Werror -pipe -std=c++11 -fPIC
ifeq ($(BUILD_TYPE),DEBUG)
	CXXFLAGS += -g -O0
else ifeq ($(BUILD_TYPE),RELEASE)
	CXXFLAGS += -O3
endif

## Machine
CORES ?= $(shell nproc)
MAKEFLAGS+="-j $(CORES)"

## Sources
SRCS = 	App.cpp \
		Surface.cpp \
		Event.cpp \
		CAnimation.cpp \
		Entity.cpp \
		CCamera.cpp \
		Grid.cpp \
		Assets.cpp \
		Board.cpp \
		IThread.cpp \
		Text.cpp \
		Sounds.cpp \
		Button.cpp \
		CountDown.cpp \
		IUiComponent.cpp \
		AppStateManager.cpp \
		AppStateIntro.cpp \
		AppStateGame.cpp \
		AppStateGameOver.cpp \
		IAppState.cpp

HDRS = 	inc/App.h \
		inc/Surface.h \
		inc/Event.h \
		inc/CAnimation.h \
		inc/Entity.h \
		inc/CCamera.h \
		inc/Grid.h \
		inc/Assets.h \
		inc/Board.h \
		inc/IThread.h \
		inc/Text.h \
		inc/Sounds.h \
		inc/Button.h \
		inc/CountDown.h \
		inc/AppStateManager.h \
		inc/AppStateIntro.h \
		inc/IAppState.h \
		inc/AppStateGame.h \
		inc/AppStateGameOver.h

OBJS = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

## Make targets
.PHONY: all lint clang-format run clean unit-tests run-unit-tests shared

help:
	@echo
	@echo '  all                   - build and create main executable.'
	@echo '  lint                  - run clang formating for c++'
	@echo '  clang-format          - run clang-format on c++ files following rules specified in .clang-format.'
	@echo '  run                   - run main executable.'
	@echo '  clean                 - clean project.'
	@echo '  unit-tests            - build unit tests.'
	@echo '  run-unit-tests        - run unit tests.'
	@echo '  shared                - build shared library.'
	@echo

all: $(BUILDDIR) $(BUILDDIR)/$(COMPONENT_NAME)

run: $(BUILDDIR)/$(COMPONENT_NAME)
	$(BUILDDIR)/$(COMPONENT_NAME)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(COMPONENT_NAME): $(BUILDDIR)/main.o $(OBJS) $(HDRS)
	$(CXX) -o $@ $(BUILDDIR)/main.o $(OBJS) $(LIBS_PATH) $(LIBS)

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp $(HDRS)
	$(CXX) -o $@ $(INCLUDE_DIRS) -c $(CXXFLAGS) $(SRCDIR)/main.cpp

$(OBJS): $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@echo [Compile] $<
	@$(CXX) $(INCLUDE_DIRS) -c $(CXXFLAGS) $< -o $@

$(BUILDDIR)/$(STATIC): $(OBJS)
	@echo "[Link (Static)]"
	@ar rcs $@ $^

lint: clang-format

clang-format:
	find ./inc ./src -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format-5.0 -style=file -i {} \;

clean:
	rm -f $(OBJS)
	rm -f $(BUILDDIR)/$(COMPONENT_NAME)
	rm -f $(BUILDDIR)/main.o
	rm -f $(BUILDDIR)/$(STATIC)

unit-tests: shared
	$(MAKE) -C tests $@

run-unit-tests: unit-tests
	$(MAKE) -C tests $@

shared: $(BUILDDIR)/$(DYNAMIC)

$(BUILDDIR)/$(DYNAMIC): $(OBJS)
	@echo "[Link (Dynamic)]"
	$(CXX) ${LDFLAGS} -o $@ $^ $(LIBS_PATH) $(LIBS)
