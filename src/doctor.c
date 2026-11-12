#include "main.h"

void getLine(char message[], char value[], int size) {
    int i;

    printf("%s", message);

    if (fgets(value, size, stdin) == NULL) {
        value[0] = '\0';
        return;
    }

    value[strcspn(value, "\n")] = '\0';

    for (i = 0; value[i] != '\0'; i++) {
        if (value[i] == '|') {
            value[i] = '/';
        }
    }
}

int getChoice(char message[]) {
    char input[20];
    int choice;

    printf("%s", message);

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }

    return choice;
}

int validDate(char date[]) {
    int i, month, day;

    if (strlen(date) != 10) {
        return 0;
    }

    if (date[4] != '-' || date[7] != '-') {
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && !isdigit((unsigned char)date[i])) {
            return 0;
        }
    }

    month = (date[5] - '0') * 10 + (date[6] - '0');
    day = (date[8] - '0') * 10 + (date[9] - '0');

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }

    return 1;
}

int validTime(char time[]) {
    int i, hour, minute;

    if (strlen(time) != 5 || time[2] != ':') {
        return 0;
    }

    for (i = 0; i < 5; i++) {
        if (i != 2 && !isdigit((unsigned char)time[i])) {
            return 0;
        }
    }

    hour = (time[0] - '0') * 10 + (time[1] - '0');
    minute = (time[3] - '0') * 10 + (time[4] - '0');

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return 0;
    }

    return 1;
}

void generateID(char filename[], char prefix[], char newID[]) {
    FILE *fp;
    char line[LINE], id[ID_SIZE];
    int max = 0, number;
    int prefixLength = strlen(prefix);

    fp = fopen(filename, "r");

    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp) != NULL) {
            if (sscanf(line, "%[^|]", id) == 1) {
                if (strncmp(id, prefix, prefixLength) == 0) {
                    number = atoi(id + prefixLength);

                    if (number > max) {
                        max = number;
                    }
                }
            }
        }

        fclose(fp);
    }

    sprintf(newID, "%s%03d", prefix, max + 1);
}

int parseDoctor(char line[], Doctor *d) {
    char *token;

    token = strtok(line, "|\n");
    if (token == NULL) return 0;
    strcpy(d->id, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->name, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->speciality, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->password, token);

    return 1;
}

