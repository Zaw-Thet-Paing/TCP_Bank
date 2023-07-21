int word_count_from_string(char str[]);

int word_count_from_string(char str[])
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