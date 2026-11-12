#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


// ==========================================
// 1. LOGIN SYSTEM
// ==========================================
int loginSystem() {
    char username[20];
    char password[20];
    int attempts = 0;

    while (attempts < 3) {
        printf("\n--- Director Login (Attempt %d/3) ---\n", attempts + 1);
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        if (strcmp(username, "director") == 0 && strcmp(password, "director123") == 0) {
            printf("\nLogin Successful! Welcome, Director.\n");
            return 1; // Success
        } else {
            attempts++;
            printf("Invalid username or password. Please try again.\n");
        }
    }

    printf("\nToo many failed attempts. Access Denied.\n");
    return 0; // Failure
}

// ==========================================
// 2. MAIN DIRECTOR MENU
// ==========================================
void directorMenu(char* directorName) {
    int choice;
    while(1) {
        printf("\n========================================\n");
        printf("      DIRECTOR MENU - Welcome %s\n", directorName);
        printf("========================================\n");
        printf("1. Staff Report (Doctors, Nurses)\n");
        printf("2. Yearly Sales Report\n");
        printf("3. View Master Schedule\n");
        printf("4. Database Backup Utility \n");
        printf("5. Patient Complaint Dashboard \n");
        printf("6. Logout\n");
        printf("========================================\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        switch(choice) {
            case 1: staffReportMenu(); break;
            case 2: salesReportMenu(); break;
            case 3: viewMasterSchedule(); break;
            case 4: backupDatabase(); break;
            case 5: viewComplaintsDirector(); break;
            case 6:
                printf("Logging out of Director module...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// ==========================================
// 3. STAFF REPORT
// ==========================================
void staffReportMenu() {
    int choice;
    while(1) {
        printf("\n--- Staff Report Menu ---\n");
        printf("1. View All Staff (Doctors, Nurses)\n");
        printf("2. Search Staff by ID\n");
        printf("3. Back to Director Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while(getchar() != '\n'); continue;
        }

        if (choice == 1) viewAllStaff();
        else if (choice == 2) searchStaff();
        else if (choice == 3) break;
        else printf("Invalid choice.\n");
    }
}

void viewAllStaff() {
    char id[20], name[50], extra[50], password[20];
    FILE *file;

    printf("\n======================= HOSPITAL STAFF REPORT =======================\n");
    printf("%-10s | %-25s | %-25s\n", "ID", "Name", "Role & Specialty");
    printf("---------------------------------------------------------------------\n");

    file = fopen(DOCTOR_FILE, "r");
    if (file != NULL) {
        while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^\n]\n", id, name, extra, password) == 4) {
            char roleDesc[50];
            sprintf(roleDesc, "Doctor (%s)", extra);
            printf("%-10s | %-25s | %-25s\n", id, name, roleDesc);
        }
        fclose(file);
    }

    file = fopen(NURSE_FILE, "r");
    if (file != NULL) {
        while (fscanf(file, " %[^|]|%[^|]|%[^\n]\n", id, name, password) == 3) {
            printf("%-10s | %-25s | Nurse\n", id, name);
        }
        fclose(file);
    }
    printf("=====================================================================\n");
}

void searchStaff() {
    char searchId[20];
    char id[20], name[50], extra[50], password[20];
    FILE *file;
    int found = 0;

    printf("\nEnter Staff ID to search: ");
    scanf("%s", searchId);
    printf("\n--- Search Results ---\n");

    file = fopen(DOCTOR_FILE, "r");
    if (file != NULL && !found) {
        while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^\n]\n", id, name, extra, password) == 4) {
            if (strcmp(id, searchId) == 0) {
                printf("ID: %s\nName: %s\nRole: Doctor\nSpecialty: %s\n", id, name, extra);
                found = 1; break;
            }
        }
        fclose(file);
    }

    file = fopen(NURSE_FILE, "r");
    if (file != NULL && !found) {
        while (fscanf(file, " %[^|]|%[^|]|%[^\n]\n", id, name, password) == 3) {
            if (strcmp(id, searchId) == 0) {
                printf("ID: %s\nName: %s\nRole: Nurse\n", id, name);
                found = 1; break;
            }
        }
        fclose(file);
    }

    if (!found) printf("Staff with ID '%s' not found.\n", searchId);
}

// ==========================================
// 4. YEARLY SALES REPORT
// ==========================================
void salesReportMenu() {
    FILE *file = fopen(PAYMENT_FILE, "r");
    if (file == NULL) {
        printf("\nError: Could not locate '%s'.\n", PAYMENT_FILE);
        return;
    }

    char payId[20], appId[20], patId[20], date[20];
    float amount;
    char searchYear[10];
    float totalSales = 0.0;
    int found = 0;

    printf("\nEnter the Year to search (e.g., 2026): ");
    scanf("%s", searchYear);
    while(getchar() != '\n');

    printf("\n================ Sales Report for %s ================\n", searchYear);
    printf("%-15s | %-15s | %-15s\n", "Payment Date", "Patient ID", "Amount (RM)");
    printf("----------------------------------------------------\n");

    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%f|%s\n", payId, appId, patId, &amount, date) == 5) {
        if (strncmp(date, searchYear, 4) == 0) {
            printf("%-15s | %-15s | RM %-10.2f\n", date, patId, amount);
            totalSales += amount;
            found = 1;
        }
    }
    fclose(file);

    if (found) {
        printf("----------------------------------------------------\n");
        printf("TOTAL SALES FOR %s: RM %.2f\n", searchYear, totalSales);
        printf("====================================================\n");
    } else {
        printf("No sales recorded for the year %s.\n", searchYear);
    }

    printf("\nPress Enter to return to menu...");
    getchar();
}

