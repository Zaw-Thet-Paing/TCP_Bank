#include<stdio.h>
#include<stdlib.h>


struct History{
    char note[100];
};

struct Data{
    unsigned int id;
    char name[50];
    char phone[50];
    char email[50];
    char password[50];
    char nrc[25];
    char address[50];
    char account_type[10];
    unsigned int monthly_income;
    unsigned int loan_amount;
    unsigned int current_amount;
    unsigned int wallet_capacity;
    unsigned int amount_limit_per_day;
    struct History hist[100];
};

typedef struct Data Data;