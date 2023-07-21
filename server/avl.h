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

struct Node{
    int height;
    struct Data data;
    struct Node* left;
    struct Node* right;
};

typedef struct Node Node;
typedef struct Data Data;

Node* root = NULL;

int getHeight(Node* node);
int getBalanceFactor(Node* node);
void updateHeight(Node* node);
Node* rotateLeft(Node* x);
Node* rotateRight(Node* y);
Node* insertNode(Node* node, Data data);
void printInOrder(Node* node);
int countNode(Node* node);
int countHistory(Node* node);

int getHeight(Node* node)
{
    if(node == NULL){
        return 0;
    }
    return node->height;
}

int getBalanceFactor(Node* node)
{
    if(node == NULL){
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(Node* node)
{
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

Node* rotateLeft(Node* x)
{
    Node* y = x->right;
    Node* tmp = y->left;

    y->left = x;
    x->right = tmp;

    updateHeight(x);
    updateHeight(y);

    return y;
}

Node* rotateRight(Node* y)
{
    Node* x = y->left;
    Node* tmp = x->right;

    x->right = y;
    y->left = tmp;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* insertNode(Node* node, Data data)
{
    if(node == NULL){
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if(node->data.id > data.id){
        node->left = insertNode(node->left, data);
    }else if(node->data.id < data.id){
        node->right = insertNode(node->right, data);
    }else{
        return node;
    }

    updateHeight(node);
    int balanceFactor = getBalanceFactor(node);

    //left left case
    if(balanceFactor > 1 && node->left->data.id > data.id){
        return rotateRight(node);
    }

    //right right case
    if(balanceFactor < -1 && node->right->data.id < data.id){
        return rotateLeft(node);
    }

    //left right case
    if(balanceFactor > 1 && node->left->data.id < data.id){
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    //right left case
    if(balanceFactor < -1 && node->right->data.id > data.id){
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;

}

void printInOrder(Node* node)
{
    if(node != NULL){
        printInOrder(node->left);

        printf("%u %s %s %s %s %s %s %s %u %u %u %u %u", node->data.id, node->data.name, node->data.phone, node->data.email, node->data.password, node->data.nrc, node->data.address, node->data.account_type, node->data.monthly_income, node->data.loan_amount, node->data.current_amount, node->data.wallet_capacity, node->data.amount_limit_per_day);

        if(countHistory(node) != 0){
            for(int i = 0; i<countHistory(node); i++){
                printf(" %s", node->data.hist[i].note);
            }
        }
        printf("%c", '\n');

        printInOrder(node->right);

    }
}

int countNode(Node* node)
{
    if(node == NULL){
        return 0;
    }else{
        int leftCount = countNode(node->left);
        int rightCount = countNode(node->right);
        return leftCount +  rightCount + 1;
    }
}

int countHistory(Node* node)
{
    int count = 0;
    for(int i = 0; i<100; i++){
        if(node->data.hist[i].note[0] != '\0'){
            count++;
        }else{
            break;
        }
    }
    return count;
}