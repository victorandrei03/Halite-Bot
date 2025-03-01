build: MyBot

MyBot: MyBot.cpp
	g++ -std=c++11 MyBot.cpp -o MyBot

run: MyBot
	./MyBot

clean:
	rm -f MyBot