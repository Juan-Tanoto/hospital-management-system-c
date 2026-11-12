#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

#define SMALL 20
#define MEDIUM 50
#define LARGE 100

typedef struct
{
    char id[MEDIUM];
    char name[MEDIUM];
    int quantity;
} Inventory;

typedef struct
{
    char pid[MEDIUM];
    char note[LARGE];
} DailyNote;

void toLowerCase(char str[])
{
    int i;

    for (i = 0; str[i] != '\0'; i++){
        str[i] = tolower(str[i]);
    }
}

void clearBuffer()
{
    while (getchar() != '\n');
}

void nurseLogin()
{
    FILE *fp;
    char line[LINE];
    char inputID[MEDIUM], inputPass[MEDIUM];
    char id[MEDIUM], name[MEDIUM], pass[MEDIUM];
    int found = 0;

    int choice, subChoice;

    printf("\n===== NURSE LOGIN =====\n");

    /* ===== LOGIN (3 ATTEMPTS) ===== */
    for (int i = 1; i <= 3; i++){
        printf("Enter Nurse ID: ");
        fgets(inputID, MEDIUM, stdin);
        inputID[strcspn(inputID, "\n")] = 0;

        if (strlen(inputID) == 0){
            printf("Nurse ID cannot be empty!\n\n");
            continue;
        }

        printf("Enter Password: ");
        fgets(inputPass, MEDIUM, stdin);
        inputPass[strcspn(inputPass, "\n")] = 0;

        if (strlen(inputPass) == 0){
            printf("Password cannot be empty!\n\n");
            continue;
        }

        fp = fopen(NURSE_FILE, "r");

        if (fp == NULL){
            printf("Error opening nurse records!\n");
            return;
        }

        found = 0;

        while (fgets(line, LINE, fp)){

            if (sscanf(line, "%49[^|]|%49[^|]|%49[^|\n]", id, name, pass) == 3){
                if (strcmp(id, inputID) == 0 && strcmp(pass, inputPass) == 0){
                    found = 1;
                    break;
                }
            }
        }

        fclose(fp);

        if (found){
            printf("Login Successful!\n");
            printf("Welcome, %s!\n", name);
            break;
        }
        else{
            printf("Wrong ID or Password!\n\n");
        }

        if (i == 3){
            printf("Login Failed.\n");
            return;
        }
    }

    do{
        choice = nurseMenu();

        if (choice == 1){
            viewDoctorSchedule();
        }

        else if (choice == 2){
            do{
                printf("\n1. Add Inventory\n");
                printf("2. View Inventory\n");
                printf("3. Search Inventory\n");
                printf("4. Back\n");
                printf("Choose: ");

                if (scanf("%d", &subChoice) != 1){

                    clearBuffer();
                    subChoice = -1;
                }
                else{
                    clearBuffer();
                }

            }while(subChoice < 1 || subChoice > 4);

            if (subChoice == 1){
                addInventory();
            }
            else if (subChoice == 2){

                viewInventory();
            }
            else if (subChoice == 3){
                searchInventory();
            }
            else{
                continue;
            }
        }

        else if (choice == 3){
            do{
                printf("\n1. Add Note\n");
                printf("2. View Notes\n");
                printf("3. Search Notes\n");
                printf("4. Back\n");
                printf("Choose: ");

                if (scanf("%d", &subChoice) != 1){
                    clearBuffer();
                    subChoice = -1;
                }
                else{
                    clearBuffer();
                }

            }while(subChoice < 1 || subChoice > 4);

            if (subChoice == 1){
                addNote();
            }

            else if (subChoice == 2){
                viewNotes();
            }

            else if (subChoice == 3){
                searchNotes();
            }

            else{
                continue;
            }
        }

        else if (choice == 4){
            printf("\nLogout successful.\n");
        }

        else{
            printf("Invalid input!\n");
        }
    }while(choice != 4);
}

