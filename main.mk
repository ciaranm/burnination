BUILD_DIR := intermediate
TARGET_DIR := ./

override CXXFLAGS += -O3 -march=native -std=c++17 -W -Wall -g -ggdb3 -pthread

TARGET := burnination

SOURCES := \
    burnination.cc

TGT_POSTMAKE := bash ./run-tests.bash

