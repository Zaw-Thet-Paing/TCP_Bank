#include "server_header.h"
#include "bank.h"

void handle_client_request(SOCKET client);
void handleCtrlC(int signum);

int main()
{

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo("127.0.0.1", "8888", &hints, &bind_address);

    //creating socket
    SOCKET server;
    server = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if(server == -1){
        printf("Error: creating socket!\n");
        exit(1);
    }

    int reuse = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        printf("Error: set socket option!\n");
        exit(1);
    }

    //binding socket to local address
    if(bind(server, bind_address->ai_addr, bind_address->ai_addrlen) == -1){
        printf("Error: binding socket to local address!\n");
        exit(1);
    }

    //listening
    if(listen(server, 10) == -1){
        printf("Error: server listen!\n");
        exit(1);
    }

    //printing listening address:port
    char listen_address[20];
    char listen_port[20];
    getnameinfo(bind_address->ai_addr, bind_address->ai_addrlen, listen_address, sizeof(listen_address), listen_port, sizeof(listen_port), NI_NUMERICHOST);
    printf("Server listen on %s:%s\n", listen_address, listen_port);
    printf("Press Ctrl+C to stop the server!\n");
    freeaddrinfo(bind_address);

    signal(SIGINT, handleCtrlC);
    space_counter();
    loading_all_data_from_file();
    
    while(1){
        printf("\nPrinting all data from AVL\n");
        printInOrder(root);
        //waiting for connection...
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET client = accept(server, (struct sockaddr*)&client_address, &client_len);
        if(client == -1){
            printf("Error: accept client connection!\n");
            exit(1);
        }
        char accept_address[20];
        char accept_port[20];
        getnameinfo((struct sockaddr*)&client_address, client_len, accept_address, sizeof(accept_address), accept_port, sizeof(accept_port), NI_NUMERICHOST);
        printf("Accept connection from %s:%s\n", accept_address, accept_port);

        handle_client_request(client);

    }


    return 0;
}

void handle_client_request(SOCKET client)
{
    //client request
    char received_buffer[4096];
    memset(&received_buffer, '\0', sizeof(received_buffer));
    int received_bytes = recv(client, received_buffer, 4096, 0);
    printf("\nClient request : %s\n", received_buffer);

    int word_count = word_count_from_string(received_buffer);
    // printf("Word count : %d\n", word_count);
    char *recv_arr[word_count];
    char *token = strtok(received_buffer, " ");
    int i = 0;
    while(token != NULL){
        recv_arr[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    // printf("Keyword : %s\n", recv_arr[0]);
    if(strcmp(recv_arr[0], "register") == 0){
        register_sector(client, recv_arr);
    }else if(strcmp(recv_arr[0], "check_email_exists") == 0){
        check_email_exists(client, root, recv_arr[1]);
    }else if(strcmp(recv_arr[0], "check_nrc_exists") == 0){
        check_nrc_exists(client, root, recv_arr[1]);
    }else if(strcmp(recv_arr[0], "check_id_exists") == 0){
        unsigned int user_id = strtoul(recv_arr[1], NULL, 10);
        check_id_exists(client, root, user_id);
    }else if(strcmp(recv_arr[0], "tr_note") == 0){
        transfer_record_note(client, recv_arr);
    }else if(strcmp(recv_arr[0], "cash_in_out") == 0){
        cash_in_out_note(client, recv_arr);
    }else if(strcmp(recv_arr[0], "loan") == 0){
        loan_note(client, recv_arr);
    }else if(strcmp(recv_arr[0], "account_type_update") == 0){
        account_type_update_note(client, recv_arr);
    }

    // printf("Keyword : %s\n", recv_arr[0]);

}

void handleCtrlC(int signum)
{
    printf("\nServer close...\n");
    recording_all_data_to_file(root);
    printf("AVL data saved to file!\n");
    exit(signum);
}