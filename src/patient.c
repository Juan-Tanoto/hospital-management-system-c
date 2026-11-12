#include <stdio.h>
#include <string.h>
#include "main.h"

void pressEnter()
{
    printf("\nPress Enter to continue...");
    getchar();
}

int getIntInput(char prompt[])
{
    int value;

    while (1)
    {
        printf("\n%s", prompt);

        if (scanf("%d", &value) == 1)
        {
            while (getchar() != '\n');
            return value;
        }

        printf("Invalid input. Numbers only.\n");

        while (getchar() != '\n');
    }
}

void getDoctorName(char doctorID[], char doctorName[])
{
    FILE *fp;

    char fileDoctorID[ID_SIZE];
    char fileDoctorName[NAME_SIZE];
    char specialty[NAME_SIZE];

    fp = fopen(DOCTOR_FILE, "r");

    if (fp == NULL)
    {
        strcpy(doctorName, doctorID);
        return;
    }

    while (fscanf(fp,
                  "%[^|]|%[^|]|%[^\n]\n",
                  fileDoctorID,
                  fileDoctorName,
                  specialty) == 3)
    {
        if (strcmp(doctorID, fileDoctorID) == 0)
        {
            strcpy(doctorName, fileDoctorName);
            fclose(fp);
            return;
        }
    }

    fclose(fp);

    strcpy(doctorName, doctorID);
}

void generateNextID(char filename[],
                    char header[],
                    char prefix[],
                    char outputID[])
{
    FILE *fp;
    char line[300];
    int lastID = 0;

    fp = fopen(filename, "r");

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strncmp(line, header, strlen(header)) == 0)
            {
                continue;
            }

            sscanf(line, "%*[^0-9]%d", &lastID);
        }

        fclose(fp);
    }

    sprintf(outputID, "%s%03d", prefix, lastID + 1);
}


