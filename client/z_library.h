#include<string.h>
#include<time.h>

struct CurrentTime{
    char time[50];
};

struct CurrentTime current_time;

struct NRC_DB{
    char nrc[25];
};

struct NRC_DB nrcDB[45];

int check_input(char input[10]);
int is_strong_password(char password[]);
int is_valid_email(char email[]);
int is_NRC_valid(char nrc[]);
int is_valid_phone(char phone[]);
int is_accountType_valid(char account_type[]);
int is_space(char text[]);
int is_valid_name(char name[]);
int count_words_from_string(char str[]);

void get_current_time();

void load_data_to_nrc_db();

int check_input(char input[10])
{
    if(input[0] >= 48 && input[0] <= 57 && input[1] == '\0'){
        return input[0];
    }
    return -1;
}

int is_strong_password(char password[])
{
    int upperCase = 0;
    int lowerCase = 0;
    int digit = 0;
    int specialChar = 0;
    int length  = 0;

    while(password[length] != '\0'){
        if(password[length] >= 'a' && password[length] <= 'z'){
            lowerCase = 1;
        }else if(password[length] >= 'A' && password[length] <= 'Z'){
            upperCase = 1;
        }else if(password[length] >= '0' && password[length] <= '9'){
            digit = 1;
        }else if(password[length] >= '!' && password[length] <= '/'){
            specialChar = 1;
        }else if(password[length] >= ':' && password[length] <= '@'){
            specialChar = 1;
        }else if(password[length] >= '[' && password[length] <= '`'){
            specialChar = 1;
        }else if(password[length] >= '{' && password[length] <= '~'){
            specialChar = 1;
        }
        length++;
    }
    return ( upperCase && lowerCase && digit && specialChar && length >= 8 );
}

int is_valid_email(char email[])
{
    int space = 0;
    int at_count = 0;
    int email_length = strlen(email);
    char domain_to_check[20] = "gmail.com";
    char email_domain[20];
    //add '\0'
    for(int i = 0; i<20; i++){
        email_domain[i] = '\0';
    }
    //space exists checking
    for(int i = 0; i<email_length; i++){
        if(email[i] == ' '){
            space = 1;
            break;
        }
    }
    if(space == 0){
        // @ count
        for(int i = 0; i<email_length; i++){
            if(email[i] == '@'){
                at_count++;
            }
        }

        if(at_count == 1){
            //find index of @
            int at_index = 0;
            for(int i = 0; i<email_length; i++){
                if(email[i] == '@'){
                    at_index = i;
                    break;
                }
            }
            //put domain to variable
            int j = 0;
            for(int i = at_index+1; i<email_length; i++){
                email_domain[j] = email[i];
                j++;
            }

            //check domain is gmail.com or not
            if(strcmp(domain_to_check, email_domain) == 0){
                return 1;
            }else{
                return 0;
            }
        }else{
            return 0;
        }
    }else{
        return 0;
    }
    
}

//only available for Yangon
int is_NRC_valid(char nrc[])
{

    if(is_space(nrc)){
        return 0;
    }

    load_data_to_nrc_db();
    int nrc_length = strlen(nrc);
    int nrc_format_end_index = 0;
    char nrc_format[15];
    char nrc_number[15];
    //put \0 to empty array
    for(int i = 0; i<15; i++){
        nrc_format[i] = '\0';
        nrc_number[i] = '\0';
    }

    //find format end index
    for(int i = 0; i<nrc_length; i++){
        if(nrc[i] == ')'){
            nrc_format_end_index = i;
            break;
        }
    }

    if(nrc_format_end_index != 0){
        //put data to nrc_format
        for(int i = 0; i<(nrc_format_end_index+1); i++){
            nrc_format[i] = nrc[i];
        }

        int flag = 0;
        for(int i = 0; i<45; i++){
            if(strcmp(nrcDB[i].nrc, nrc_format) == 0){
                flag = 1;
                break;
            }
        }

        if(flag != 0){
            //put data to nrc_number
            int j = 0;
            for(int i = (nrc_format_end_index+1); i<nrc_length; i++){
                nrc_number[j] = nrc[i];
                j++;
            }
            
            if(strlen(nrc_number) == 6){
                return 1;
            }else{
                return 0;
            }

        }else{
            return 0;
        }

    }else{
        return 0;
    }
}

int is_valid_phone(char phone[])
{
    if(is_space(phone)){
        return 0;
    }
    int phone_length = strlen(phone);
    if(phone_length == 12){
        if(phone[0] == '9' && phone[1] == '5' && phone[2] == '9'){
            return 1;
        }else{
            return 0;
        }
    }else{  
        return 0;
    }
}

int is_accountType_valid(char account_type[])
{
    if(strcmp(account_type, "Personal") == 0 || strcmp(account_type, "Business") == 0){
        return 1;
    }
    return 0;
}

