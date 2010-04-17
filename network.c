#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "network.h"
#define ERROR(X) { perror(X); exit(1); }
#define MAXCLIENTS 4

connection_t * initServer (size_t nclients, int port)
{
	int i;
	struct sockaddr_in server;
	connection_t * c = (connection_t *) malloc (sizeof (connection_t));

	if ((c->server = socket (AF_INET, SOCK_STREAM, 0)) == -1) 
		ERROR("socket");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons (port);
	
	if (bind (c->server, (struct sockaddr *) &server, sizeof (server)) == -1)
		ERROR("bind");

	printf ("socket has port %d\n", ntohs (server.sin_port));

	if (listen (c->server, MAXCLIENTS) == -1)
		ERROR("listen");

	c->client = (int *) malloc (sizeof (int) * nclients);
	c->nclients = nclients;
	for (i = 0; i < nclients; i++)
	{
		if ((c->client[i] = accept (c->server, 0, 0)) == -1)
			ERROR("accept");
		
		write(c->client[i], &i, sizeof(int));
		printf("Connected.\n");
	}
	
	return c;
}

connection_t * initClient (const char * host, int port)
{
	struct hostent * server;
	struct sockaddr_in serv_addr;
	
	connection_t *c = (connection_t *) malloc (sizeof (connection_t));
	c->server = socket (AF_INET, SOCK_STREAM, 0);
	
	if (c->server < 0)
		ERROR("opening socket");

	server = gethostbyname (host);

	if (server == NULL)
		ERROR("no such host");

	serv_addr.sin_family = AF_INET;
	bcopy ((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons (port);

	if (connect (c->server, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) 
		ERROR("connecting");

	read (c->server, &c->id, sizeof (int));

	return c;
}

void closeClient (connection_t * connection)
{
	close (connection->server);
	free (connection);
}

void broadcastData (connection_t * connection, void * data, size_t size)
{
	int i;
	
	for (i = 0; i<  connection->nclients; i++)
		write(connection->client[i], data, size);
}

void * receiveData (connection_t * connection, void * data, size_t size)
{
	read (connection->server, data, size);
	
	return data;
}

void sendData (connection_t * connection, void * data, size_t size)
{
	write (connection->server, data, size);
}

void gatherData (connection_t * connection, void * data, size_t size)
{
	int i;
	
	for (i = 0; i < connection->nclients; i++) 
		read (connection->client[i], data + size * i, size);
}

void closeServer (connection_t * connection)
{
	int i;
	
	for (i = 0; i < connection->nclients; i++)
		close (connection->client[i]);

	free (connection->client);
	close (connection->server);
	free (connection);
}