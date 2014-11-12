#
# CS3307 asn1 SimpleBank
# Sept, 2014
#
# Zhao Lin, Ziqi Rui

CC=g++ 
OFLAGS=-std=c++11 -c 

all: main.o User.o Logger.o ClientDB.o BankAccount.o Utils.o SimpleBank.o BankClient.o
	$(CC) main.o User.o Logger.o ClientDB.o BankAccount.o Utils.o SimpleBank.o BankClient.o -o SimpleBank.exe

main.o: Main.cpp
	$(CC) $(OFLAGS) Main.cpp

User.o: User.cpp
	$(CC) $(OFLAGS) User.cpp

Logger.o: Logger.cpp
	$(CC) $(OFLAGS) Logger.cpp

ClientDB.o: ClientDB.cpp
	$(CC) $(OFLAGS) ClientDB.cpp

BankAccount.o: BankAccount.cpp
	$(CC) $(OFLAGS) BankAccount.cpp

Utils.o: Utils.cpp
	$(CC) $(OFLAGS) Utils.cpp

SimpleBank.o: SimpleBank.cpp
	$(CC) $(OFLAGS) SimpleBank.cpp

BankClient.o: BankClient.cpp
	$(CC) $(OFLAGS) BankClient.cpp

clean:
	rm *.o
	rm -f SimpleBank.exe
	rm -f trace.txt
reset:
	rm clientinfo.data
	make clean
