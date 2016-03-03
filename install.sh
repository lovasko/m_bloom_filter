#!/bin/sh

INC_DIR=/usr/include
LIB_DIR=/usr/lib

cp -v bin/libmbloomfilter.so "${LIB_DIR}"
cp -v src/m_bloom_filter.h "${INC_DIR}"

