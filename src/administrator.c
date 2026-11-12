#include <stdio.h>
#include <string.h>
#include "main.h"

// Structure Declarations
// =========================================================


// Login Menu
// =========================================================
void adminLogin()
{
    char password[20] = "admin123";
    char attempt[20];
    int choice, subChoice;

    printf("\n===== ADMIN LOGIN =====\n");

    for (int i = 1; i < 4; i++) {

        printf("Enter Password: ");
        fgets(attempt, 20, stdin);
        attempt[strcspn(attempt, "\n")] = 0;

        if (strcmp(password, attempt) == 0) {
            printf("Login Successful!\n\n");
            break;
        } 
        else {
            printf("Wrong Password!\n");
            printf("Login Failed!\n\n");
        }

        if (i == 3) {
            return;
        }
    }

    do {
        choice = adminMenu();

        switch (choice) {
            case 1:
                do {
                    printf("\n1. Patient\n");
                    printf("2. Doctor\n");
                    printf("3. Nurse\n");
                    printf("4. Back\n");
                    printf("Choose: ");

                    if (scanf("%d", &subChoice) != 1){
                        while (getchar() != '\n');
                        subChoice = -1;
                    }
                    getchar();
                } while (subChoice < 1 || subChoice > 4);

                switch (subChoice) {
                    case 1:
                        registerPatient();
                        break;
                    case 2:
                        registerDoctor();
                        break;
                    case 3:
                        registerNurse();
                        break;
                    case 4:
                        continue;
                }
                break;

            case 2:
                do {
                    printf("\n1. Patient\n");
                    printf("2. Doctor\n");
                    printf("3. Nurse\n");
                    printf("4. Back\n");
                    printf("Choose: ");

                    if (scanf("%d", &subChoice) != 1){
                        while (getchar() != '\n');
                        subChoice = -1;
                    }
                    getchar();
                } while (subChoice < 1 || subChoice > 4);

                switch (subChoice) {
                    case 1:
                        deleteRecord(PATIENT_FILE, "Patient");
                        break;
                    case 2:
                        deleteRecord(DOCTOR_FILE, "Doctor");
                        break;
                    case 3:
                        deleteRecord(NURSE_FILE, "Nurse");
                        break;
                    case 4:
                        continue;
                }
                break;

            case 3:
                do {
                    printf("\n1. Patients\n");
                    printf("2. Doctors\n");
                    printf("3. Nurses\n");
                    printf("4. Complaints\n");
                    printf("5. Back\n");
                    printf("Choose: ");

                    if (scanf("%d", &subChoice) != 1){
                        while (getchar() != '\n');
                        subChoice = -1;
                    }
                    getchar();
                } while (subChoice < 1 || subChoice > 5);

                switch (subChoice) {
                    case 1:
                        viewPatients();
                        break;
                    case 2:
                        viewDoctors();
                        break;
                    case 3:
                        viewNurses();
                        break;
                    case 4:
                        viewComplaintsAdmin();
                        break;
                    case 5:
                        continue;
                }
                break;

            case 4:
                do {
                    printf("\n1. Doctor\n");
                    printf("2. Nurse\n");
                    printf("3. Complaints\n");
                    printf("4. Back\n");
                    printf("Choose: ");

                    if (scanf("%d", &subChoice) != 1){
                        while (getchar() != '\n');
                        subChoice = -1;
                    }
                    getchar();
                } while (subChoice < 1 || subChoice > 4);

                switch (subChoice) {
                    case 1:
                        searchDoctor();
                        break;
                    case 2:
                        searchNurse();
                        break;
                    case 3:
                        searchComplaintAdmin();
                        break;
                    case 4:
                        continue;
                }
                break;

            case 5:
                printf("\nLogout successful.\n");
                break;

            default:
                printf("Invalid Input!\n\n");
                break;
        }

    } while (choice != 5);
}

// Helper Function 1: Admin Menu 
// =========================================================
int adminMenu()
{
    int choice;

    printf("\n===== ADMIN MENU =====\n");
    printf("1. Register\n");
    printf("2. Delete\n");
    printf("3. View\n");
    printf("4. Search\n");
    printf("5. Logout\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1){
        while (getchar() != '\n');
        return -1;
    }

    getchar();

    return choice;
}

// Helper Function 2: Ensure New Line 
// =========================================================
void ensureNewline(FILE *fp)
{
    long size;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    if (size > 0)
    {
        fseek(fp, -1, SEEK_END);

        if (fgetc(fp) != '\n') {
            fputc('\n', fp);
        }
    }
    fseek(fp, 0, SEEK_END);
}

