/*
 * ============================================
 *   Student Management System
 *   CodeAlpha C Programming Internship
 *   Task 3: Student Management System
 * ============================================
 *   Features:
 *   1. Add Student Record
 *   2. Display All Records
 *   3. Search Student (by Roll Number)
 *   4. Update Student Record
 *   5. Delete Student Record
 *   6. Exit
 *
 *   Uses: Structures + File Handling
 * ============================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Constants ---- */
#define FILE_NAME "students.dat"
#define MAX_NAME  50
#define MAX_COURSE 30

/* ============================================
   STRUCTURE: Student
   ============================================ */
typedef struct {
    int    rollNo;
    char   name[MAX_NAME];
    char   course[MAX_COURSE];
    int    age;
    float  marks;
    char   grade;
} Student;

/* ---- Function Prototypes ---- */
void        addStudent();
void        displayAll();
void        searchStudent();
void        updateStudent();
void        deleteStudent();
char        calculateGrade(float marks);
void        printSeparator();
void        printHeader();
int         recordExists(int rollNo);

/* ============================================
   MAIN FUNCTION
   ============================================ */
int main() {
    int choice;
    int running = 1;

    printHeader();

    while (running) {
        printf("\n╔══════════════════════════════════════════╗\n");
        printf("║           STUDENT MANAGEMENT             ║\n");
        printf("╠══════════════════════════════════════════╣\n");
        printf("║  1. Add Student Record                   ║\n");
        printf("║  2. Display All Records                  ║\n");
        printf("║  3. Search Student (by Roll No)          ║\n");
        printf("║  4. Update Student Record                ║\n");
        printf("║  5. Delete Student Record                ║\n");
        printf("║  6. Exit                                 ║\n");
        printf("╚══════════════════════════════════════════╝\n");
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent();    break;
            case 2: displayAll();    break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("\n  ✅ Thank you for using Student Management System!\n");
                printf("  All records saved successfully. Goodbye!\n\n");
                running = 0;
                break;
            default:
                printf("\n  ❌ Invalid choice! Please select between 1 and 6.\n");
        }
    }

    return 0;
}

/* ============================================
   FUNCTION: Print Welcome Header
   ============================================ */
void printHeader() {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║      STUDENT MANAGEMENT SYSTEM           ║\n");
    printf("║        CodeAlpha Internship              ║\n");
    printf("║  Data is saved permanently to file!      ║\n");
    printf("╚══════════════════════════════════════════╝\n");
}

/* ============================================
   FUNCTION: Print Separator
   ============================================ */
void printSeparator() {
    printf("\n  ══════════════════════════════════════════\n");
}

/* ============================================
   FUNCTION: Calculate Grade from Marks
   ============================================ */
char calculateGrade(float marks) {
    if (marks >= 90) return 'A';
    else if (marks >= 75) return 'B';
    else if (marks >= 60) return 'C';
    else if (marks >= 45) return 'D';
    else return 'F';
}

/* ============================================
   FUNCTION: Check if Roll Number Already Exists
   ============================================ */
int recordExists(int rollNo) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return 0;

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.rollNo == rollNo) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ============================================
   FUNCTION: Add a New Student Record
   ============================================ */
void addStudent() {
    printSeparator();
    printf("  [ ADD STUDENT RECORD ]\n");
    printSeparator();

    Student s;

    printf("\n  Enter Roll Number : ");
    scanf("%d", &s.rollNo);

    // Check for duplicate roll number
    if (recordExists(s.rollNo)) {
        printf("\n  ❌ Roll Number %d already exists! Use Update to modify.\n", s.rollNo);
        return;
    }

    printf("  Enter Student Name: ");
    scanf(" %[^\n]", s.name);  // reads full name with spaces

    printf("  Enter Course      : ");
    scanf(" %[^\n]", s.course);

    printf("  Enter Age         : ");
    scanf("%d", &s.age);

    printf("  Enter Marks (0-100): ");
    scanf("%f", &s.marks);

    // Validate marks
    if (s.marks < 0 || s.marks > 100) {
        printf("\n  ❌ Invalid marks! Please enter a value between 0 and 100.\n");
        return;
    }

    s.grade = calculateGrade(s.marks);

    // Open file in append+binary mode
    FILE *fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf("\n  ❌ Error opening file! Cannot save record.\n");
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\n  ✅ Student record added successfully!\n");
    printf("  Grade assigned: %c\n", s.grade);
}

/* ============================================
   FUNCTION: Display All Student Records
   ============================================ */
void displayAll() {
    printSeparator();
    printf("  [ ALL STUDENT RECORDS ]\n");
    printSeparator();

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("\n  ℹ️  No records found. Please add students first.\n");
        return;
    }

    Student s;
    int count = 0;

    printf("\n  %-6s %-20s %-15s %-5s %-8s %-6s\n",
           "Roll", "Name", "Course", "Age", "Marks", "Grade");
    printf("  %-6s %-20s %-15s %-5s %-8s %-6s\n",
           "------", "--------------------", "---------------", "-----", "--------", "------");

    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("  %-6d %-20s %-15s %-5d %-8.2f %-6c\n",
               s.rollNo, s.name, s.course, s.age, s.marks, s.grade);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("\n  ℹ️  No records found.\n");
    else
        printf("\n  Total Records: %d\n", count);
}