// ==========================================
// 5. VIEW MASTER SCHEDULE
// ==========================================
void viewMasterSchedule() {
    FILE *schedFile = fopen(SCHEDULE_FILE, "r");
    if (schedFile == NULL) {
        printf("\nError: Could not locate '%s'.\n", SCHEDULE_FILE);
        return;
    }

    char schedId[20], docId[20], date[20], time[20], status[20];
    char searchDocId[20], docName[50], docSpecialty[50], docPass[20];
    FILE *docFile;
    int foundSchedule = 0;

    printf("\n================================== HOSPITAL MASTER SCHEDULE ==================================\n");
    printf("%-12s | %-6s | %-25s | %-15s | %-12s\n", "Date", "Time", "Doctor Name", "Specialty", "Status");
    printf("----------------------------------------------------------------------------------------------\n");

    while (fscanf(schedFile, " %[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", schedId, docId, date, time, status) == 5) {
        foundSchedule = 1;
        char currentName[50] = "Unknown Doctor";
        char currentSpec[50] = "N/A";

        docFile = fopen(DOCTOR_FILE, "r");
        if (docFile != NULL) {
            while (fscanf(docFile, " %[^|]|%[^|]|%[^|]|%[^\n]\n", searchDocId, docName, docSpecialty, docPass) == 4) {
                if (strcmp(searchDocId, docId) == 0) {
                    strcpy(currentName, docName);
                    strcpy(currentSpec, docSpecialty);
                    break;
                }
            }
            fclose(docFile);
        }
        printf("%-12s | %-6s | %-25s | %-15s | %-12s\n", date, time, currentName, currentSpec, status);
    }
    fclose(schedFile);

    if (!foundSchedule) printf("No schedules found in the system.\n");
    printf("==============================================================================================\n");
}

// ==========================================
// 6. DATABASE BACKUP UTILITY
// ==========================================
void backupFile(const char* sourceFileName, const char* backupFileName) {
    FILE *source = fopen(sourceFileName, "r");
    if (source == NULL) {
        printf("Skipping %s (File not found or empty).\n", sourceFileName);
        return;
    }

    FILE *dest = fopen(backupFileName, "w");
    if (dest == NULL) {
        printf("Error: Could not create backup file for %s.\n", sourceFileName);
        fclose(source);
        return;
    }

    char ch;
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, dest);
    }
    printf("[SUCCESS] Backed up '%s' to '%s'\n", sourceFileName, backupFileName);
    fclose(source);
    fclose(dest);
}

void backupDatabase() {
    printf("\n========================================\n");
    printf("      INITIATING SYSTEM BACKUP...       \n");
    printf("========================================\n");

    backupFile(DOCTOR_FILE, "doctors_backup.txt");
    backupFile(NURSE_FILE, "nurses_backup.txt");
    backupFile(PAYMENT_FILE, "payments_backup.txt");
    backupFile(SCHEDULE_FILE, "schedule_backup.txt");
    backupFile(COMPLAINT_FILE, "complaints_backup.txt");

    printf("========================================\n");
    printf("           BACKUP COMPLETED!            \n");
    printf("========================================\n");
}

// ==========================================
// 7. PATIENT COMPLAINT DASHBOARD
// ==========================================
void viewComplaintsDirector() {
    FILE *file = fopen(COMPLAINT_FILE, "r");
    if (file == NULL) {
        printf("\nError: Could not locate '%s'.\n", COMPLAINT_FILE);
        return;
    }

    char compId[20], patId[20], patName[50], complaintDesc[200];
    int count = 0;

    printf("\n================================== PATIENT COMPLAINT DASHBOARD ==================================\n");
    printf("%-15s | %-15s | %-25s | %-30s\n", "Complaint ID", "Patient ID", "Patient Name", "Complaint Details");
    printf("-------------------------------------------------------------------------------------------------\n");

    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^\n]\n", compId, patId, patName, complaintDesc) == 4) {
        printf("%-15s | %-15s | %-25s | %-30s\n", compId, patId, patName, complaintDesc);
        count++;
    }
    fclose(file);

    if (count == 0) printf("No complaints found.\n");
    printf("=================================================================================================\n");
}

// ==========================================
// MAIN FUNCTION
// ==========================================
void directorMainMenu() {
    printf("Hospital Management System - Director Module\n");

    // Perform Login Check
    if (loginSystem()) {
        directorMenu("Director");
    } else {
        printf("Exiting program due to failed login.\n");
    }

    printf("Program ended.\n");
    return;
}
