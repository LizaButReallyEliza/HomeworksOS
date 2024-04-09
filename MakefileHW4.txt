run: phi
	./phi

phi: main.cpp
	g++ main.cpp -o phi -pthread

clean:
	rm -f *.o phi