int is_space(char text[])
{
    if(text[0] != '\0'){
        int text_count = strlen(text);
        for(int i = 0; i<text_count; i++){
            if(text[i] == ' '){
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

int is_valid_name(char name[])
{

    if(is_space(name)){
        return 0;
    }

    int i = 0;
    int notAlphabetChar = 0;
    int alphabetChar = 0;
    while(name[i] != '\0'){
        if((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z')){
            alphabetChar++;
        }else{
            notAlphabetChar++;
        }
        i++;
    }
    if(alphabetChar == strlen(name)){
        if(notAlphabetChar == 0){
            return 1;
        }
    }
    return 0;
}

int count_words_from_string(char str[])
{
    int count = 0;
    int i = 0;
    while(str[i] != '\0'){
        if(str[i] == ' '){
            count++;
        }
        i++;
    }
    return count+1;
}

void load_data_to_nrc_db()
{
    strcpy(nrcDB[0].nrc, "12/AaLaNa(N)");
    strcpy(nrcDB[1].nrc, "12/BaHaNa(N)");
    strcpy(nrcDB[2].nrc, "12/BaTaHta(N)");
    strcpy(nrcDB[3].nrc, "12/KaKaKa(N)");
    strcpy(nrcDB[4].nrc, "12/DaGaNa(N)");
    strcpy(nrcDB[5].nrc, "12/DaGaYa(N)");
    strcpy(nrcDB[6].nrc, "12/DaGaMa(N)");
    strcpy(nrcDB[7].nrc, "12/DaSaKa(N)");
    strcpy(nrcDB[8].nrc, "12/DaGaTa(N)");
    strcpy(nrcDB[9].nrc, "12/DaLaNa(N)");
    strcpy(nrcDB[10].nrc, "12/DaPaNa(N)");
    strcpy(nrcDB[11].nrc, "12/LaMaNa(N)");
    strcpy(nrcDB[12].nrc, "12/LaThaYa(N)");
    strcpy(nrcDB[13].nrc, "12/LaKaNa(N)");
    strcpy(nrcDB[14].nrc, "12/MaBaNa(N)");
    strcpy(nrcDB[15].nrc, "12/HtaTaPa(N)");
    strcpy(nrcDB[16].nrc, "12/AhSaNa(N)");
    strcpy(nrcDB[17].nrc, "12/KaMaYa(N)");
    strcpy(nrcDB[18].nrc, "12/KhaYaNa(N)");
    strcpy(nrcDB[19].nrc, "12/KaKhaKa(N)");
    strcpy(nrcDB[20].nrc, "12/KaTaTa(N)");
    strcpy(nrcDB[21].nrc, "12/KaTaNa(N)");
    strcpy(nrcDB[22].nrc, "12/KaMaTa(N)");
    strcpy(nrcDB[23].nrc, "12/LaMaTa(N)");
    strcpy(nrcDB[24].nrc, "12/LaThaNa(N)");
    strcpy(nrcDB[25].nrc, "12/MaYaKa(N)");
    strcpy(nrcDB[26].nrc, "12/MaGaTa(N)");
    strcpy(nrcDB[27].nrc, "12/MaGaDa(N)");
    strcpy(nrcDB[28].nrc, "12/OuKaMa(N)");
    strcpy(nrcDB[29].nrc, "12/PaBaTa(N)");
    strcpy(nrcDB[30].nrc, "12/PaZaDa(N)");
    strcpy(nrcDB[31].nrc, "12/SaKhaNa(N)");
    strcpy(nrcDB[32].nrc, "12/SaKaKha(N)");
    strcpy(nrcDB[33].nrc, "12/SaKaNa(N)");
    strcpy(nrcDB[34].nrc, "12/YaPaKa(N)");
    strcpy(nrcDB[35].nrc, "12/YaPaTha(N)");
    strcpy(nrcDB[36].nrc, "12/OuKaTa(N)");
    strcpy(nrcDB[37].nrc, "12/TaKaNa(N)");
    strcpy(nrcDB[38].nrc, "12/TaMaNa(N)");
    strcpy(nrcDB[39].nrc, "12/ThaKaTa(N)");
    strcpy(nrcDB[40].nrc, "12/ThaLaNa(N)");
    strcpy(nrcDB[41].nrc, "12/ThaGaKa(N)");
    strcpy(nrcDB[42].nrc, "12/ThaKhaNa(N)");
    strcpy(nrcDB[43].nrc, "12/TaTaNa(N)");
    strcpy(nrcDB[44].nrc, "12/YaKaNa(N)");
}

void get_current_time()
{
    time_t tm;
    time(&tm);
    char cTime[50];
    memset(&cTime, '\0', sizeof(cTime));
    strcpy(cTime, ctime(&tm));
    // int cTime_length = strlen(cTime);
    // printf("Current updated time is : %s\n", cTime);

    int i = 0;
    int space = 1;
    while(cTime[i] != '\0'){
        //carry on time function
        if(cTime[i] == '\n'){
            break;
        }
        if(cTime[i] == ' '){
            if(space == 1){
                current_time.time[i] = '!';
            }else if(space == 2){
                current_time.time[i] = '#';
            }else if(space == 4){
                current_time.time[i] = '@';
            }else{
                current_time.time[i] = '_';
            }
            space++;
        }else{
            current_time.time[i] = cTime[i];
        }
        i++;
    }

    // printf("Modified time : %s\n", current_time.time);

}