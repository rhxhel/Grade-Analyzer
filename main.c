#include "records.h" // Accesses the modular functions and structures
#include <stdio.h>
#include <stdlib.h> // For system("cls")

// ===============================
// MAIN PROGRAM
// ===============================
int main() {
    initStack(); // Initialize the undo stack (prepares the stack for storing deleted students)
    int choice;

    do {
        system("cls"); // Clear the console for a clean menu display

        // Display the main menu
        printf("===== Student Grade Record Analyzer =====\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Undo Delete\n");
        printf("4. Sort List\n");
        printf("5. Search Student Record\n");
        printf("6. Display Student Records\n");
        printf("0. Exit\n");

        // ===============================
        // Input validation for menu choice
        // ===============================
        int valid = 0;
        do {
            printf("Choose: ");

            if (scanf("%d", &choice) != 1) {
                printf("Invalid input! Please enter a number from 0 to 6.\n");
                while (getchar() != '\n'); // clear input buffer
            }
            else if (choice < 0 || choice > 6) {
                printf("Invalid choice! Please enter only options 0 to 6.\n");
            }
            else {
                valid = 1; // valid input received
            }
        } while (!valid);

        system("cls"); // Clear the console after choice selection

        // ===============================
        // CASE 1: Add Student
        // ===============================
        if (choice == 1) {
            int id;
            char name[50];
            float grade;

            printf("============= Add Student =============\n\n");   

            // Loop to ensure a valid and unique student ID is entered
            do {
                printf("Enter ID: ");
                while (scanf("%d", &id) != 1) { // Validate numeric input
                    printf("Invalid input! ID must be a number. \nEnter ID: ");
                    while (getchar() != '\n'); // clear invalid input
                }

                if (studentExists(id)) { // Check if ID already exists in the linked list
                    printf("Error: Student ID %d already exists! Please enter a different ID.\n", id);
                }
            } while (studentExists(id)); // Repeat until a unique ID is entered

            // Input student name (allows spaces)
            printf("Enter Name: ");
            scanf(" %[^\n]", name);

            // Input student grade with validation
            printf("Enter Grade: ");
            while (scanf("%f", &grade) != 1) {
                printf("Invalid input! Grade must be a number. \nEnter Grade: ");
                while (getchar() != '\n'); // clear invalid input
            }

            // Add the new student to the linked list
            addStudent(id, name, grade);

            printf("\nPress Enter to return to menu...");
            getchar(); getchar(); // Pause before returning to menu
        }

        // ===============================
        // CASE 2: Delete Student
        // ===============================
        else if (choice == 2) {
            int id;
            printf("============ Delete Student ============\n\n");   
            printf("Enter ID to delete: ");
            scanf("%d", &id);

            // Remove the student with the entered ID from the linked list
            // The deleted student is also pushed onto the undo stack for possible restoration
            deleteStudent(id);

            printf("\nPress Enter to return to menu...");
            getchar(); getchar();
        }

        // ===============================
        // CASE 3: Undo Delete
        // ===============================
        else if (choice == 3) {
            printf("============= Undo Delete =============\n\n");   

            // Restore the most recently deleted student from the undo stack
            // If a student with the same ID already exists, restoration is skipped
            undoDelete();

            printf("\nPress Enter to return to menu...");
            getchar(); getchar();
        }

        // ===============================
        // CASE 4: Sort List
        // ===============================
        else if (choice == 4) {
            int asc, opt;
            printf("============== Sort List ==============\n");   
            printf("\nSort by:\n");
            printf("1. ID\n");
            printf("2. Name\n");
            printf("3. Grade\n");
            
            // Validate sort criterion selection
            int validOpt = 0;
            do {
                printf("Choose: ");
                if (scanf("%d", &opt) != 1) {
                    printf("Invalid input! Please enter a number 1 to 3.\n");
                    while (getchar() != '\n');
                } else if (opt < 1 || opt > 3) {
                    printf("Invalid choice! Please enter only options 1 to 3.\n");
                } else validOpt = 1;
            } while (!validOpt);

            // Ascending or Descending selection
            printf("\nSort Type:");
            printf("\n1. Ascending");
            printf("\n2. Descending\n");
            
            int validAsc = 0;
            do {
                printf("Choose: ");
                if (scanf("%d", &asc) != 1) {
                    printf("Invalid input! Please enter 1 or 2.\n");
                    while (getchar() != '\n');
                } else if (asc != 1 && asc != 2) {
                    printf("Invalid choice! Enter only 1 for Ascending or 2 for Descending.\n");
                } else validAsc = 1;
            } while (!validAsc);

            asc = (asc == 1); // Convert choice to boolean flag

            // Perform sorting of the linked list based on user selection
            if (opt == 1) sortStudentsInsertion(asc, SORT_ID);       // Sort by ID
            else if (opt == 2) sortStudentsInsertion(asc, SORT_NAME); // Sort by Name
            else if (opt == 3) sortStudentsInsertion(asc, SORT_GRADE);// Sort by Grade

            printf("\nPress Enter to return to menu...");            
            getchar(); getchar();
        }

        // ===============================
        // CASE 5: Search Student Record
        // ===============================
        else if (choice == 5) {
            Student arr[MAX];
            int n = listToArray(arr); // Copy linked list to array for searching

            if (n == 0) { // No students in the system
                printf("========= Search Student Record =========\n\n");      
                printf("No students available.\n");
                printf("\nPress Enter to return...");
                getchar(); getchar();
                continue;
            }

            int searchID;
            printf("========= Search Student Record =========\n\n");                  
            printf("Enter ID to search: ");
            scanf("%d", &searchID);

            // Search for the student in the array and display if found
            linearSearch(arr, n, searchID); 
            printf("\nPress Enter to return...");
            getchar(); getchar();
        }

        // ===============================
        // CASE 6: Display All Student Records
        // ===============================
        else if (choice == 6) {
            printf("============ Student Record ============\n");            

            // Traverse the linked list and print all student records
            displayStudents();        
            printf("\nPress Enter to return...");
            getchar(); getchar();
        }

    } while (choice != 0); // Exit the loop when the user chooses 0

    // ===============================
    // Exit Message
    // ===============================
    printf("================= Exit =================\n");       
    printf("\nExiting Student Grade Record Analyzer...\n");
    printf("\nGoodbye!\n\n");
    return 0;
}
