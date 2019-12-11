out:main.cpp
		g++ test.cpp -std=c++11 -o main `pkg-config --cflags --libs opencv4`
