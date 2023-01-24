PH = ./headers/
PB = ./bin/
PS = ./sources/

all: $(PB)server $(PB)client

$(PB)server: $(PB)discuServer.o $(PB)req.o $(PB)server.o
	gcc -Wall $(PB)discuServer.o $(PB)req.o $(PB)server.o -o $(PB)server

$(PB)server.o : $(PS)server.c $(PH)discuServer.h $(PH)req.h
	gcc -Wall -c $(PS)server.c -o $(PB)server.o

$(PB)discuServer.o : $(PS)discuServer.c $(PH)discuServer.h
	gcc -Wall  -c  $(PS)discuServer.c -o $(PB)discuServer.o

$(PB)req.o : $(PS)req.c $(PH)req.h
	gcc -Wall -c  $(PS)req.c -o $(PB)req.o

$(PB)discuClient.o : $(PS)discuClient.c $(PH)discuClient.h $(PH)req.h
	gcc -Wall -c $(PS)discuClient.c -o $(PB)discuClient.o

$(PB)client.o: $(PS)client.c $(PH)struct.h $(PH)req.h $(PH)discuClient.h
	gcc -Wall -c $(PS)client.c -o $(PB)client.o

$(PB)client: $(PB)client.o $(PB)req.o $(PB)discuClient.o
	gcc -Wall $(PB)client.o $(PB)req.o $(PB)discuClient.o -o $(PB)client

clean :
	-rm $(PB)*
