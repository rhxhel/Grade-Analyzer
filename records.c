#include "records.h" // Includes the prototypes and data structures
#include <string.h>  // For strcpy and strcmp
#include <stdlib.h>  // For malloc and free
#include <stdio.h>   // For printf (if needed for debugging/logging within functions)

// --- PRIVATE GLOBAL STATE (Encapsulated using static) ---

// Global variables - HIDDEN from main.c
static Student *head = NULL; // Head of the linked list storing student records
static Stack undoStack;      // Stack to store deleted students for undo functionality

// ===============================
// FUNCTION IMPLEMENTATIONS
// ===============================

// --------------------
// STACK FUNCTIONS
// --------------------

// CASE: Utility function for stack initialization
// ALGORITHM: Array-based stack (for preparing the stack for storing deleted students so that the “undo delete” feature works)
void initStack() {
    undoStack.top = -1; // Reset top index to -1 indicating an empty stack
}

// CASE: Check if stack is empty
// ALGORITHM: Array-based stack (for preventing popping from an empty stack, avoiding runtime errors)
int isEmpty() {
    return undoStack.top == -1; // Returns 1 if stack is empty, 0 otherwise
}

// CASE: Push a student onto the stack
// ALGORITHM: Array-based stack (LIFO - Last in, First out), (the last deleted student will be the first one retrieved for undo),
// (when deleting a student, their record is “pushed” onto the stack so it can be restored later) 
void push(Student s) {
    if (undoStack.top < MAX - 1) // Prevent overflow
        undoStack.data[++undoStack.top] = s; // Increment top and store student
}

// CASE: Pop a student from the stack
// ALGORITHM: Array-based stack (LIFO - Last in, First out), (the last student pushed is the first one popped),
// (restores the most recently deleted student when the user selects “Undo Delete”)
Student pop() {
    Student empty = {0, "", 0, NULL}; // Default empty student if stack is empty
    if (isEmpty()) return empty;      // Return empty if stack is empty
    return undoStack.data[undoStack.top--]; // Return top element and decrement top
}

// --------------------
// STUDENT MANAGEMENT (For Main Program)
// --------------------

// CASE: 1 - Add a new student
// DATA STRUCTURE: Linked List (insert at head), (for adding student record - ID, name, and grade)
void addStudent(int id, char name[], float grade) {
    // Check for duplicate ID in the linked list
    Student *temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Error: Student ID %d already exists. Cannot add duplicate.\n", id);
            return;
        }
        temp = temp->next;
    }

    // Allocate memory for the new student
    Student *newS = (Student *)malloc(sizeof(Student));
    if (!newS) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Assign values and insert at head
    newS->id = id;
    strcpy(newS->name, name);
    newS->grade = grade;
    newS->next = head;
    head = newS;

    printf("Student added successfully!\n");
}

// CASE: Check if a student ID already exists
// RETURNS: 1 if student exists, 0 otherwise
int studentExists(int id) {
    Student *temp = head;
    while (temp) {
        if (temp->id == id) return 1; // Found a student with matching ID
        temp = temp->next;
    }
    return 0; // ID not found
}

// CASE: 2 - Delete a student by ID
// DATA STRUCTURE: Linked List deletion + Stack push for undo (for deleting student record)
void deleteStudent(int id) {
    Student *temp = head, *prev = NULL;

    // Traverse the list to find the student
    while (temp && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) { // Student not found
        printf("Student not found.\n");
        return;
    }

    // Print student before deleting
    printf("\nStudent Deleted:\n");
    printf("%-10s %-20s %-10s\n", "ID", "Name", "Grade");
    printf("----------------------------------------\n");
    printf("%-10d %-20s %-10.2f\n", temp->id, temp->name, temp->grade);

    // Remove student from linked list
    if (prev) prev->next = temp->next; // Middle or tail
    else head = temp->next;            // Head node

    // Push student to undo stack
    push(*temp);
    free(temp);

    printf("\n(Undo available)\n");
}