// ===== MENU =====
int nurseMenu(){
    int choice;

    printf("\n===== NURSE MENU =====\n");
    printf("1. View Doctor Schedule\n");
    printf("2. Inventory Management\n");
    printf("3. Daily Notes\n");
    printf("4. Logout\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1){
        clearBuffer();
        return -1;
    }

    clearBuffer();

    return choice;
}

// ===== VIEW DOCTOR SCHEDULE ===== 
void viewDoctorSchedule(){
    FILE *fp;
    char line[LINE];
    char sid[MEDIUM], did[MEDIUM], date[SMALL], time[SMALL], status[SMALL];

    printf("\n--- Doctor Schedule ---\n");

    fp = fopen(SCHEDULE_FILE, "r");

    if (fp == NULL){
        printf("No schedule available.\n");
        return;
    }

    while (fgets(line, LINE, fp)){
        if (sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^|\n]", sid, did, date, time, status) == 5){
            printf("\nSchedule ID : %s\n", sid);
            printf("Doctor ID   : %s\n", did);
            printf("Date        : %s\n", date);
            printf("Time        : %s\n", time);
            printf("Status      : %s\n", status);
            printf("-------------------------\n");
        }
    }

    fclose(fp);
}

// ===== INVENTORY ===== 
void addInventory(){
    FILE *fp;
    Inventory item;
    char quantityStr[SMALL];
    int i;

    printf("\n--- Add Inventory ---\n");

    printf("Item ID: ");
    fgets(item.id, MEDIUM, stdin);
    item.id[strcspn(item.id, "\n")] = 0;

    if (strlen(item.id) == 0){
        printf("Item ID cannot be empty!\n");
        return;
    }

    // ===== DUPLICATE CHECK =====
    if (isDuplicateID(INVENTORY_FILE, item.id)){
        printf("Item ID already exists!\n");
        return;
    }

    printf("Item Name: ");
    fgets(item.name, MEDIUM, stdin);
    item.name[strcspn(item.name, "\n")] = 0;

    if (strlen(item.name) == 0){
        printf("Item Name cannot be empty!\n");
        return;
    }

    printf("Quantity: ");
    fgets(quantityStr, SMALL, stdin);
    quantityStr[strcspn(quantityStr, "\n")] = 0;

    if (strlen(quantityStr) == 0){
        printf("Quantity cannot be empty!\n");
        return;
    }

    for (i = 0; quantityStr[i] != '\0'; i++){
        if (!isdigit(quantityStr[i])){
            printf("Quantity must be numeric!\n");
            return;
        }
    }

    sscanf(quantityStr, "%d", &item.quantity);
    
    fp = fopen(INVENTORY_FILE, "a+");

    if (fp == NULL){
        printf("Error opening file!\n");
        return;
    }

    ensureNewline(fp);

    fprintf(fp, "%s|%s|%d\n", item.id, item.name, item.quantity);

    fclose(fp);

    printf("Inventory added!\n");
}

// ==== VIEW INVENTORY ====
void viewInventory(){
    FILE *fp;
    char line[LINE];
    Inventory item;

    printf("\n--- Inventory List ---\n");

    fp = fopen(INVENTORY_FILE, "r");

    if (fp == NULL){
        printf("No inventory records.\n");
        return;
    }

    while (fgets(line, LINE, fp)){
        if (sscanf(line, "%49[^|]|%49[^|]|%d",
                   item.id,
                   item.name,
                   &item.quantity) == 3){

            printf("\nID   : %s\n", item.id);
            printf("Name : %s\n", item.name);
            printf("Qty  : %d\n", item.quantity);
            printf("------------------\n");
        }
    }
    fclose(fp);
}

