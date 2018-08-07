// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"


//#define DEBUG_PARSE false

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {

	int done = FALSE;
	int cmd_cnt = 0;
	int i = 0;

	char shell_cmd[100];

	command_t command;

	// -------------------------------------------
	// Go ahead and allocate memory
	// makes things less complicated :)
	command.path = malloc(sizeof(char)*100);
	command.argv = malloc(sizeof(char*)*50);
	
	for ( i=0; i<50; i++ ) command.argv[i] = malloc(sizeof(char)*100);

#ifdef DEBUG_PARSE
	strcpy(shell_cmd, "cd /mnt/cdrom");
	parse(shell_cmd, &command);
	if(!equals(command.path, "cd")) printf("Command path fail: %s\n", command.path);
	if(command.argc != 2) printf("Command argc fail: %i\n", command.argc);
	if(!equals(command.argv[0], "cd"))  printf("Command argv fail: %s\n", command.argv[0]);
	if(!equals(command.argv[1], "/mnt/cdrom"))  printf("Command argv fail: %s\n", command.argv[1]);

	cleanup( &command );

	command_t command1;
	command = command1;

	command.path = malloc(sizeof(char)*100);
	command.argv = malloc(sizeof(char*)*50);
	
	for ( i=0; i<50; i++ ) command.argv[i] = malloc(sizeof(char)*100);
	strcpy(shell_cmd, "cd /mnt/cdrom");
	parse(shell_cmd, &command);
	if(!equals(command.path, "cd")) printf("Command path fail: %s\n", command.path);
	if(command.argc != 2) printf("Command argc fail: %i\n", command.argc);
	if(!equals(command.argv[0], "cd"))  printf("Command argv fail: %s\n", command.argv[0]);
	if(!equals(command.argv[1], "/mnt/cdrom"))  printf("Command argv fail: %s\n", command.argv[1]);
	if(command.argv[2] != NULL)  printf("Command argv fail (not null): %s\n", command.argv[2]);
	
#else
	while ( !done ) {
		char curr_dir_buff[1024];
   		getcwd(curr_dir_buff, sizeof(curr_dir_buff));
   		char* curr_dir = malloc(strlen(curr_dir_buff));  //This program leaks more than a broken bucket.
   		strcpy(curr_dir, curr_dir_buff);



   		char curr_hostname_buff[1024];
   		gethostname(curr_hostname_buff, sizeof(curr_hostname_buff));
   		char* curr_hostname = malloc(strlen(curr_hostname_buff));  //This program leaks more than a broken bucket.
   		strcpy(curr_hostname, curr_hostname_buff);

		printf("[%d] %s@%s:%s$ ", cmd_cnt++, getenv("USER"), curr_hostname, curr_dir );
		gets( shell_cmd );

		parse( shell_cmd, &command );
		if ( ( command.argc > 0 ) && equals( command.path, "exit" )  ) {

			done = TRUE;

		} else if ( command.argc == ERROR ) { 

			fprintf( stderr, "%s: command not found!\n", command.path );

		} else if ( command.argc > 0 ) {

			if ( is_builtin( &command ) ) {

				if ( do_builtin( &command ) != SUCCESSFUL ) {

					fprintf( stderr, "%s command failed: requested folder [%s] does not exist!\n", 
					command.path, command.argv[0] );

				}

			} else if ( execute( &command ) != SUCCESSFUL ) {

				fprintf( stderr, "%s command failed!\n", command.path );

			} 

		}

	}

	cleanup( &command );

	return 0;
#endif
} // end main function
