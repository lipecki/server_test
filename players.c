//
//  players.c
//  Test
//
//  Created by Johan Lipecki on 2016-04-15.
//  Copyright © 2016 Johan Lipecki. All rights reserved.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include "players.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* player_waits_or_plays (void *arguments) {


    Player *me = (Player *) arguments;
    char *player_hand[13];
    char cards_to_send[40];
    int len;
    //memcpy(player_hand,me->game.hand,39);

    char trick_to_send[20];
    int i =0;
    socklen_t slen=sizeof(struct sockaddr_in);
    while(i < 2) {
        pthread_mutex_lock(&mutex1);
        char *tmp[200];
        printf("\nI'm pos :%d\n", me->pos);
        printf("My hand is: %s\n",me->game->buffer[me->pos]);
        len =sizeof(me->game->hands[me->pos]);
        printf("client buffer: %s\n,len: %d\n ip-address: %s",me->game->buffer[me->pos],len, inet_ntoa(me->si_other->sin_addr));

        if (sendto(*(me->sockfd),(void *) me->game->buffer[me->pos],
                   (size_t) sizeof(me->game->buffer[me->pos]), 0,
                   (const struct sockaddr *) me->si_other,
                   slen)==-1)
            perror("sendto() FAILED");
        i++;
        }
    //if (sendto(me->sockfd, cards_to_send, (size_t) (len * 13), 0, (const struct sockaddr *) me->si_other, (socklen_t) slen) == -1)
    pthread_mutex_unlock(&mutex1);
    sleep(3);

    pthread_exit(EXIT_SUCCESS);
}
int Initiate_players() {
    char *card = malloc(3);
    strcpy(card, "FF");

    Card sorted_deck[52];
    Card shuffled_deck[52];
    char *deck[52];
    char buf[BUFLEN]={"initiator"};
    new_deck(sorted_deck);
    shuffle_deck(sorted_deck,shuffled_deck);
    convert_card_struct(shuffled_deck,deck,buf);
    Game *game = malloc(sizeof(Game));
    memcpy(game->deck,deck,sizeof(deck));

    int counter[4];
    for(int i=0;i<4;i++){
        counter[i] = i;
    }

    pthread_t players[4];
    for(int i=0;i<4;i++){
        pthread_t tmp=0;
        players[i] = tmp;
    }
    //Player thread arguments are initiated
    Player player[4];
    for(int i=0;i<4;i++){
        Player tmp={i,0,game};
        memcpy(&player[i], (void *) &tmp, sizeof(tmp));
    }

    int j=0;
    printf("\nSpelare %d initierad!\n", player[j].pos);
    counter[j++] = pthread_create(&players[j], NULL, &player_waits_or_plays, (void *) &player[j]);
    printf("\nSpelare %d initierad!\n", player[j].pos);
    counter[j++] = pthread_create(&players[j], NULL, &player_waits_or_plays, (void *) &player[j]);
    printf("\nSpelare %d initierad!\n", player[j].pos);
    counter[j++] = pthread_create(&players[j], NULL, &player_waits_or_plays, (void *) &player[j]);
    printf("\nSpelare %d initierad!\n", player[j].pos);
    counter[j++] = pthread_create(&players[j], NULL, &player_waits_or_plays, (void *) &player[j]);



    int k=0;
    pthread_join((players[k++]),NULL);
    pthread_join((players[k++]),NULL);
    pthread_join((players[k++]),NULL);
    pthread_join((players[k++]),NULL);
    return 0;
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
     return 0;
}
*/



