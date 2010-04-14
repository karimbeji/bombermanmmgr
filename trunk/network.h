typedef struct {
	int * client;
	size_t nclients;
	int server;
  	int id;
} connection_t;

connection_t * initServer (size_t nclients, int port);
connection_t * initClient (const char * host, int port);
void closeClient (connection_t * c);
void sendData (connection_t * connection, void * data, size_t size);
void * receiveData (connection_t * connection, void * data, size_t size);
void gatherData (connection_t * connection, void * data, size_t size);
void broadcastData (connection_t * connection, void * data, size_t size);
void closeServer (connection_t * connection);