int login(char filename[], char id[], char password[], Patient *currentPatient)
{
    FILE *fp = fopen(filename, "r");

    Patient p;

    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }

    while (fscanf(fp,
                  "%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  p.id,
                  p.name,
                  p.phone,
                  p.password) == 4)
    {
        //check if id and password is correct and return 1
        if (strcmp(id, p.id) == 0 && strcmp(password, p.password) == 0)
        {
            *currentPatient = p;

            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}







/* ================= MAIN PATIENT MENU ================= */
void patientMenu()
{
    //LOGIN
    char patientID[ID_SIZE];
    char password[PASS_SIZE];

    Patient currentPatient;

    int attempts = 3;
    int success = 0;

    while (attempts > 0)
    {
        printf("\n--- Patient Login ---\n");

        printf("Enter ID (case sensitive): ");
        scanf("%14s", patientID);

        printf("Enter Password (case sensitive): ");
        scanf("%29s", password);

        if (login(PATIENT_FILE,
                  patientID,
                  password,
                  &currentPatient))
        {
            printf("\nLogin successful!\n");
            printf("Welcome %s\n", currentPatient.name);

            success = 1;
            break;
        }

        else
        {
            attempts--;

            printf("Wrong credentials. Attempts left: %d\n", attempts);
        }
    }

    if (!success)
    {
        printf("Too many failed attempts.\n");
        return;
    }

    int choice;

    do
    {
        printf("\n--- Patient Menu ---\n");

        printf("1. Appointment Management\n");
        printf("2. Payment Management\n");
        printf("3. Complaint Management\n");
        printf("4. Logout\n");

        choice = getIntInput("Enter choice: ");

        switch (choice)
        {       
            case 1:
                appointmentMenu(&currentPatient);
                break;

            case 2:
                paymentMenu(&currentPatient);
                break;

            case 3:
                complaintMenu(&currentPatient);
                break;

            case 4:
                printf("Logging out...\n");
                pressEnter();
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 4);
}









/* ================= APPOINTMENT ================= */

void appointmentMenu(Patient *currentPatient)
{
    int choice;

    do {
        printf("\n--- Appointment Menu ---\n");
        printf("1. Book new appointment\n");
        printf("2. Manage appointment\n");
        
        choice = getIntInput("Enter choice (0 to return): ");
        

        switch (choice) {
            case 0:
                return;
            case 1:
                bookAppointment(currentPatient);
                break;
            case 2:
                manageAppointment(currentPatient);
                break;
            default:
                printf("Invalid choice\n");
        }

    } while (choice != 0);
}


int chooseAvailableSlot(char doctorID[],
                          char selectedScheduleID[],
                          char selectedDate[],
                          char selectedTime[])
{
    FILE *fp;

    char fileScheduleID[ID_SIZE];
    char fileDoctorID[ID_SIZE];
    char fileDate[DATE_SIZE];
    char fileTime[TIME_SIZE];
    char fileStatus[STATUS_SIZE];

    char slotIDs[MAX_RECORDS][ID_SIZE];
    char slotsDate[DATE_SIZE][DATE_SIZE];
    char slotsTime[TIME_SIZE][TIME_SIZE];

    int slot_count = 0;
    int slot_choice;

    fp = fopen(SCHEDULE_FILE, "r");

    if (fp == NULL)
    {
        printf("Doctor schedule file not found.\n");
        pressEnter();
        return 0;
    }

    printf("\nAvailable Slots:\n");

    while (fscanf(fp,
                  "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  fileScheduleID,
                  fileDoctorID,
                  fileDate,
                  fileTime,
                  fileStatus) == 5)
    {
        if (strcmp(fileDoctorID, doctorID) == 0 &&
            strcmp(fileStatus, "AVAILABLE") == 0)
        {
            printf("%d. %s - %s\n",
                   slot_count + 1,
                   fileDate,
                   fileTime);

            strcpy(slotIDs[slot_count], fileScheduleID);
            strcpy(slotsDate[slot_count], fileDate);
            strcpy(slotsTime[slot_count], fileTime);

            slot_count++;
        }
    }

    fclose(fp);

    if (slot_count == 0)
    {
        printf("No available slots.\n");
        pressEnter();
        return 0;
    }

    while (1)
    {
        slot_choice = getIntInput("Choose slot (0 to return): ");

        if (slot_choice == 0)
            return 0;

        if (slot_choice >= 1 && slot_choice <= slot_count)
            break;

        printf("Invalid slot selection.\n");
    }

    strcpy(selectedScheduleID, slotIDs[slot_choice - 1]);
    strcpy(selectedDate, slotsDate[slot_choice - 1]);
    strcpy(selectedTime, slotsTime[slot_choice - 1]);

    return 1;
}

void bookAppointment(Patient *currentPatient)
{
    char appointmentID[ID_SIZE];
    char line[300];

    char doctorID[ID_SIZE];
    char service[NAME_SIZE];

    char selectedDate[DATE_SIZE];
    char selectedTime[TIME_SIZE];
    char selectedScheduleID[ID_SIZE];

    char fileDoctorID[ID_SIZE];
    char fileScheduleID[ID_SIZE];
    char fileDate[DATE_SIZE];
    char fileTime[TIME_SIZE];
    char fileStatus[STATUS_SIZE];

    char availableDoctors[MAX_RECORDS][ID_SIZE];
    char doctorNames[MAX_RECORDS][NAME_SIZE];

    int doctor_choice;
    int service_choice;
    int doctorCount = 0;
    int exists;
    int i;

    float price = 0.0;

    generateNextID(APPOINTMENT_FILE,
                "AppointmentID",
                "A",
                appointmentID);

    FILE *fp;

    /* Build available doctor list */
    fp = fopen(SCHEDULE_FILE, "r");

    if (fp == NULL)
    {
        printf("Doctor schedule file not found.\n");
        pressEnter();
        return;
    }

    while (fscanf(fp,
                  "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  fileScheduleID,
                  fileDoctorID,
                  fileDate,
                  fileTime,
                  fileStatus) == 5)
    {
        if (strcmp(fileStatus, "AVAILABLE") == 0)
        {
            exists = 0;

            for (i = 0; i < doctorCount; i++)
            {
                if (strcmp(availableDoctors[i], fileDoctorID) == 0)
                {
                    exists = 1;
                    break;
                }
            }

            if (!exists)
            {
                strcpy(availableDoctors[doctorCount], fileDoctorID);


                getDoctorName(fileDoctorID, doctorNames[doctorCount]);

                doctorCount++;
            }
        }
    }

    fclose(fp);

    if (doctorCount == 0)
    {
        printf("No doctors available.\n");
        pressEnter();
        return;
    }

    /* Doctor selection loop */
    while (1)
    {
        printf("\nSelect Doctor:\n");

        for (i = 0; i < doctorCount; i++)
        {
            printf("%d. %s - %s\n",
                   i + 1,
                   availableDoctors[i],
                   doctorNames[i]);
        }

        doctor_choice = getIntInput("Enter choice (0 to return): ");

        if (doctor_choice == 0)
        {
            return;
        }

        if (doctor_choice < 1 || doctor_choice > doctorCount)
        {
            printf("Invalid doctor selection.\n");
            continue;
        }

        strcpy(doctorID, availableDoctors[doctor_choice - 1]);

        /* Slot selection */
        while (1)
        {
            if (!chooseAvailableSlot(doctorID,
                                       selectedScheduleID,
                                       selectedDate,
                                       selectedTime))
            {
                break;
            }

            /* Service selection */
            while (1)
            {
                printf("\nSelect Service:\n");
                printf("1. Consultation (RM100)\n");
                printf("2. Follow-up (RM50)\n");
                printf("3. Check-up (RM250)\n");
                service_choice = getIntInput("Enter choice: ");

                switch (service_choice)
                {

                    case 1:
                        strcpy(service, "Consultation");
                        price = 100.00;
                        break;

                    case 2:
                        strcpy(service, "Follow-up");
                        price = 50.00;
                        break;

                    case 3:
                        strcpy(service, "Check-up");
                        price = 250.00;
                        break;

                    default:
                        printf("Invalid service selection.\n");
                        continue;
                }


                /* Save appointment */
                fp = fopen(APPOINTMENT_FILE, "a");

                if (fp == NULL)
                {
                    printf("Error opening appointments file.\n");
                    pressEnter();
                    return;
                }

                fprintf(fp,
                        "%s|%s|%s|%s|%.2f|%s|%s|UPCOMING|UNPAID\n",
                        appointmentID,
                        currentPatient->id,
                        doctorID,
                        service,
                        price,
                        selectedDate,
                        selectedTime);

                fclose(fp);

                /* Update doctor schedule */
                fp = fopen(SCHEDULE_FILE, "r");
                FILE *temp_fp = fopen(TEMP_FILE, "w");

                if (fp == NULL || temp_fp == NULL)
                {
                    printf("Error updating doctor schedule.\n");
                    pressEnter();
                    return;
                }

                while (fscanf(fp,
                              "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                              fileScheduleID,
                              fileDoctorID,
                              fileDate,
                              fileTime,
                              fileStatus) == 5)
                {
                    if (strcmp(fileScheduleID, selectedScheduleID) == 0)
                    {
                        fprintf(temp_fp,
                                "%s|%s|%s|%s|BOOKED\n",
                                fileScheduleID,
                                fileDoctorID,
                                fileDate,
                                fileTime);
                    }
                    else
                    {
                        fprintf(temp_fp,
                                "%s|%s|%s|%s|%s\n",
                                fileScheduleID,
                                fileDoctorID,
                                fileDate,
                                fileTime,
                                fileStatus);
                    }
                }

                fclose(fp);
                fclose(temp_fp);

                remove(SCHEDULE_FILE);
                rename(TEMP_FILE, SCHEDULE_FILE);

                printf("Appointment booked successfully!\n");
                pressEnter();
                return;
            }
        }
    }
}

void manageAppointment(Patient *currentPatient)
{
    FILE *fp;

    char line[300];

    char appointmentID[ID_SIZE];
    char patientID[ID_SIZE];
    char doctorID[ID_SIZE];
    char doctorName[NAME_SIZE];
    char service[50];
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    char appointmentStatus[STATUS_SIZE];
    char paymentStatus[STATUS_SIZE];

    float price;

    char appointmentIDs[MAX_RECORDS][ID_SIZE];
    char doctorIDs[MAX_RECORDS][ID_SIZE];
    char dates[MAX_RECORDS][DATE_SIZE];
    char times[MAX_RECORDS][TIME_SIZE];

    int choice;
    int action;
    int count = 0;
    int found = 0;

    fp = fopen(APPOINTMENT_FILE, "r");

    if (fp == NULL)
    {
        printf("No appointments found.\n");
        pressEnter();
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "AppointmentID", 13) == 0)
        {
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^\n]",
                   appointmentID,
                   patientID,
                   doctorID,
                   service,
                   &price,
                   date,
                   time,
                   appointmentStatus,
                   paymentStatus) == 9)
        {
            if (strcmp(patientID, currentPatient->id) == 0 &&
                strcmp(appointmentStatus, "UPCOMING") == 0)
            {
                if (!found)
                {
                    printf("\n--- Manage Appointments ---\n");

                    printf("------------------------------------------------------------------------------------------------------\n");

                    printf("| %-2s | %-6s | %-20s | %-18s | %-8s | %-10s | %-5s | %-8s |\n",
                           "No",
                           "App ID",
                           "Doctor",
                           "Service",
                           "Price",
                           "Date",
                           "Time",
                           "Payment");

                    printf("------------------------------------------------------------------------------------------------------\n");
                }

                getDoctorName(doctorID, doctorName);

                printf("| %-2d | %-6s | %-20s | %-18s | RM%-5.2f | %-10s | %-5s | %-8s |\n",
                       count + 1,
                       appointmentID,
                       doctorName,
                       service,
                       price,
                       date,
                       time,
                       paymentStatus);

                strcpy(appointmentIDs[count], appointmentID);
                strcpy(doctorIDs[count], doctorID);
                strcpy(dates[count], date);
                strcpy(times[count], time);

                count++;
                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No upcoming appointments available.\n");
        pressEnter();
        return;
    }

    printf("------------------------------------------------------------------------------------------------------\n");

    choice = getIntInput("Choose appointment (0 to return): ");

    if (choice == 0)
    {
        return;
    }

    if (choice < 1 || choice > count)
    {
        printf("Invalid selection.\n");
        pressEnter();
        return;
    }

    while (1)
    {
        printf("\n1. Reschedule Appointment\n");
        printf("2. Cancel Appointment\n");
        action = getIntInput("Enter choice (0 to return): ");

        switch (action)
        {
            case 1:
                rescheduleAppointment(appointmentIDs[choice - 1],
                                       doctorIDs[choice - 1],
                                       dates[choice - 1],
                                       times[choice - 1]);
                return;

            case 2:
            {
                char confirm;

                printf("\nAre you sure you want to cancel this appointment? (Y/N): ");
                scanf(" %c", &confirm);

                if (confirm == 'Y' || confirm == 'y')
                {
                    cancelAppointment(appointmentIDs[choice - 1],
                                       doctorIDs[choice - 1],
                                       dates[choice - 1],
                                       times[choice - 1]);
                    return;
                }
                else if (confirm == 'N' || confirm == 'n')
                {
                    break;
                }
                else
                {
                    printf("Invalid input.\n");
                }

                break;
            }

            case 0:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}


void rescheduleAppointment(char targetAppointmentID[],
                            char targetDoctorID[],
                            char oldDate[],
                            char oldTime[])
{
    FILE *fp;
    FILE *temp_fp;

    char line[300];

    char fileScheduleID[ID_SIZE];
    char fileDoctorID[ID_SIZE];
    char fileDate[DATE_SIZE];
    char fileTime[TIME_SIZE];
    char fileStatus[STATUS_SIZE];

    char appointmentID[ID_SIZE];
    char patientID[ID_SIZE];
    char doctorID[ID_SIZE];
    char service[50];
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    char appointmentStatus[STATUS_SIZE];
    char paymentStatus[STATUS_SIZE];

    float price;

    char newDate[DATE_SIZE];
    char newTime[TIME_SIZE];
    char newScheduleID[ID_SIZE];
    
    /* Choose new slot first */

    if (!chooseAvailableSlot(targetDoctorID, newScheduleID, newDate, newTime))
    {
        return;
    }

    /* Update doctor schedule */

    fp = fopen(SCHEDULE_FILE, "r");
    temp_fp = fopen(TEMP_FILE, "w");

    if (fp == NULL || temp_fp == NULL)
    {
        printf("Error opening schedule file.\n");
        return;
    }

    while (fscanf(fp,
                "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                fileScheduleID,
                fileDoctorID,
                fileDate,
                fileTime,
                fileStatus) == 5)
    {
        if (strcmp(fileDoctorID, targetDoctorID) == 0 &&
            strcmp(fileDate, oldDate) == 0 &&
            strcmp(fileTime, oldTime) == 0)
        {
            fprintf(temp_fp,
                    "%s|%s|%s|%s|AVAILABLE\n",
                    fileScheduleID,
                    fileDoctorID,
                    fileDate,
                    fileTime);      
        }
        else if (strcmp(fileScheduleID, newScheduleID) == 0)
        {
            fprintf(temp_fp,
                    "%s|%s|%s|%s|BOOKED\n",
                    fileScheduleID,
                    fileDoctorID,
                    fileDate,
                    fileTime);
        }
        else
        {
            fprintf(temp_fp,
                    "%s|%s|%s|%s|%s\n",
                    fileScheduleID,
                    fileDoctorID,
                    fileDate,
                    fileTime,
                    fileStatus);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(SCHEDULE_FILE);
    rename(TEMP_FILE, SCHEDULE_FILE);

    /* Update appointment */

    fp = fopen(APPOINTMENT_FILE, "r");
    temp_fp = fopen(TEMP_FILE, "w");

    if (fp == NULL || temp_fp == NULL)
    {
        printf("Error opening appointments file.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "AppointmentID", 13) == 0)
        {
            fputs(line, temp_fp);
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^\n]",
                   appointmentID,
                   patientID,
                   doctorID,
                   service,
                   &price,
                   date,
                   time,
                   appointmentStatus,
                   paymentStatus) == 9)
        {
            if (strcmp(appointmentID, targetAppointmentID) == 0)
            {
                fprintf(temp_fp,
                        "%s|%s|%s|%s|%.2f|%s|%s|%s|%s\n",
                        appointmentID,
                        patientID,
                        doctorID,
                        service,
                        price,
                        newDate,
                        newTime,
                        appointmentStatus,
                        paymentStatus);
            }
            else
            {
                fputs(line, temp_fp);
            }
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(APPOINTMENT_FILE);
    rename(TEMP_FILE, APPOINTMENT_FILE);

    printf("Appointment rescheduled successfully!\n");
    pressEnter();
}


void cancelAppointment(char targetAppointmentID[],
                        char targetDoctorID[],
                        char appointmentDate[],
                        char appointmentTime[])
{
    FILE *fp;
    FILE *temp_fp;

    char line[300];

    char fileScheduleID[ID_SIZE];
    char fileDoctorID[ID_SIZE];
    char fileDate[DATE_SIZE];
    char fileTime[TIME_SIZE];
    char fileStatus[STATUS_SIZE];

    char appointmentID[ID_SIZE];
    char patientID[ID_SIZE];
    char doctorID[ID_SIZE];
    char service[50];
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    char appointmentStatus[STATUS_SIZE];
    char paymentStatus[STATUS_SIZE];

    float price;

    /* Update appointment */

    fp = fopen(APPOINTMENT_FILE, "r");
    temp_fp = fopen(TEMP_FILE, "w");

    if (fp == NULL || temp_fp == NULL)
    {
        printf("Error opening appointments file.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "AppointmentID", 13) == 0)
        {
            fputs(line, temp_fp);
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^\n]",
                   appointmentID,
                   patientID,
                   doctorID,
                   service,
                   &price,
                   date,
                   time,
                   appointmentStatus,
                   paymentStatus) == 9)
        {
            if (strcmp(appointmentID, targetAppointmentID) == 0)
            {
                fprintf(temp_fp,
                        "%s|%s|%s|%s|%.2f|%s|%s|CANCELLED|%s\n",
                        appointmentID,
                        patientID,
                        doctorID,
                        service,
                        price,
                        date,
                        time,
                        paymentStatus);
            }
            else
            {
                fputs(line, temp_fp);
            }
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(APPOINTMENT_FILE);
    rename(TEMP_FILE, APPOINTMENT_FILE);

    /* Free doctor slot */

    fp = fopen(SCHEDULE_FILE, "r");
    temp_fp = fopen(TEMP_FILE, "w");

    if (fp == NULL || temp_fp == NULL)
    {
        printf("Error opening schedule file.\n");
        return;
    }

    while (fscanf(fp,
                "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                fileScheduleID,
                fileDoctorID,
                fileDate,
                fileTime,
                fileStatus) == 5)
    {
        if (strcmp(fileDoctorID, targetDoctorID) == 0 &&
            strcmp(fileDate, appointmentDate) == 0 &&
            strcmp(fileTime, appointmentTime) == 0)
        {
            fprintf(temp_fp,
                    "%s|%s|%s|%s|AVAILABLE\n",
                    fileScheduleID,
                    fileDoctorID,
                    fileDate,
                    fileTime);
        }
        else
        {
            fprintf(temp_fp,
                    "%s|%s|%s|%s|%s\n",
                    fileScheduleID,
                    fileDoctorID,
                    fileDate,
                    fileTime,
                    fileStatus);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(SCHEDULE_FILE);
    rename(TEMP_FILE, SCHEDULE_FILE);

    printf("Appointment cancelled successfully!\n");
    getchar();
    pressEnter();
}








// /* ================= PAYMENT ================= */

void paymentMenu(Patient *currentPatient)
{
    int choice;

    do {
        printf("\n--- Payment Menu ---\n");
        printf("1. Make payment\n");
        printf("2. View payment\n");
        printf("3. Search payment\n");

        choice = getIntInput("Enter choice (0 to return): ");

        switch (choice) {
            case 0:
                return;
            case 1:
                makePayment(currentPatient);
                break;
            case 2:
                viewPayment(currentPatient);
                break;
            case 3:
                searchPayment(currentPatient);
                break;
            default:
                printf("Invalid choice\n");
        }

    } while (choice != 0);
}


void makePayment(Patient *currentPatient)
{
    FILE *fp;
    FILE *temp_fp;
    FILE *payment_fp;

    char line[300];

    char appointmentID[ID_SIZE];
    char patientID[ID_SIZE];
    char doctorID[ID_SIZE];
    char doctorName[NAME_SIZE];
    char service[50];
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    char appointmentStatus[STATUS_SIZE];
    char paymentStatus[STATUS_SIZE];

    char appointmentIDs[MAX_RECORDS][ID_SIZE];
    float prices[MAX_RECORDS];

    char paymentID[ID_SIZE];
    char paymentDate[DATE_SIZE];
    char confirm;

    int count = 0;
    int found = 0;
    int choice;

    float price;

    fp = fopen(APPOINTMENT_FILE, "r");

    if (fp == NULL)
    {
        printf("No appointments found.\n");
        pressEnter();
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "AppointmentID", 13) == 0)
        {
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^\n]",
                   appointmentID,
                   patientID,
                   doctorID,
                   service,
                   &price,
                   date,
                   time,
                   appointmentStatus,
                   paymentStatus) == 9)
        {
            if (strcmp(patientID, currentPatient->id) == 0 &&
                strcmp(paymentStatus, "UNPAID") == 0 &&
                strcmp(appointmentStatus, "CANCELLED") != 0)
            {
                if (!found)
                {
                    printf("\n--- Make Payment ---\n");

                    printf("---------------------------------------------------------------------------------------------\n");

                    printf("| %-3s | %-10s | %-15s | %-15s | %-8s | %-12s | %-8s |\n",
                           "No",
                           "App ID",
                           "Doctor",
                           "Service",
                           "Amount",
                           "Date",
                           "Time");

                    printf("---------------------------------------------------------------------------------------------\n");
                }

                getDoctorName(doctorID, doctorName);

                printf("| %-3d | %-10s | %-15s | %-15s | RM%-5.2f | %-12s | %-8s |\n",
                       count + 1,
                       appointmentID,
                       doctorName,
                       service,
                       price,
                       date,
                       time);

                strcpy(appointmentIDs[count], appointmentID);
                prices[count] = price;

                count++;
                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No unpaid appointments found.\n");
        pressEnter();
        return;
    }

    printf("---------------------------------------------------------------------------------------------\n");

    choice = getIntInput("Choose appointment to pay (0 to return): ");

    if (choice == 0)
    {
        return;
    }

    if (choice < 1 || choice > count)
    {
        printf("Invalid selection.\n");
        pressEnter();
        return;
    }

    printf("Confirm payment of RM%.2f? (Y/N): ", prices[choice - 1]);
    scanf(" %c", &confirm);

    if (confirm == 'N' || confirm == 'n')
    {
        return;
    }

    if (confirm != 'Y' && confirm != 'y')
    {
        printf("Invalid input.\n");
        pressEnter();
        return;
    }

    printf("Enter payment date (YYYY-MM-DD): ");
    scanf("%14s", paymentDate);

    generateNextID(PAYMENT_FILE,
                   "PaymentID",
                   "PAY",
                   paymentID);

    payment_fp = fopen(PAYMENT_FILE, "a");

    if (payment_fp == NULL)
    {
        printf("Error opening payments file.\n");
        return;
    }

    fprintf(payment_fp,
            "%s|%s|%s|%.2f|%s\n",
            paymentID,
            appointmentIDs[choice - 1],
            currentPatient->id,
            prices[choice - 1],
            paymentDate);

    fclose(payment_fp);

    fp = fopen(APPOINTMENT_FILE, "r");
    temp_fp = fopen(TEMP_FILE, "w");

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "AppointmentID", 13) == 0)
        {
            fputs(line, temp_fp);
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^\n]",
                   appointmentID,
                   patientID,
                   doctorID,
                   service,
                   &price,
                   date,
                   time,
                   appointmentStatus,
                   paymentStatus) == 9)
        {
            if (strcmp(appointmentID, appointmentIDs[choice - 1]) == 0)
            {
                fprintf(temp_fp,
                        "%s|%s|%s|%s|%.2f|%s|%s|%s|PAID\n",
                        appointmentID,
                        patientID,
                        doctorID,
                        service,
                        price,
                        date,
                        time,
                        appointmentStatus);
            }
            else
            {
                fputs(line, temp_fp);
            }
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(APPOINTMENT_FILE);
    rename(TEMP_FILE, APPOINTMENT_FILE);

    printf("Payment successful!\n");
    getchar();
    pressEnter();
}


void viewPayment(Patient *currentPatient)
{
    FILE *fp;

    char line[TEXT_SIZE];

    char paymentID[ID_SIZE];
    char appointmentID[ID_SIZE];
    char patientID[ID_SIZE];
    char paymentDate[DATE_SIZE];

    float amount;

    int found = 0;

    fp = fopen(PAYMENT_FILE, "r");

    if (fp == NULL)
    {
        printf("No payment records found.\n");
        pressEnter();
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "PaymentID", 9) == 0)
        {
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%f|%[^\n]",
                   paymentID,
                   appointmentID,
                   patientID,
                   &amount,
                   paymentDate) == 5)
        {
            if (strcmp(patientID, currentPatient->id) == 0)
            {
                if (!found)
                {
                    printf("\n--- Payment History ---\n");

                    printf("-----------------------------------------------------------\n");

                    printf("| %-10s | %-12s | %-12s | %-12s |\n",
                           "Payment ID",
                           "App ID",
                           "Amount",
                           "Payment Date");

                    printf("-----------------------------------------------------------\n");
                }

                printf("| %-10s | %-12s | RM%-10.2f | %-12s |\n",
                       paymentID,
                       appointmentID,
                       amount,
                       paymentDate);

                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No payment history found.\n");
        pressEnter();
        return;
    }

    printf("-----------------------------------------------------------\n");

    pressEnter();
}


void searchPayment(Patient *currentPatient)
{
    FILE *fp;

    char line[300];

    char searchID[ID_SIZE];

    char paymentID[ID_SIZE];
    char appointmentID[ID_SIZE];
    char patientID[ID_SIZE];
    char paymentDate[DATE_SIZE];

    float amount;

    int found = 0;

    fp = fopen(PAYMENT_FILE, "r");

    if (fp == NULL)
    {
        printf("No payment records found.\n");
        pressEnter();
        return;
    }

    printf("\nEnter Payment ID to search (case sensitive): ");
    scanf("%14s", searchID);
    getchar();
    
    printf("\n--- Search Result ---\n");

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "PaymentID", 9) == 0)
        {
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%f|%[^\n]",
                   paymentID,
                   appointmentID,
                   patientID,
                   &amount,
                   paymentDate) == 5)
        {
            if (strcmp(paymentID, searchID) == 0 &&
                strcmp(patientID, currentPatient->id) == 0)
            {
                printf("-----------------------------------------------------------\n");

                printf("| %-10s | %-12s | %-12s | %-12s |\n",
                       "Payment ID",
                       "App ID",
                       "Amount",
                       "Payment Date");

                printf("-----------------------------------------------------------\n");

                printf("| %-10s | %-12s | RM%-10.2f | %-12s |\n",
                       paymentID,
                       appointmentID,
                       amount,
                       paymentDate);

                printf("-----------------------------------------------------------\n");

                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        printf("Payment record not found.\n");
    }

    fclose(fp);
    pressEnter();
}







