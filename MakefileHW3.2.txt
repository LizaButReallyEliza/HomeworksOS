run: smoke
	./smoke

smoke: smokers.cpp
	g++ smokers.cpp -o smoke -pthread

clean:
	rm -f *.o smoke