int parsePatient(char line[], Patient *p) {
    char *token;

    token = strtok(line, "|\n");
    if (token == NULL) return 0;
    strcpy(p->id, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(p->name, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(p->phone, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(p->password, token);

    return 1;
}

int parseSchedule(char line[], Schedule *s) {
    char *token;

    token = strtok(line, "|\n");
    if (token == NULL) return 0;
    strcpy(s->id, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(s->doctorID, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(s->date, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(s->time, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(s->status, token);

    return 1;
}

int parseDiagnosis(char line[], Diagnosis *d) {
    char *token;

    token = strtok(line, "|\n");
    if (token == NULL) return 0;
    strcpy(d->id, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->doctorID, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->patientID, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->date, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->diagnosis, token);

    token = strtok(NULL, "|\n");
    if (token == NULL) return 0;
    strcpy(d->summary, token);

    return 1;
}

int patientExists(char patientID[]) {
    FILE *fp;
    char line[LINE];
    Patient p;

    fp = fopen(PATIENT_FILE, "r");

    if (fp == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parsePatient(line, &p) && strcmp(p.id, patientID) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void getPatientName(char patientID[], char patientName[]) {
    FILE *fp;
    char line[LINE];
    Patient p;

    strcpy(patientName, "Unknown");

    fp = fopen(PATIENT_FILE, "r");

    if (fp == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parsePatient(line, &p) && strcmp(p.id, patientID) == 0) {
            strcpy(patientName, p.name);
            fclose(fp);
            return;
        }
    }

    fclose(fp);
}

int checkDoctorLogin(Doctor *loggedDoctor) {
    FILE *fp;
    char inputID[ID_SIZE], inputPassword[PASS_SIZE], line[LINE];
    Doctor d;

    getLine("Doctor ID: ", inputID, sizeof(inputID));
    getLine("Password : ", inputPassword, sizeof(inputPassword));

    fp = fopen(DOCTOR_FILE, "r");

    if (fp == NULL) {
        printf("doctors.txt not found.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseDoctor(line, &d)) {
            if (strcmp(d.id, inputID) == 0 && strcmp(d.password, inputPassword) == 0) {
                *loggedDoctor = d;
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    printf("Invalid login.\n");
    return 0;
}

void doctorLogin() {
    Doctor doctor;

    if (checkDoctorLogin(&doctor)) {
        printf("Login successful. Welcome, %s.\n", doctor.name);
        doctorMenu(doctor);
    }
}

void addSchedule(Doctor doctor) {
    FILE *fp;
    Schedule s;

    generateID(SCHEDULE_FILE, "S", s.id);

    strcpy(s.doctorID, doctor.id);
    strcpy(s.status, "AVAILABLE");

    do {
        getLine("Enter date (YYYY-MM-DD): ", s.date, sizeof(s.date));

        if (!validDate(s.date)) {
            printf("Invalid date format.\n");
        }
    } while (!validDate(s.date));

    do {
        getLine("Enter time (HH:MM): ", s.time, sizeof(s.time));

        if (!validTime(s.time)) {
            printf("Invalid time format.\n");
        }
    } while (!validTime(s.time));

    fp = fopen(SCHEDULE_FILE, "a");

    if (fp == NULL) {
        printf("Unable to open schedule.txt.\n");
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s\n", s.id, s.doctorID, s.date, s.time, s.status);

    fclose(fp);

    printf("Schedule added successfully. Schedule ID: %s\n", s.id);
}

void printSchedule(Schedule s) {
    printf("%-6s %-8s %-12s %-8s %-10s\n", s.id, s.doctorID, s.date, s.time, s.status);
}

void viewSchedules(Doctor doctor) {
    FILE *fp;
    char line[LINE];
    Schedule s;
    ScheduleNode *head = NULL;
    ScheduleNode *tail = NULL;
    ScheduleNode *newNode;
    ScheduleNode *current;
    int found = 0;

    fp = fopen(SCHEDULE_FILE, "r");

    if (fp == NULL) {
        printf("schedule.txt not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseSchedule(line, &s) && strcmp(s.doctorID, doctor.id) == 0) {
            newNode = (ScheduleNode *)malloc(sizeof(ScheduleNode));

            if (newNode == NULL) {
                printf("Memory allocation failed.\n");
                fclose(fp);
                return;
            }

            newNode->data = s;
            newNode->next = NULL;

            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    fclose(fp);

    printf("\nMy Schedules\n");
    printf("%-6s %-8s %-12s %-8s %-10s\n", "ID", "Doctor", "Date", "Time", "Status");

    current = head;

    while (current != NULL) {
        printSchedule(current->data);
        found = 1;
        current = current->next;
    }

    current = head;

    while (current != NULL) {
        ScheduleNode *temp = current;
        current = current->next;
        free(temp);
    }

    if (!found) {
        printf("No schedule found.\n");
    }
}

void searchSchedule(Doctor doctor) {
    FILE *fp;
    char line[LINE];
    Schedule s;
    SearchKey key;
    int found = 0;

    getLine("Enter Schedule ID / Date / Status to search: ", key.text, sizeof(key.text));

    fp = fopen(SCHEDULE_FILE, "r");

    if (fp == NULL) {
        printf("schedule.txt not found.\n");
        return;
    }

    printf("\nSearch Result\n");
    printf("%-6s %-8s %-12s %-8s %-10s\n", "ID", "Doctor", "Date", "Time", "Status");

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseSchedule(line, &s) && strcmp(s.doctorID, doctor.id) == 0) {
            if (strstr(s.id, key.text) || strstr(s.date, key.text) || strstr(s.status, key.text)) {
                printSchedule(s);
                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found) {
        printf("No matching schedule found.\n");
    }
}

void addDiagnosis(Doctor doctor) {
    FILE *fp;
    Diagnosis d;

    generateID(DIAGNOSIS_FILE, "DX", d.id);

    strcpy(d.doctorID, doctor.id);

    do {
        getLine("Enter Patient ID: ", d.patientID, sizeof(d.patientID));

        if (!patientExists(d.patientID)) {
            printf("Patient ID not found.\n");
        }
    } while (!patientExists(d.patientID));

    do {
        getLine("Enter date (YYYY-MM-DD): ", d.date, sizeof(d.date));

        if (!validDate(d.date)) {
            printf("Invalid date format.\n");
        }
    } while (!validDate(d.date));

    getLine("Enter diagnosis: ", d.diagnosis, sizeof(d.diagnosis));
    getLine("Enter summary: ", d.summary, sizeof(d.summary));

    fp = fopen(DIAGNOSIS_FILE, "a");

    if (fp == NULL) {
        printf("Unable to open diagnosis.txt.\n");
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s|%s\n", d.id, d.doctorID, d.patientID, d.date, d.diagnosis, d.summary);

    fclose(fp);

    printf("Diagnostic report added successfully. Diagnosis ID: %s\n", d.id);
}

void printDiagnosis(Diagnosis d) {
    char patientName[NAME_SIZE];

    getPatientName(d.patientID, patientName);

    printf("\nDiagnosis ID : %s\n", d.id);
    printf("Patient      : %s (%s)\n", patientName, d.patientID);
    printf("Date         : %s\n", d.date);
    printf("Diagnosis    : %s\n", d.diagnosis);
    printf("Summary      : %s\n", d.summary);
}

void viewDiagnosis(Doctor doctor) {
    FILE *fp;
    char line[LINE];
    Diagnosis d;
    int found = 0;

    fp = fopen(DIAGNOSIS_FILE, "r");

    if (fp == NULL) {
        printf("diagnosis.txt not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseDiagnosis(line, &d) && strcmp(d.doctorID, doctor.id) == 0) {
            printDiagnosis(d);
            found = 1;
        }
    }

    fclose(fp);

    if (!found) {
        printf("No diagnostic report found.\n");
    }
}

void searchByPatient(Doctor doctor) {
    FILE *fp;
    char line[LINE];
    char patientName[NAME_SIZE];
    Diagnosis d;
    SearchKey key;
    int found = 0;

    getLine("Enter Patient ID or Name: ", key.text, sizeof(key.text));

    fp = fopen(DIAGNOSIS_FILE, "r");

    if (fp == NULL) {
        printf("diagnosis.txt not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseDiagnosis(line, &d) && strcmp(d.doctorID, doctor.id) == 0) {
            getPatientName(d.patientID, patientName);

            if (strstr(d.patientID, key.text) || strstr(patientName, key.text)) {
                printDiagnosis(d);
                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found) {
        printf("No matching report found.\n");
    }
}

void searchByDateRange(Doctor doctor) {
    FILE *fp;
    char line[LINE];
    char startDate[DATE_SIZE];
    char endDate[DATE_SIZE];
    Diagnosis d;
    int found = 0;

    do {
        getLine("Enter start date (YYYY-MM-DD): ", startDate, sizeof(startDate));

        if (!validDate(startDate)) {
            printf("Invalid start date format.\n");
        }
    } while (!validDate(startDate));

    do {
        getLine("Enter end date (YYYY-MM-DD): ", endDate, sizeof(endDate));

        if (!validDate(endDate)) {
            printf("Invalid end date format.\n");
        }
    } while (!validDate(endDate));

    fp = fopen(DIAGNOSIS_FILE, "r");

    if (fp == NULL) {
        printf("diagnosis.txt not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseDiagnosis(line, &d) && strcmp(d.doctorID, doctor.id) == 0) {
            if (strcmp(d.date, startDate) >= 0 && strcmp(d.date, endDate) <= 0) {
                printDiagnosis(d);
                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found) {
        printf("No matching report found.\n");
    }
}

void searchByDiagnosis(Doctor doctor) {
    FILE *fp;
    char line[LINE];
    Diagnosis d;
    SearchKey key;
    int found = 0;

    getLine("Enter diagnosis keyword: ", key.text, sizeof(key.text));

    fp = fopen(DIAGNOSIS_FILE, "r");

    if (fp == NULL) {
        printf("diagnosis.txt not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (parseDiagnosis(line, &d) && strcmp(d.doctorID, doctor.id) == 0) {
            if (strstr(d.diagnosis, key.text)) {
                printDiagnosis(d);
                found = 1;
            }
        }
    }

    fclose(fp);

    if (!found) {
        printf("No matching report found.\n");
    }
}

void searchDiagnosis(Doctor doctor) {
    int choice;

    printf("\nSearch Diagnostic Report\n");
    printf("1. Search by Patient ID / Name\n");
    printf("2. Search by Date Range\n");
    printf("3. Search by Diagnosis\n");

    choice = getChoice("Enter choice: ");

    if (choice == 1) {
        searchByPatient(doctor);
    } else if (choice == 2) {
        searchByDateRange(doctor);
    } else if (choice == 3) {
        searchByDiagnosis(doctor);
    } else {
        printf("Invalid choice.\n");
    }
}

void doctorMenu(Doctor doctor) {
    int choice;

    do {
        printf("\n=================================\n");
        printf("Doctor Menu - %s (%s)\n", doctor.name, doctor.id);
        printf("=================================\n");
        printf("1. Add Schedule\n");
        printf("2. View My Schedules\n");
        printf("3. Search My Schedule\n");
        printf("4. Add Diagnostic Information\n");
        printf("5. View Diagnostic Reports\n");
        printf("6. Search Diagnostic Reports\n");
        printf("7. Logout\n");

        choice = getChoice("Enter choice: ");

        if (choice == 1) {
            addSchedule(doctor);
        } else if (choice == 2) {
            viewSchedules(doctor);
        } else if (choice == 3) {
            searchSchedule(doctor);
        } else if (choice == 4) {
            addDiagnosis(doctor);
        } else if (choice == 5) {
            viewDiagnosis(doctor);
        } else if (choice == 6) {
            searchDiagnosis(doctor);
        } else if (choice == 7) {
            printf("Logged out successfully.\n");
        } else {
            printf("Invalid choice.\n");
        }

    } while (choice != 7);
}