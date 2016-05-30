//
// Created by Johan Lipecki on 2016-05-28.
//

#ifndef TEST_GAME_FUNCTIONS_FILOSOFER_H
#define TEST_GAME_FUNCTIONS_FILOSOFER_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t mutex1 =PTHREAD_MUTEX_INITIALIZER;

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

void* philosophize (void* parameters) {

        Philosopher *filosof = (Philosopher *) parameters;
        printf("I'm philosopher no %d out of %d\n",(int) filosof->pos, (int) filosof->noPhils);
        srand((unsigned int) time(NULL));
        unsigned int sec;
        do{
                //En av två filosofer måste ges prioritet när det blir konflikt!
                int priority = (filosof->pos+(*(filosof->round)))%2;
                int assert;

                if(!pthread_mutex_trylock(filosof->chop_stick_l)) {
                        //printf("The table: %s at pos %d\n", filosof->table,filosof->pos);
                        //ätpinne till vänster lyfts
                        if(filosof->pos==0)             //samma vänstra pinne visas två gånger på pos 0
                                filosof->table[filosof->noPhils*2] = '|';
                        filosof->table[(filosof->pos)*2] = '|';
                        if(!pthread_mutex_trylock(filosof->chop_stick_r)) {
                                sec=(unsigned int) rand()%3+1;
                                printf("Philosopher %d eating for %d secs\n",filosof->pos,sec);
                                filosof->status='E';

                                //ätpinne till vänster lyfts
                                if(filosof->pos==0)             //samma vänstra pinne visas två gånger på pos 0
                                        filosof->table[filosof->noPhils*2] = '|';
                                filosof->table[(filosof->pos)*2] = '|';
                                //filosofer äter mellan pinnarna
                                filosof->table[(filosof->pos)*2+1]=filosof->status;
                                //ätpinne till höger lyfts
                                if(filosof->pos==(filosof->noPhils-1)) //samma högra pinne visas två gånger på högsta pos
                                        filosof->table[0]='|';
                                filosof->table[(filosof->pos)*2+2] = '|';

                                sleep(sec);

                                filosof->status='T';

                                //ätpinne till vänster läggs ned
                                if(filosof->pos==0)             //samma vänstra pinne visas två gånger på pos 0
                                        filosof->table[filosof->noPhils*2] = '_';
                                filosof->table[(filosof->pos)*2] = '_';
                                //filosofer äter mellan pinnarna
                                filosof->table[(filosof->pos)*2+1]=filosof->status;
                                if(filosof->pos==(filosof->noPhils-1)) //samma högra pinne visas två gånger på högsta pos
                                        filosof->table[0]='_';
                                filosof->table[(filosof->pos)*2+2] = '_';
                                pthread_mutex_unlock(filosof->chop_stick_l);
                                pthread_mutex_unlock(filosof->chop_stick_r);

                                sec=(unsigned int) rand()%5+2;
                                sleep(sec);
                        }
                        else {
                                //printf("DL %d!\t",filosof->pos);
                                filosof->status='W';
                                //filosofer väntar på att nästa pinne ska bli ledig
                                filosof->table[(filosof->pos)*2+1]=filosof->status;
                                if(filosof->deadlock) (pthread_mutex_lock(filosof->chop_stick_r));

                                else if ((filosof->pos+priority)%2 && !filosof->deadlock) {
                                        int i=2;
                                        while ((assert = pthread_mutex_trylock(filosof->chop_stick_r)) && priority) {
                                                //if(0 < i--)
                                                //printf("phil%d", filosof->pos);
                                        }
                                        if(!assert) {
                                                printf("Philosopher %d eating!\n", filosof->pos);
                                                //ätpinne till vänster lyfts
                                                if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                                        filosof->table[filosof->noPhils * 2] = '|';
                                                filosof->table[(filosof->pos) * 2] = '|';
                                                //filosofer äter mellan pinnarna
                                                filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                                //ätpinne till höger lyfts
                                                if (filosof->pos == (filosof->noPhils - 1)) //samma högra pinne visas två gånger på högsta pos
                                                        filosof->table[0] = '|';
                                                filosof->table[(filosof->pos) * 2 + 2] = '|';

                                                sec=(unsigned int) rand()%3+1;
                                                sleep(sec);

                                                filosof->status = 'T';
                                                //ätpinne till vänster läggs ned
                                                if (filosof->pos == 0)             //samma vänstra pinne visas två gånger på pos 0
                                                        filosof->table[filosof->noPhils * 2] = '_';
                                                filosof->table[(filosof->pos) * 2] = '_';
                                                //filosofer äter mellan pinnarna
                                                filosof->table[(filosof->pos) * 2 + 1] = filosof->status;
                                                if (filosof->pos == (filosof->noPhils - 1)) //samma högra pinne visas två gånger på högsta pos
                                                        filosof->table[0] = '_';
                                                filosof->table[(filosof->pos) * 2 + 2] = '_';
                                                pthread_mutex_unlock(filosof->chop_stick_l);
                                                pthread_mutex_unlock(filosof->chop_stick_r);

                                                sec=(unsigned int) rand()%5+2;
                                                sleep(sec);
                                        }
                                        else pthread_mutex_unlock(filosof->chop_stick_l);
                                }
                                else {
                                        pthread_mutex_unlock(filosof->chop_stick_l);
                                        //printf("Player %d: \"OK, I'll sit here and ponder world peace
                                        // for %d seconds!\"\n",filosof->pos, sec=rand()%5+2);
                                        filosof->status='T';
                                        filosof->table[(filosof->pos)*2+1]=filosof->status;
                                        sec=(unsigned int) rand()%5+2;
                                        sleep(sec);
                                }
                        }
                }
                else {
                        filosof->status='T';
                        filosof->table[(filosof->pos)*2+1]=filosof->status;
                        //printf("Philosopher %d philosophizing for %d secs\n",filosof->pos,sec=rand()%5+2);
                        sec=(unsigned int) rand()%5+2;
                        sleep(sec);
                }
                //printf("Player %d in round %d\n", filosof->pos,*(filosof->round));
        }while((*(filosof->round))<48);
        printf("Player %d in round %d\n", filosof->pos,*(filosof->round));
        return NULL;
}

#endif //TEST_GAME_FUNCTIONS_FILOSOFER_H
