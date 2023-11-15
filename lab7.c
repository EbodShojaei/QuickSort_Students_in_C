# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>

// Global error output
const char *error_output;

// Create a struct for the entity
typedef struct Student {
	char *first_name; // Alphabet
	char *last_name; // Alphabet
	char *number; // A followed by 7 digits
	char *midterm; // Ranges from 0 to 100
	char *final; // Ranges from 0 to 100

	struct Student *next;
} Student_t;

/**
 * Function to call error.
 * Prints error message and exits.
 */
void callError(char *message) {
	printf("%s\n", message);
	printf("\n");
	if (error_output == NULL) exit(1);
	FILE *file = fopen(error_output, "w");
	fprintf(file, "%s\n", message);
	fprintf(file, "\n");
	fclose(file);
	exit(1);
}

/**
 * Function to create a node.
 * Dynamically allocates memory for the node.
 * If the node is NULL, then error.
 */
Student_t *createNode() {
	Student_t *node = (Student_t *) malloc(sizeof(Student_t));
	if (node == NULL) callError("Error: Memory could not be allocated.");

	node->first_name = NULL;
	node->last_name = NULL;
	node->a_number = NULL;
	node->midterm = NULL;
	node->final = NULL;
	node->next = NULL;

	return node;
}

/**
 * Function to append a node to the end of the linked list.
 * If the head is NULL, then the head is the node.
 */
void appendList(Student_t **head, Student_t *next) {
	Student_t *current = *head;
	if (current == NULL) {
		*head = next;
		return;
	}

	while (current->next != NULL) current = current->next;
	current->next = next;
}

/**
 * Function to free the linked list.
 * Frees by all fields.
 */
void freeList(Student_t *head) {
	if (head == NULL) return;

	// Free other dynamically allocated
	freeList(head->next);
	free(head);
}

/**
 * Function to compare by last name.
 * NULL precedes non-NULL.
 */
int compareByLastName(Student_t *a, Student_t *b) {
	if (a->last_name == NULL && b->last_name != NULL) return 1;
	if (a->last_name != NULL && b->last_name == NULL) return -1;
	if (a->last_name == NULL && b->last_name == NULL) return 0;

	return strcmp(a->last_name, b->last_name);
}

/**
 * Function to compare by first name.
 * NULL precedes non-NULL.
 */
int compareByFirstName(Student_t *a, Student_t *b) {
	if (a->first_name == NULL && b->first_name != NULL) return 1;
	if (a->first_name != NULL && b->first_name == NULL) return -1;
	if (a->first_name == NULL && b->first_name == NULL) return 0;

	return strcmp(a->first_name, b->first_name);
}

/**
 * Function to compare by A number.
 * NULL precedes non-NULL.
 */
int compareByANumber(Student_t *a, Student_t *b) {
	if (a->a_number == NULL && b->a_number != NULL) return 1;
	if (a->a_number != NULL && b->a_number == NULL) return -1;
	if (a->a_number == NULL && b->a_number == NULL) return 0;
	return strcmp(a->a_number, b->a_number);
}

/**
 * Function to compare two students.
 * Compares by all fields.
 */
int compareStudents(Student_t *a, Student_t *b) {
	int result;

	// Use each compare function in the given order until a difference is found
	if ((result = compareByLastName(a, b)) != 0) return result;
	if ((result = compareByFirstName(a, b)) != 0) return result;
	if ((result = compareByANumber(a, b)) != 0) return result;
	if ((result = compareByMidterm(a, b)) != 0) return result;
	if ((result = compareByFinal(a, b)) != 0) return result;

	return 0; // a is equal to b
}

/**
 * Function to sort a linked list using merge sort.
 * Sorts by all fields.
 */
void sortList(Student_t **head) {
	if (*head == NULL || (*head)->next == NULL) return;

	Student_t *left = NULL;
	Student_t *right = NULL;

	splitList(*head, &left, &right);

	sortList(&left);
	sortList(&right);

	*head = mergeList(left, right);
}

/**
 * Function to check if valid name.
 * Valid name contains letters.
 * Checks first last name.
 */
