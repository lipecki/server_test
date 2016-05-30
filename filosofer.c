//
// Created by Johan Lipecki on 2016-05-28.
//

#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include "filosofer.h"


char table[20] = "_W_W_W_W_W_";
/*Example of illustration of five philosphers with
five chopstick. Chopsticks are lying on the table*/
/*
_W_W|E|W_W_         This is when the third guy is eating.
|W|E|T_T_W|         This is when the second guy is eating.
                    third and fourth are thinking. (Philosophizing.)
_W|E|T|E|W_         This is when the second and fourth guys are eating.
|E|W|E|W_W|         This is when the first guy is eating.
                    To note: The first "|" symbolizes one chopstick to
                    the left of the first philosopher. But the last "|"
                    is also symbolizing the same chopstick since they
                    are sitting around a round table.
*/


int main(int argc,char *argv[])
{
        int noPhils=7, i=0;
        bool deadlock = false;

        if(argc < 2){
                printf("Please provide number of philosophers as argument 1!");
                exit(EXIT_FAILURE);
        }
        else if(argc > 3) {
                printf("Too many arguments!\n");
                exit(EXIT_FAILURE);
        }
        else {
                if (argc == 3) {
			assert((strcmp(argv[2],"deadlock"))==0);
			deadlock = true;
		}
                noPhils = atoi(argv[1]);
        }
        for (int k = 0; k < noPhils; k++) {
                if(!k) strcpy(table,"_W");
                else if (k==(noPhils-1)) strcat(table,"_W_");
                else strcat(table,"_W");
        }
        printf("The table: %s\n",table);
        pthread_t phils[noPhils]; 		/*Some sort of array of phils are needed*/
        Philosopher ctrl_phil[noPhils];
        pthread_mutex_t chop_stick[noPhils];

        int round = 0;

        for (int j = 0; j < noPhils; j++) {
                char *waits = {"W"};
                ctrl_phil[j].pos = j;
                phils[j]= (pthread_t) j;
                ctrl_phil[j].noPhils = noPhils;
                ctrl_phil[j].status='W';
                pthread_mutex_t tmp =PTHREAD_MUTEX_INITIALIZER;
                chop_stick[j]=tmp;
                ctrl_phil[j].chop_stick_l=&chop_stick[j];
                ctrl_phil[j].chop_stick_r=&chop_stick[(j+1)%noPhils];
                ctrl_phil[j].round = &round;
                ctrl_phil[j].table =  table;
                ctrl_phil[j].deadlock=deadlock;
                pthread_create(&phils[j],NULL,&philosophize,(void *) &ctrl_phil[j]);
        }
        /*Overall design of the program
        1. Take in commandline arguments to set up how many phils are going to be
        simulated and if deadlock is going occur. Commandline arguments need to
        be checked and the program needs to exit if they are not in the correct format
        see Advanced Linux Programming for excellent advice on commandline arguments.
        2. Start simulation by starting the phil-threads and let the main program
        print out the contents of the string table declared above. No thread is going
        to communicate with the user but through the string table, it is the main
        program that prints out the contents of the string table. This means that
        we are separating the task of computation/simulation from the task of
        presentation of the results*/
        while(round<48)
        {

                printf("Round %2d: %s\n", round, ctrl_phil->table);
                sleep(1);
                (round)++;
        }

        /* The above loop runs in parallel to the threads/phils that affect the
        common resource table.
        IMPORTANT: The synchronization must not be through one mutex! We must have
        one mutex per chopstick, that means an array of mutexes is also needed!
        IMPORTANT: Remember that the program should also make deadlock possible
        through commandline arguments, there must be a way to force a deadlock to
        occur. Remember in this context that all thread-functions are to be based on
        one function, philosophize(), and that this function is the same for all
        threads.
        Of course it can behave differently for different philosopher-id's, but
        it must be one function which needs to be written to enable a forced deadlock
        so we can compare and understand why it normally avoids deadlock.
        3. When the loop has finished, all the threads are joined to the main program
        and then the main program exits.
        */
        for (int j = 0; j < noPhils; j++){

                pthread_join(phils[j],NULL);
        }
        return 0;
}
