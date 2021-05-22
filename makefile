main: main.o card.o ljh.o wzp.o cwy.o
	g++ -o main main.o card.o ljh.o wzp.o cwy.o -O2 -std=gnu++11

main.o: main.cpp card.h card.hpp main.h
	g++ -c main.cpp -O2 -std=gnu++11

card.o: card.cpp card.h card.hpp
	g++ -c card.cpp -O2 -std=gnu++11

ljh.o: ljh.cpp card.h card.hpp main.h
	g++ -c ljh.cpp -O2 -std=gnu++11

wzp.o: wzp.cpp card.h card.hpp main.h
	g++ -c wzp.cpp -O2 -std=gnu++11

cwy.o: ljh.cpp card.h card.hpp main.h
	g++ -c cwy.cpp -O2 -std=gnu++11

clean:
	rm main main.o card.o ljh.o wzp.o cwy.o
