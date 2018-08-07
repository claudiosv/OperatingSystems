// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2016
// server.h header file
// Homework 1
//
// -----------------------------------

#define OK 0
#define FAIL -1
#define DEBUG 1

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
//
// Arguments:	port_number = port number the server 
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number );

// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//

int accept_client( int server_socket_fd );

char* find_entities(char* request);

char* parse_print_entities(char* entities, char* entity_buffer);