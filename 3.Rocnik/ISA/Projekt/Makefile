all: myldap

myldap: *.cc
	g++ -std=c++11 $^ -lpthread -O0 -o myldap

debug: *.cc
	g++ -std=c++11 -DNDEBUG $^ -lpthread -O0 -o myldap

clean:
	rm -f myldap

tar:
	tar -cvf xkolcu00.tar *.cc *.h manual.pdf README.md Makefile
