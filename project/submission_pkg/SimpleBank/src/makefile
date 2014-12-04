#
# CS3307 asn1 SimpleBank
# Sept, 2014
#
# Zhao Lin, Ziqi Rui

CC=g++ 
OFLAGS=-c 

all: bank vendor

bank: main.o DBDelegate.o  User.o Logger.o ClientDB.o BankAccount.o Utils.o SimpleBank.o BankClient.o sqlite3.o
	$(CC) main.o DBDelegate.o User.o Logger.o ClientDB.o BankAccount.o Utils.o SimpleBank.o BankClient.o sqlite3.o -o SimpleBank.exe

vendor: v_main.o DBDelegate.o Logger.o Utils.o sqlite3.o Vendor.o VendorClient.o 
	$(CC) v_main.o DBDelegate.o Logger.o Utils.o sqlite3.o vendor.o VendorClient.o -o vendor.exe

Vendor.o: Vendor.cpp
	$(CC) $(OFLAGS) Vendor.cpp

VendorClient.o: VendorClient.cpp
	$(CC) $(OFLAGS) VendorClient.cpp

main.o: main.cpp
	$(CC) $(OFLAGS) main.cpp

v_main.o: v_main.cpp
	$(CC) $(OFLAGS) v_main.cpp

sqlite3.o:
	gcc -c sqlite3.c

DBDelegate.o: DBDelegate.cpp
	$(CC) $(OFLAGS) DBDelegate.cpp 

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
	rm -f  *.o
	rm -f SimpleBank.exe
	rm -f trace.txt
	rm -f vendor.exe
	rm -f FailedCreditPayments.txt
reset:
	rm -f clientinfo.data
	rm -f bank_del.db
	make clean
	
