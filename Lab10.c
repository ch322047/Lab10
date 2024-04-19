#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie{
    int WordCount;
    struct Trie *Next[26];
};

// Initializes a trie structure
struct Trie *createTrie(){
    //creating the struct
    struct Trie *TheTree = malloc(sizeof(struct Trie));
    TheTree->WordCount = 0;

    //setting the pointers to NULL
    int i;
    for(i = 0; i < 26; i ++){
        TheTree->Next[i] = NULL;
    }
    return TheTree;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word){
    int WordLength = strlen(word);
    //iterate through whole word to create the trie path for it
    for(int i = 0; i < WordLength; i++){
        int NextIndex = (word[i] - 'a');
        //printf("\tNextIndex Number: %d, Word: %s , Word Count:%d \n", NextIndex, word, pTrie->WordCount);
        //if the next letter for the trie path does not exist make it
        if(pTrie->Next[NextIndex] == NULL){
            pTrie->Next[NextIndex] = createTrie();
            pTrie = pTrie->Next[NextIndex];
        }
        //if the next letter for the trie path already exists
        else if(pTrie->Next[NextIndex] != NULL){
            pTrie = pTrie->Next[NextIndex];
        }
    }
    //once I get to the end of the word/trie, incremenet the WordCount
    pTrie->WordCount += 1;
    //printf("\tWord: %s , Word Count:%d \n-----\n", word, pTrie->WordCount);
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word){
int WordLength = strlen(word);
int j = 0;
//iterate through whole trie to find the word
for(int i = 0; i < WordLength; i++){
        int NextIndex = (word[i] - 'a');
        //printf("\tNextIndex Number: %d, Word: %s , Word Count:%d \n", NextIndex, word, pTrie->WordCount);
        //if the next letter for the trie path does not exist make it
        if(pTrie->Next[NextIndex] != NULL){
            pTrie = pTrie->Next[NextIndex];
            j++;
        }
        
}
if (j == WordLength)//if it is in our dictionary, we should have gone the exact number of entries 
    return pTrie->WordCount;

else//if the word is not in our dictonary, i.e. j is not equal WordCount
    return 0;

}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    //deallocate trie from inside out
    for(int i = 0; i < 26; i++){
        if (pTrie->Next[i] != NULL){
        deallocateTrie(pTrie->Next[i]);
        }
    }
    
    free(pTrie);
    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* InputFile = fopen(filename, "r");
    if(InputFile == NULL){
        printf("unable to open file");
        return -1;
    } 
    int count = 0;

    fscanf(InputFile, "%d\n", &count);
    char word[100];
    for(int i = 0; i < count; i++){
        fscanf(InputFile, "%s\n", word);
        pInWords[i] = (char*)calloc(strlen(word), sizeof(char));
        strcpy(pInWords[i], word);
    }
    return count;//number of words
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}