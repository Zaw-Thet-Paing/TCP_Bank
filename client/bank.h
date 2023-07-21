/*
Personal Account
----------------
Wallet Capacity : 1,000,000
Amount Limit Per Day : 500,000

Business Account
----------------
Wallet Capacity : 10,000,000
Amount Limit Per Day : 5,000,000
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "z_library.h"
#include "data.h"

struct Date{
    char day[5];
    char month[5];
    char year[5];
};

typedef struct Date Date;

void main_sector();
void register_sector();
void login_sector();

void user_sector();
void user_profile();
void user_profile_update_record();
void user_transfer_money();
void continue_user_transfer_money();
void user_transfer_money_record(Data receiver, unsigned int amount);
void user_cash_in_out();
void user_cash_in();
void user_cash_out();
void user_cash_in_out_record(char type[], unsigned int amount);
void user_loan();
void continue_user_loan_take();
void user_loan_record(char type[], unsigned int amount);
void user_history();
int count_history(Data data);

//amount limit per day
Date get_current_date();
Date get_date_from_record(char note[]);
unsigned int get_total_amount_of_same_day(Data userData);
unsigned int get_amount_from_record(char note[]);

//global variable
Data data; // to store user data
// int record = count_history(data); // to count how many record in hist

void main_sector()
{
    char input[10];
    int option = 0;
    printf("\n*********************\n");
    printf("*Welcome to our Bank*\n");
    printf("*********************\n");
    printf("1. Login\n2. Register\n0. Exit\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);
    if(option == 49){
        login_sector();
    }else if(option == 50){
        register_sector();
    }else if(option == 48){
        printf("Exit...\n");
        exit(0);
    }else{
        printf("Invalid option! Try again!\n");
        main_sector();
    }
}

void register_sector()
{
    printf("\n*****************\n");
    printf("*Register Sector*\n");
    printf("*****************\n");

    char rName[50];
    memset(&rName, '\0', sizeof(rName));
    char rPhone[50];
    memset(&rPhone, '\0', sizeof(rPhone));
    char rEmail[50];
    memset(&rEmail, '\0', sizeof(rEmail));
    char rPassword[50];
    memset(&rPassword, '\0', sizeof(rPassword));
    char rNRC[25];
    memset(&rNRC, '\0', sizeof(rNRC));
    char rAddress[50];
    memset(&rAddress, '\0', sizeof(rAddress));
    char rAccountType[10];
    memset(&rAccountType, '\0', sizeof(rAccountType));
    unsigned int rMonthlyIncome = 0;
    char monthlyIncome[20];
    memset(&monthlyIncome, '\0', sizeof(monthlyIncome));
    unsigned int rLoanAmount = 0;
    char loanAmount[20];
    memset(&loanAmount, '\0', sizeof(loanAmount));
    unsigned int rCurrentAmount = 0;
    char currentAmount[20];
    memset(&currentAmount, '\0', sizeof(currentAmount));
    unsigned int rWalletCapacity = 0;
    char walletCapacity[20];
    memset(&walletCapacity, '\0', sizeof(walletCapacity));
    unsigned int rAmountLimitPerDay = 0;
    char amountLimitPerDay[20];
    memset(&amountLimitPerDay, '\0', sizeof(amountLimitPerDay));

    printf("Enter your email to register : ");
    scanf(" %[^\n]", rEmail);

    if(is_valid_email(rEmail)){
        //check email exists in server
        SOCKET client_email_check = start_client();
        char request_buffer[] = "check_email_exists ";
        strcat(request_buffer, rEmail);
        int request_bytes = send(client_email_check, request_buffer, strlen(request_buffer), 0);

        char response_buffer[4096];
        memset(&response_buffer, '\0', sizeof(response_buffer));
        int response_bytes = recv(client_email_check, response_buffer, 4096, 0);
        // printf("Response Data : %s\n", response_buffer);
        close(client_email_check);
        if(strcmp(response_buffer, "NotExists") == 0){
            //you can register
            while(1){
                printf("Enter your name to register : ");
                scanf(" %[^\n]", rName);
                if(is_valid_name(rName)){
                    break;
                }else{
                    printf("Name can't contain white space or special char or number!\n");
                }
            }

            while(1){
                printf("Enter your phone to register : ");
                scanf(" %[^\n]", rPhone);
                if(is_valid_phone(rPhone)){
                    break;
                }else{
                    printf("Invalid phone number! (e.g., 959XXXXXXXXX)\n");
                }
            }

            while(1){
                printf("Enter your password to register : ");
                scanf(" %[^\n]", rPassword);
                if(is_strong_password(rPassword)){
                    break;
                }else{
                    printf("Weak Password!\nPassword must contain upper case, lower case, number, special char and must be at least 8 char\n");
                }
            }

            //nrc checking
            while(1){
                printf("Enter you nrc to register : ");
                scanf(" %[^\n]", rNRC);
                if(is_NRC_valid(rNRC)){
                    SOCKET client_nrc_check = start_client();
                    char nrc_request[20] = "check_nrc_exists ";
                    strcat(nrc_request, rNRC);
                    int request_bytes = send(client_nrc_check, nrc_request, strlen(nrc_request), 0);

                    char nrc_response[20];
                    memset(&nrc_response, '\0', sizeof(nrc_response));
                    int response_bytes = recv(client_nrc_check, nrc_response, 20, 0);
                    // printf("Response Data : %s\n", response_buffer);
                    close(client_nrc_check);

                    if(strcmp(nrc_response, "NotExists") == 0){
                        break;
                    }else{
                        printf("Your NRC have already taken! Try with another NRC!\n");
                    }
                    
                }else{
                    printf("Invalid NRC! Only support yangon NRC!\n");
                }
            }

            while(1){
                printf("Enter your account type(Personal, Business) to register : ");
                scanf(" %[^\n]", rAccountType);
                if(is_accountType_valid(rAccountType)){
                    break;
                }else{
                    printf("Account type must be only 'Personal' or 'Business'!\n");
                }
            }

            printf("Enter your monthly income to register : ");
            scanf("%u", &rMonthlyIncome);

            while(1){
                printf("Enter your address to register : ");
                scanf(" %[^\n]", rAddress);
                if(is_space(rAddress)){
                    printf("Address can't contain white space!\n");
                }else{
                    break;
                }
            }

            rLoanAmount = 0;
            rCurrentAmount = 0;
            if(strcmp(rAccountType, "Personal") == 0){
                rWalletCapacity = 1000000;
                rAmountLimitPerDay = 500000;
            }else{
                rWalletCapacity = 10000000;
                rAmountLimitPerDay = 5000000;
            }

            //converting all unsigned int to char array
            sprintf(monthlyIncome, "%u", rMonthlyIncome);
            sprintf(loanAmount, "%u", rLoanAmount);
            sprintf(currentAmount, "%u", rCurrentAmount);
            sprintf(walletCapacity, "%u", rWalletCapacity);
            sprintf(amountLimitPerDay, "%u", rAmountLimitPerDay);
            //client request
            //register data send
            SOCKET client_register = start_client();
            char to_send[1024] = "register ";
            strcat(to_send, rName);
            strcat(to_send, " ");
            strcat(to_send, rPhone);
            strcat(to_send, " ");
            strcat(to_send, rEmail);
            strcat(to_send, " ");
            strcat(to_send, rPassword);
            strcat(to_send, " ");
            strcat(to_send, rNRC);
            strcat(to_send, " ");
            strcat(to_send, rAddress);
            strcat(to_send, " ");
            strcat(to_send, rAccountType);
            strcat(to_send, " ");
            strcat(to_send, monthlyIncome);
            strcat(to_send, " ");
            strcat(to_send, loanAmount);
            strcat(to_send, " ");
            strcat(to_send, currentAmount);
            strcat(to_send, " ");
            strcat(to_send, walletCapacity);
            strcat(to_send, " ");
            strcat(to_send, amountLimitPerDay);
            // printf("Data : %s\n", to_send);
            int send_bytes = send(client_register, to_send, strlen(to_send), 0);

            main_sector();
            close(client_register);
        }else{
            printf("Your email have already in used! Try with another email!\n");
            register_sector();
        }
        
    }else{
        printf("Invalid format! only support valid gmail address!\n");
        register_sector();
    }
    
}

void login_sector()
{
    printf("\n**************\n");
    printf("*Login Sector*\n");
    printf("**************\n");

    char input[10];
    int option = 0;
    printf("1. Login with ID\n2. Login with email\n0. Back to main sector\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    unsigned int login_id = 0;
    char lEmail[50];
    memset(&lEmail, '\0', sizeof(lEmail));
    char lPassword[50];
    memset(&lPassword, '\0', sizeof(lPassword));
    char user_data[4096];
    memset(&user_data, '\0', sizeof(user_data));

    if(option == 49){
        //login with ID
        printf("Enter your ID to login : ");
        scanf(" %u", &login_id);

        SOCKET client_id = start_client();
        //send request
        char str_login_id[10];
        sprintf(str_login_id, "%u", login_id);
        char id_send[20] = "check_id_exists ";
        strcat(id_send, str_login_id);
        int id_send_bytes = send(client_id, id_send, strlen(id_send), 0);

        //receive response
        int user_data_bytes = recv(client_id, user_data, 4096, 0);
        close(client_id);

        if(strcmp(user_data, "NotExists") == 0){
            printf("Your login ID does not exists!\n");
            main_sector();
        }


    }else if(option == 50){
        //login with email

        while(1){
            printf("Enter your email to login : ");
            scanf(" %[^\n]", lEmail);
            if(is_valid_email(lEmail)){
                SOCKET client_email = start_client();
                //send request
                char email_send[100] = "check_email_exists ";
                strcat(email_send, lEmail);
                int email_send_bytes = send(client_email, email_send, strlen(email_send), 0);

                //receive response
                int user_data_bytes = recv(client_email, user_data, 4096, 0);
                close(client_email);

                if(strcmp(user_data, "NotExists") == 0){
                    printf("Your login email haven't register yet!");
                    main_sector();
                }else{
                    //email exists
                    break;
                }

            }else{
                printf("Invalid format! Try again!\n");
            }
        }
        
        //checking email exists or not in the server
    }else if(option == 48){
        main_sector();
    }else{
        printf("Invalid option! Try again!\n");
        login_sector();
    }

    printf("user data : %s\n", user_data);

    int count_words = count_words_from_string(user_data);
    char *user_data_arr[count_words];
    char *token = strtok(user_data, " ");

    int i = 0;
    while(token != NULL){
        user_data_arr[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    // printf("Password : %s\n", user_data_arr[5]);
    printf("Enter your password to login : ");
    scanf(" %[^\n]", lPassword);
    if(strcmp(lPassword, user_data_arr[5]) == 0){
        unsigned int id = strtoul(user_data_arr[1], NULL, 10);
        unsigned int monthlyIncome = strtoul(user_data_arr[9], NULL, 10);
        unsigned int loanAmount = strtoul(user_data_arr[10], NULL, 10);
        unsigned int currentAmount = strtoul(user_data_arr[11], NULL, 10);
        unsigned int walletCapacity = strtoul(user_data_arr[12], NULL, 10);
        unsigned int amountLimitPerDay = strtoul(user_data_arr[13], NULL, 10);
        // Data data;
        data.id = id;
        strcpy(data.name, user_data_arr[2]);
        strcpy(data.phone, user_data_arr[3]);
        strcpy(data.email, user_data_arr[4]);
        strcpy(data.password, user_data_arr[5]);
        strcpy(data.nrc, user_data_arr[6]);
        strcpy(data.address, user_data_arr[7]);
        strcpy(data.account_type, user_data_arr[8]);
        data.monthly_income = monthlyIncome;
        data.loan_amount = loanAmount;
        data.current_amount = currentAmount;
        data.wallet_capacity = walletCapacity;
        data.amount_limit_per_day = amountLimitPerDay;

        for(int i = 0; i<count_words - 14; i++){
            strcpy(data.hist[i].note, user_data_arr[14+i]);
        }

        user_sector();
        printf("Login success...\n");
    }else{
        printf("Login credential wrong!\n");
        login_sector();
    }

}

void user_sector()
{
    char input[10];
    int option = 0;
    printf("\n*************\n");
    printf("*User Sector*\n");
    printf("*************\n");
    printf("Welcome %s! Your current balance is %u\n", data.name, data.current_amount);
    printf("1. Profile\n2. Transfer Money\n3. Cash in/Cash out\n4. Loan\n5. History\n0. Logout\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    if(option == 49){
        user_profile();
    }else if(option == 50){
        user_transfer_money();
    }else if(option == 51){
        user_cash_in_out();
    }else if(option == 52){
        user_loan();
    }else if(option == 53){
        user_history();
    }else if(option == 48){
        printf("Logout...\n");
        memset(&data, 0, sizeof(Data));
        main_sector();
    }else{
        printf("Invalid option! Try again!\n");
        user_sector();
    }
}

void user_profile()
{
    printf("\n**************\n");
    printf("*User Profile*\n");
    printf("**************\n");

    printf("ID : %u\n", data.id);
    printf("Name : %s\n", data.name);
    printf("Phone : %s\n", data.phone);
    printf("Email : %s\n", data.email);
    printf("Password : %s\n", data.password);
    printf("NRC : %s\n", data.nrc);
    printf("Address : %s\n", data.address);
    printf("Account Type : %s\n", data.account_type);
    printf("Monthly Income : %u\n", data.monthly_income);
    printf("Loan Amount : %u\n", data.loan_amount);
    printf("Current Amount : %u\n", data.current_amount);
    printf("Wallet Capacity : %u\n", data.wallet_capacity);
    printf("Amount Limit Per Day : %u\n", data.amount_limit_per_day);

    char input[10];
    int option = 0;

    if(strcmp(data.account_type, "Personal") == 0){
        printf("1. Update account type to business\n");
    }

    printf("0. Back to user sector\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    if(option == 49){
        if(strcmp(data.account_type, "Personal") == 0){
            //update account
            memset(&data.account_type, '\0', sizeof(data.account_type));
            strcpy(data.account_type, "Business");
            user_profile_update_record();
            user_profile();
        }else{
            printf("Invalid option! Try again!\n");
            user_profile();
        }
    }else if(option == 48){
        user_sector();
    }else{
        printf("Invalid option! Try again!\n");
        user_profile();
    }

}

void user_profile_update_record()
{

    char update_note[100];
    memset(&update_note, '\0', sizeof(update_note));
    strcat(update_note, "1_updated_account_type_to_business_at_");
    get_current_time();
    strcat(update_note, current_time.time);

    // strcpy(data.hist[count_history(data)].note, loan_note);
    data.wallet_capacity = 10000000;
    data.amount_limit_per_day = 5000000;
    strcpy(data.hist[count_history(data)].note, update_note);

    //send to server
    char update_send[4096];
    memset(&update_send, '\0', sizeof(update_send));
    //account_type_update email note
    strcat(update_send, "account_type_update ");
    strcat(update_send, data.email);
    strcat(update_send, " ");
    strcat(update_send, update_note);

    SOCKET update_account = start_client();

    int update_bytes = send(update_account, update_send, strlen(update_send), 0);

    close(update_account);

    printf("User account type update success!\n");

}

void user_transfer_money()
{
    printf("\n*********************\n");
    printf("*User Transfer Money*\n");
    printf("*********************\n");
    printf("Your current money is %u\n", data.current_amount);
    char input[10];
    int option = 0;
    if(data.current_amount != 0){
        printf("1. Continue to transfer\n");
    }
    printf("0. Back to user sector\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    if(option == 49){
        if(data.current_amount != 0){
            continue_user_transfer_money();
        }else{
            printf("Invalid option! Try again!\n");
            user_transfer_money();
        }
    }else if(option == 48){
        user_sector();
    }else{
        printf("Invalid option! Try again!\n");
        user_transfer_money();
    }

}

void continue_user_transfer_money()
{
    char input[10];
    int option = 0;
    printf("1. Transfer with ID\n2. Transfer with Email\n0. Back to user sector\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    Data receiverData;
    char receiver_str[4096];
    char confirmPassword[50];
    unsigned int amountToTransfer = 0;
    unsigned int totalReceiveAmount = 0;

    if(option == 49){
        //transfer with ID
        unsigned int receiver_id = 0;
        while(1){
            printf("Enter receiver ID : ");
            scanf("%u", &receiver_id);

            if(receiver_id == data.id){
                printf("You can not transfer to yourself.\n");
                continue;
            }
            
            //checking id exists or not
            SOCKET client_id = start_client();
            //send request
            char str_receiver_id[10];
            sprintf(str_receiver_id, "%u", receiver_id);
            char id_send[20] = "check_id_exists ";
            strcat(id_send, str_receiver_id);
            int id_send_bytes = send(client_id, id_send, strlen(id_send), 0);

            //receive response
            int user_data_bytes = recv(client_id, receiver_str, 4096, 0);
            close(client_id);

            if(strcmp(receiver_str, "NotExists") == 0){
                printf("Your receiver ID does not exists!\n");
            }else{
                break;
            }

        }

    }else if(option == 50){
        //transfer with email
        char receiver_email[50];

        while(1){
            printf("Enter your email to transfer : ");
            scanf(" %[^\n]", receiver_email);
            
            if(strcmp(receiver_email, data.email) == 0){
                printf("You can not transfer to yourself.\n");
                continue;
            }

            if(is_valid_email(receiver_email)){
                SOCKET client_email = start_client();
                //send request
                char email_send[100] = "check_email_exists ";
                strcat(email_send, receiver_email);
                int email_send_bytes = send(client_email, email_send, strlen(email_send), 0);

                //receive response
                int user_data_bytes = recv(client_email, receiver_str, 4096, 0);
                close(client_email);

                if(strcmp(receiver_str, "NotExists") == 0){
                    printf("Your receiver email haven't exists!");
                }else{
                    //email exists
                    break;
                }

            }else{
                printf("Invalid format! Try again!\n");
            }
        }

    }else if(option == 48){
        user_sector();
    }else{
        printf("Invalid option! Try again!\n");
        continue_user_transfer_money();
    }

    int word_count = count_words_from_string(receiver_str);
    char *receiver_data[word_count];
    char *token = strtok(receiver_str, " ");

    int i = 0;
    while(token != NULL){
        receiver_data[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    printf("Receiver info : %s - %s - %s\n", receiver_data[1], receiver_data[2], receiver_data[4]);

    char yOrn;
    printf("Are you sure to transfer to this user(y/n)? :-> ");
    scanf(" %c", &yOrn);
    if(yOrn == 'y' || yOrn == 'Y'){

        unsigned int id = strtoul(receiver_data[1], NULL, 10);
        unsigned int monthlyIncome = strtoul(receiver_data[9], NULL, 10);
        unsigned int loanAmount = strtoul(receiver_data[10], NULL, 10);
        unsigned int currentAmount = strtoul(receiver_data[11], NULL, 10);
        unsigned int walletCapacity = strtoul(receiver_data[12], NULL, 10);
        unsigned int amountLimitPerDay = strtoul(receiver_data[13], NULL, 10);

        receiverData.id = id;
        strcpy(receiverData.name, receiver_data[2]);
        strcpy(receiverData.phone, receiver_data[3]);
        strcpy(receiverData.email, receiver_data[4]);
        strcpy(receiverData.password, receiver_data[5]);
        strcpy(receiverData.nrc, receiver_data[6]);
        strcpy(receiverData.address, receiver_data[7]);
        strcpy(receiverData.account_type, receiver_data[8]);
        receiverData.monthly_income = monthlyIncome;
        receiverData.loan_amount = loanAmount;
        receiverData.current_amount = currentAmount;
        receiverData.wallet_capacity = walletCapacity;
        receiverData.amount_limit_per_day = amountLimitPerDay;

        //transfer
        while(amountToTransfer < data.current_amount){
            
            printf("Enter your amount to transfer : ");
            scanf("%u", &amountToTransfer);

            if(data.current_amount > amountToTransfer){
                
                totalReceiveAmount = amountToTransfer + receiverData.current_amount;

                if(totalReceiveAmount > receiverData.wallet_capacity){
                    printf("Receiver wallet capacity was exceeded! You can transfer at most %u!\n", (receiverData.wallet_capacity - receiverData.current_amount));
                    amountToTransfer = 0;
                }else{
                    //check amount limit per day here
                    unsigned int user_total_amount_same_day = get_total_amount_of_same_day(data);
                    unsigned int receiver_total_amount_same_day = get_total_amount_of_same_day(receiverData);
                    //check for current user
                    if((user_total_amount_same_day+amountToTransfer) > data.amount_limit_per_day){
                        unsigned int transfer_at_most = data.amount_limit_per_day - user_total_amount_same_day;
                        printf("Your amount limit per day was exceeded! You can transfer at most %u toady\n", transfer_at_most);
                        amountToTransfer = 0;
                    }else{
                        //check for receiver
                        if((receiver_total_amount_same_day+amountToTransfer) > receiverData.amount_limit_per_day){
                            unsigned int receive_at_most = receiverData.amount_limit_per_day - receiver_total_amount_same_day;
                            printf("Receiver amount limit per day was exceeded! Receiver can receive at most %u today\n", receive_at_most);
                            amountToTransfer = 0;
                        }else{
                            break;
                        }
                    }
                    break;
                }

            }else{
                printf("Your current amount is not enough to transfer!\n");
                amountToTransfer = 0;
            }

        }

            while(strcmp(confirmPassword, data.password) != 0){
                printf("Enter your password to confirm transfer money : ");
                scanf(" %[^\n]", confirmPassword);
                if(strcmp(confirmPassword, data.password) != 0){
                    printf("Incorrect password! Try again!\n");
                }
            }

            printf("Transfer $%u to %s complete!\n", amountToTransfer, receiverData.name);
            data.current_amount -= amountToTransfer;
            user_transfer_money_record(receiverData, amountToTransfer);

            user_sector();

    }else{
        printf("Cancelled continue to transfer!\n");
        user_sector();
    }

}

void user_transfer_money_record(Data receiver, unsigned int amount)
{

    //amount to send
    unsigned int userActualAmount = data.current_amount;
    unsigned int receiverActualAmount = receiver.current_amount + amount;
    printf("User amount : %u\n", userActualAmount);
    printf("Receiver amount : %u\n", receiverActualAmount);

    char amount_str[10];
    memset(&amount_str, '\0', sizeof(amount_str));
    sprintf(amount_str, "%u", amount);

    //transferer record
    char transfer_note[100];
    memset(&transfer_note, '\0', sizeof(transfer_note));
    strcat(transfer_note, "2_Send_Money_To_");
    strcat(transfer_note, receiver.name);
    strcat(transfer_note, "_$");
    strcat(transfer_note, amount_str);
    strcat(transfer_note, "_at_");
    get_current_time();
    strcat(transfer_note, current_time.time);

    strcpy(data.hist[count_history(data)].note, transfer_note);
    
    //receiver record
    char receive_note[100];
    memset(&receive_note, '\0', sizeof(receive_note));
    strcat(receive_note, "2_Receive_Money_From_");
    strcat(receive_note, data.name);
    strcat(receive_note, "_$");
    strcat(receive_note, amount_str);
    strcat(receive_note, "_at_");
    get_current_time();
    strcat(receive_note, current_time.time);

    //unsigned int to char array
    char userAmount[10];
    memset(&userAmount, '\0', sizeof(userAmount));
    sprintf(userAmount, "%u", userActualAmount);

    char receiverAmount[10];
    memset(&receiverAmount, '\0', sizeof(receiverAmount));
    sprintf(receiverAmount, "%u", receiverActualAmount);

    //user email, user amount, user note, receiver email, receiver amount, receiver note
    SOCKET trNote = start_client();

    char to_send[1024] = "tr_note ";
    strcat(to_send, data.email);
    strcat(to_send, " ");
    strcat(to_send, userAmount);
    strcat(to_send, " ");
    strcat(to_send, transfer_note);
    strcat(to_send, " ");
    strcat(to_send, receiver.email);
    strcat(to_send, " ");
    strcat(to_send, receiverAmount);
    strcat(to_send, " ");
    strcat(to_send, receive_note);

    int tr_bytes = send(trNote, to_send, strlen(to_send), 0);

    close(trNote);

}

void user_cash_in_out()
{
    printf("\n***************************\n");
    printf("*User Cash in and Cash out*\n");
    printf("***************************\n");
    printf("Your current balance is %u\n\n", data.current_amount);
    char input[10];
    int option = 0;
    printf("1. Cash in money\n");
    if(data.current_amount != 0){
        printf("2. Cash out money\n");
    }
    printf("0. Back to user sector\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    if(option == 49){
        //cash in
        user_cash_in();
    }else if(option == 50){
        if(data.current_amount != 0){
            //cash out
            user_cash_out();
        }else{
            printf("Invalid Option! Try again!\n");
            user_cash_in_out();
        }
    }else if(option == 48){
        user_sector();
    }else{
        printf("Invalid option! Try again!\n");
        user_cash_in_out();
    }

}

void user_cash_in()
{
    unsigned int amount = 0;
    printf("Your current balance is %u\n", data.current_amount);
    printf("Enter your amount to cash in : ");
    scanf("%u", &amount);
    //check amount limit per day too
    if((data.current_amount + amount) <= data.wallet_capacity){
        unsigned int total_amount_for_same_day = get_total_amount_of_same_day(data);
        if((amount + total_amount_for_same_day) > data.amount_limit_per_day){
            unsigned int at_most_cashin = data.amount_limit_per_day - total_amount_for_same_day;
            printf("Your amount limit per day was exceeded! You can cash in at most %u today!\n", at_most_cashin);
            user_cash_in();
        }else{
            data.current_amount += amount;
            user_cash_in_out_record("in", amount);
            printf("Cash in %u success!\n", amount);
            user_sector();
        }
    }else{
        printf("Wallet capacity was exceeded! You can cash in only %u!\nTry again!\n", (data.wallet_capacity - data.current_amount));
        user_cash_in();
    }
}

void user_cash_out()
{
    unsigned int amount = 0;
    printf("Your current balance is %u\n", data.current_amount);
    printf("Enter your amount to cash out : ");
    scanf("%u", &amount);
    //check amount limit per day too
    if(data.current_amount >= amount){
        data.current_amount -= amount;
        user_cash_in_out_record("out", amount);
        printf("Cash out %u success!\n", amount);
        user_sector();
    }else{
        printf("You don't have enough balance to cash out!\nTry again!\n");
        user_cash_out();
    }
}

void user_cash_in_out_record(char type[], unsigned int amount)
{

    char amount_str[10];
    memset(&amount_str, '\0', sizeof(amount_str));
    sprintf(amount_str, "%u", amount);

    unsigned int actual_amount = data.current_amount;
    char actual_amount_str[10];
    memset(&actual_amount_str, '\0', sizeof(actual_amount_str));
    sprintf(actual_amount_str, "%u", actual_amount);

    SOCKET in_out = start_client();

    char note[100];
    memset(&note, '\0', sizeof(note));

    //cash_in_out email actual_amount note
    if(strcmp(type, "in") == 0){
        //cash in record
        //3_cash_in_money_$1000_at_time
        strcat(note, "3_cash_in_money_$");
        strcat(note, amount_str);
        strcat(note, "_at_");
        get_current_time();
        strcat(note, current_time.time);
        
    }else{
        //cash out record
        //3_cash_out_money_$1000_at_time
        strcat(note, "3_cash_out_money_$");
        strcat(note, amount_str);
        strcat(note, "_at_");
        get_current_time();
        strcat(note, current_time.time);
    }

    strcpy(data.hist[count_history(data)].note, note);

    char io_send[4096];
    memset(&io_send, '\0', sizeof(io_send));
    strcat(io_send, "cash_in_out ");
    strcat(io_send, data.email);
    strcat(io_send, " ");
    strcat(io_send, actual_amount_str);
    strcat(io_send, " ");
    strcat(io_send, note);

    // printf("Note: %s\n", note);
    // printf("Server Request IO : %s\n", io_send);

    int io_bytes = send(in_out, io_send, strlen(io_send), 0);

    close(in_out);
}

void user_loan()
{
    printf("\n***********\n");
    printf("*User Loan*\n");
    printf("***********\n");
    printf("Your current balance is %u\n", data.current_amount);
    printf("Your loan amount will base on your monthly income!\n");
    printf("You can take the loan for 50%% of your monthly income!\n");
    printf("Your loan amount at most is %u\n", (data.monthly_income/2));
    printf("The interest rate for the loan is 10%\n\n");

    char input[10];
    memset(&input, '\0', sizeof(input));
    int option = 0;
    if(data.loan_amount == 0){

        printf("1. Continue to take the loan\n");
        printf("0. Back to user sector\n");
        printf("Enter your option :-> ");
        scanf(" %[^\n]", input);
        option = check_input(input);
        
        if(option == 49){
            //continue to take the loan
            continue_user_loan_take();
        }else if(option == 48){
            user_sector();
        }else{
            printf("Invalid option! Try again!\n");
            user_loan();
        }

    }else{
        
        unsigned int pay_loan = data.loan_amount + (data.loan_amount * 0.1);
        printf("You have taken the loan. Please pay for it first.\n");
        printf("The amount to pay for the loan is %u.\n", pay_loan);
        //pay it now or not here
        if(data.current_amount >= pay_loan){
            printf("1. Continue to pay the loan\n");
        }else{
            printf("You don't have enough balance to pay the loan.\n");
        }
        printf("0. Back to user sector\n");
        printf("Enter your option :-> ");
        scanf(" %[^\n]", input);
        option = check_input(input);

        if(option == 49){
            
            if(data.current_amount >= pay_loan){
                data.current_amount -= pay_loan;
                data.loan_amount = 0;
                user_loan_record("pay", pay_loan);
                printf("Loan paying success!\n");
                user_sector();
            }else{
                printf("Invalid option! Try again!\n");
                user_loan();
            }

        }else if(option == 48){
            user_sector();
        }else{
            printf("Invalid option! Try again!\n");
            user_loan();
        }

    }

}

void continue_user_loan_take()
{
    unsigned int loan_amount = 0;
    printf("Enter your amount to take the loan : ");
    scanf("%u", &loan_amount);
    if(loan_amount <= (data.monthly_income * 0.5)){

        //check amount limit per day
        if((loan_amount + data.current_amount) <= data.wallet_capacity){
            unsigned int total_amount_for_same_day = get_total_amount_of_same_day(data);
            if((total_amount_for_same_day+loan_amount) > data.amount_limit_per_day){

                unsigned int take_loan_atMost = data.amount_limit_per_day - total_amount_for_same_day;
                printf("Your amount limit per day was exceeded! You can loan at most %u today!\n", take_loan_atMost);
                continue_user_loan_take();

            }else{

                data.loan_amount = loan_amount;
                data.current_amount += loan_amount;
                user_loan_record("take", loan_amount);
                printf("Loan taking success!\n");
                user_sector();
            }

        }else{
            printf("Wallet capacity was exceeded!\n");
            printf("Your loan amount to take at most is %u\n", (data.wallet_capacity - data.current_amount));
            continue_user_loan_take();
        }

    }else{
        printf("Exceeded loan amount! Try again!\n");
        continue_user_loan_take();
    }

}

void user_loan_record(char type[], unsigned int amount)
{
    char amount_str[10];
    memset(&amount_str, '\0', sizeof(amount_str));
    sprintf(amount_str, "%u", amount);

    char current_amount_str[10];
    memset(&current_amount_str, '\0', sizeof(current_amount_str));
    sprintf(current_amount_str, "%u", data.current_amount);

    char loan_amount_str[10];
    memset(&loan_amount_str, '\0', sizeof(loan_amount_str));
    sprintf(loan_amount_str, "%u", data.loan_amount);

    char loan_note[100];
    memset(&loan_note, '\0', sizeof(loan_note));
    if(strcmp(type, "take") == 0){
        //loan take record
        strcat(loan_note, "4_loan_take_$");
        strcat(loan_note, amount_str);
        strcat(loan_note, "_at_");
        get_current_time();
        strcat(loan_note, current_time.time);

    }else{
        //loan pay record
        strcat(loan_note, "4_loan_pay_$");
        strcat(loan_note, amount_str);
        strcat(loan_note, "_at_");
        get_current_time();
        strcat(loan_note, current_time.time);
    }

    strcpy(data.hist[count_history(data)].note, loan_note);

    SOCKET loan = start_client();
    
    //loan email actual_amount loan_amount note
    char loan_send[4096];
    memset(&loan_send, '\0', sizeof(loan_send));
    strcat(loan_send, "loan ");
    strcat(loan_send, data.email);
    strcat(loan_send, " ");
    strcat(loan_send, current_amount_str);
    strcat(loan_send, " ");
    strcat(loan_send, loan_amount_str);
    strcat(loan_send, " ");
    strcat(loan_send, loan_note);

    int loan_bytes = send(loan, loan_send, strlen(loan_send), 0);

    close(loan);


}

void user_history()
{
    printf("\n**************\n");
    printf("*User History*\n");
    printf("**************\n");

    char input[10];
    int option = 0;

    int history_count = count_history(data);

    printf("1. All history\n2. Account Type update history\n3. Transfer and receive money history\n4. Cash in and Cash out history\n5. Loan history\n0. Back to user sector\n");
    printf("Enter your option :-> ");
    scanf(" %[^\n]", input);
    option = check_input(input);

    if(option == 49){
        //all history
        printf("Total history : %d\n", history_count);
        if(history_count == 0){
            printf("There is no history to show!\n");
        }else{
            for(int i = 0; i<history_count; i++){
                printf("%s\n", data.hist[i].note);
            }
        }
    }else if(option == 50){
        //update history
        if(history_count == 0){
            printf("There is no history to show!\n");
        }else{
            int flag = 0;
            for(int i = 0; i<history_count; i++){
                if(data.hist[i].note[0] == '1'){
                    printf("%s\n", data.hist[i].note);
                    flag++;
                }
            }
            if(flag == 0){
                printf("There is no history to show!\n");
            }
        }
    }else if(option == 51){
        // tr history
        if(history_count == 0){
            printf("There is no history to show!\n");
        }else{
            int flag = 0;
            for(int i = 0; i<history_count; i++){
                if(data.hist[i].note[0] == '2'){
                    printf("%s\n", data.hist[i].note);
                    flag++;
                }
            }
            if(flag == 0){
                printf("There is no history to show!\n");
            }
        }
    }else if(option == 52){
        // io history
        if(history_count == 0){
            printf("There is no history to show!\n");
        }else{
            int flag = 0;
            for(int i = 0; i<history_count; i++){
                if(data.hist[i].note[0] == '3'){
                    printf("%s\n", data.hist[i].note);
                    flag++;
                }
            }
            if(flag == 0){
                printf("There is no history to show!\n");
            }
        }
    }else if(option == 53){
        //loan history
        if(history_count == 0){
            printf("There is no history to show!\n");
        }else{
            int flag = 0;
            for(int i = 0; i<history_count; i++){
                if(data.hist[i].note[0] == '4'){
                    printf("%s\n", data.hist[i].note);
                    flag++;
                }
            }
            if(flag == 0){
                printf("There is no history to show!\n");
            }
        }
    }else if(option == 48){
        user_sector();
    }else{
        printf("Invalid option! Try again!\n");
        user_history();
    }
    user_history();
}

int count_history(Data data)
{
    int count = 0;
    for(int i = 0; i<100; i++){
        if(data.hist[i].note[0] != '\0'){
            count++;
        }else{
            break;
        }
    }
    return count;
}

Date get_current_date()
{
    get_current_time();

    char day[5];
    memset(&day, '\0', sizeof(day));
    char month[5];
    memset(&month, '\0', sizeof(month));
    char year[5];
    memset(&year, '\0', sizeof(year));

    int time_counter = 0;
    int time_length = strlen(current_time.time);
    for(int i = 0; i<time_length; i++){
        if(current_time.time[i] == '!'){
            time_counter++;
            break;
        }
        time_counter++;
    }
    //get month
    for(int i=0; i<5; i++){
        if(current_time.time[time_counter] == '#'){
            time_counter++;
            break;
        }
        month[i] = current_time.time[time_counter];
        time_counter++;
    }
    // printf("Month : %s\n", month);

    //get day
    for(int i = 0; i<5; i++){
        if(current_time.time[time_counter] == '_'){
            time_counter++;
            break;
        }
        day[i] = current_time.time[time_counter];
        time_counter++;
    }
    // printf("Day : %s\n", day);

    for(int i = time_counter; i<time_length; i++){
        if(current_time.time[i] == '@'){
            time_counter++;
            break;
        }
        time_counter++;
    }

    //get year
    for(int i = 0; i<5; i++){
        if(current_time.time[time_counter] == '\0'){
            time_counter++;
            break;
        }
        year[i] = current_time.time[time_counter];
        time_counter++;
    }
    // printf("Year : %s\n", year);

    Date date;
    strcpy(date.day, day);
    strcpy(date.month, month);
    strcpy(date.year, year);

    return date;
}

Date get_date_from_record(char note[])
{
    char day[5];
    memset(&day, '\0', sizeof(day));
    char month[5];
    memset(&month, '\0', sizeof(month));
    char year[5];
    memset(&year, '\0', sizeof(year));

    int note_counter = 0;
    int note_length = strlen(note);
    for(int i = 0; i<note_length; i++){
        if(note[i] == '!'){
            note_counter++;
            break;
        }
        note_counter++;
    }
    //get month
    for(int i = 0; i<5; i++){
        if(note[note_counter] == '#'){
            note_counter++;
            break;
        }
        month[i] = note[note_counter];
        note_counter++;
    }
    // printf("Month : %s\n", month);

    //get day
    for(int i = 0; i<5; i++){
        if(note[note_counter] == '_'){
            note_counter++;
            break;
        }
        day[i] = note[note_counter];
        note_counter++;
    }
    // printf("Day : %s\n", day);

    for(int i = note_counter; i<note_length; i++){
        if(note[i] == '@'){
            note_counter++;
            break;
        }
        note_counter++;
    }

    //get year
    for(int i = 0; i<5; i++){
        if(note[note_counter] == '\0'){
            note_counter++;
            break;
        }
        year[i] = note[note_counter];
        note_counter++;
    }
    // printf("Year : %s\n", year);

    Date date;
    strcpy(date.day, day);
    strcpy(date.month, month);
    strcpy(date.year, year);
    return date;
}

unsigned int get_total_amount_of_same_day(Data userData)
{
    unsigned int total_amount = 0;
    Date current_date = get_current_date();
    int record_count = count_history(userData);
    // printf("Count : %d\n", record_count);
    for(int i = 0; i<record_count; i++){
        Date note_date = get_date_from_record(userData.hist[i].note);
        // printf("counter %d - cday %s - nday %s\n", i, current_date.day, note_date.day);
        if(strcmp(current_date.year, note_date.year) == 0){
            // printf("Same year\n");
            if(strcmp(current_date.month, note_date.month) == 0){
                // printf("same month\n");
                if(strcmp(current_date.day, note_date.day) == 0){
                    // printf("same day\n");
                    total_amount += get_amount_from_record(userData.hist[i].note);
                }
            }
        }
    }
    return total_amount;
}

unsigned int get_amount_from_record(char note[])
{
    char amount_str[10];
    memset(&amount_str, '\0', sizeof(amount_str));

    int note_length = strlen(note);
    int note_counter = 0;
    for(int i = 0; i<note_length; i++){
        if(note[i] == '$'){
            note_counter++;
            break;
        }
        note_counter++;
    }

    for(int i = 0; i<10; i++){
        if(note[note_counter] == '_'){
            break;
        }
        amount_str[i] = note[note_counter];
        note_counter++;
    }

    unsigned int amount = strtoul(amount_str, NULL, 10);

    return amount;

}

/*
    keep on writing amount limit per day
*/