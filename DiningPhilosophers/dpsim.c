#include "dpsim.h"


static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosophers[5];


void* th_main( void* th_main_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here
/**************************************************
 
 Main Thread Function:
 void* th_main( void* th_main_args )
 
 Note: NO mutex locks or unlocks should be performed by main 
 thread, however the chopsticks array is being read by the 
 main_thread, and is vulnerable to race conditions. You must 
 consider the effect of these race conditions, and the 
 correctness of the check/comparsions and output of the 
 main_thread. 
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 1. Initialize all element values in the chopsticks array to -1*/
for(int i = 0; i < 5; i++)
{
	chopsticks[i] = -1;
}
	/*
 2. Create a thread for each philosopher (hint: use philosophers[5] array)*/
for(int j = 0; j<5; j++)
{
	int* arg = malloc(sizeof(*arg));
    *arg = j;
	if(pthread_create(&philosophers[j], NULL, th_phil, arg) != 0)
	{
		int* one = malloc(sizeof(int));
		*one = 1;
		pthread_exit(one);
	}//exit or return 1
}
/*
 - If a error condition occurs, then immediately exit this function with status value 1.
 3. Execute a infinite loop that does the following:*/
while(TRUE)
{
int deadlock = FALSE;
//deadlock is when all chopsticks are 1
//pthread_mutex_trylock and all mutexes are locked
int all_chopsticks = TRUE;
//has to be 0 1 2 3 4
//make memcpy
int chopstick_cpy[5];
/*chopstick_cpy[0] = chopsticks[0];
chopstick_cpy[1] = chopsticks[1];
chopstick_cpy[2] = chopsticks[2];
chopstick_cpy[3] = chopsticks[3];
chopstick_cpy[4] = chopsticks[4];*/
memcpy(chopstick_cpy, chopsticks, sizeof(chopsticks));
/*for(int lm=0; lm<5; lm++)
{
	if(chopsticks[lm]==-1)
	{
		all_chopsticks = FALSE;
		break;
	}
}*/
//printf("chopstick_cpy[0] == %i\nchopstick_cpy[1] == %i\nchopstick_cpy[2] == %i\nchopstick_cpy[3] == %i\nchopstick_cpy[4] == %i\n", 
//	chopstick_cpy[0], chopstick_cpy[1], chopstick_cpy[2], chopstick_cpy[3], chopstick_cpy[4]);



deadlock = chopstick_cpy[0] == 0
&& chopstick_cpy[1] == 1
&& chopstick_cpy[2] == 2
&& chopstick_cpy[3] == 3
&& chopstick_cpy[4] == 4;//(all_chopsticks == TRUE);// && (all_locked == TRUE);
if(deadlock)
{
	printf("%s\n", "Deadlock condition (0,1,2,3,4) ... terminating");
	//terminate loop
	break;
}
else
{
	char philosopher_string[50];
	strcpy(philosopher_string, "");
	int is_set = FALSE;
	//philosopher 0
	if(chopstick_cpy[0] == 0 && chopstick_cpy[1] == 0)
	{
		strcat(philosopher_string, "0, ");
		is_set = TRUE;
	}

	if(chopstick_cpy[1] == 1 && chopstick_cpy[2] == 1)
	{
		strcat(philosopher_string, "1, ");
		is_set = TRUE;
	}
	if(chopstick_cpy[2] == 2 && chopstick_cpy[3] == 2)
	{
		strcat(philosopher_string, "2, ");
		is_set = TRUE;
	}
	if(chopstick_cpy[3] == 3 && chopstick_cpy[4] == 3)
	{
		strcat(philosopher_string, "3, ");
		is_set = TRUE;
	}
	//philosopher 4
	if(chopstick_cpy[0] == 4 && chopstick_cpy[4] == 4)
	{
		strcat(philosopher_string, "4, ");
		is_set = TRUE;
	}
	

	philosopher_string[strlen(philosopher_string)-2] = '\0';
	if(is_set) printf("Philopsher(s) %s are eating\n", philosopher_string);
	//printf("Philosophers are eating\n");

	delay(20);
	//printf("Error123\n");

}

//free(chopstick_cpy);
}
/*
 If a deadlock condition is found then 
 display deadlock
 terminate the loop and goto step 4
 otherwise 
 display which philosophers are eating
 and then call the delay function (you specify nanosec sleep value)
 
 An portion of example output:
 Philopsher(s) 0, 2 are eating
 Philospher(s) 1 are eating
 .
 .
 .
 Deadlock condition (0,1,2,3,4) ... terminating
 
 4. Kill each philosopher thread*/

for(int k=0; k<5; k++)
{
	 pthread_kill(philosophers[k], 0);
}
	/*
 5. Exit the main thread with status value equal to 0.
 
 */
	pthread_exit(0);

} // end th_main function


void* th_phil( void* th_phil_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here
/**************************************************
 
 Philosopher Thread Function: 
 void* th_phil( void* th_phil_args );
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 1. Get the philosopher id (hint: use th_phil_args)
 2. Execute an infinite loop that does the following:
 - call the delay function for thinking (you specify nanosec sleep value)
 - call the eat function (argument is the philosopher id)
 
 */
	int* phil_id = (int*)th_phil_args;//(int)(th_phil_args);
	//printf("Error eat: %i\n", *phil_id);
	while(1)
	{
		delay(5000000);
		eat(*phil_id);
		//
	}


} // end th_phil function


void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {

	// ---------------------------------------
	// TODO: you add your implementation here
	/**************************************************
 
 Eat Function: 
 void eat( int phil_id )
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 This philosopher, phil_id, picks up chopstick phil_id
 (i.e. the right chopstick) then delays for no more than 
 20,000 nanoseconds. Next the philosopher picks up the 
 left chopstick. Note: mutual exclusion is necessary when 
 updating a shared resource. After having picked up both 
 chopsticks (as described) the philosopher will delay a 
 number of nanoseconds that is determined by you experimentally.
 After the delay completes, the philosopher will release 
 the two chopsticks in the reverse order in which they were
 picked up.
 
 */
	//pickup chopstick phil_id i.e. right chopstick
	//set mutex
	//printf("Error eat: %i \n", phil_id);
	int mutex_lock = pthread_mutex_lock(&mutex[phil_id]);
	chopsticks[phil_id] = phil_id;
	//delay(1000);
//printf("Error eat1\n");
	int left_chopstick = phil_id;
	if(phil_id == 4)
		left_chopstick = 0;
	else
		left_chopstick++;

	pthread_mutex_lock(&mutex[left_chopstick]);
//printf("Error eat2\n");
	chopsticks[left_chopstick] = phil_id;
	//pidk up left chopstick
	delay(20000);//determine experimentally
	//release left
	chopsticks[left_chopstick] = -1;	
	pthread_mutex_unlock(&mutex[left_chopstick]);
//printf("Error eat3\n");
	//release right
	chopsticks[phil_id] = -1;
	pthread_mutex_unlock(&mutex[phil_id]);


} // end eat function
