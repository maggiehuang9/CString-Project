#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>
#include<cassert>
using namespace std;
const int MAX_WORD_LENGTH = 20;
const int MAX_DOCUMENT_LENGTH = 250;

int makeProper(char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int separation[],
	int nPatterns);
void toLowerCase(char word[]);
bool isValidWord(char word[]);
void removeSamePattern(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int nPatterns, int separation[]);
bool samePattern(char w1[], char w2[], char x1[], char x2[]);
void processDocument(const char document[], char copy[]);
int splitStringIntoWords(char str[], char words[][MAX_WORD_LENGTH + 1]);
void copyString(char d[], const char s[]);
int rate(const char document[],
	const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	const int separation[],
	int nPatterns);

int main() {
	char test1w1[][MAX_WORD_LENGTH + 1] = { "Jim", "Pam", "Michael", "Angela", "Kevin", "Kelly", "Jan", "paper0", "Jim", "", "Jim", "Pam", "jim", "michael" };
	char test1w2[][MAX_WORD_LENGTH + 1] = { "Pam", "Pam", "David", "Liza", "Alex", "Scott", "Todd", "Zane", "Pam", "Heath", "Pam", "Jim", "pam", "heath" };
	int separation[] = { 1, 2, 3, 2, -2, 0, 5, 2, 1, 4, 2, 3, 6, 1 };
	assert(makeProper(test1w1, test1w2, separation, 3) == 3);//pattern contains same w1 and w2 values
	assert(makeProper(test1w1, test1w2, separation, 5) == 4);//negative separation in pattern
	assert(makeProper(test1w1, test1w2, separation, 2) == 2);//pattern has same w1 and w2
	assert(makeProper(test1w1, test1w2, separation, 8) == 6);//word in pattern contains a nonletter
	assert(makeProper(test1w1, test1w2, separation, 0) == 0);//nPatterns is 0
	assert(makeProper(test1w1, test1w2, separation, -2) == 0);//nPatterns is negative
	assert(makeProper(test1w1, test1w2, separation, 9) == 6);//two patterns have same w1 and w2 with same separation
	assert(makeProper(test1w1, test1w2, separation, 10) == 6);//word contains no letters
	assert(makeProper(test1w1, test1w2, separation, 11) == 6);//patterns have same w1 and w2 with different separation values
	assert(makeProper(test1w1, test1w2, separation, 12) == 6);//patterns have same w1 and w2 in different order
	assert(makeProper(test1w1, test1w2, separation, 14) == 7);//patterns have same w1 and and different w2

	char document[MAX_DOCUMENT_LENGTH + 1] = "Hello, my name is Jenna and I have a basketball game tomorrow!";
	char test2w1[][MAX_WORD_LENGTH + 1] = { "Hello", "jenna", "cat", "basketball",     "I", "basketball" };
	char test2w2[][MAX_WORD_LENGTH + 1] = { "Hello",    "my",  "is",          "a", "Jenna",       "game" };
	int separation1[] = { 0, 2, 2, 0, -2, 2 };

	assert(makeProper(test2w1, test2w2, separation1, 6) == 5);//nPatterns is 5
	assert(rate(document, test2w1, test2w2, separation1, -2) == 0);//nPatterns is negative
	assert(rate(document, test2w1, test2w2, separation1, 0) == 0);//nPatterns is 0
	assert(rate(document, test2w1, test2w2, separation1, 1) == 0);//find pattern when w1 and w2 are the same value
	assert(rate(document, test2w1, test2w2, separation1, 2) == 1);//find pattern with exact amount of intervening words
	assert(rate(document, test2w1, test2w2, separation1, 3) == 1);//find a nonexisting pattern
	assert(rate(document, test2w1, test2w2, separation1, 4) == 2);//find a pattern with less intervening words than separation
	assert(rate(document, test2w1, test2w2, separation1, 5) == 3);//find a pattern that has more intervening words than separation
																 
	char document1[MAX_DOCUMENT_LENGTH + 1] = "I am excited to be here and I am excited to see you see";
	char test3w1[][MAX_WORD_LENGTH + 1] = { "I",   "to", "yeet",     "to",  "see", "you", "Am", "a", "and" };
	char test3w2[][MAX_WORD_LENGTH + 1] = { "am", "here",   "hi", "excited", "you", "see", "excited", "excited",   "i" };
	int separation2[] = { 0, 2, 2, 0, -2, 2, 3, 4, 20 };
	assert(makeProper(test3w1, test3w2, separation2, 9) == 8); //nPatterns is 5
	for (int i = 0; i < 9; i++)
		cout << test3w1[i] << " " << test3w2[i] << " " << separation2[i] << endl;
	assert(rate(document1, test3w1, test3w2, separation2, 1) == 1);//multiple patterns in the document
	assert(rate(document1, test3w1, test3w2, separation2, 2) == 2);//w2 is found after w1
	assert(rate(document1, test3w1, test3w2, separation2, 4) == 3);//w2 is found before w1
	assert(rate(document1, test3w1, test3w2, separation2, 5) == 4);//w1 is found before and after w2
	assert(rate(document1, test3w1, test3w2, separation2, 6) == 5);//w2 is found before and after w1
	assert(rate(document1, test3w1, test3w2, separation2, 7) == 5);//pattern is found with different capitalization
	assert(rate(document1, test3w1, test3w2, separation2, 8) == 6);//w1 is found in only part of the word
	
	const int TEST2_NRULES = 5;
	char str1[] = " The mad UCLA scientist d evil giant robot, notthatlonglonglongp g kindoflongkindoflongq verylonglonglonglonglong";
	char test4w1[TEST2_NRULES][MAX_WORD_LENGTH + 1] = { "", "notthatlonglonglongp", "longlonglonglonglong", "hajsgve", "pqpq" };
	char test4w2[TEST2_NRULES][MAX_WORD_LENGTH + 1] = { "scientist", "g", "pp","test", "pqpq" };
	int test2dist[TEST2_NRULES] = { 1, 0, 0, 0, 0 };
	assert(rate(str1, test4w1, test4w2, test2dist, TEST2_NRULES) == 1);//test word with more than 20 characters
	cout << "All tests succeeded" << endl;
}

