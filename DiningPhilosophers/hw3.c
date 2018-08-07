#include <stdio.h>
#include "dpsim.h"

/**************************************************

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread 
	- If the return value != 0, then display an error message and 
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {

	//printf("Error\n");
	// ---------------------------------------
	// TODO: you add your implementation here
	pthread_t main_thread;
	
	//status variable
	int pthread_status = pthread_create(&main_thread, NULL, th_main, NULL);
	if(pthread_status != 0)
	{
		//error
		printf("Error");
		exit(1);
	}

	//join thread

	int join_thread_status = 0;
	pthread_join(main_thread, &join_thread_status);

	if(join_thread_status != 0)
	{
		//error
		printf("Error");
		exit(2);
	}

	//join status value
	printf("Join status: %i\n", join_thread_status);

	return 0;

} // end main function