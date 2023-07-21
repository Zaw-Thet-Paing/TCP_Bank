#include "server_header.h"
#include "z_library.h"
#include "avl.h"

void register_sector(SOCKET client, char *client_data[]);
void recording_all_data_to_file(Node* node);
void write_data_to_file(Node* node, FILE *fptr);
void loading_all_data_from_file();
int count_line_from_file();

Node* searchNodeByEmail(Node* node, char email[]);
Node* searchNodeByNRC(Node* node, char nrc[]);
Node* searchNodeByID(Node* node, unsigned int id);
void check_email_exists(SOCKET client, Node* node, char email[]);
void check_nrc_exists(SOCKET client, Node* node, char nrc[]);
void check_id_exists(SOCKET client, Node* node, unsigned int id);

//user sector
void transfer_record_note(SOCKET client, char *client_data[]);
void cash_in_out_note(SOCKET client, char *client_data[]);
void loan_note(SOCKET client, char *client_data[]);
void account_type_update_note(SOCKET client, char *client_data[]);

void space_counter();

//global variables
int space_array[100];

void register_sector(SOCKET client, char *client_data[])
{

    unsigned int id = countNode(root);

    unsigned int monthlyIncome = strtoul(client_data[8], NULL, 10);
    unsigned int loanAmount = strtoul(client_data[9], NULL, 10);
    unsigned int currentAmount = strtoul(client_data[10], NULL, 10);
    unsigned int walletCapacity = strtoul(client_data[11], NULL, 10);
    unsigned int amountLimitPerDay = strtoul(client_data[12], NULL, 10);

    Data data;
    data.id = id + 1001;
    strcpy(data.name, client_data[1]);
    strcpy(data.phone, client_data[2]);
    strcpy(data.email, client_data[3]);
    strcpy(data.password, client_data[4]);
    strcpy(data.nrc, client_data[5]);
    strcpy(data.address, client_data[6]);
    strcpy(data.account_type, client_data[7]);
    data.monthly_income = monthlyIncome;
    data.loan_amount = loanAmount;
    data.current_amount = currentAmount;
    data.wallet_capacity = walletCapacity;
    data.amount_limit_per_day = amountLimitPerDay;

    root = insertNode(root, data);

    //success response
    char register_response_buffer[10] = "success";
    int response_bytes = send(client, register_response_buffer, strlen(register_response_buffer), 0);

}

void recording_all_data_to_file(Node* node)
{
    FILE *fptr;
    fptr = fopen("db.txt", "w");

    if(fptr == NULL){
        printf("Error: recording all data to file!\n");
        exit(1);
    }else{
        write_data_to_file(node, fptr);
    }

    fclose(fptr);
}

void write_data_to_file(Node* node, FILE *fptr)
{
    if(node != NULL){
        write_data_to_file(node->left, fptr);

        fprintf(fptr, "%u %s %s %s %s %s %s %s %u %u %u %u %u", node->data.id, node->data.name, node->data.phone, node->data.email, node->data.password, node->data.nrc, node->data.address, node->data.account_type, node->data.monthly_income, node->data.loan_amount, node->data.current_amount, node->data.wallet_capacity, node->data.amount_limit_per_day);

        if(countHistory(node) != 0){
            for(int i = 0; i<countHistory(node); i++){
                fprintf(fptr, " %s", node->data.hist[i].note);
            }
        }

        fprintf(fptr, "\n");

        write_data_to_file(node->right, fptr);
    }
}

void loading_all_data_from_file()
{

    FILE *fptr;
    fptr = fopen("db.txt", "r");
    if(fptr == NULL){
        printf("Error: loading all data from file!\n");
        exit(1);
    }else{
        int line_count = count_line_from_file();
        for(int i = 0; i<line_count; i++){
            Data data;
            memset(&data, 0, sizeof(Data));
            fscanf(fptr, "%u %s %s %s %s %s %s %s %u %u %u %u %u", &data.id, data.name, data.phone, data.email, data.password, data.nrc, data.address, data.account_type, &data.monthly_income, &data.loan_amount, &data.current_amount, &data.wallet_capacity, &data.amount_limit_per_day);

            for(int j = 0; j<space_array[i]-12;  j++){
                fscanf(fptr, " %s", data.hist[j].note);
            }
            
            root = insertNode(root, data);
        }
    }
    fclose(fptr);
}

void space_counter()
{
    FILE *fptr;
    fptr = fopen("db.txt", "r");
    if(fptr == NULL){
        printf("Error: Call Space Counter Function\n");
        exit(1);
    }else{
        char c = fgetc(fptr);
        int index = 0;
        while(!feof(fptr)){
            if(c != '\n'){
                if(c == ' '){
                    space_array[index]++;
                }
                c= fgetc(fptr);
            }else{
                index++;
                c = fgetc(fptr);
            }
        }

        // for(int i = 0; i<100; i++){
        //     printf("%d ", space_array[i]);
        // }
        // printf("\n");

    }
    fclose(fptr);
}

