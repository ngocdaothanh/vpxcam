all:
	g++ `pkg-config --cflags --libs opencv vpx` src/cam.cpp src/encode.cpp src/main.cpp -o vpxcam
