#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include "game.h"
#include "players.h"

#define BUFLEN 512
#define NPACK 4
#define GAMEPORT 41337

void diep(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}
// from UDP made simple at https://www.abc.se/~m6695/udp.html
int main(int argc, char *argv[]) {
	

	struct sockaddr_in si_me, si_other[4];
	int fd, s, i, len=13, slen=sizeof(si_other);

	fd = fopen("/var/tmp/serve_client","w");
	char buf[BUFLEN]={"start"};
	char *trick[4];
	FF_trick(trick);

	//Game initiation, shuffling and dealing.
	Card sorted_deck[52];
	Card shuffled_deck[52];
	char *deck[52];
	new_deck(sorted_deck);
	shuffle_deck(sorted_deck,shuffled_deck);
	convert_card_struct(shuffled_deck,deck,buf);
	Game *game = malloc(sizeof(Game));
	memcpy(game->deck,deck,sizeof(deck));
	printf("something! %s\n",buf);

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");
	printf("socket open\n");
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(GAMEPORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, &si_me, sizeof(si_me))==-1)
		diep("bind");
	printf("bound\n");

	//Player initiation
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
		memset((char *) &si_other[i], 0, sizeof(si_other));
		Player tmp={i,s,game,&si_me,&si_other[i]};
		memcpy(&player[i], (void *) &tmp, sizeof(tmp));
	}


	int j=0;
	void *buffer = (void *) strdup(buf);
	while(strcmp(buffer,"quit")){
		strcpy(player[j].game->buffer,buf);
		printf("buffer: %s \n",buf);
		if ((len = recvfrom(player[j].sockfd, player[j].game->buffer, BUFLEN, 0, &si_other[i], &slen)) == -1) diep("recvfrom()");
		counter[j] = pthread_create(&players[j], NULL, &player_waits_or_plays, (void *) &player[j]);
		printf(buf);

	/*	//Receive data and start game threads for each client
		for(int j=0;j<4;j++) {
			if ((len = recvfrom(player[j].sockfd, player[j].game->buffer, BUFLEN, 0, &si_other[i], &slen)) == -1) diep("recvfrom()");
			counter[j++] = pthread_create(&players[j], NULL, &player_waits_or_plays, (void *) &player[j]);
			printf("Received packet from %s:%d\nData: %s\nLength: %d\n",
			       inet_ntoa(si_other[i].sin_addr), ntohs(si_other[i].sin_port), (char *) buffer,len);
		}

// Jag försöker föra över detta till trådarna--------------------
*/		compile_card_string(shuffled_deck,buf);
		for(int j=0;j<13;j++) printf("%s;",deck);
		for (i=3; i<NPACK; i++) {
			printf("Sending packet %s\n", buffer);
			sprintf(buf, "This is packet %d\n", i);
			if (sendto(s, buffer, BUFLEN, 0, &si_other,  slen)==-1)
				diep("sendto()");
			}
	}
	int k=0;
	pthread_join((players[k++]),NULL);
	pthread_join((players[k++]),NULL);
	pthread_join((players[k++]),NULL);
	pthread_join((players[k++]),NULL);
	printf("the end!");
    	close(s);
    	return 0;
}