/* ============================================
   FUNCTION: Search Student by Roll Number
   ============================================ */
void searchStudent() {
    printSeparator();
    printf("  [ SEARCH STUDENT ]\n");
    printSeparator();

    int rollNo;
    printf("\n  Enter Roll Number to search: ");
    scanf("%d", &rollNo);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("\n  ℹ️  No records found. Please add students first.\n");
        return;
    }

    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.rollNo == rollNo) {
            found = 1;
            printf("\n  ✅ Student Found!\n");
            printf("  ─────────────────────────────────────\n");
            printf("  Roll Number : %d\n",   s.rollNo);
            printf("  Name        : %s\n",   s.name);
            printf("  Course      : %s\n",   s.course);
            printf("  Age         : %d\n",   s.age);
            printf("  Marks       : %.2f\n", s.marks);
            printf("  Grade       : %c\n",   s.grade);
            printf("  ─────────────────────────────────────\n");
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\n  ❌ Student with Roll Number %d not found!\n", rollNo);
}

/* ============================================
   FUNCTION: Update Student Record
   ============================================ */
void updateStudent() {
    printSeparator();
    printf("  [ UPDATE STUDENT RECORD ]\n");
    printSeparator();

    int rollNo;
    printf("\n  Enter Roll Number to update: ");
    scanf("%d", &rollNo);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("\n  ℹ️  No records found.\n");
        return;
    }

    // Read all records into a temporary array
    Student records[100];
    int count = 0;
    int found = 0;

    while (fread(&records[count], sizeof(Student), 1, fp)) {
        count++;
    }
    fclose(fp);

    // Find and update the record
    for (int i = 0; i < count; i++) {
        if (records[i].rollNo == rollNo) {
            found = 1;
            printf("\n  Current Details:\n");
            printf("  Name   : %s\n", records[i].name);
            printf("  Course : %s\n", records[i].course);
            printf("  Age    : %d\n", records[i].age);
            printf("  Marks  : %.2f\n", records[i].marks);

            printf("\n  Enter New Name   : ");
            scanf(" %[^\n]", records[i].name);

            printf("  Enter New Course : ");
            scanf(" %[^\n]", records[i].course);

            printf("  Enter New Age    : ");
            scanf("%d", &records[i].age);

            printf("  Enter New Marks  : ");
            scanf("%f", &records[i].marks);

            if (records[i].marks < 0 || records[i].marks > 100) {
                printf("\n  ❌ Invalid marks! Update cancelled.\n");
                return;
            }

            records[i].grade = calculateGrade(records[i].marks);
            printf("\n  ✅ Record updated! New Grade: %c\n", records[i].grade);
            break;
        }
    }

    if (!found) {
        printf("\n  ❌ Student with Roll Number %d not found!\n", rollNo);
        return;
    }

    // Write all records back to file
    FILE *fw = fopen(FILE_NAME, "wb");
    if (fw == NULL) {
        printf("\n  ❌ Error saving updated records!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fwrite(&records[i], sizeof(Student), 1, fw);
    }
    fclose(fw);
}

/* ============================================
   FUNCTION: Delete Student Record
   ============================================ */
void deleteStudent() {
    printSeparator();
    printf("  [ DELETE STUDENT RECORD ]\n");
    printSeparator();

    int rollNo;
    printf("\n  Enter Roll Number to delete: ");
    scanf("%d", &rollNo);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("\n  ℹ️  No records found.\n");
        return;
    }

    Student records[100];
    int count = 0;
    int found = 0;

    while (fread(&records[count], sizeof(Student), 1, fp)) {
        count++;
    }
    fclose(fp);

    // Find the record
    for (int i = 0; i < count; i++) {
        if (records[i].rollNo == rollNo) {
            found = 1;
            printf("\n  Found: %s (Roll No: %d)\n", records[i].name, records[i].rollNo);

            // Confirm before deleting
            char confirm;
            printf("  Are you sure you want to delete? (y/n): ");
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y') {
                // Shift remaining records
                for (int j = i; j < count - 1; j++) {
                    records[j] = records[j + 1];
                }
                count--;
                printf("\n  ✅ Student record deleted successfully!\n");
            } else {
                printf("\n  ℹ️  Deletion cancelled.\n");
                return;
            }
            break;
        }
    }

    if (!found) {
        printf("\n  ❌ Student with Roll Number %d not found!\n", rollNo);
        return;
    }

    // Write updated records back to file
    FILE *fw = fopen(FILE_NAME, "wb");
    if (fw == NULL) {
        printf("\n  ❌ Error saving records after deletion!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fwrite(&records[i], sizeof(Student), 1, fw);
    }
    fclose(fw);
}
