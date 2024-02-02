#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/**
 * * _maxLen = maximum no. of characters in that char array
 * * size = maximum no. of items in that array
 * * r, c : to specify row and column
 * * divider = character that split the string (eg: ' ', ':', etc.)
 * * ascending: 0 for False 1 for True
 * ? Echo
 * ! void echoArr(int size, int array[])
    // To print Array in a visually readable format 
 * ! void echoMat(int r, int c, int matrix[r][c]) 
    // To print Matrix in a visually readable format
 * ! void echoStrArr(int size, int _maxLen, char array[][_maxLen])
    // To print array of strings in a visually readable format
 * ? String Split
 * ! void explodeStr(char string[], char divider, int _maxLen, char strArr[][_maxLen])
    // To split the string into an array of strings
 * ! int wordCount(char string[], char divider)
    // total no of words separated by this character (specified by divider)
 * ? Sort
 * ! void sortIntArr(int size, int array[], int ascending)
    // to Sort the int array in ascending order or descending order
 * ! void sortStrArr(int size, int _maxLen, char data[][_maxLen])
    // to Sort the string array in alphabetical order
 * ? String Modify
 * ! void removeWord(int _maxLen, char str[_maxLen], char word[_maxLen])
    // to remove a word from a sentence
 * ! void titleStr(char string[])
    // to title case the string (eg: bill gates -> Bill Gates)
*/

void echoArr(int size, int array[]){
    printf("[%d", array[0]);
    for (int i = 1; i < size; i++){
        printf(", %d", array[i]);
    }
    printf("]\n");
}

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

void echoStrArr(int size, int _maxLen, char array[][_maxLen]){
    printf("[%s", array[0]);
    for (int i = 1; i < size; i++){
        printf(", %s", array[i]);
    }
    printf("]\n");
}

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

int wordCount(char string[], char divider){
    int count = 1;
    for (int i = 0; string[i]!='\0'; i++){
        if (string[i]==divider){
            count++;
        }
    }
    return count;
}

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

void titleStr(char string[]){
    string[0] = toupper(string[0]);
    for (int i = 0; string[i]!='\0';i++){
        if (string[i]==' '){
            string[i+1] = toupper(string[i+1]);
        }
    }
}
