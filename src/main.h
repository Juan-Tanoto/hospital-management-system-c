#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE 300

/* FILES */
#define PATIENT_FILE "patients.txt"
#define DOCTOR_FILE "doctors.txt"
#define NURSE_FILE "nurses.txt"
#define COMPLAINT_FILE "complaints.txt"
#define INVENTORY_FILE "inventory.txt"
#define NOTES_FILE "notes.txt"
#define SCHEDULE_FILE "schedule.txt"
#define DIAGNOSIS_FILE "diagnosis.txt"
#define APPOINTMENT_FILE "appointments.txt"
#define PAYMENT_FILE "payments.txt"
#define TEMP_FILE "temp.txt"

/* DOCTOR CONSTANTS */
#define ID_SIZE 15
#define NAME_SIZE 50
#define PASS_SIZE 30
#define DATE_SIZE 15
#define TIME_SIZE 10
#define STATUS_SIZE 15
#define TEXT_SIZE 150
#define PHONE_SIZE 20
#define MAX_RECORDS 20
#define MESSAGE_SIZE 100

/* DOCTOR STRUCTURES */
typedef struct {
    char id[ID_SIZE];
    char name[NAME_SIZE];
    char speciality[NAME_SIZE];
    char password[PASS_SIZE];
} Doctor;

typedef struct {
    char id[ID_SIZE];
    char name[NAME_SIZE];
    char phone[PHONE_SIZE];
    char password[PASS_SIZE];
} Patient;

typedef struct {
    char id[ID_SIZE];
    char name[NAME_SIZE];
    char password[PASS_SIZE];
} Nurse;

typedef struct {
    char cid[ID_SIZE];
    char pid[ID_SIZE];
    char pname[NAME_SIZE];
    char message[MESSAGE_SIZE];
} Complaint;

typedef struct {
    char id[ID_SIZE];
    char doctorID[ID_SIZE];
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    char status[STATUS_SIZE];
} Schedule;

typedef struct {
    char id[ID_SIZE];
    char doctorID[ID_SIZE];
    char patientID[ID_SIZE];
    char date[DATE_SIZE];
    char diagnosis[NAME_SIZE];
    char summary[TEXT_SIZE];
} Diagnosis;

typedef union {
    char text[TEXT_SIZE];
    int number;
} SearchKey;

typedef struct ScheduleNode {
    Schedule data;
    struct ScheduleNode *next;
} ScheduleNode;

/* DIRECTOR */
void directorMainMenu();
int loginSystem();
void directorMenu(char* directorName);
void staffReportMenu();
void viewAllStaff();
void searchStaff();
void salesReportMenu();
void viewMasterSchedule();
void backupDatabase();
void backupFile(const char* sourceFileName, const char* backupFileName);
void viewComplaintsDirector();

/* ADMIN */
void adminLogin();
int adminMenu();

void ensureNewline(FILE *fp);
int isDuplicateID(const char *filename, const char *newID);
void deleteRecord(const char *filename, const char *role);

void registerPatient();
void registerDoctor();
void registerNurse();

void viewPatients();
void viewDoctors();
void viewNurses();
void viewComplaintsAdmin();

void searchDoctor();
void searchNurse();
void searchComplaintAdmin();

/* NURSE */
void nurseLogin();
int nurseMenu();

void viewDoctorSchedule();

void addInventory();
void viewInventory();
void searchInventory();

void addNote();
void viewNotes();
void searchNotes();

/* DOCTOR */
void doctorLogin();
int checkDoctorLogin(Doctor *loggedDoctor);
void doctorMenu(Doctor doctor);

void addSchedule(Doctor doctor);
void viewSchedules(Doctor doctor);
void searchSchedule(Doctor doctor);

void addDiagnosis(Doctor doctor);
void viewDiagnosis(Doctor doctor);
void searchDiagnosis(Doctor doctor);

// Utilities
void pressEnter();
int getIntInput(char prompt[]);

void getDoctorName(char doctorID[], char doctorName[]);
void generateNextID(char filename[],
                    char header[],
                    char prefix[],
                    char outputID[]);

//Main Patient Menu
void patientMenu();

// Authentication
int login(char filename[], char id[], char password[], Patient *currentPatient);

// Appointment Management
void appointmentMenu(Patient *currentPatient);
void bookAppointment(Patient *currentPatient);
void manageAppointment(Patient *currentPatient);

void rescheduleAppointment(char appointmentID[], char doctorID[], char oldDate[], char oldTime[]);

void cancelAppointment(char appointmentID[], char doctorID[], char date[], char time[]);

int chooseAvailableSlot(char doctorID[], char selectedScheduleID[], char selectedDate[], char selectedTime[]);

// Payment Management
void paymentMenu(Patient *currentPatient);
void makePayment(Patient *currentPatient);
void viewPayment(Patient *currentPatient);
void searchPayment(Patient *currentPatient);

// Complaint Management
void complaintMenu(Patient *currentPatient);
void addComplaint(Patient *currentPatient);
void viewComplaint(Patient *currentPatient);
void searchComplaint(Patient *currentPatient);

#endif
