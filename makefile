CC = g++
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -std=c++17
INCLUDES =-I${OPENCV_PATH}/include
LIBS =-L${OPENCV_PATH}/lib  -lopencv_dnn -lopencv_ml -lopencv_objdetect -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_core

imageworker: ImageWorker.cpp PointsClassifier.o
	$(CC) $(CFLAGS) -o imageworker ImageWorker.cpp PointsClassifier.o $(INCLUDES) $(LIBS)

PointsClassifier.o: PointsClassifier.cpp PointsClassifier.h
	$(CC) $(CFLAGS) -c PointsClassifier.cpp $(INCLUDES) $(LIBS)