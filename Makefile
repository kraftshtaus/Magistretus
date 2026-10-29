
# =====================================
# MAGISTRETUS - BUILD CONFIGURATION
# =====================================

PROJECT_NAME := game

# Paths
RAYLIB_PATH ?= ../..
COMPILER_PATH ?= C:/raylib/w64devkit/bin

export PATH := $(COMPILER_PATH):$(PATH)

# Compiler
CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2 -DPLATFORM_DESKTOP

# raylib
INCLUDES := -I$(RAYLIB_PATH)/src
LIBPATH := -L$(RAYLIB_PATH)/src
LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm

# Automatically find all C++ source files
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCES := main.cpp $(call rwildcard,src/,*.cpp)

TARGET := $(PROJECT_NAME).exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $@ $(LIBPATH) $(LIBS)

clean:
	-cmd /C del /Q "$(TARGET)"
