ver = release
CXXFLAGS = -O2 -std=gnu++11

log = false

ifeq ($(ver), debug)
	CXXFLAGS = -D_DEBUG -g3 -std=gnu++11
endif
ifeq ($(ver), profile)
	CXXFLAGS = -D_PROFILE -g3 -pg -std=gnu++11
endif

ifeq ($(log), true)
	CXXFLAGS := $(CXXFLAGS) -D_LOG
endif


main: main.o card.o ljh.o wzp.o cwy.o
	g++ -o main main.o card.o ljh.o wzp.o cwy.o $(CXXFLAGS)

main.o: main.cpp card.h card.hpp main.h
	g++ -c main.cpp $(CXXFLAGS)

card.o: card.cpp card.h card.hpp
	g++ -c card.cpp $(CXXFLAGS)

ljh.o: ljh.cpp card.h card.hpp main.h
	g++ -c ljh.cpp $(CXXFLAGS)

wzp.o: wzp.cpp card.h card.hpp main.h
	g++ -c wzp.cpp $(CXXFLAGS)

cwy.o: ljh.cpp card.h card.hpp main.h
	g++ -c cwy.cpp $(CXXFLAGS)

clean:
	rm main main.o card.o ljh.o wzp.o cwy.o
