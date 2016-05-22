#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "game.h"
#include "players.h"

int main(void) {
    char *card = malloc(3);
    strcpy(card, "FF");

    Card sorted_deck[52];
    Card shuffled_deck[52];
    new_deck(sorted_deck);
    shuffle_deck(sorted_deck,shuffled_deck);
    char *deck[52];
    convert_card_struct(shuffled_deck,deck);
    //for(int i=0;i<52;i++){
    //    deck[i] = malloc(3);
    //    sprintf(deck[i],"%x%x",(shuffled_deck[i].suit),(shuffled_deck[i].value));
    //}

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
        Game game;

        memcpy(game.deck,deck,sizeof(deck));
        Player tmp={i,0,game};
        memcpy(&player[i], (void *) &tmp, sizeof(tmp));
    }


/*    Game game={{card},{card},{card},{card},{0},{0},{0}};

    for(int i=0;i<13;i++){
        game.hand[i] = malloc(3);
        memcpy(game.hand[i],card,3);
    }
    for(int i=0;i<4;i++){
        memcpy(game.hands[i],game.hand,sizeof(game.hand));
    }
    printf("random hand: %s\n", game.hands[2][2]);

    //player[0].pos = 0;
    //player[0].game.hand[0]="FF";



    char *hand[13] = {"00"};
    char *hands[4][13] = {hand[0]};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            hand[j] = malloc(3);
            sprintf(hand[j], "%x%x", i, j);
        }
        memcpy(hands[i], hand, 39);
    }*/
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
/*    for (int i=0;i<4;i++){
        player[i].pos = i;
        if(!(pthread_create(player_id[i],NULL,&player_waits_or_plays,NULL))){ //(void *) &player[i]))){
            printf("Error: %s %d\n","pthread_create",i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i=0;i<4;i++){
        pthread_join(threads[i],NULL);
    }*/

/*
int test(void){

    char *trick[] = {"00", "25", "2A", "3B"};
    int hand_score[4] = {0, 0, 0, 0};
    int round_score[4] = {0, 0, 0, 0};
    int i = 0, j = 0;
    for (i = 0; i < 4; i++) {
        if (is_hearts(trick[i][0])) fprintf(stdout, "%s \n", trick[i]);
        if (is_queen_of_spades(trick[i])) fprintf(stdout, "%s \n", "Queen of spades");
        if (is_two_of_clubs(trick[i])) fprintf(stdout, "%s \n", "Two of clubs!");
    }
    printf("score: %d\n", calculate_trick(trick));
    printf("winner: %d\n", check_winner(trick, 2));
    char *hand0[] = {"00","01","02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C"};
    char *hand_1[] = {"10","11","12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C"};
    char *hand_2[] = {"02", "03", "00", "06", "0A", "1B", "1C", "20", "28", "2A", "30", "33", "3A"};
    char *hand_3[] = {"02", "03", "01", "06", "0A", "1B", "1C", "20", "28", "2A", "30", "33", "3A"};
    char **playerhands[13] = {hand0, hand_1, hand_2, hand_3};
    printf("I start: %d\n", do_I_start(hand0));
    printf("starter: %d\n", who_starts(playerhands));
    printf("winner: %d\n", check_winner(trick, who_starts(playerhands)));
    for (i = 0; i < 3; i++) {
        update_hand_score(hand_score, calculate_trick(trick), i % 4);
        printf("scores: ");
        for (j = 0; j < 4; j++) printf("%d ", hand_score[j]);
        printf("\n");
    }

    add_list1_to_list0(round_score, hand_score, 0, 4);
    print_list(stdout, ";", hand_score, 4);
    printf("\n");
    add_list1_to_list0(round_score, round_score, 0, 4);
    print_list(stdout, " ", round_score, 4);
    printf("\n");

    int list[] = {0, 26, 26, 0};
    int winner = check_winner(trick, 2);
    add_list1_to_list0(hand_score, list, winner, winner + 1);
    print_list(stdout, "-", hand_score, 4);
    printf("\n");
    is_twenty_six(list,4);
    printf("hand: ");
    for (i = 0; i < 4; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
    for (i; !is_hundred(list); list[0] += 16) printf("is list 100: %d\n", list[(i++) % 4]);
    print_list(stdout, ";", list, 4);
    printf("\n");

    update_round_score(round_score, hand_score, 4);
    //print_list(stdout," ",round_score,4);
    printf("\n");
    char *trickr[4] = {"Hello!", "can you hear me?!"};
    int list2[] = {5, 10, 20, 0};
    //add_list1_to_list0(list,list2,0,4);
    printf("\n%d \n", list2[1]);
    printf("list printed\n");
    //print_list(stderr,";",list2,2);

    printf("02;3A;11;0C\n");
    char what[] = "02;3A;11;0C";
    printf("What: %s\n", what);

    char *token, *string, *tofree;

    tofree = string = strdup(what);
    assert(string != NULL);

    free(tofree);
    free(token);
    printf("yo!\n");
    printf("hur lång är strängen?: %d\n", (int) strlen(string));
    int ret;
    ret = split(string, ';', trickr);
    printf("strings: %d\n", ret);


    i = 0;
    while(i<4) printf("%s ",trickr[i++]);
    printf("\n");

    char *hex[13];
    char *two[2];
    FF_hand(hand0);
    i=0;
    while(i<13) printf("%s-",hand0[i++]);

    return 0;


}*/