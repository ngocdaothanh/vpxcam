all:
	g++ `pkg-config --cflags --libs opencv vpx` -lboost_system -lboost_thread-mt src/cam.cpp src/encode.cpp src/main.cpp -o vpxcams
