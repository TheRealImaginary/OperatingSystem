void init();
void printChar(char);
void printString(char*);
char readChar();
void readString(char*);
void readSector(char*, int);
void writeSector(char*, int);
int mod(int, int);
int div(int, int);
int strCompare(char*, char*);
void readFile(char*, char*);
void writeFile(char*, char*, int);
void deleteFile(char*);
void terminateProgram();
void executeProgram(char*, int);
void killProcess(int);
void handleInterrupt21(int, int, int, int);
void clearArray(char*, int);
void handleTimerInterrupt(int, int);

int active[8];
int stackPtr[8];
int currentProcess;
int counter;

int main() {
	int i;

	for (i = 0; i < 8; i++) {
		active[i] = 0;
		stackPtr[i] = 0xFF00;
	}
	currentProcess = -1;
	counter = 100;

	makeTimerInterrupt();
	makeInterrupt21();
	init();
	while (1);
	printString("You Should not see me\n\0");
}

void init() {
	// char input[80];

	// printString("Enter A String: ");
	// readString(input);
	// printString(input);

	// readSector(buffer, 30);
	// printString(buffer);

	// //Read
	// interrupt(0x21, 1, input, 0, 0);
	// //Print
	// interrupt(0x21, 0, input, 0, 0);
	// //Read From Disk
	// interrupt(0x21, 2, buffer, 30, 0);
	// //Print
	// interrupt(0x21, 0, buffer, 0, 0);
	// //Error
	// interrupt(0x21, 3, 0, 0, 0);

	//Loads Message From Drive
	// interrupt(0x21, 3, "messag\0", buffer, 0);
	//	// Prints Loaded Program
	// interrupt(0x21, 0, buffer, 0, 0);

	// //Load and Test "testprg"
	// interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
	// // Load and Test "testprg2"
	// interrupt(0x21, 4, "tstpr2", 0x2000, 0);
	// printString("Is This Working?!!!!!!\0");

	// interrupt(0x21, 4, "hello1\0", 0, 0);
	// interrupt(0x21, 4, "hello2\0", 0, 0);
	interrupt(0x21, 4, "shell\0", 0, 0);

	// int i;
	// char buffer1[13312];
	// char buffer2[13312];
	// buffer2[0] = 'h'; buffer2[1] = 'e'; buffer2[2] = 'l'; buffer2[3] = 'l';
	// buffer2[4] = 'o';
	// for (i = 5; i < 13312; i++)
	// 	buffer2[i] = 0x0;
	// interrupt(0x21, 8, "testW\0", buffer2, 1);
	// interrupt(0x21, 3, "testW\0", buffer1, 0);
	// interrupt(0x21, 0, buffer1, 0, 0);

	// char buffer[13312];
	// char buffer1[13312];
	// char fileName[15];
	// fileName[0] = 'm';
	// fileName[1] = '2';
	// fileName[2] = 0x0;
	// fileName[3] = 0x0;
	// fileName[4] = 0x0;
	// fileName[5] = 0x0;
	// fileName[6] = 0x0;
	// interrupt(0x21, 3, "messag\0", buffer, 0);
	// interrupt(0x21, 0, buffer, 0, 0);
	// interrupt(0x21, 8, fileName, buffer, 1);
	// interrupt(0x21, 3, fileName, buffer1, 0);
	// interrupt(0x21, 0, buffer1, 0, 0);
}

void printChar(char c) {
	interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
}

void printString(char* chars) {
	int i;
	for (i = 0; chars[i] != '\0'; i++) {
		if (chars[i] == '\n') {
			//Prints New Line
			interrupt(0x10, 0xE * 256 + 0xD, 0, 0, 0);
			interrupt(0x10, 0xE * 256 + 0xA, 0, 0, 0);
		}
		else {
			printChar(chars[i]);
		}
	}
}

char readChar() {
	return interrupt(0x16, 0x0, 0, 0, 0);
}