int count_line_from_file()
{
    FILE *fptr;
    int count = 0;
    fptr = fopen("db.txt", "r");
    if(fptr == NULL){
        printf("Error: count line from file!\n");
        exit(1);
    }else{
        char c = fgetc(fptr);
        while(c != EOF){
            if(c == '\n'){
                count++;
            }
            c = fgetc(fptr);
        }
    }
    fclose(fptr);
    return count;
}

Node* searchNodeByEmail(Node* node, char email[])
{
    if(node == NULL || strcmp(node->data.email, email) == 0){
        return node;
    }

    if(strcmp(node->data.email, email) > 0){
        return searchNodeByEmail(node->left, email);
    }
    return searchNodeByEmail(node->right, email);
}

Node* searchNodeByNRC(Node* node, char nrc[])
{
    if(node == NULL || strcmp(node->data.nrc, nrc) == 0){
        return node;
    }

    if(strcmp(node->data.nrc, nrc) > 0){
        return searchNodeByNRC(node->left, nrc);
    }
    return searchNodeByNRC(node->right, nrc);
}

Node* searchNodeByID(Node* node, unsigned int id)
{
    if(node == NULL || node->data.id == id){
        return node;
    }

    if(node->data.id > id){
        return searchNodeByID(node->left, id);
    }
    return searchNodeByID(node->right, id);
}

void check_email_exists(SOCKET client, Node* node, char email[])
{
    Node* result = searchNodeByEmail(node, email);
    if(result == NULL){
        // printf("NotExists response!\n");
        char response_buffer[10] = "NotExists";
        int response_bytes = send(client, response_buffer, strlen(response_buffer), 0);
    }else{
        // printf("Node response!\n");
        //changing unsigned int to char array
        char id[10];
        memset(&id, '\0', sizeof(id));
        char monthlyIncome[10];
        memset(&monthlyIncome, '\0', sizeof(monthlyIncome));
        char loanAmount[10];
        memset(&loanAmount, '\0', sizeof(loanAmount));
        char currentAmount[10];
        memset(&currentAmount, '\0', sizeof(currentAmount));
        char walletCapacity[10];
        memset(&walletCapacity, '\0', sizeof(walletCapacity));
        char amountLimitPerDay[10];
        memset(&amountLimitPerDay, '\0', sizeof(amountLimitPerDay));

        sprintf(id, "%u", result->data.id);
        sprintf(monthlyIncome, "%u", result->data.monthly_income);
        sprintf(loanAmount, "%u", result->data.loan_amount);
        sprintf(currentAmount, "%u", result->data.current_amount);
        sprintf(walletCapacity, "%u", result->data.wallet_capacity);
        sprintf(amountLimitPerDay, "%u", result->data.amount_limit_per_day);
        // printf("Changing int to char success!\n");

        char response_buffer[4096] = "email_exists ";
        
        strcat(response_buffer, id);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.name);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.phone);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.email);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.password);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.nrc);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.address);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.account_type);
        strcat(response_buffer, " ");
        strcat(response_buffer, monthlyIncome);
        strcat(response_buffer, " ");
        strcat(response_buffer, loanAmount);
        strcat(response_buffer, " ");
        strcat(response_buffer, currentAmount);
        strcat(response_buffer, " ");
        strcat(response_buffer, walletCapacity);
        strcat(response_buffer, " ");
        strcat(response_buffer, amountLimitPerDay);

        for(int i = 0; i<countHistory(result); i++){
            strcat(response_buffer, " ");
            strcat(response_buffer, result->data.hist[i].note);
        }

        // printf("Data response : %s\n", response_buffer);
        // printf("set data to response buffer success!\n");

        int responst_bytes = send(client, response_buffer, strlen(response_buffer), 0);
        // printf("End of node response!\n");
    }
}

void check_nrc_exists(SOCKET client, Node* node, char nrc[])
{
    Node* result = searchNodeByNRC(node, nrc);
    if(result == NULL){
        //NotExists
        char nrc_exists_check[10] = "NotExists";
        int bytes = send(client, nrc_exists_check, strlen(nrc_exists_check), 0);
    }else{
        //exists
        char nrc_exists_check[10] = "Exists";
        int bytes = send(client, nrc_exists_check, strlen(nrc_exists_check), 0);
    }
}

