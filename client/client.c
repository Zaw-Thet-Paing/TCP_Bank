#include "client_header.h"

SOCKET start_client();

#include "bank.h"

char address[20];
char port[20];

int main(int argc, char *argv[])
{

    // if(argc < 3){
    //     printf("Usage: tcp_client hostname port\n");
    //     exit(1);
    // }

    strcpy(address, "127.0.0.1");
    strcpy(port, "8888");

    main_sector();

    // get_current_time();

    //test total amount same day
    // strcpy(data.hist[0].note, "2_Send_Money_To_Bob_$20000_at_Thu!Jun#20_22:18:58@2023");
    // strcpy(data.hist[1].note, "2_Send_Money_To_Bob_$2000_at_Thu!Jul#21_22:18:58@2023");
    // strcpy(data.hist[2].note, "2_Send_Money_To_Bob_$2000_at_Thu!Jul#21_22:18:58@2023");
    // unsigned int total_amount = get_total_amount_of_same_day(data);

    // printf("Total amount same day : %u\n", total_amount);


    return 0;
}

SOCKET start_client()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *peer_address;
    getaddrinfo(address, port, &hints, &peer_address);

    //creating socket
    SOCKET client;
    client = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
    if(client == -1){
        printf("Error: creating socket!\n");
        exit(1);
    }

    //connection
    if(connect(client, peer_address->ai_addr, peer_address->ai_addrlen) == -1){
        printf("Error: connection to server!\n");
        exit(1);
    }
    freeaddrinfo(peer_address);

    return client;

}