//
// Created by Johan Lipecki on 2016-05-28.
//

#include <memory.h>
#include <stdlib.h>
#include <assert.h>
//#include "filosofer.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

struct phil_parms
{
    int pos;
    int noPhils;
    char status;
    pthread_mutex_t *chop_stick_l;
    pthread_mutex_t *chop_stick_r;
    int *round;
    char *table;
    bool deadlock;
};
typedef struct phil_parms Philosopher;

// En snyggare lösning vore att ha spelbordets data i en egen struct och låta den ingå i Philosopher
struct set_table{
    pthread_mutex_t *chop_stick_l;
    pthread_mutex_t *chop_stick_r;
    int *round;
    char *state;
    bool deadlock;
};
typedef struct set_table Table;

void* philosophize (void*);


char table[20] = "_T_T_T_T_T_";
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
                printf("Please provide number of philosophers as argument 1!\n");
                //exit(EXIT_FAILURE);
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
                if(!k) strcpy(table,"|W");
                else if (k==(noPhils-1)) strcat(table,"|W|");
                else strcat(table,"|W");
        }
        if (!deadlock) {
                for (int k = 0; k < noPhils; k++) {
                        if (!k) strcpy(table, "_T");
                        else if (k == (noPhils - 1)) strcat(table, "_T_");
                        else
                                strcat(table, "_T");
                }
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
void* philosophize (void* parameters) {

        Philosopher *filosof = (Philosopher *) parameters;
        printf("I'm philosopher no %d out of %d\n",(int) filosof->pos, (int) filosof->noPhils);
        srand((unsigned int) time(NULL));
        unsigned int sec;
        do{
                //En av två filosofer måste ges prioritet när det blir konflikt!
                //int priority = (filosof->pos+(*(filosof->round)))%2;
                int assert;

                if(!pthread_mutex_lock(filosof->chop_stick_l)) {
                        filosof->status = 'W';
                        //printf("The table: %s at pos %d\n", filosof->table,filosof->pos);
                        //ätpinne till vänster lyfts
                        if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                filosof->table[filosof->noPhils * 2] = '|';
                        filosof->table[(filosof->pos) * 2] = '|';

                        //filosofer väntar på att nästa pinne ska bli ledig
                        filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                        if (!(filosof->deadlock)) {
                                if (!pthread_mutex_trylock(filosof->chop_stick_r)) {
                                        filosof->status = 'E';

                                        //ätpinne till vänster lyfts
                                        if (filosof->pos ==
                                            0)             //samma vänstra pinne visas två gånger på pos 0
                                                filosof->table[filosof->noPhils * 2] = '|';
                                        filosof->table[(filosof->pos) * 2] = '|';
                                        //filosofer äter mellan pinnarna
                                        filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                        //ätpinne till höger lyfts
                                        if (filosof->pos ==
                                            (filosof->noPhils - 1)) //samma högra pinne visas två gånger på högsta pos
                                                filosof->table[0] = '|';
                                        filosof->table[(filosof->pos) * 2 + 2] = '|';
                                        sec = (unsigned int) rand() % 3 + 1;
                                        printf("Philosopher %d eating for %d secs\n", filosof->pos, sec);

                                        sleep(sec);

                                        filosof->status = 'T';

                                        //ätpinne till vänster läggs ned
                                        if (filosof->pos ==
                                            0)             //samma vänstra pinne visas två gånger på pos 0
                                                filosof->table[filosof->noPhils * 2] = '_';
                                        filosof->table[(filosof->pos) * 2] = '_';
                                        //filosofer äter mellan pinnarna
                                        filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                        if (filosof->pos ==
                                            (filosof->noPhils - 1)) //samma högra pinne visas två gånger på högsta pos
                                                filosof->table[0] = '_';
                                        filosof->table[(filosof->pos) * 2 + 2] = '_';
                                        pthread_mutex_unlock(filosof->chop_stick_l);
                                        pthread_mutex_unlock(filosof->chop_stick_r);

                                        sec = (unsigned int) rand() % 5 + 2;
                                        sleep(sec);
                                }
                                else {

                                        /*printf("Player %d: \"OK, I'll sit here and ponder world peace"
                                                " for %d seconds!\"\n",filosof->pos, sec=(unsigned int) rand()%5+2); */
                                        filosof->status = 'T';
                                        if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                                filosof->table[filosof->noPhils * 2] = '_';
                                        filosof->table[(filosof->pos) * 2] = '_';
                                        filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                        pthread_mutex_unlock(filosof->chop_stick_l);
                                        //sec = (unsigned int) rand() % 5 + 2;
                                        sleep(sec);
                                }
                        }
                        else if (filosof->deadlock){                    // för tydlighetens skull
                                //printf("DL %d!\t",filosof->pos);
                                filosof->status = 'W';

                                //ätpinne till vänster lyfts
                                if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                        filosof->table[filosof->noPhils * 2] = '|';
                                filosof->table[(filosof->pos) * 2] = '|';
                                //filosofer väntar på att nästa pinne ska bli ledig
                                filosof->table[(filosof->pos) * 2 + 1] = filosof->status;

                                // Det är här det händer, när ingen återtagning av resurser är möjlig
                                if(pthread_mutex_lock(filosof->chop_stick_r)){              //Evig väntan!

                                        printf("Philosopher %d eating!\n", filosof->pos);
                                        filosof->status = 'E';

                                        //ätpinne till vänster lyfts
                                        if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                                filosof->table[filosof->noPhils * 2] = '|';
                                        filosof->table[(filosof->pos) * 2] = '|';
                                        //filosofer äter mellan pinnarna
                                        filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                        //ätpinne till höger lyfts
                                        if (filosof->pos ==
                                            (filosof->noPhils - 1)) //samma högra pinne visas två gånger på högsta pos
                                                filosof->table[0] = '|';
                                        filosof->table[(filosof->pos) * 2 + 2] = '|';

                                        sec = (unsigned int) rand() % 3 + 1;
                                        sleep(sec);

                                        filosof->status = 't';
                                        //ätpinne till vänster läggs ned
                                        if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                                filosof->table[filosof->noPhils * 2] = '_';
                                        filosof->table[(filosof->pos) * 2] = '_';
                                        //filosofer äter mellan pinnarna
                                        filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                        if (filosof->pos ==
                                            (filosof->noPhils - 1)) //samma högra pinne visas två gånger på högsta pos
                                                filosof->table[0] = '_';
                                        filosof->table[(filosof->pos) * 2 + 2] = '_';
                                        pthread_mutex_unlock(filosof->chop_stick_l);
                                        pthread_mutex_unlock(filosof->chop_stick_r);

                                        sec = (unsigned int) rand() % 5 + 2;
                                        sleep(sec);
                                }
                        }

                }
                //printf("Player %d in round %d\n", filosof->pos,*(filosof->round));
        }while((*(filosof->round))<48);
        printf("Player %d in round %d\n", filosof->pos,*(filosof->round));
        return NULL;
}