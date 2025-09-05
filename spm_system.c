#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100   // maximum students
#define SUBJECTS 3

typedef struct {
    int roll_no;
    char name[50];
    int marks[SUBJECTS];
} Student;

Student students[MAX];
int count = 0;

// ---------- Function Prototypes ----------
void addStudent();
void displayStudents();
int searchStudent(int roll_no);
void updateStudent(int roll_no);
void deleteStudent(int roll_no);
void computeStatistics();
void saveToFile();
void loadFromFile();

// ---------- Main Function ----------
int main() {
    loadFromFile();
    int choice, roll;

    do {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Class Statistics\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: 
                printf("Enter roll number to search: ");
                scanf("%d", &roll);
                int idx = searchStudent(roll);
                if (idx != -1) {
                    printf("Found: %s | Roll: %d\n", students[idx].name, students[idx].roll_no);
                } else {
                    printf("Student not found!\n");
                }
                break;
            case 4:
                printf("Enter roll number to update: ");
                scanf("%d", &roll);
                updateStudent(roll);
                break;
            case 5:
                printf("Enter roll number to delete: ");
                scanf("%d", &roll);
                deleteStudent(roll);
                break;
            case 6: computeStatistics(); break;
            case 7: 
                saveToFile();
                printf("Data saved. Exiting...\n");
                break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 7);

    return 0;
}

// ---------- Function Definitions ----------
void addStudent() {
    if(count >= MAX) {
        printf("Storage full!\n");
        return;
    }
    Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll_no);
    printf("Enter name: ");
    getchar(); // to clear newline
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0; // remove newline

    for(int i = 0; i < SUBJECTS; i++) {
        printf("Enter marks for subject %d: ", i+1);
        scanf("%d", &s.marks[i]);
    }
    students[count++] = s;
    printf("Student added successfully!\n");
}

void displayStudents() {
    if(count == 0) {
        printf("No records found.\n");
        return;
    }
    printf("\n%-10s %-20s %-10s %-10s %-10s\n", "RollNo", "Name", "Sub1", "Sub2", "Sub3");
    for(int i = 0; i < count; i++) {
        printf("%-10d %-20s %-10d %-10d %-10d\n", students[i].roll_no, students[i].name,
               students[i].marks[0], students[i].marks[1], students[i].marks[2]);
    }
}

int searchStudent(int roll_no) {
    for(int i = 0; i < count; i++) {
        if(students[i].roll_no == roll_no)
            return i;
    }
    return -1;
}

void updateStudent(int roll_no) {
    int idx = searchStudent(roll_no);
    if(idx == -1) {
        printf("Student not found!\n");
        return;
    }
    printf("Updating record for %s\n", students[idx].name);
    for(int i = 0; i < SUBJECTS; i++) {
        printf("Enter new marks for subject %d: ", i+1);
        scanf("%d", &students[idx].marks[i]);
    }
    printf("Record updated!\n");
}

void deleteStudent(int roll_no) {
    int idx = searchStudent(roll_no);
    if(idx == -1) {
        printf("Student not found!\n");
        return;
    }
    for(int i = idx; i < count-1; i++) {
        students[i] = students[i+1];
    }
    count--;
    printf("Record deleted!\n");
}

void computeStatistics() {
    if(count == 0) {
        printf("No records to analyze.\n");
        return;
    }
    int totalMarks = 0;
    for(int i = 0; i < count; i++) {
        for(int j = 0; j < SUBJECTS; j++) {
            totalMarks += students[i].marks[j];
        }
    }
    float avg = (float)totalMarks / (count * SUBJECTS);
    printf("Total Students: %d\n", count);
    printf("Class Average Marks: %.2f\n", avg);
}

void saveToFile() {
    FILE *fp = fopen("students.dat", "wb");
    if(fp == NULL) {
        printf("Error saving file!\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("students.dat", "rb");
    if(fp == NULL) return;
    fread(&count, sizeof(int), 1, fp);
    fread(students, sizeof(Student), count, fp);
    fclose(fp);
}