// CASE: 3 - Undo the last deletion
// ALGORITHM: Stack pop + Linked List insertion (for restoring deleted record)
void undoDelete() {
    if (isEmpty()) { // Check if undo stack is empty
        printf("Undo stack empty.\n");
        return;
    }

    // Get last deleted student from stack
    Student s = pop();

    // Check if same ID already exists in the current list
    Student *temp = head;
    while (temp != NULL) {
        if (temp->id == s.id) {
            printf("Cannot undo! A student with ID %d already exists.\n", s.id);
            return;  // Prevent duplicate on undo
        }
        temp = temp->next;
    }

    // Restore student to the linked list (insert at head)
    Student *newS = malloc(sizeof(Student));
    if (!newS) {
        printf("Memory allocation failed!\n");
        return;
    }

    newS->id = s.id;
    strcpy(newS->name, s.name);
    newS->grade = s.grade;
    newS->next = head;
    head = newS;

    // Print restored student
    printf("Undo successful! Student restored:\n");
    printf("%-10s %-20s %-10s\n", "ID", "Name", "Grade");
    printf("----------------------------------------\n");
    printf("%-10d %-20s %-10.2f\n", newS->id, newS->name, newS->grade);
}

// CASE: 4 - Sort students by grade, ID, or name (ascending/descending)
// ALGORITHM: Insertion Sort (convert linked list to array for sorting), (for sorting student records)
void sortStudentsInsertion(int ascending, SortType type) {
    if (!head || !head->next) return; // Nothing to sort if 0 or 1 student

    Student *arr[MAX]; // Temporary array for sorting
    int n = 0;

    // Copy linked list pointers to array
    for (Student *t = head; t != NULL; t = t->next)
        arr[n++] = t;

    // Insertion sort on array
    for (int i = 1; i < n; i++) {
        Student *key = arr[i];
        int j = i - 1;

        if (type == SORT_GRADE) {
            while (j >= 0 && ((ascending && arr[j]->grade > key->grade) ||
                              (!ascending && arr[j]->grade < key->grade))) {
                arr[j + 1] = arr[j];
                j--;
            }
        }
        else if (type == SORT_ID) {
            while (j >= 0 && ((ascending && arr[j]->id > key->id) ||
                              (!ascending && arr[j]->id < key->id))) {
                arr[j + 1] = arr[j];
                j--;
            }
        }
        else if (type == SORT_NAME) {
            while (j >= 0 && ((ascending && strcmp(arr[j]->name, key->name) > 0) ||
                              (!ascending && strcmp(arr[j]->name, key->name) < 0))) {
                arr[j + 1] = arr[j];
                j--;
            }
        }

        arr[j + 1] = key;
    }

    // Reconstruct linked list from sorted array
    head = arr[0];
    Student *cur = head;
    for (int i = 1; i < n; i++) {
        cur->next = arr[i];
        cur = cur->next;
    }
    cur->next = NULL;

    printf("Student Record sorted successfully!\n");
}

// CASE: 5 - Convert linked list to array for searching
// DATA STRUCTURE: Linked List traversal (for displaying records)
int listToArray(Student arr[]) {
    Student *t = head;
    int count = 0;

    // Copy data from linked list to array
    while (t) {
        arr[count++] = *t;
        t = t->next;
    }
    return count; // Return number of students
}

// CASE: 5 - Linear search a student by ID
// ALGORITHM: Linear Search (on array), (for searching student record)
void linearSearch(Student arr[], int n, int id) {
    int found = 0;

    // Iterate through array to find matching ID
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id) {
            printf("\n%-10s %-20s %-10s\n", "ID", "Name", "Grade");
            printf("----------------------------------------\n");
            printf("%-10d %-20s %-10.2f\n", arr[i].id, arr[i].name, arr[i].grade);
            found = 1;
        }
    }

    if (!found) // No matching student found
        printf("ID not found.\n");
}

// CASE: 6 - Display all student records
// DATA STRUCTURE: Linked List traversal (for displaying records)
void displayStudents() {
    Student *temp = head;

    if (!temp) { // No students in list
        printf("\nNo students found.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s\n", "ID", "Name", "Grade");
    printf("----------------------------------------\n");

    // Traverse and print all students
    while (temp) {
        printf("%-10d %-20s %-10.2f\n", temp->id, temp->name, temp->grade);
        temp = temp->next;
    }
}
