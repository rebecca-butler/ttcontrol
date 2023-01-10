all: helloworld

libfoo.so: foo.cpp
	g++ -I C:\Eigen foo.cpp -static-libstdc++ -shared -fPIC -o libfoo.so

helloworld: main.c libfoo.so
	gcc main.c -L. -l:libfoo.so -o main