// ==== Search Inventory ====
void searchInventory(){
    FILE *fp;
    char line[LINE], keyword[MEDIUM], keywordLower[MEDIUM];
    char idLower[MEDIUM], nameLower[MEDIUM];
    Inventory item;
    int found = 0;

    printf("\n--- Search Inventory ---\n");
    printf("Enter keyword: ");

    fgets(keyword, MEDIUM, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    if (strlen(keyword) == 0){
        printf("Keyword cannot be empty!\n");
        return;
    }

    strcpy(keywordLower, keyword);
    toLowerCase(keywordLower);

    fp = fopen(INVENTORY_FILE, "r");

    if (fp == NULL){
        printf("No records.\n");
        return;
    }

    while (fgets(line, LINE, fp)){
        if (sscanf(line, "%49[^|]|%49[^|]|%d", item.id, item.name, &item.quantity) == 3){
            strcpy(idLower, item.id);
            strcpy(nameLower, item.name);

            toLowerCase(idLower);
            toLowerCase(nameLower);

            if (strstr(idLower, keywordLower) || strstr(nameLower, keywordLower)){
                printf("\nID   : %s\n", item.id);
                printf("Name : %s\n", item.name);
                printf("Qty  : %d\n", item.quantity);
                printf("------------------\n");

                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found){
        printf("No item found.\n");
    }
}

// ===== ADD DAILY NOTES ===== 
void addNote(){
    FILE *fp;
    DailyNote notes;

    printf("\n--- Add Daily Note ---\n");

    printf("Patient ID: ");
    fgets(notes.pid, MEDIUM, stdin);
    notes.pid[strcspn(notes.pid, "\n")] = 0;

    if (strlen(notes.pid) == 0){
        printf("Patient ID cannot be empty!\n");
        return;
    }

    printf("Note: ");
    fgets(notes.note, LARGE, stdin);
    notes.note[strcspn(notes.note, "\n")] = 0;

    if (strlen(notes.note) == 0){
        printf("Note cannot be empty!\n");
        return;
    }

    fp = fopen(NOTES_FILE, "a+");

    if (fp == NULL){
        printf("Error opening file!\n");
        return;
    }
    ensureNewline(fp);

    fprintf(fp, "%s|%s\n", notes.pid, notes.note);

    fclose(fp);

    printf("Note added!\n");
}

// ===== VIEW DAILY NOTES =====
void viewNotes(){
    FILE *fp;
    char line[LINE];
    DailyNote notes;

    printf("\n--- Daily Notes ---\n");

    fp = fopen(NOTES_FILE, "r");

    if (fp == NULL){
        printf("No notes available.\n");
        return;
    }

    while (fgets(line, LINE, fp)){
        if (sscanf(line, "%49[^|]|%99[^|\n]", notes.pid, notes.note) == 2){
            printf("\nPatient ID : %s\n", notes.pid);
            printf("Note       : %s\n", notes.note);
            printf("---------------------\n");
        }
    }

    fclose(fp);
}

// ===== SEARCH DAILY NOTES =====
void searchNotes(){
    FILE *fp;
    char line[LINE], keyword[MEDIUM], keywordLower[MEDIUM];
    char pidLower[MEDIUM], noteLower[LARGE];
    DailyNote notes;
    int found = 0;

    printf("\n--- Search Notes ---\n");
    printf("Enter keyword: ");

    fgets(keyword, MEDIUM, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    if (strlen(keyword) == 0){
        printf("Keyword cannot be empty!\n");
        return;
    }

    strcpy(keywordLower, keyword);
    toLowerCase(keywordLower);

    fp = fopen(NOTES_FILE, "r");

    if (fp == NULL){
        printf("No records.\n");
        return;
    }

    while (fgets(line, LINE, fp)){
        if (sscanf(line, "%49[^|]|%99[^|\n]", notes.pid, notes.note) == 2){
            strcpy(pidLower, notes.pid);
            strcpy(noteLower, notes.note);

            toLowerCase(pidLower);
            toLowerCase(noteLower);

            if (strstr(pidLower, keywordLower) ||
                strstr(noteLower, keywordLower)){

                printf("\nPatient ID : %s\n", notes.pid);
                printf("Note       : %s\n", notes.note);
                printf("---------------------\n");

                found = 1;
            }
        }
    }
    fclose(fp);

    if (!found){
        printf("No notes found.\n");
    }
}