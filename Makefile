all: TestMain.exe

default: TestMain.exe

TestMain.exe: ./test/TestMain.cpp ./test/test-classes/Test.h ./test/test-classes/TestState.h ./test/test-classes/ModeTest.h ./test/test-classes/ModeTest.cpp ./src/Chrono.h
	g++ -o TestMain.exe ./test/TestMain.cpp ./test/test-classes/Test.h ./test/test-classes/TestState.h ./test/test-classes/ModeTest.h ./test/test-classes/ModeTest.cpp ./src/Chrono.h --std=c++11

clean:
	rm -rf *.o
	rm -rf ./src/*.o
	rm -rf ./test/*.o
	rm -rf *.exe