// Helper Function 3: Duplicate ID Checker
// =========================================================
int isDuplicateID(const char *filename, const char *newID)
{
    FILE *fp;
    char line[LINE], existingID[15];

    fp = fopen(filename, "r");

    if (fp == NULL){
        return 0;
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]", existingID);

        if (strcmp(existingID, newID) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    return 0;
}

// Admin's Function 1: Generic Delete 
// =========================================================
void deleteRecord(const char *filename, const char *role)
{
    FILE *fp, *temp;
    char line[LINE], targetID[15], existingID[15];
    int found = 0;

    printf("\n--- Delete %s ---\n", role);
    printf("Enter %s ID to delete: ", role);

    fgets(targetID, 15, stdin);
    targetID[strcspn(targetID, "\n")] = 0;

    fp = fopen(filename, "r");

    if (fp == NULL){ 
        printf("No records.\n"); 
        return; 
    }

    temp = fopen("temp.txt", "w");

    if (temp == NULL){
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]", existingID);

        if (strcmp(existingID, targetID) == 0){
            found = 1;
        } 
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found){
        remove(filename);

        if (rename("temp.txt", filename) != 0){
            printf("Error updating file!\n");
        } 
        else {
            printf("%s deleted!\n", role);
        }
    } 
    else {
        remove("temp.txt");
        printf("%s ID not found!\n", role);
    }
}

// Admin's Function 2: Register Patient
// =========================================================
void registerPatient()
{
    FILE *fp;
    Patient p;

    printf("\n--- Register Patient ---\n");

    printf("Patient ID: ");
    fgets(p.id, 15, stdin);
    p.id[strcspn(p.id, "\n")] = 0;

    if (isDuplicateID(PATIENT_FILE, p.id)){
        printf("Patient ID already exists!\n");
        return;
    }

    printf("Name: ");
    fgets(p.name, 15, stdin);
    p.name[strcspn(p.name, "\n")] = 0;

    printf("Phone: ");
    fgets(p.phone, 20, stdin);
    p.phone[strcspn(p.phone, "\n")] = 0;

    printf("Password: ");
    fgets(p.password, 30, stdin);
    p.password[strcspn(p.password, "\n")] = 0;

    fp = fopen(PATIENT_FILE, "a+");

    if (fp == NULL){ 
        printf("Error opening file!\n"); 
        return; 
    }
    ensureNewline(fp);

    fprintf(fp, "%s|%s|%s|%s\n", p.id, p.name, p.phone, p.password);

    fclose(fp);

    printf("Patient added!\n");
}

// Admin's Function 3: Register Doctor
// =========================================================
void registerDoctor()
{
    FILE *fp;
    Doctor d;

    printf("\n--- Register Doctor ---\n");

    printf("Doctor ID: ");
    fgets(d.id, 15, stdin);
    d.id[strcspn(d.id, "\n")] = 0;

    if (isDuplicateID(DOCTOR_FILE, d.id)){
        printf("Doctor ID already exists!\n");
        return;
    }

    printf("Name: ");
    fgets(d.name, 50, stdin);
    d.name[strcspn(d.name, "\n")] = 0;

    printf("Speciality: ");
    fgets(d.speciality, 50, stdin);
    d.speciality[strcspn(d.speciality, "\n")] = 0;

    printf("Password: ");
    fgets(d.password, 30, stdin);
    d.password[strcspn(d.password, "\n")] = 0;

    fp = fopen(DOCTOR_FILE, "a+");

    if (fp == NULL){ 
        printf("Error opening file!\n"); 
        return; 
    }
    ensureNewline(fp);

    fprintf(fp, "%s|%s|%s|%s\n", d.id, d.name, d.speciality, d.password);

    fclose(fp);

    printf("Doctor added!\n");
}

// Admin's Function 4: Register Nurse
// =========================================================
void registerNurse()
{
    FILE *fp;
    Nurse n;

    printf("\n--- Register Nurse ---\n");

    printf("Nurse ID  : ");
    fgets(n.id, 15, stdin);
    n.id[strcspn(n.id, "\n")] = 0;

    if (isDuplicateID(NURSE_FILE, n.id)){
        printf("Nurse ID already exists!\n");
        return;
    }

    printf("Name      : ");
    fgets(n.name, 50, stdin);
    n.name[strcspn(n.name, "\n")] = 0;

    printf("Password  : ");
    fgets(n.password, 30, stdin);
    n.password[strcspn(n.password, "\n")] = 0;

    fp = fopen(NURSE_FILE, "a+");

    if (fp == NULL){ 
        printf("Error opening file!\n"); 
        return; 
    }
    ensureNewline(fp);

    fprintf(fp, "%s|%s|%s\n", n.id, n.name, n.password);

    fclose(fp);

    printf("Nurse added!\n");
}

// Admin's Function 5: View Patients
// =========================================================
void viewPatients()
{
    FILE *fp;
    char line[LINE];
    Patient p;

    printf("\n--- Patient List ---\n");

    fp = fopen(PATIENT_FILE, "r");

    if (fp == NULL){ 
        printf("No records.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]|%49[^|]|%19[^|]|%29[^|]", p.id, p.name, p.phone, p.password);

        printf("\nPatient ID : %s\n", p.id);
        printf("Name       : %s\n", p.name);
        printf("Phone      : %s\n", p.phone);
        printf("---------------------\n");
    }
    fclose(fp);
}

