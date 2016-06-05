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


#endif //TEST_GAME_FUNCTIONS_FILOSOFER_H
