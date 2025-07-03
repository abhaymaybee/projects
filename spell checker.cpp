#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

void toLowerCase(char* word) {
for (int i = 0; word[i] != '\0'; i++) {
word[i] = tolower(word[i]);
}
}

int main() {
FILE* dictionaryFile = fopen("dictionary.txt", "r");
if (dictionaryFile == NULL) {
printf("Error opening dictionary file.\n");
return 1;
}

char word[MAX_WORD_LENGTH];
printf("Enter a word: ");
scanf("%s", word);

toLowerCase(word);

int found = 0;
char suggestedWord[MAX_WORD_LENGTH];

    
char dictWord[MAX_WORD_LENGTH];
while (fscanf(dictionaryFile, "%s", dictWord) != EOF) {
if (strcmp(dictWord, word) == 0) {
found = 1;
break;
}
}

if (found) {
printf("The word '%s' is spelled correctly.\n", word);
} 
else {
printf("The word '%s' is misspelled.\n", word);

        
fseek(dictionaryFile, 0, SEEK_SET);
int minDistance = MAX_WORD_LENGTH;
while (fscanf(dictionaryFile, "%s", dictWord) != EOF) {
int len1 = strlen(word);
int len2 = strlen(dictWord);
if (abs(len1 - len2) > minDistance) {
continue;  
}

        
int dp[MAX_WORD_LENGTH][MAX_WORD_LENGTH];
for (int i = 0; i <= len1; i++) {
for (int j = 0; j <= len2; j++) {
if (i == 0) {
dp[i][j] = j;
} else if (j == 0) {
    dp[i][j] = i;
} else if (word[i - 1] == dictWord[j - 1]) {
dp[i][j] = dp[i - 1][j - 1];
} 
else {
dp[i][j] = 1 + (dp[i - 1][j - 1] < dp[i][j - 1] ? dp[i - 1][j - 1] : dp[i][j - 1]);
dp[i][j] = (dp[i][j] < dp[i - 1][j] ? dp[i][j] : dp[i - 1][j]);
}
}
}

if (dp[len1][len2] < minDistance) {
 minDistance = dp[len1][len2];
 strcpy(suggestedWord, dictWord);
}
}

if (minDistance < MAX_WORD_LENGTH) {
printf("Did you mean '%s'?\n", suggestedWord);
}
 else {
  printf("No suggestions found.\n");
}
}

fclose(dictionaryFile);

return 0;
}

