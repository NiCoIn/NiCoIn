#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/nicoin.png
ICON_DST=../../src/qt/res/icons/nicoin.ico
convert ${ICON_SRC} -resize 16x16 nicoin-16.png
convert ${ICON_SRC} -resize 32x32 nicoin-32.png
convert ${ICON_SRC} -resize 48x48 nicoin-48.png
convert nicoin-16.png nicoin-32.png nicoin-48.png ${ICON_DST}