// Admin's Function 6: View Doctors
// =========================================================
void viewDoctors()
{
    FILE *fp;
    char line[LINE];
    Doctor d;

    printf("\n--- Doctor List ---\n");

    fp = fopen(DOCTOR_FILE, "r");

    if (fp == NULL){ 
        printf("No records.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]|%49[^|]|%49[^|]|%29[^|]", d.id, d.name, d.speciality, d.password);

        printf("\nDoctor ID : %s\n", d.id);
        printf("Name      : %s\n", d.name);
        printf("speciality : %s\n", d.speciality);
        printf("---------------------\n");
    }
    fclose(fp);
}

// Admin's Function 7: View Nurses
// =========================================================
void viewNurses()
{
    FILE *fp;
    char line[LINE];
    Nurse n;

    printf("\n--- Nurse List ---\n");

    fp = fopen(NURSE_FILE, "r");

    if (fp == NULL){ 
        printf("No records.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]|%49[^|]|%29[^|]", n.id, n.name, n.password);

        printf("\nNurse ID  : %s\n", n.id);
        printf("Name      : %s\n", n.name);
        printf("---------------------\n");
    }
    fclose(fp);
}

// Admin's Function 8: View Complaints
// =========================================================
void viewComplaintsAdmin()
{
    FILE *fp;
    char line[LINE];
    Complaint c;

    printf("\n--- Complaint List ---\n");

    fp = fopen(COMPLAINT_FILE, "r");

    if (fp == NULL){ 
        printf("No complaints.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]|%14[^|]|%49[^|]|%99[^|\n]", c.cid, c.pid, c.pname, c.message);

        printf("\nComplaint ID : %s\n", c.cid);
        printf("Patient ID   : %s\n", c.pid);
        printf("Patient Name : %s\n", c.pname);
        printf("Message      : %s\n", c.message);
        printf("---------------------\n");
    }
    fclose(fp);
}

// Admin's Function 9: Search Doctor
// =========================================================
void searchDoctor()
{
    FILE *fp;
    char line[LINE], keyword[50];
    Doctor d;
    int found = 0;

    printf("\n--- Search Doctor ---\n");
    printf("Enter Doctor ID or Name: ");

    fgets(keyword, 50, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    fp = fopen(DOCTOR_FILE, "r");

    if (fp == NULL){ 
        printf("No records found.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {

        sscanf(line, "%14[^|]|%49[^|]|%49[^|]|%29[^|]", d.id, d.name, d.speciality, d.password);

        if (strstr(d.id, keyword) || strstr(d.name, keyword)) {
            printf("\nDoctor ID : %s\n", d.id);
            printf("Name      : %s\n", d.name);
            printf("speciality : %s\n", d.speciality);
            printf("---------------------\n");

            found = 1;
        }
    }
    fclose(fp);

    if (!found){
        printf("No doctor found.\n");
    }
}

// Admin's Function 10: Search Nurse
// =========================================================
void searchNurse()
{
    FILE *fp;
    char line[LINE], keyword[50];
    Nurse n;
    int found = 0;

    printf("\n--- Search Nurse ---\n");
    printf("Enter Nurse ID or Name: ");

    fgets(keyword, 50, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    fp = fopen(NURSE_FILE, "r");

    if (fp == NULL){ 
        printf("No records found.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]|%49[^|]|%29[^|]", n.id, n.name, n.password);

        if (strstr(n.id, keyword) || strstr(n.name, keyword)) {
            printf("\nNurse ID  : %s\n", n.id);
            printf("Name      : %s\n", n.name);
            printf("---------------------\n");

            found = 1;
        }
    }
    fclose(fp);

    if (!found){
        printf("No nurse found.\n");
    }
}

// Admin's Function 11: Search Complaint
// =========================================================
void searchComplaintAdmin()
{
    FILE *fp;
    char line[LINE], keyword[50];
    Complaint c;
    int found = 0;

    printf("\n--- Search Complaint ---\n");
    printf("Enter keyword: ");

    fgets(keyword, 50, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    fp = fopen(COMPLAINT_FILE, "r");

    if (fp == NULL){ 
        printf("No records found.\n"); 
        return; 
    }

    while (fgets(line, LINE, fp)) {
        sscanf(line, "%14[^|]|%14[^|]|%49[^|]|%99[^|\n]", c.cid, c.pid, c.pname, c.message);

        if (strstr(c.cid, keyword) ||
            strstr(c.pid, keyword) ||
            strstr(c.pname, keyword) ||
            strstr(c.message, keyword)) {

            printf("\nComplaint ID : %s\n", c.cid);
            printf("Patient ID   : %s\n", c.pid);
            printf("Patient Name : %s\n", c.pname);
            printf("Message      : %s\n", c.message);
            printf("---------------------\n");

            found = 1;
        }
    }
    fclose(fp);

    if (!found){
        printf("No complaint found.\n");
    }
}