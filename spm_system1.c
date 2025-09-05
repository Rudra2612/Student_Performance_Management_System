#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUBJECTS 3
#define FILENAME "students.txt"

typedef struct {
    int roll_no;
    char name[50];
    int marks[SUBJECTS];
    int totalM;
    float averageM;
} Student;

// Function prototypes
void addStudent();
void displayStudents();
int searchStudent(int roll_no, Student *s);
void updateStudent(int roll_no);
void deleteStudent(int roll_no);
void computeStatistics();

// ---------- Main ----------
int main() {
    int choice, roll;
    Student s;

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
                printf("\n");
                printf("Enter roll number to search: ");
                scanf("%d", &roll);
                if (searchStudent(roll, &s)) {
                    printf("\n--- ::Student Details:: --- \n");
                    printf("Name: %s | Roll: %d | Marks: %d %d %d | Total_marks: %d | Average: %.2f\n",
                           s.name, s.roll_no, s.marks[0], s.marks[1], s.marks[2], s.totalM, s.averageM);
                } else {
                    printf("Student not found!\n");
                }
                break;
            case 4:
                printf("\n");
                printf("Enter roll number to update: ");
                scanf("%d", &roll);
                updateStudent(roll);
                break;
            case 5:
                printf("\n");
                printf("Enter roll number to delete: ");
                scanf("%d", &roll);
                deleteStudent(roll);
                break;
            case 6: computeStatistics(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 7);

    return 0;
}

// ---------- Add Student ----------
void addStudent() {
    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    Student s;
    printf("\n");
    printf("Enter roll number: ");
    scanf("%d", &s.roll_no);
    printf("Enter name: ");
    getchar(); // clear newline
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    for (int i = 0; i < SUBJECTS; i++) {
        printf("Enter marks for subject %d: ", i + 1);
        scanf("%d", &s.marks[i]);        
    }        

    s.totalM = s.marks[0] + s.marks[1] + s.marks[2];
    s.averageM = (float) s.totalM / SUBJECTS;

    fprintf(fp, "%d,%s,%d,%d,%d,%d,%.2f\n", s.roll_no, s.name,
            s.marks[0], s.marks[1], s.marks[2],s.totalM,s.averageM);
    fclose(fp);

    printf("Student added successfully!\n");
}

// ---------- Display Students ----------
void displayStudents() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    Student s;
    printf("\n-------------------- Student Records --------------------");
    printf("\n%-10s %-20s %-10s %-10s %-10s %-10s %-10s\n",
           "RollNo", "Name", "Sub1", "Sub2", "Sub3", "TotalMarks", "Average");
    while (fscanf(fp, "%d,%49[^,],%d,%d,%d,%d,%f\n",
                  &s.roll_no, s.name,
                  &s.marks[0], &s.marks[1], &s.marks[2], &s.totalM, &s.averageM) == 7) {                    
        printf("%-10d %-20s %-10d %-10d %-10d %-10d %-10.2f\n",
               s.roll_no, s.name, s.marks[0], s.marks[1], s.marks[2], s.totalM, s.averageM);
    }    
    fclose(fp);
}

// ---------- Search Student ----------
int searchStudent(int roll_no, Student *s) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) return 0;
    while (fscanf(fp, "%d,%49[^,],%d,%d,%d,%d,%f",
                  &s->roll_no, s->name,
                  &s->marks[0], &s->marks[1], &s->marks[2], &s->totalM, &s->averageM) == 7) {
        if (s->roll_no == roll_no) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ---------- Update Student ----------
void updateStudent(int roll_no) {
    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }
    Student s;
    int found = 0;
    while (fscanf(fp, "%d,%49[^,],%d,%d,%d,%d,%f\n",
                  &s.roll_no, s.name,
                  &s.marks[0], &s.marks[1], &s.marks[2], &s.totalM, &s.averageM) == 7) {
        if (s.roll_no == roll_no) {
            printf("\nUpdating record for %s\n", s.name);
            for (int i = 0; i < SUBJECTS; i++) {
                printf("Enter new marks for subject %d: ", i+1);
                scanf("%d", &s.marks[i]);                
            }

            s.totalM = s.marks[0] + s.marks[1] + s.marks[2];
            s.averageM = (float) s.totalM / SUBJECTS;

            found = 1;
        }
        fprintf(temp, "%d,%s,%d,%d,%d,%d,%.2f\n", s.roll_no, s.name,
                s.marks[0], s.marks[1], s.marks[2], s.totalM, s.averageM);
    }
    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) printf("Record updated!\n");
    else printf("\nStudent not found!\n");
}

// ---------- Delete Student ----------
void deleteStudent(int roll_no) {
    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }
    Student s;
    int found = 0;
    while (fscanf(fp, "%d,%49[^,],%d,%d,%d,%d,%f\n",
                  &s.roll_no, s.name,
                  &s.marks[0], &s.marks[1], &s.marks[2], &s.totalM, &s.averageM) == 7) {
        if (s.roll_no == roll_no) {
            found = 1;
            continue; // skip writing this student
        }
        fprintf(temp, "%d,%s,%d,%d,%d,%d,%.2f\n", s.roll_no, s.name,
                s.marks[0], s.marks[1], s.marks[2], s.totalM, s.averageM);
    }
    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) printf("Record deleted!\n");
    else printf("\nStudent not found!\n");
}

// ---------- Compute Statistics ----------
void computeStatistics() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    Student s;
    int totalMarks = 0, studentCount = 0;
    while (fscanf(fp, "%d,%49[^,],%d,%d,%d,%d,%f\n",
                  &s.roll_no, s.name,
                  &s.marks[0], &s.marks[1], &s.marks[2], &s.totalM, &s.averageM) == 7) {
        totalMarks += s.totalM;
        studentCount++;
    }
    fclose(fp);

    if (studentCount == 0) {
        printf("No data for statistics.\n");
        return;
    }
    float avg = (float) totalMarks / (studentCount * SUBJECTS);
    printf("\nTotal Students: %d\n", studentCount);
    printf("Class Average Marks: %.2f\n", avg);
}
