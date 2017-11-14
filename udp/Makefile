

CC=gcc

CFLAGS=-g

udp_client=udp_client
udp_server=udp_server
tcp_client=tcp_client
tcp_server=tcp_server

all_arget=$(udp_client) $(udp_server) $(tcp_client) $(tcp_server)


udp_client_obj=udp_client.o

all:$(all_arget) 

$(udp_client): 
	$(CC) $(CFLAGS) -o $(udp_client) udp_client.c 

$(udp_server):
	$(CC) $(CFLAGS) -o $(udp_server) udp_server.c

$(tcp_client):
	$(CC) $(CFLAGS) -o $(tcp_client) tcp_client.c 
$(tcp_server):
	$(CC) $(CFLAGS) -o $(tcp_server) tcp_server.c 

clean:
	rm -f $(all_arget) *.o
