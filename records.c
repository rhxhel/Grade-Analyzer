#include "records.h" // Includes the prototypes and data structures
#include <string.h>  // For strcpy and strcmp
#include <stdlib.h>  // For malloc and free
#include <stdio.h>   // For printf (if needed for debugging/logging within functions)

// --- PRIVATE GLOBAL STATE (Encapsulated using static) ---

// Global variables - HIDDEN from main.c
static Student *head = NULL; // head of linked list
static Stack undoStack;      // undo stack

// ===============================
// FUNCTION IMPLEMENTATIONS
// ===============================

// --------------------
// STACK FUNCTIONS
// --------------------

// CASE: Utility function for stack initialization
// ALGORITHM: Array-based stack (for preparing the stack for storing deleted students so that the “undo delete” feature works)
void initStack() {
    undoStack.top = -1;
}

// CASE: Check if stack is empty
// ALGORITHM: Array-based stack (for preventing popping from an empty stack, avoiding runtime errors)
int isEmpty() {
    return undoStack.top == -1;
}

// CASE: Push a student onto the stack
// ALGORITHM: Array-based stack (LIFO - Last in, First out), (the last deleted student will be the first one retrieved for undo),
//(when deleting a student, their record is “pushed” onto the stack so it can be restored later) 
void push(Student s) {
    if (undoStack.top < MAX - 1)
        undoStack.data[++undoStack.top] = s;
}

// CASE: Pop a student from the stack
// ALGORITHM: Array-based stack (LIFO - Last in, First out), (the last student pushed is the first one popped),
//(restores the most recently deleted student when the user selects “Undo Delete”)
Student pop() {
    Student empty = {0, "", 0, NULL};
    if (isEmpty()) return empty;
    return undoStack.data[undoStack.top--];
}

// --------------------
// STUDENT MANAGEMENT (For Main Program)
// --------------------

// CASE: 1 - Add a new student
// DATA STRUCTURE: Linked List (insert at head), (for adding student record - ID, name, and grade)
void addStudent(int id, char name[], float grade) {
    Student *temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Error: Student ID %d already exists. Cannot add duplicate.\n", id);
            return;
        }
        temp = temp->next;
    }

    Student *newS = (Student *)malloc(sizeof(Student));
    if (!newS) {
        printf("Memory allocation failed!\n");
        return;
    }

    newS->id = id;
    strcpy(newS->name, name);
    newS->grade = grade;
    newS->next = head; // insert at head
    head = newS;

    printf("Student added successfully!\n");
}

// CASE: 2 - Delete a student by ID
// DATA STRUCTURE: Linked List deletion + Stack push for undo (for deleting student record)
void deleteStudent(int id) {
    Student *temp = head, *prev = NULL;

    while (temp && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Student not found.\n");
        return;
    }

    if (prev) prev->next = temp->next;
    else head = temp->next;

    push(*temp); // store deleted node in stack
    free(temp);

    printf("Student deleted. (Undo available)\n");
}

// CASE: 3 - Undo the last deletion
// ALGORITHM: Stack pop + Linked List insertion (for restoring deleted record)
void undoDelete() {
    if (isEmpty()) {
        printf("Undo stack empty.\n");
        return;
    }

    Student s = pop();
    // This calls the addStudent function, which is also in this file.
    addStudent(s.id, s.name, s.grade);
}

// CASE: 4 - Sort students by grade, ID, or name (ascending/descending)
// ALGORITHM: Insertion Sort (convert linked list to array for sorting), (for sorting student records)
void sortStudentsInsertion(int ascending, SortType type) {
    if (!head || !head->next) return;

    Student *arr[MAX]; // array for insertion sort
    int n = 0;
    for (Student *t = head; t != NULL; t = t->next)
        arr[n++] = t;

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
    while (t) {
        arr[count++] = *t;
        t = t->next;
    }
    return count;
}

// CASE: 5 - Linear search a student by ID
// ALGORITHM: Linear Search (on array), (for searching student record)
void linearSearch(Student arr[], int n, int id) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id) {
            printf("\n%-10s %-20s %-10s\n", "ID", "Name", "Grade");
            printf("----------------------------------------\n");
            printf("%-10d %-20s %-10.2f\n", arr[i].id, arr[i].name, arr[i].grade);
            found = 1;
        }
    }

    if (!found)
        printf("ID not found.\n");
}

// CASE: 6 - Display all student records
// DATA STRUCTURE: Linked List traversal (for displaying records)
void displayStudents() {
    Student *temp = head;
    if (!temp) {
        printf("\nNo students found.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s\n", "ID", "Name", "Grade");
    printf("----------------------------------------\n");

    while (temp) {
        printf("%-10d %-20s %-10.2f\n", temp->id, temp->name, temp->grade);
        temp = temp->next;
    }
}