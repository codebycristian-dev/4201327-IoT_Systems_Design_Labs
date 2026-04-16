# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.3.1/components/bootloader/subproject"
  "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader"
  "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix"
  "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix/tmp"
  "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix/src/bootloader-stamp"
  "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix/src"
  "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/4201327-IoT_Systems_Design_Labs/Labs/simple/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