void addName(char *name, char *type, Student_t *node) {
	char *error_message = "Error: Invalid first name.";

	// If the name does not contain letters, error.
	for (int i = 0; i < strlen(name); i++)
		if (!isalpha(name[i])) callError(error_message);
	switch (type) {
		case "first": node->first_name = strdup(name); break;
		case "last": node->last_name = strdup(name); break;
		default: callError("Error: Invalid name type.");
	}
}

/**
 * Function to check if valid A number.
 * Valid A number contains 8 digits.
 * Starts with 'A'.
 */
void addANumber(char *a_number, Student_t *node) {
	char *error_message = "Error: Invalid A number.";
	if (strlen(a_number) != 8) callError(error_message); // If the a_number is not size of 8, error
	if (a_number[0] != 'A') callError(error_message); // If the a_number does not start with 'A', error	
	for (int i = 1; i < strlen(a_number); i++) // If the a_number contains non-digits, error
		if (!isdigit(a_number[i])) callError(error_message);
	node->a_number = strdup(a_number);
	if (node->a_number == NULL) callError(error_message);
}

/**
 * Function to check if unique A number.
 */
void isUniqueANumber(char *a_number, Student_t *head) {
	Student_t *current = head;
	while (current != NULL) {
		if (strcmp(current->a_number, a_number) == 0) callError("Error: Duplicate A number.");
		current = current->next;
	}
}

/**
 * Function to check if valid grade.
 * Valid grade contains digits.
 */
void addGrade(char *grade, char *type, Student_t *node) {
	char *error_message = "Error: Invalid midterm grade.";
	char *endptr;
	long val = strtol(grade, &endptr, 10); // Convert string to long
	if (*endptr != '\0') callError(error_message); // If the midterm contains non-digits, error
	if (val < 0 || val > 100) callError(error_message); // If the midterm is not in range, error
	if (data[0] == '0' && strlen(data) > 1) callError(error_message); // If the midterm starts with '0', error
	node->midterm = strdup(midterm);
	if (node->midterm == NULL) callError(error_message);
}

/**
 * Function to add a Student to a linked list.
 */
void addStudent(Student_t **head, Student_t **current, int option) {
		// Append Student to linked list
		switch (option) {
			case 1: // Domestic
				if (strcmp((*current)->status, "D") == 0) {
					appendList(head, createNode());
					*current = (*current)->next;
				} else {
					freeList(*current);
					*current = createNode();
				}
				break;
			case 2: // International
				if (strcmp((*current)->status, "I") == 0) { 
					appendList(head, createNode());
					*current = (*current)->next;
				} else {
					freeList(*current);
					*current = createNode();
				}
				break;
			case 3: // All
				appendList(head, createNode());
				*current = (*current)->next;
				break;
			default: callError("Error: Invalid option.");
		}

}

/**
 * Function to process word into Student struct.
 */
void processWord(char *word, Student_t *current, int word_count) {
	switch (word_count) {
		case 1: addFirstName(word, current); break;
		case 2: addLastName(word, current); break;
		case 3: addANumber(word, current); break;
		case 4: addMidterm(word, current); break;
		case 5: addFinal(word, current); break;
		default: callError("Error: Incorrect input format.");
	}
}

/**
 * Function to read text from input file. 
 */ 