void check_id_exists(SOCKET client, Node* node, unsigned int id)
{
    Node* result = searchNodeByID(node, id);
    if(result == NULL){
        //NotExists
        char id_exists_check[10] = "NotExists";
        int bytes = send(client, id_exists_check, strlen(id_exists_check), 0);
    }else{
        //Exists
        //changing unsigned int to char array
        char id[10];
        memset(&id, '\0', sizeof(id));
        char monthlyIncome[10];
        memset(&monthlyIncome, '\0', sizeof(monthlyIncome));
        char loanAmount[10];
        memset(&loanAmount, '\0', sizeof(loanAmount));
        char currentAmount[10];
        memset(&currentAmount, '\0', sizeof(currentAmount));
        char walletCapacity[10];
        memset(&walletCapacity, '\0', sizeof(walletCapacity));
        char amountLimitPerDay[10];
        memset(&amountLimitPerDay, '\0', sizeof(amountLimitPerDay));

        sprintf(id, "%u", result->data.id);
        sprintf(monthlyIncome, "%u", result->data.monthly_income);
        sprintf(loanAmount, "%u", result->data.loan_amount);
        sprintf(currentAmount, "%u", result->data.current_amount);
        sprintf(walletCapacity, "%u", result->data.wallet_capacity);
        sprintf(amountLimitPerDay, "%u", result->data.amount_limit_per_day);

        char response_buffer[4096] = "id_exists ";
        
        strcat(response_buffer, id);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.name);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.phone);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.email);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.password);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.nrc);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.address);
        strcat(response_buffer, " ");
        strcat(response_buffer, result->data.account_type);
        strcat(response_buffer, " ");
        strcat(response_buffer, monthlyIncome);
        strcat(response_buffer, " ");
        strcat(response_buffer, loanAmount);
        strcat(response_buffer, " ");
        strcat(response_buffer, currentAmount);
        strcat(response_buffer, " ");
        strcat(response_buffer, walletCapacity);
        strcat(response_buffer, " ");
        strcat(response_buffer, amountLimitPerDay);

        for(int i = 0; i<countHistory(result); i++){
            strcat(response_buffer, " ");
            strcat(response_buffer, result->data.hist[i].note);
        }

        // printf("Data response : %s\n", response_buffer);
        // printf("set data to response buffer success!\n");

        int responst_bytes = send(client, response_buffer, strlen(response_buffer), 0);
    }
}

void transfer_record_note(SOCKET client, char *client_data[])
{
    Node* userNode = searchNodeByEmail(root, client_data[1]);
    Node* receiverNode = searchNodeByEmail(root, client_data[4]);

    // printf("User name : %s\n", userNode->data.name);
    // printf("Receiver name : %s\n", receiverNode->data.name);

    unsigned int userCurrentAmount = strtoul(client_data[2], NULL, 10);
    unsigned int receiverCurrentAmount = strtoul(client_data[5], NULL, 10);

    userNode->data.current_amount = userCurrentAmount;
    receiverNode->data.current_amount = receiverCurrentAmount;

    int userHistCount = countHistory(userNode);
    int receiverHistCount = countHistory(receiverNode);

    //to handle ramdon data
    memset(&userNode->data.hist[userHistCount+1].note, '\0', sizeof(userNode->data.hist[userHistCount+1].note));
    memset(&receiverNode->data.hist[receiverHistCount+1].note, '\0', sizeof(receiverNode->data.hist[receiverHistCount+1].note));

    strcpy(userNode->data.hist[userHistCount].note, client_data[3]);
    strcpy(receiverNode->data.hist[receiverHistCount].note, client_data[6]);

    printf("\nTransfer Receive Record saved!\n");

}

void cash_in_out_note(SOCKET client, char *client_data[])
{
    Node *user = searchNodeByEmail(root, client_data[1]);
    unsigned int currentAmount = strtoul(client_data[2], NULL, 10);

    user->data.current_amount = currentAmount;
    
    int histCount = countHistory(user);

    //handle random input
    memset(&user->data.hist[histCount+1].note, '\0', sizeof(user->data.hist[histCount+1].note));

    strcpy(user->data.hist[histCount].note, client_data[3]);

    printf("\nCash in out Record saved!\n");
}

void loan_note(SOCKET client, char *client_data[])
{

    Node *user = searchNodeByEmail(root, client_data[1]);
    unsigned int currentAmount = strtoul(client_data[2], NULL, 10);
    unsigned int loanAmount = strtoul(client_data[3], NULL, 10);

    user->data.current_amount = currentAmount;
    user->data.loan_amount = loanAmount;

    int histCount = countHistory(user);

    //handle random input
    memset(&user->data.hist[histCount+1].note, '\0', sizeof(user->data.hist[histCount+1].note));

    strcpy(user->data.hist[histCount].note, client_data[4]);

    printf("\nLoan Record saved!\n");
}   

void account_type_update_note(SOCKET client, char *client_data[])
{
    Node *user = searchNodeByEmail(root, client_data[1]);

    int histCount = countHistory(user);
    memset(&user->data.hist[histCount+1].note, '\0', sizeof(user->data.hist[histCount+1].note));
    memset(&user->data.account_type, '\0', sizeof(user->data.account_type));

    strcpy(user->data.account_type, "Business");
    user->data.wallet_capacity = 10000000;
    user->data.amount_limit_per_day = 5000000;
    strcpy(user->data.hist[histCount].note, client_data[2]);
    

    printf("\nAccount type update record saved!\n");
}