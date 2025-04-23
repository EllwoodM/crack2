#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find a matching hash in the hashFile.
// Get this function working first!
char * tryWord(char * plaintext, char * hashFilename)
{
    // Hash the plaintext
    int len = strlen(plaintext);
    if (len > 0 && plaintext[len - 1] == '\n') {
        plaintext[len - 1] = '\0';  // Strip newline if present
        len--;
    }
    char *hashedWord = md5(plaintext, len);  // md5 function returns a malloc'd string
    
    // Open the hash file
    FILE *hashFile = fopen(hashFilename, "r");
    if (!hashFile) {
        perror("Failed to open hash file");
        free(hashedWord);
        return NULL;
    }
    // Loop through the hash file, one line at a time.
    char line[HASH_LEN + 1];
    while (fgets(line, sizeof(line), hashFile)) {
        line[strcspn(line, "\n")] = '\0';  // Strip newline
        if (strcmp(line, hashedWord) == 0) {
            fclose(hashFile);
            return hashedWord;  // Return the hash if a match is found
        }
    }

    fclose(hashFile);
    free(hashedWord);  // Clean up the malloc'd memory
    return NULL;  // Return NULL if no match is found
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    char *found = tryWord("hello", argv[1]);  // argv[1] is the hash file
    if (found) {
        printf("%s %s\n", found, "hello");
        free(found);  // Free the memory allocated by md5 function
    } else {
        printf("No match found for 'hello'\n");
    }

    // Open the dictionary file for reading.
    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) {
        perror("Failed to open dictionary file");
        exit(1);
    }

    int crackedCount = 0;

    // For each dictionary word, pass it to tryWord, which will attempt to match it against the hashes in the hash_file.
    char word[PASS_LEN];
    while (fgets(word, sizeof(word), dictFile)) {
        // Strip the newline character
        word[strcspn(word, "\n")] = '\0';

        char *hash = tryWord(word, argv[1]);  // Try to find the hash of the current word
        if (hash) {
            printf("%s %s\n", hash, word);  // Print the cracked hash and word
            crackedCount++;
            free(hash);  // Free the memory allocated by md5 function
        }
    }        
 
    // Close the dictionary file.
    fclose(dictFile);

    // Display the number of hashes that were cracked.
    printf("%d hashes cracked!\n", crackedCount);
    return 0;
}