// /* ================= COMPLAINT ================= */

void complaintMenu(Patient *currentPatient)
{
    int choice;

    do {
        printf("\n--- Complaint Menu ---\n");
        
        printf("Patient: %s\n", currentPatient->name);

        printf("1. Add complaint\n");
        printf("2. View complaint\n");
        printf("3. Search complaint\n");

        choice = getIntInput("Enter choice (0 to return): ");

        switch (choice) {
            case 0:
                return;
            case 1:
                addComplaint(currentPatient);
                break;
            case 2:
                viewComplaint(currentPatient);
                break;
            case 3:
                searchComplaint(currentPatient);
                break;
            default:
                printf("Invalid choice\n");
                
        }

    } while (choice != 0);
}


void addComplaint(Patient *currentPatient) 
{
    FILE *fp;

    char complaint[TEXT_SIZE];
    char complaintID[ID_SIZE];

    int is_empty;
    int i;

    /* =========================
       GET LAST COMPLAINT ID
       ========================= */

    generateNextID(COMPLAINT_FILE,
               "ComplaintID",
               "C",
               complaintID);

    /* =========================
       GET COMPLAINT
       ========================= */

    printf("\nComplaint ID: %s\n", complaintID);

    while (1)
    {
        printf("Enter complaint: ");

        fgets(complaint, sizeof(complaint), stdin);

        complaint[strcspn(complaint, "\n")] = '\0';

        is_empty = 1;

        for (i = 0; complaint[i] != '\0'; i++)
        {
            if (complaint[i] != ' ' &&
                complaint[i] != '\t')
            {
                is_empty = 0;
                break;
            }
        }

        if (is_empty)
        {
            printf("Complaint cannot be empty.\n");
            continue;
        }

        break;
    }

    /* =========================
       SAVE
       ========================= */

    fp = fopen(COMPLAINT_FILE, "a");

    if (fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(fp,
            "%s|%s|%s|%s\n",
            complaintID,
            currentPatient->id,
            currentPatient->name,
            complaint);

    fclose(fp);

    printf("Complaint submitted!\n");
    pressEnter();
}


void viewComplaint(Patient *currentPatient)
{
    FILE *fp;

    char complaintID[ID_SIZE];
    char patientID[ID_SIZE];
    char name[NAME_SIZE];
    char complaint[TEXT_SIZE];
    char line[300];

    int found = 0;

    fp = fopen(COMPLAINT_FILE, "r");

    if (fp == NULL)
    {
        printf("No complaints found.\n");
        pressEnter();
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "ComplaintID", 11) == 0)
        {
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^\n]",
                   complaintID,
                   patientID,
                   name,
                   complaint) == 4)
        {
            if (strcmp(patientID, currentPatient->id) == 0)
            {
                if (!found)
                {
                    printf("\n--- My Complaints ---\n");

                    printf("----------------------------------------------------------------------------------\n");

                    printf("| %-12s | %-12s | %-15s | %-30s |\n",
                           "Complaint ID",
                           "Patient ID",
                           "Name",
                           "Complaint");

                    printf("----------------------------------------------------------------------------------\n");
                }

                printf("| %-12s | %-12s | %-15s | %-30s |\n",
                       complaintID,
                       patientID,
                       name,
                       complaint);

                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No complaints found.\n");
        pressEnter();
        return;
    }

    printf("----------------------------------------------------------------------------------\n");

    pressEnter();
}


