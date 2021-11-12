#!/bin/bash

PREFIX=/home/taste/tool-inst
SOURCES=$(dirname $0)

mkdir -p "${PREFIX}/include/TASTE-SAMV71-Drivers/src"
rm -rf "${PREFIX}/include/TASTE-SAMV71-Drivers/src/*"
cp -r "${SOURCES}/src/samv71_serial_ccsds" "${PREFIX}/include/TASTE-SAMV71-Drivers/src"
cp -r "${SOURCES}/configurations" "${PREFIX}/include/TASTE-SAMV71-Drivers/configurations"
