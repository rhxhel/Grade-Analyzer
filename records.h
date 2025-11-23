#ifndef RECORDS_H
#define RECORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h> // Added for boolean types if needed, though your code uses int for bools

#define MAX 100

// ===============================
// DATA STRUCTURE DEFINITIONS
// ===============================

// --------------------
// Linked List Node (for general student record storage)
// --------------------
typedef struct Student {
    int id;
    char name[50];
    float grade;
    struct Student *next; // pointer to next node
} Student;

// --------------------
// Stack 
// --------------------
typedef struct Stack {
    Student data[MAX];
    int top;
} Stack;

// Enum for sorting type
typedef enum {
    SORT_GRADE,
    SORT_ID,
    SORT_NAME
} SortType;


// ===============================
// FUNCTION PROTOTYPES
// ===============================

// --------------------
// STACK UTILITY (array-based stack)
// --------------------
void initStack();
int isEmpty();
int studentExists(int id);
void push(Student s); // push onto stack
Student pop();        // pop from stack

// --------------------
// STUDENT MANAGEMENT
// --------------------
// linked list insertion (for adding a new student record to the system.)
void addStudent(int id, char name[], float grade);

// linked list deletion + push to stack (for removing a student record by ID.)
void deleteStudent(int id);

// stack pop + linked list insertion (for restoring the most recently deleted student)
void undoDelete();

// insertion sort using array (for sorting student records by id, name, or grade)
void sortStudentsInsertion(int ascending, SortType type);

// linked list to array conversion (for copying linked list into an array)
int listToArray(Student arr[]);

// linear search on array (for searching student record by ID)
void linearSearch(Student arr[], int n, int id);

// traverse linked list to display (for displaying all student records)
void displayStudents();

#endif // RECORDS_H