void searchComplaint(Patient *currentPatient)
{
    FILE *fp;

    char line[300];

    char searchID[ID_SIZE];
    char complaintID[ID_SIZE];
    char patientID[ID_SIZE];
    char name[NAME_SIZE];
    char complaint[TEXT_SIZE];

    int found = 0;

    fp = fopen(COMPLAINT_FILE, "r");

    if (fp == NULL)
    {
        printf("No complaints found.\n");
        pressEnter();
        return;
    }

    printf("\nEnter Complaint ID to search (case sensitive): ");
    scanf("%19s", searchID);
    getchar();
    
    printf("\n--- Search Result ---\n");

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "ComplaintID", 11) == 0)
        {
            continue;
        }

        if (sscanf(line,
                   "%[^|]|%[^|]|%[^|]|%[^\n]",
                   complaintID,
                   patientID,
                   name,
                   complaint) == 4)
        {
            if (strcmp(complaintID, searchID) == 0 &&
                strcmp(patientID, currentPatient->id) == 0)
            {
                printf("---------------------------------------------------------------------------------------------\n");

                printf("| %-15s | %-15s | %-20s | %-30s |\n",
                       "ComplaintID",
                       "PatientID",
                       "Name",
                       "Complaint");

                printf("---------------------------------------------------------------------------------------------\n");

                printf("| %-15s | %-15s | %-20s | %-30s |\n",
                       complaintID,
                       patientID,
                       name,
                       complaint);

                printf("---------------------------------------------------------------------------------------------\n");

                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        printf("Complaint not found.\n");
    }

    fclose(fp);

    pressEnter();
}