void toLowerCase(char word[])
{
	//iterate through word and turn every letter lowercase
	for (int j = 0; word[j] != '\0'; j++)
	{
		word[j] = (char)tolower(word[j]);
	}

}

bool isValidWord(char word[])
{
	//word contains no letters
	if (word[0] == '\0') return false;
	//if w1 or w2 contain a character that is not a letter
	for (int j = 0; word[j] != '\0'; j++)
	{
		if (!isalpha(word[j]))
		{
			return false;
		}
	}

	return true;
}

bool samePattern(char w1[], char w2[], char x1[], char x2[])
{
	//if two patterns have the same w1 and w2 values
	if (strcmp(w1, x1) == 0 && strcmp(w2, x2) == 0)
		return true;
	//another possible combination if two patterns have same w1 and w2 values
	if (strcmp(w1, x2) == 0 && strcmp(w2, x1) == 0)
		return true;
	return false;
}
void removeSamePattern(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int nPatterns, int separation[])
{
	for (int i = 0; i < nPatterns; i++)
	{
		//if separation is negative, continue to next element
		if (separation[i] < 0)
			continue;
		for (int j = i + 1; j < nPatterns; j++)
		{
			//if separation is negative, continue to next element
			if (separation[j] < 0)
				continue;
			if (samePattern(word1[i], word2[i], word1[j], word2[j]))
			{
				//if patterns have same w1 and w2 values, set pattern with smaller separation to flag -1
				if (separation[i] < separation[j])
					separation[i] = -1;
				else
					separation[j] = -1;
			}

		}
	}
}
int makeProper(char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int separation[],
	int nPatterns)
{
	//switch all letters to lowercase
	for (int i = 0; i < nPatterns; i++)
	{
		toLowerCase(word1[i]);
		toLowerCase(word2[i]);
	}
	//if not a valid word or separation is negative
	int nValidPatterns = 0;
	for (int i = 0; i < nPatterns; i++) {
		if (!isValidWord(word1[i]) || !isValidWord(word2[i]) || separation[i] < 0) {
			separation[i] = -1; //set separation of invalid patterns to flag -1
		}
	}

	//if two patterns have the same w1 and w2 values
	removeSamePattern(word1, word2, nPatterns, separation);
	for (int i = 0; i < nPatterns; i++)
	{
		if (separation[i] >= 0)
		{
			//copy all valid patterns and skip invalid patterns which were flagged with -1
			copyString(word1[nValidPatterns], word1[i]);
			copyString(word2[nValidPatterns], word2[i]);
			separation[nValidPatterns] = separation[i];
			//count number of valid patterns
			nValidPatterns++;
		}
	}
	return nValidPatterns;
}

