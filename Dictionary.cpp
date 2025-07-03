#include <stdio.h>
#include <ctype.h>

int main() {
FILE *inputFile = fopen("wordlist.txt", "r");
FILE *outputFile = fopen("dictionary.txt", "w");

if (inputFile == NULL || outputFile == NULL) {
printf("Failed to open files!\n");
return 1;
}

char word[100];
while (fgets(word, sizeof(word), inputFile)) { 
word[strcspn(word, "\n")] = '\0';

        
int i = 0;
while (word[i]) {
if (!isalpha(word[i])) {
break;
}
i++;
}
if (word[i] == '\0') {
fprintf(outputFile, "%s\n", word);
}
}

fclose(inputFile);
fclose(outputFile);

printf("Dictionary file created successfully!\n");
return 0;
}

