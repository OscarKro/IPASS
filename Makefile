#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := WeatherStationDisplay.cpp BMP280.cpp KromWorksWeatherStation.cpp BoschBM.cpp

# header files in this project
HEADERS := EnvironmentReader.hpp WeatherStationDisplay.hpp BMP280.hpp KromWorksWeatherStation.hpp BoschBM.hpp 

# other places to look for files for this project
SEARCH  := 

# settings for Arduino Due projects
TARGET            := teensy_40
CONSOLE_BAUDRATE  := 115200

# the location of the ti software directory
RELATIVE          ?= .
TI-SOFTWARE       := $(RELATIVE)/..

# add hwlib
HWLIB             ?= $(TI-SOFTWARE)/hwlib
BMPTK             ?= $(TI-SOFTWARE)/bmptk
include           $(HWLIB)/makefile.inc