void processDocument(const char document[], char copy[])
{
	int newLength = 0;
	copyString(copy, document);
	//get rid of non-letters 
	for (int i = 0; i < strlen(copy); i++)
	{
		if (isalpha(copy[i]))
		{
			//save only letters into copy
			copy[newLength] = copy[i];
			//calculate new length once non-letters are removed
			newLength++;
		}
		else if (copy[i] == ' ')
		{
			if (newLength > 0)
			{
				if (copy[i - 1] != ' ') //if space is the first space after a letter
				{
					copy[newLength] = '-'; //set space to flag '-'
					newLength++; //calculate new length including all the dashes
				}
			}
		}
	}

	if (copy[newLength - 1] == '-') //if last character of original document was space
		copy[newLength] = '\0';  //set character after dash to zero byte
	else //if last character of original document was not a space
	{
		copy[newLength] = '-'; //add dash after last word
		copy[newLength + 1] = '\0'; //set character after dash to zero byte
	}

	toLowerCase(copy); //turn all letters of new document to lowercase
}

int splitStringIntoWords(char str[], char words[][MAX_WORD_LENGTH + 1])
{
	int nWords = 0;
	char word[MAX_DOCUMENT_LENGTH + 1];
	int count = 0;
	//iterate through str to separate each word
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == '-') //'-' flag separates every word
		{
			if (count <= MAX_WORD_LENGTH) {  // discard any word with more than MAX_WORD_LENGTH letters
				word[count] = '\0'; //end word with zero byte
				copyString(words[nWords], word); //save to the array
			}
			else { // for word longer than 20 letters, replace with an invalid string for space holder
				copyString(words[nWords], "###"); // use ### for space holder
			}
			nWords++; //count number of words
			count = 0;
		}
		else
		{
			word[count] = str[i]; //save word from str to word array
			count++;
		}
	}
	return nWords;
}

bool matchPattern(const char words[][MAX_WORD_LENGTH + 1], const int nWords, const char word1[], const char word2[], const int separation)
{
	//iterate to check the number of patterns the document matches
	for (int i = 0; i < nWords; i++)
	{
		if (strcmp(words[i], word1) == 0)//if doucument contains word1
		{
			int start = i - 1 - separation; //check for word2 if it is before word1
			int end = i + 1 + separation; //check for word2 if it is after word1
										  //make sure start and end are in bounds
			if (start < 0)	start = 0;
			if (end > nWords - 1)	end = nWords - 1;
			//check to see if there's match for word2
			for (int j = start; j <= end; j++)
			{
				if (j == i)	continue;
				//match found
				if (strcmp(words[j], word2) == 0)
					return true;
			}
		}
	}
	return false;
}

int rate(const char document[],
	const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	const int separation[],
	int nPatterns)
{
	char copy[MAX_DOCUMENT_LENGTH + 1];
	char words[126][MAX_WORD_LENGTH + 1];
	//save new document into copy
	processDocument(document, copy);
	for (int i = 0; i < 3; i++)
		cout << copy[i] << endl;
	//split document into array
	int nWords = splitStringIntoWords(copy, words);
	int nMatch = 0;
	for (int i = 0; i < nPatterns; i++)
	{
		//calculate number of patterns in document
		if (matchPattern(words, nWords, word1[i], word2[i], separation[i]))
			nMatch++;
	}
	return nMatch;
}

void copyString(char d[], const char s[]) {
	//copies s into d
	int i = 0;
	while (s[i] != '\0') {
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';//to end the cstring
}
