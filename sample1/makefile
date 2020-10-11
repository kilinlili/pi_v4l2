
all:v4l2_for_opencv


#C_SRC := $(wildcard *.c)
#OBJ := $(subst .c, .o, $(C_SRC))

v4l2_for_opencv: main.o camconfig.o
	$(CXX) -Wall -O0 -g -o v4l2_run $^ $(shell pkg-config --libs --cflags opencv tbb libv4l2)

.cpp.o:
	$(CXX) -c $<

.PHONY:clean
clean:
	rm *.o
	rm v4l2_run
