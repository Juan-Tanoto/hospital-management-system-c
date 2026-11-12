#include <stdio.h>
#include "main.h"

int main()
{
    int choice;
    do 
    {    
        printf("\n=== Hospital Management System ===\n");
        printf("1. Director\n");
        printf("2. Administrator\n");
        printf("3. Patient\n");
        printf("4. Doctor\n");
        printf("5. Nurse\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
    
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                directorMainMenu();
                break;
            case 2:
                adminLogin();
                break;
            case 3:
                patientMenu();
                break;
            case 4:
                doctorLogin();
                break;
            case 5:
                nurseLogin();
                break;
            case 6:
                printf("Exiting system...\n");
                break;
            default:
                printf("Invalid choice\n");
        }

    } while (choice != 6);

    return 0;
}