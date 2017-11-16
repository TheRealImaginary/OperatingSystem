int strCompare(char*, char*, int);
int getSectorAmount(char*);
int div(int, int);
int mod(int, int);
void clearArray(char*, int);
void extractFileName(char*, int);
void handleCommand(char*, char*, char*);

int main() {

	char input[512];
	char fileName[15];
	char fileName2[15];
	char fileContent[13312];

	while (1) {
		clearArray(input, 512);
		clearArray(fileName, 15);
		clearArray(fileName2, 15);
		clearArray(fileContent, 13312);
		//Print Shell >>
		interrupt(0x21, 0, "Shell >> \0", 0, 0);
		//Read Command
		interrupt(0x21, 1, input, 0, 0);
		handleCommand(input, fileName, fileName2, fileContent);
	}
}

void handleCommand(char* input, char* fileName, char* fileName2, char* fileContent) {
	int sectorAmount = 0, i = 0, j = 0;
	if (strCompare(input, "view", 4)) {
		extractFileName(input, 5, fileName, fileName2);
		if (fileName[0] == '\n' || fileName[0] == 0x0) {
			interrupt(0x21, 0, "Please Enter A File\n\0", 0, 0);
		}
		else {
			interrupt(0x21, 3, fileName, fileContent, 0);
			if (fileContent[0] == 0x0) {
				interrupt(0x21, 0, "File Not Found!\n\0");
			}
			else {
				interrupt(0x21, 0, fileContent, 0, 0);
			}
		}
	}
	else if (strCompare(input, "execute", 7)) {
		extractFileName(input, 8, fileName, fileName2);
		if (fileName[0] == '\n') {
			interrupt(0x21, 0, "Please Enter A File\n\0", 0, 0);
		}
		else {
			interrupt(0x21, 3, fileName, fileContent, 0);
			if (fileContent == 0x0) {
				interrupt(0x21, 0, "File Not Found!\n\0", 0, 0);
			}
			else {
				interrupt(0x21, 4, fileName, 0x2000, 0);
			}
		}
	}
	else if (strCompare(input, "delete", 6)) {
		extractFileName(input, 7, fileName, fileName2);
		if (fileName[0] == '\n') {
			interrupt(0x21, 0, "Please Enter A File\n\0", 0, 0);
		}
		else {
			interrupt(0x21, 3, fileName, fileContent, 0);
			if (fileContent[0] == 0x0) {
				interrupt(0x21, 0, "File Not Found!\n\0", 0, 0);
			}
			else {
				interrupt(0x21, 7, fileName, 0, 0);
				interrupt(0x21, 0, "Deleted File Successfully!\n\0", 0, 0);
			}
		}
	}
	// Doesn't work when name is less than 6
	else if (strCompare(input, "copy", 4)) {
		extractFileName(input, 5, fileName, fileName2);
		// interrupt(0x21, 0, fileName, 0, 0);
		// interrupt(0x21, 0, fileName2, 0, 0);
		if (fileName == '\n' || fileName2[0] == '\n') {
			interrupt(0x21, 0, "Please Enter A File\n\0", 0, 0);
		}
		else {
			interrupt(0x21, 3, fileName, fileContent, 0);
			if (fileContent[0] == 0x0) {
				interrupt(0x21, 0, "File Not Found!\n\0", 0, 0);
			}
			else {
				sectorAmount = getSectorAmount(fileContent);
				interrupt(0x21, 8, fileName2, fileContent, sectorAmount);
				interrupt(0x21, 0, "Copied File Successfully!\n\0", 0, 0);
			}
		}
	}
	else if (strCompare(input, "dir", 3)) {
		extractFileName(input, 4, fileName, fileName2);
		if (fileName[0] == 0x0 || fileName[0] == 0xD) {
			interrupt(0x21, 0, "Please Enter A File!\n\0", 0, 0);
		}
		else {
			interrupt(0x21, 2, fileContent, 2, 0);
			for (i = 0; i < 13312; i += 32) {
				if (fileContent[i] != 0x0) {
					for (j = 0; j < 6; j++) {
						if (fileContent[i + j] == 0x0)
							break;
						fileName[j] = fileContent[i + j];
					}
					fileName[j] = 0x20;
					fileName[++j] = '\0';
					for (j = i + 6; j < 13312; j++) {
						if (fileContent[j] == 0x0) {
							break;
						}
						sectorAmount++;
					}
					fileName2[0] = (sectorAmount + '0');
					fileName2[1] = '\n';
					fileName2[2] = '\0';
					interrupt(0x21, 0, fileName, 0, 0);
					interrupt(0x21, 0, fileName2, 0, 0);
					clearArray(fileName, 15);
					clearArray(fileName2, 15);
					sectorAmount = 0;
				}
			}
		}
	}
	// Doesn't work when name is less than 6
	else if (strCompare(input, "create", 6)) {
		extractFileName(input, 7, fileName, fileName2);
		if (fileName[0] == 0x0 || fileName[0] == 0xD) {
			interrupt(0x21, 0, "Please Enter A File!\n\0", 0, 0);
		}
		else {
			interrupt(0x21, 0, "Please Enter Content. End With an Empty Line!\n\0");
			while (1) {
				j = 0;
				clearArray(input, 512);
				interrupt(0x21, 1, input, 0, 0);
				if (input[0] == 0x0 || input[0] == 0xD || input[0] == 0xA) {
					break;
				}
				for (; j < 512; i++, j++) {
					if (input[j] == 0x0 || input[j] == 0xA || input[j] == 0xD)
						break;
					fileContent[i] = input[j];
				}
				fileContent[i++] = 0xD;
				fileContent[i++] = 0xA;
			}
			// fileContent[i++] = 0xD;
			// fileContent[i++] = 0xA;
			fileContent[i] = '\0';
			j = 0;
			for (; j < 13312; j++)
				if (fileContent[j] != 0x0)
					sectorAmount++;
			sectorAmount = getSectorAmount(sectorAmount);
			// interrupt(0x10, 0xE * 256 + (sectorAmount + '0'), 0, 0, 0);
			// interrupt(0x21, 0, fileContent, 0, 0);
			interrupt(0x21, 8, fileName, fileContent, sectorAmount);
			interrupt(0x21, 0, "Created File Successfully!\n\0", 0, 0);
		}
	}
	else if (strCompare(input, "kill", 4)) {
		extractFileName(input, 5, fileName, fileName2);
		if (fileName[0] == 0x0 || fileName[0] == 0xD) {
			interrupt(0x21, 0, "Please Enter A Process Number!\n\0", 0, 0);
		}
		else {
			i = fileName[0] - '0';
			interrupt(0x21, 9, i, 0, 0);
			interrupt(0x21, 0, "Process Killed Successfully!\n\0", 0, 0);
		}
	}
	else {
		interrupt(0x21, 0, "Not A Command\n\0", 0, 0);
	}
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

int getSectorAmount(char* fileContent) {
	int res = 0, i;
	for (i = 0; i < 13312; i++)
		if (fileContent[i] == 0x0)
			break;
	res += div(i, 512);
	if (mod(i, 512) != 0)
		res += 1;
	return res;
}

void clearArray(char* arr, int size) {
	int i;
	for (i = 0; i < size; i++)
		arr[i] = 0x0;
}

int strCompare(char* s1, char* s2, int j) {
	int i = 0;
	while ((i < j) && (s1[i] == s2[i]))
		i++;
	return i == j;
}

void extractFileName(char* chars, int startIndex, char* fileName, char* fileName2) {
	int i = 0;
	clearArray(fileName, 15);
	clearArray(fileName2, 15);
	while (chars[startIndex] == 0x20)
		startIndex++;
	for (; (i < 6 && chars[startIndex] != '\n'); startIndex++, i++) {
		fileName[i] = chars[startIndex];
	}
	fileName[i++] = '\n';
	fileName[i] = '\0';

	i = 0;
	while (chars[startIndex] == 0x20)
		startIndex++;

	for (; (i < 6 && chars[startIndex] != '\n'); startIndex++, i++) {
		fileName2[i] = chars[startIndex];
	}
	fileName2[i++] = '\n';
	fileName2[i] = '\0';
}
