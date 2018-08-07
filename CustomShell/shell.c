// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};


void parse( char* line, command_t* p_cmd ) {

	// ----------------------------------------
	// TODO: you fully implement this function
	int arg_offset = 0;
	int line_len = strlen(line);
	int j=0;
	
	while ( p_cmd->argv[j] != NULL ) {
		p_cmd->argv[j] = '\0';
		j++;
	}
	p_cmd->argc = 0;
	for (int i=0; i<50; i++ ) p_cmd->argv[i] = malloc(sizeof(char)*100); //This program leaks more than a broken bucket.
	strcpy(p_cmd->path, "");

	
	if(line_len == 0)
	{		
		return;
	}
	

	char main_command[50];

	for (char *p = strtok(line," "); p != NULL; p = strtok(NULL, " "))
	{
		if(arg_offset == 0)
		{
			strcpy(main_command, p);

		}
			p_cmd->argc++;
			strcpy(p_cmd->argv[arg_offset], p);
			arg_offset++;
  		
	}
	p_cmd->argv[arg_offset] = '\0';
	
	find_fullpath(main_command, p_cmd);
	//printf("cmd: %s, offset: %i, argc: %i\n", main_command, arg_offset, p_cmd->argc);
} // end parse function


int execute( command_t* p_cmd ) {

	
	// ----------------------------------------
	// TODO: you fully implement this function

	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed 
	// to reflect the correct return 
	// value described in the header file
	int status_code = 0;
	if ( p_cmd->argc > 0 ) {
		pid_t child_pid = fork();
		if ( child_pid  == 0 ) {
			//printf("%s\n", p_cmd->path);
			execv( p_cmd->path, p_cmd->argv );
			perror("Execute terminated with an error condition!\n");
			exit( 1 );
			//exit(0);
					
		}
			
		wait(&status_code);//waitpid( child_pid, &status_code, 0 );
	} else if ( p_cmd->argc == -1 ) {
		// display to user cmd not found
	}

	return status_code;


} // end execute function


int find_fullpath( char* command_name, command_t* p_cmd ) {


	char* path_o = getenv( "PATH" );
	char* path = malloc(strlen(path_o));  //This program leaks more than a broken bucket.
	strcpy(path, path_o);
	int command_exists = FALSE;
	strcpy(p_cmd->path, command_name);
	if(is_builtin(p_cmd) == TRUE)
	{
		command_exists = TRUE;
		
	}
	else
	{
	// ----------------------------------------
	for (char *p = strtok(path,":"); p != NULL; p = strtok(NULL, ":"))
	{
  		
  		char* temp_buffer = malloc(strlen(p) + strlen(command_name) + 2);
  		strcpy(temp_buffer, p);
  		char* possible_cmd_path = strcat(temp_buffer, "/");
  		possible_cmd_path = strcat(temp_buffer, command_name);
  		
  		struct stat buffer;
		int exists;
		
	// string that represents the fully qualified
	// path of a file or directory on the file system
		
		exists = stat( possible_cmd_path, &buffer );

	//if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) ) {
		// Directory exists BAD
	//} else

		if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
		// File exists GOOD
		
			strcpy(p_cmd->path, possible_cmd_path);
			
			command_exists = TRUE;
		} else {
		// Not a valid file or directory BAD
			//
			
		}

		
		free(temp_buffer); //Except here!
		

		
	}


	if(command_exists == FALSE)
		{
			strcpy(p_cmd->path, command_name);
			p_cmd->argc = -1;
			int j=0;
	
	while ( p_cmd->argv[j] != NULL ) {
		p_cmd->argv[j] = '\0';
		j++;
	}
			//p_cmd->argv = NULL;
			//printf("invalid command1\n");
		}
	}

return command_exists; //jump out the window
	

	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed 
	// to reflect the correct return 
	// value described in the header file


} // end find_fullpath function


int is_builtin( command_t* p_cmd ) {

	int cnt = 0;

	while ( valid_builtin_commands[cnt] != NULL ) {

		if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

			return TRUE;

		}

		cnt++;

	}

	return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

	// only builtin command is cd

	if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

	struct stat buff;
	int status = ERROR;

	if ( p_cmd->argc == 1 ) {

		// -----------------------
		// cd with no arg
		// -----------------------
		// change working directory to that
		// specified in HOME environmental 
		// variable

		status = chdir( getenv("HOME") );

	} else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


		// -----------------------
		// cd with one arg 
		// -----------------------
		// only perform this operation if the requested
		// folder exists

		status = chdir( p_cmd->argv[1] );

	} 

	return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

	int i=0;
	
	while ( p_cmd->argv[i] != NULL ) {
		free( p_cmd->argv[i] );
		i++;
	}

	free( p_cmd->argv );
	free( p_cmd->path );	

} // end cleanup function


int equals( char* str1, const char* str2 ) {

	// First check length

	int len[] = {0,0};

	char* b_str1 = str1;
	const char* b_str2 = str2;

	while( (*str1) != '\0' ) { 
		len[0]++;
		str1++;
	}

	while( (*str2) != '\0' ) {
		len[1]++;
		str2++;
	}

	if ( len[0] != len[1] ) {

		return FALSE;

	} else {

		while ( (*b_str1) != '\0' ) {

			// don't care about case (you did not have to perform
			// this operation in your solution

			if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

				return FALSE;

			}

			b_str1++;
			b_str2++;

		}

	} 

	return TRUE;


} // end compare function definition

