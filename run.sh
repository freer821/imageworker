#!/bin/bash
export OPENCV_PATH=/home/ubuntu/work/cprogramms/imageworker/opencvlibs
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib:$OPENCV_PATH/lib
make
./imageworker 25063_56256_200_DB_REF_20141010.jpg