void readFile(FILE *input, Student_t *head, const int option) {
	if (input == NULL) callError("Error: Could not read file."); // Error handle reading file

	Student_t *current = head;
	int size = 20;
	char *buffer = (char *) malloc(sizeof(char) * size);
	if (buffer == NULL) callError("Error: Memory could not be allocated.");

	char c;
	char last_char;
	char *word = buffer; // Pointer to buffer
	int characters = 0;
	int word_count = 0;
	int word_length = 0;
	int space_count = 0;
	bool in_word = false;

	while ((c = fgetc(input)) != EOF) {
		if (ferror(input)) { // Error handle reading file
			free(buffer);
			fclose(input);
			callError("Error: Could not read file.");
		}
		if (space_count > 1) { // Error handle consecutive spaces
			free(buffer);
			fclose(input);
			callError("Error: Consecutive spaces is invalid format.");
		}
		if (word_count > 6) { // Error handle too many words
			free(buffer);
			fclose(input);
			callError("Error: Too many fields."); 
		}
		if (word_length >= (size - 1)) { // Reallocate memory if word is too long
			size *= 2;
			char *temp = (char *) realloc(buffer, sizeof(char) * size);
			if (temp == NULL) {
				free(buffer);
				fclose(input);
				callError("Error: Memory could not be allocated.");
			}
			buffer = temp;
			word = buffer + word_length; // Continue building string from last char
		}

		if (!isspace(c)) {
			if (!in_word) { // Start of word 
				word_count++;
				space_count = 0;
				in_word = true;
			}
			*word++ = c;
			word_length++;
		} else if (isspace(c)) {
			if (c != '\n' && word_count == 0) 
				callError("Error: Leading spaces is invalid format."); // Error handle leading spaces
		
			if (in_word) { // End of word
				*word = '\0';
				processWord(buffer, current, word_count); // Process word	
				word = buffer; // Reset word
				memset(buffer, 0, 20); // Reset buffer
				word_length = 0;
				in_word = false;
			}
			space_count++;
		}
		// Reset word count if end of line
		if (c == '\n') {
			// Error handle empty line
			// Only last line can be empty
			if (word_count == 0) {
				last_char = (char) c;
				char next_char = fgetc(input); // Peek next character
				if (next_char == EOF && characters != 0) break;
				else callError("Error: Empty line is invalid format.");
			}
			
			// Error handle trailing spaces
			if (space_count > 1) callError("Error: Trailing spaces is invalid format.");
			
			// Reset counts for next line
			word_count = 0;
			space_count = 0;
		
			// Append Student to linked list
			addStudent(&head, &current, option);
		}
		characters++;
	} // End of while loop
	free(buffer);
	if (last_char != '\n') callError("Error: Last line is invalid format.");
}

/**
 * Function to write text to output file.
 * Writes by all fields.
 */
void writeFile(FILE *output, Student_t *head) {
	Student_t *current = head;
	while (current != NULL) {
		if (current->first_name != NULL) fprintf(output, "%s ", current->first_name);
		if (current->last_name != NULL) fprintf(output, "%s ", current->last_name);
		if (current->a_number != NULL) fprintf(output, "%s ", current->a_number);
		if (current->midterm != NULL) fprintf(output, "%s ", current->midterm);
		if (current->final != NULL) fprintf(output, "%s ", current->final);
		if (current->next != NULL) fprintf(output, "\n");
		current = current->next;
	}
	// Output file must end with a new line
	fprintf(output, "\n");

	// Close the output file
	fclose(output);

	printf("Successfully wrote to output file.\n");
	printf("\n");
}

/**
 * Driver program.
 *
 * Usage:
 * 		./<name of executable> <input file> <output file> <option>
 *
 * Options as follows:
 * 		[1] Allow for sorting by just domestic students.
 * 		[2] Allow for sorting by just international students.
 * 		[3] Allow for sorting by all students.
 *
 * Example input: 
 * 		"Mary Jackson A01234567 100 100"
 */
int main(int argc, char *argv[]) {
	char *ANum = "A01351112";
	FILE *outputFile = fopen(ANum, "w");
	if (outputFile == NULL) {
		printf("Error: Failed to create output file.\n");
		return 1;
	}

	fclose(outputFile);

	if (argc != 4) {
		printf("Usage %s <input_file> <output_file> <option>\n", argv[0]);
		callError("Error: Invalid number of arguments.");
	}

	const char *input_name = argv[1]; // Input file name
	const char *output_name = argv[2]; // Output file name
	error_output = output_name; // Set global error output
	
	FILE *file = fopen(input_name, "r");
	if (file == NULL) callError("Error: Input file not found.");
	fseek(file, 0, SEEK_SET); // Ensure cursor at start of file

	const int option = atoi(argv[3]);
	if (option < 1 || option > 3) {
		printf("Usage %s <input_file> <output_file> <option>\n", argv[0]);
		callError("Error: Invalid option.");
	}
	Student_t *head = createNode();
	readFile(file, head, option);
	sortList(&head);
	fclose(file);

	file = fopen(output_name, "w");
	if (file == NULL) {
		callError("Error: Output file could not open.");
	}
	writeFile(file, head);

	return 0;
}
