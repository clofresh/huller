huller: huller.cpp
	g++ $(shell pkg-config --cflags --libs opencv) huller.cpp -o huller

clean:
	rm -f huller
