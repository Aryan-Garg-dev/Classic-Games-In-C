#include <stdio.h>
#include <stdlib.h>
#include <string.h> //!implement
#include <ctype.h>
#include <stdbool.h> //!Implement
#include <time.h> //! Implement

#define true 1
#define false 0
#define and &&
#define or ||
#define bool _Bool
#define MIN(x, y) (x<y?x:y)
#define MAX(x, y) (x>y?x:y)
#define print(...)\
    printf(__VA_ARGS__)
#define TIME() (__TIME__)
#define DATE() (__DATE__)

//To print an array
void echoArr(int size, int array[]){
    printf("[%d", array[0]);
    for (int i = 1; i < size; i++){
        printf(", %d", array[i]);
    }
    printf("]\n");
}
//To print a matrix
void echoMat(int r, int c, int matrix[r][c]){
    for (int j = 0; j < r; j++){
        printf("[%d", matrix[j][0]);
        for (int i = 1; i < c; i++){
            printf(", %d", matrix[j][i]);
        }
        printf("]\n");
    }
    printf("\n");
}

//To print a string array
void echoStrArr(int size, int _maxLen, char array[][_maxLen]){
    printf("[%s", array[0]);
    for (int i = 1; i < size; i++){
        printf(", %s", array[i]);
    }
    printf("]\n");
}

//To split a string
void explodeStr(char string[], char divider, int _maxLen, char strArr[][_maxLen]){
    int start = 0, counter = 0;
    for (int i = 0; string[i] != '\0'; i++){
        if (string[i] == divider || i == strlen(string)-1){
            int end = i, count = 0;
            for (int j = start; j < end+1; j++){
                if (string[j] != ' '){
                    strArr[counter][count] = string[j]; 
                    count++;
                }
            }
            strArr[counter][count] = '\0';
            counter++;
            start = i+1;
        }   
    }
}

//To count the number of words in a string
int wordCount(char string[], char divider){
    int count = 1;
    for (int i = 0; string[i]!='\0'; i++){
        if (string[i]==divider){
            count++;
        }
    }
    return count;
}

//To sort an Integer array
void sortIntArr(int size, int array[], int ascending){
    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            if (ascending){
                if (array[i]>array[j]){
                    int temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                }
            } else {
               if (array[i]<array[j]){
                    int temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                } 
            }
        }
    }
}

//To sort an array of strings
void sortStrArr(int size, int _maxLen, char data[][_maxLen]){
    char temp[_maxLen];
    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            if (strcmp(data[i], data[j])>0){
                strcpy(temp, data[i]);
                strcpy(data[i], data[j]);
                strcpy(data[j], temp);
            }
        }
    }
}

// To remove word from a string
void removeWord(int _maxLen, char str[_maxLen], char word[_maxLen]){
    int count = 0, start = 0, counter = 0;
    char temp[_maxLen], newStr[_maxLen];
    for (int i = 0; i < strlen(str); i++){
        if (str[i] != ' '){
            temp[count] = str[i];
            count++;
        } else {
            temp[count] = '\0';
            int end = start + count;
            if (strcmp(temp, word)==0){
                for (int i = 0; i < strlen(str); i++){
                    if (i < start|| i > end){
                        newStr[counter] = str[i];
                        counter++;
                    }
                }
                newStr[counter] = '\0';
                strcpy(str, newStr);
                break;
            } else {
                count = 0; 
                start = i;
            }
        }
    }
}

// To titlecase a string
void titleStr(char string[]){
    string[0] = toupper(string[0]);
    for (int i = 0; string[i]!='\0';i++){
        if (string[i]==' '){
            string[i+1] = toupper(string[i+1]);
        }
    }
}
