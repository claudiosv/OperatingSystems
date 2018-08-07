#define DEBUG 1

#include <stdio.h>		// IO functions
#include <stdlib.h> 	// atof function
#include <sys/socket.h>	// listen function
#include <unistd.h>		// close function
#include <signal.h>	// signals
#include <wait.h>	// waitpid 
#include "server.h"		// our code :)

// -------------------------------------------------
// Global variables
static int server_socket_fd = 0;


// -------------------------------------------------
// Function prototypes
static void sig_child_handler( int sig );
static void sig_int_handler( int sig );

/*

Project 1: CSIS 614 
Advanced Operating Systems

Developing a http server that forks child processes, and uses signal handlers to 
remove children from process table.

*/
int main( int argc, char *argv[] ) {

	unsigned int port_number = 0;

	// -------------------------------------------------
	// Register the signal handlers

	if ( signal( SIGINT, sig_int_handler ) == SIG_ERR ) {

		perror( "Unable to create signal interupt handler!");
		exit( FAIL );
	}

	if ( signal( SIGCHLD, sig_child_handler ) == SIG_ERR ) {

		perror( "Unable to create signal child handler!");
		exit( FAIL );
	}

	// -------------------------------------------------
	// Verify correct number of arguments are provided
	// when the server is executed.

	if ( argc == 2 ) { 

		port_number = (unsigned int) atoi( argv[1] );

		server_socket_fd = bind_port( port_number );

		if ( DEBUG ) printf( "server socket fd = %d\n", server_socket_fd );

		printf(" ------------------------------------------------\n");

		if ( server_socket_fd == FAIL ) {

			printf("Failed to bind socket to port %d\n", port_number );
			printf("Stopping http server!\n" );

		} else {

			while ( listen( server_socket_fd, 0 ) == 0 ) {

				printf("Server socket listening and accepting connections on port %d\n", port_number );

				accept_client( server_socket_fd );

			}

		}

	} else {

		printf( "Port number is not specified!\n" );
		printf( "Example usage: %s %s\n", argv[0], "8888" );

	}

	return OK;
     
} // end main function

static void sig_int_handler( int sig ) {

	printf("Unbinding server from port and exiting ... \n");
	close( server_socket_fd );
	exit( OK );

} // end sig_int_handler function

static void sig_child_handler( int sig ) {

	// -------------------------------------
	// TODO:
	// -------------------------------------
	// You complete child signal handler code to remove child process from process 
	// table (i.e. reap the child)
	// -------------------------------------
	pid_t cpid = wait(NULL);
	//waitpid(-1, status,0)
	

} // end sig_child_handler function