void readString(char* c) {
	int idx = 0;
	while (1) {
		c[idx] = readChar();
		printChar(c[idx]);
		if (c[idx] == 0xD)
			break;
		else if (c[idx] == 0x8) {
			if (idx > 0) {
				printChar(0x0);
				idx--;
				printChar(0x8);
			}
			else {
				printChar(0x20);
			}
			continue;
		}
		idx++;
	}
	printChar(0xA);
	c[++idx] = '\n';
	c[++idx] = '\0';
}

void readSector(char* buffer, int sector) {
	interrupt(0x13, 2 * 256 + 1, buffer, div(sector, 36) * 256 + (mod(sector, 18) + 1), mod(div(sector, 18), 2) * 256 + 0 );
}

void writeSector(char* buffer, int sector) {
	interrupt(0x13, 3 * 256 + 1, buffer, div(sector, 36) * 256 + (mod(sector, 18) + 1), mod(div(sector, 18), 2) * 256 + 0 );
}

int mod (int num, int div) {
	while (num >= div) {
		num = num - div;
	}
	return num;
}


int div(int num, int div) {
	int temp;
	temp = num / div;
	return temp;
}

int strCompare(char* s1, char* s2) {
	int i = 0;
	while ((i < 6) && ((s1[i] == s2[i])))
		i++;
	return i == 6;
}

void readFile(char* fileName, char* file) {
	char directory[512];
	char temp[512];
	int i = 0, j = 0, k = 0, l = 0;
	// interrupt(0x21, 0, fileName, 0, 0);
	// interrupt(0x21, 0, fileName, 0, 0);
	// Read Directory Map
	readSector(directory, 2);
	while (j < 512) {
		for (i = 0; i < 6; i++)
			temp[i] = directory[i + j];
		if (strCompare(fileName, temp)) {
			// interrupt(0x21, 0, "Found\0", 0, 0);
			// printString("Found\n\0");
			j += 6;
			while (directory[j] != 0x00) {
				readSector(temp, directory[j]);
				k = 0;
				for (; k < 512; k++, l++)
					file[l] = temp[k];
				j++;
			}
			// printString("Freedom\n\0");
			return;
		}
		j += 32;
	}
	// interrupt(0x21, 0, "Not Found\n\0", 0, 0);
	// printString("Not Found\0");
}

void writeFile(char* name, char* buffer, int secNum) {
	char map[512];
	char directory[512];
	char finalName[7];
	char filledSec[26];
	int temp = secNum;
	char tempFile[512];
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;

	clearArray(finalName, 7);
	readSector(map, 1);
	readSector(directory, 2);
	// printString("I have loaded map and directory\n\0");

	while (i < 6) {
		if (name[i] == 0xD || name[i] == '\0' || name[i] == 0x0)
			break;
		finalName[i] = name[i];
		i++;
	}

	i = 0;
	while (i < 512) {
		if (map[i] == 0x00 && temp > 0) {
			map[i] = 0xFF;
			filledSec[j] = i;
			j++;
			temp--;
		}
		i++;
	}
	// printString("I am working in map\n\0");
	if (temp > 0) {
		// printString("Not enough space\n\0");
		return;
	}
	i = 0;
	j = 0;
	while (i < 512) {
		if (directory[i] == 0x00) {
			// printString("Found Directory To Write In\n\0");
			while (k < 6) {
				directory[i + k] = finalName[k];
				k++;
			}
			k = 0;
			i += 6;
			while (k < secNum) {
				directory[i] = filledSec[k];
				l = 0;
				while (l < 512) {
					tempFile[l] = buffer[j];
					j++;
					l++;
				}
				writeSector(tempFile, filledSec[k]);
				k++;
				i++;
			}
			writeSector(map, 1);
			writeSector(directory, 2);
			// printString("Done Writing\n\0");
			return;
		}
		else {
			i += 32;
		}
	}
	// printString("No Empty Dir, Exiting");
}

