#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h>
#include <ctype.h>

char *GetLine(FILE *file);
char *LineAtIndex(FILE *file, int index);
FILE * OpenFile(char *filename);
int counterLine(FILE *file);
char *GetLine(FILE *file);
char *LowerCase(char *line);
void FindAllWords(char *line, char *string, int stringLength, FILE *file,
 int mid, int fFlag, int dFlag);
void BinarySearch(int lineCount, FILE *file, int stringLength, char *string,
 int fFlag, int dFlag);

char *LineAtIndex(FILE *file, int index) {
	int i;
	char *line;

	i = 0;
	while (i < index) {
 		line = GetLine(file);
 		i++;
 	}
 	return line;
}

FILE * OpenFile(char *filename) { 
   FILE *file;

   file = fopen(filename, "r");
   if (file == NULL) {
      printf("look: %s: No such file or directory\n", filename);
      exit(2);
   }
   return file;    
}

int counterLine(FILE *file) {
	char c;
	int lctr = 0;

	while((c = fgetc(file)) != EOF)
	{
		if(c == '\n') {
			lctr++;
		}
	}
    return lctr;
}

char *GetLine(FILE *file) {                                                     
    int ctr = 0;                                                                 
    char c, *line;                                                               
                                                                                 
    line = calloc(sizeof(char), 1);                                              
                                                                                 
    while ((c = fgetc(file)) != '\n' && c != EOF) {                              
       *(line + ctr) = c;                                                        
       ctr++;                                                                    
       line = realloc(line, ctr + 1);                                            
    }                                                                            
                                                                                 
    *(line + ctr) = 0;                                                           
    return line;                                                                 
}   

char *LowerCase(char *line) {
	char *lowerLine;
	int i;

	lowerLine = strdup(line);
	i = 0;
	
	while(lowerLine[i]) 
   	{
    	lowerLine[i] = tolower(lowerLine[i]);
        i++;
   	}
   	return lowerLine;
}

char *AlphaNumeric(char *line) {
	char *alphaLine;
	int i, length, counter;

	alphaLine = calloc(strlen(line) + 1, 1);
	length = strlen(line);
	counter = 0;

	for (i = 0; i < length; i++) {
		if (isalnum(line[i])) {
			alphaLine[counter] = line[i];
			counter++;
		}
	}
	return alphaLine;
}

void FindAllWords(char *line, char *string, int stringLength, FILE *file,
 int mid, int fFlag, int dFlag) {
	int j;
	char *line2;

	j = 1;
	line2 = strdup(line);
	if (fFlag)
		line2 = LowerCase(line2);
	if (dFlag)
		line2 = AlphaNumeric(line2);

	while (strncmp(line2, string, stringLength) == 0 && (mid - j) > 0) {
 		line2 = LineAtIndex(file, mid - j);
	 	rewind(file);

	 	if (fFlag)
			line2 = LowerCase(line2);
		if (dFlag)
			line2 = AlphaNumeric(line2);
	 	if (strncmp(line2, string, stringLength) != 0) 
	 		break;
	 	j++;
	}

	
	line = LineAtIndex(file, mid - j + 1);
	line2 = strdup(line);
	
	if (fFlag)
		line2 = LowerCase(line2);
	if (dFlag)
		line2 = AlphaNumeric(line2);
	
	while (strncmp(line2, string, stringLength) == 0) {
		puts(line);
		line = GetLine(file);
		line2 = strdup(line);
		
		if (fFlag)
			line2 = LowerCase(line2);
		if (dFlag)
			line2 = AlphaNumeric(line2);
	}
}


void BinarySearch(int lineCount, FILE *file, int stringLength,
 char *string, int fFlag, int dFlag) {
	int bottom, top, mid;
	char *line, *line2;

	bottom = 0;
	top = lineCount;
	mid = (top + bottom) / 2;

	while (bottom <= top) {
		line = LineAtIndex(file, mid);
		line2 = strdup(line);
		
		if (fFlag)
			line2 = LowerCase(line2);
		if (dFlag) {
			line2 = AlphaNumeric(line2);
		}

	 	rewind(file);

 		if (strncmp(line2, string, stringLength) < 0)
	 		bottom = mid + 1;
	 	else if (strncmp(line2, string, stringLength) > 0)
	 		top = mid - 1;
	 	else {
			FindAllWords(line, string, stringLength, file, mid,
			 fFlag, dFlag);
			break;
		}
	 	mid = (top + bottom) / 2;
	 }
    if (bottom > top) 
    	exit(1);
    exit(0);
}


int main(int argc, char **argv) {
	int i, stringLength, textFile, dFlag, tFlag, fFlag, lineCount, saved;
	char termChar, dictionary[25], string[20], *pch;
	FILE *file;

	i = saved = dFlag = fFlag = tFlag = textFile = 0;
	strcpy(dictionary, "/usr/share/dict/words");
	for (i = 1; i < argc; i++) {
		
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'd'){
				dFlag = 1;
			}
			else if (argv[i][1] == 'f') {
				fFlag = 1;
			}
			else if (argv[i][1] == 't') {
				if (i == argc - 1) {
					printf("look: option requires an argument -- t\n");
					return 2;
				}
				termChar = *(argv[i + 1]);
				if (strlen((argv[i + 1])) > 1) {
					printf("look: invalid termination character\n");
					return 2;
				}
				i++;
				tFlag = 1;
			}
			else 
				printf("look: illegal option -- %c\n", argv[i][1]);
		}
		else { 
			if (saved == 0) {
				strcpy(string, argv[i]);
				saved = 1;
			}
			else {
				file = OpenFile(argv[i]);
				textFile = 1;
			}
		}
	}

	if (saved == 0) {
		printf("usage: look [-df] [-t char] string [file ...]\n");
		exit(2);
	}

	if (!textFile && argc > 1 && saved) {
		file = OpenFile(dictionary);
		fFlag = dFlag = 1;
	}

	stringLength = strlen(string);
	lineCount = counterLine(file);
	rewind(file);

	if (tFlag) {
		pch = strchr(string, termChar);
		if (pch != NULL)
			*(pch + 1) = 0;
		stringLength = strlen(string);	
	}

	if (fFlag)
		strcpy(string, LowerCase(string));
	if (dFlag) {
		strcpy(string, AlphaNumeric(string));
	}
	BinarySearch(lineCount, file, stringLength, string, fFlag, dFlag);
}
