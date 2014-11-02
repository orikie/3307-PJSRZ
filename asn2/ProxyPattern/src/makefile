#
# CS3307 asn2 proxy pattern
# Nov 2, 2014
#
# 

CC=g++ 
OFLAGS=-std=c++11 -c 

all: main.o DiscoveryClient.o ItemProxy.o SearchEngine.o
	$(CC) main.o DiscoveryClient.o ItemProxy.o SearchEngine.o -o eDiscvoery.out

main.o: main.cpp
	$(CC) $(OFLAGS) main.cpp

DiscoveryClient.o: DiscoveryClient.cpp
	$(CC) $(OFLAGS) DiscoveryClient.cpp

ItemProxy.o: ItemProxy.cpp
	$(CC) $(OFLAGS) ItemProxy.cpp

SearchEngine.o: SearchEngine.cpp
	$(CC) $(OFLAGS) SearchEngine.cpp
	

clean:
	rm *.o
	rm -f eDiscovery.out