void deleteFile(char* name) {
	char map[512];
	char directory[512];
	char temp[512];
	char sectors[26];
	int i = 0, j = 0, k = 0, l = 0;
	int sector = 0;
	int numberOfSectors = 0;

	readSector(map, 1);
	readSector(directory, 2);

	while (j < 512) {
		for (i = 0; i < 6; i++)
			temp[i] = directory[i + j];
		if (strCompare(name, temp)) {
			// printString("File Name Found\n\0");
			directory[j] = 0x0;
			j += 6;
			while (directory[j] != 0x00) {
				sectors[k] = directory[j];
				numberOfSectors++;
				k++;
				j++;
			}
			for (l = 0; l < numberOfSectors; l++) {
				sector = sectors[l];
				if (sector != 0) {
					map[sector] = 0x00;
				} else {
					break;
				}
			}
			writeSector(map, 1);
			writeSector(directory, 2);
		}
		j += 32;
	}
}

void executeProgram(char* name) {
	char program[13312];
	int i, segment;
	readFile(name, program);
	if (!program[0] || program[0] == 0x0) {
		return;
	}
	setKernelDataSegment();
	for (i = 0; i < 8; i++) {
		if (active[i] == 0) {
			active[i] = 1;
			segment = (i + 2) * 0x1000;
			break;
		}
	}
	restoreDataSegment();
	for (i = 0; i < 13312; i++)
		putInMemory(segment, i, program[i]);
	initializeProgram(segment);
}

void terminateProgram() {
	// char shell[6];
	// shell[0] = 's';
	// shell[1] = 'h';
	// shell[2] = 'e';
	// shell[3] = 'l';
	// shell[4] = 'l';
	// shell[5] = 0x0;
	// interrupt(0x21, 4, shell, 0x2000, 0);
	setKernelDataSegment();
	active[currentProcess] = 0;
	while (1);
}

void killProcess(int pid) {
	if (pid >= 8)
		return;
	setKernelDataSegment();
	active[pid] = 0;
	stackPtr[pid] = 0xFF00;
	restoreDataSegment();
}

void handleTimerInterrupt(int segment, int sp) {
	int chosenSegment = segment, chosenSP = sp;
	int i;
	setKernelDataSegment();
	if (counter == 100) {
		counter = 0;
		if (currentProcess >= 0)
			stackPtr[currentProcess] = sp;
		i = currentProcess + 1 == 8 ? 0 : currentProcess + 1;
		for (; i < 8;) {
			if (active[i] == 1) {
				currentProcess = i;
				chosenSegment = (i + 2) * 0x1000;
				chosenSP = stackPtr[i];
				break;
			}
			i = i + 1 == 8 ? 0 : i + 1;
			if (i == currentProcess || (currentProcess == -1 && i == 0)) {
				break;
			}
		}
	} else {
		counter++;
	}
	restoreDataSegment();
	returnFromTimer(chosenSegment, chosenSP);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	}
	else if (ax == 1) {
		readString(bx);
	}
	else if (ax == 2) {
		readSector(bx, cx);
	}
	else if (ax == 3) {
		readFile(bx, cx);
	}
	else if (ax == 4) {
		// printString("We Will Execute\n\0");
		executeProgram(bx);
	}
	else if (ax == 5) {
		// printString("Terminating\n\0");
		terminateProgram();
	}
	else if (ax == 6) {
		writeSector(bx, cx);
	}
	else if (ax == 7) {
		deleteFile(bx);
	}
	else if (ax == 8) {
		writeFile(bx, cx, dx);
	}
	else if (ax == 9) {
		killProcess(bx);
	}
	else {
		printString("This is not a valid AX value! \0");
	}
}

void clearArray(char* arr, int size) {
	int i;
	for (i = 0; i < size; i++)
		arr[i] = 0x0;
}