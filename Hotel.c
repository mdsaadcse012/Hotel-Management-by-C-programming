#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Reservation {
    int id;
    char name[50];
    char phone[20];
    int roomNumber;
    char roomType[20];
    int days;
    float bill;
    char facilities[300];  // <-- New field for facilities
};

struct Reservation bookings[MAX];
int totalBookings = 0;

// ---------- Function Declarations ----------
void login();
void menu();
void makeReservation();
void viewReservations();
void searchReservation();
void cancelReservation();
float calculateBill(char roomType[], int days);
void saveData();
void loadData();
// ------------------------------------------

int main() {
    loadData();
    login();
    menu();
    saveData();
    return 0;
}

// ---------- Admin Login Function ----------
void login() {
    char username[20], password[20];
    printf("==== Admin Login ====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        printf("Login successful!\n\n");
    } else {
        printf("Invalid credentials. Exiting...\n");
        exit(0);
    }
}

// ---------- Menu Function ----------
void menu() {
    int choice;
    do {
        printf("\n===== Hotel Reservation System =====\n");
        printf("1. Make a Reservation\n");
        printf("2. View All Reservations\n");
        printf("3. Search Reservation by Name\n");
        printf("4. Cancel Reservation\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: makeReservation(); break;
            case 2: viewReservations(); break;
            case 3: searchReservation(); break;
            case 4: cancelReservation(); break;
            case 5: printf("Thank you for using the system!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
}

// ---------- Make Reservation ----------
void makeReservation() {
    if (totalBookings >= MAX) {
        printf("Sorry, hotel is fully booked!\n");
        return;
    }

    struct Reservation r;
    r.id = totalBookings + 1;

    printf("Enter guest name: ");
    getchar(); // consume newline
    fgets(r.name, sizeof(r.name), stdin);
    r.name[strcspn(r.name, "\n")] = 0;

    printf("Enter phone number: ");
    scanf("%s", r.phone);

    printf("Choose room type (Single/Double/Suite): ");
    scanf("%s", r.roomType);

    printf("Enter number of days: ");
    scanf("%d", &r.days);

    getchar(); // consume newline
    printf("Enter required facilities (comma separated):\n");
    printf("Options: Wifi, Swimming Pool, Sallon, Restaurant, Gym, Car/Motorbike Booking, 24hr Room Service, Laundry, Bar, Spa/Massage, Tour Guide\n");
    fgets(r.facilities, sizeof(r.facilities), stdin);
    r.facilities[strcspn(r.facilities, "\n")] = 0;

    r.roomNumber = 100 + r.id;
    r.bill = calculateBill(r.roomType, r.days);

    bookings[totalBookings++] = r;

    printf("Booking successful! Room Number: %d, Bill: %.2f\n", r.roomNumber, r.bill);
}

// ---------- Calculate Bill ----------
float calculateBill(char roomType[], int days) {
    float rate;
    if (strcmp(roomType, "Single") == 0) rate = 1500;
    else if (strcmp(roomType, "Double") == 0) rate = 2500;
    else if (strcmp(roomType, "Suite") == 0) rate = 4000;
    else rate = 2000; // default

    return rate * days;
}

// ---------- View All Reservations ----------
void viewReservations() {
    printf("\n=== All Reservations ===\n");
    if (totalBookings == 0) {
        printf("No bookings found.\n");
        return;
    }

    for (int i = 0; i < totalBookings; i++) {
        struct Reservation r = bookings[i];
        printf("\nBooking ID: %d\n", r.id);
        printf("Name: %s\n", r.name);
        printf("Phone: %s\n", r.phone);
        printf("Room Number: %d\n", r.roomNumber);
        printf("Room Type: %s\n", r.roomType);
        printf("Days: %d\n", r.days);
        printf("Bill: %.2f\n", r.bill);
        printf("Facilities: %s\n", r.facilities); // <-- New line
    }
}

// ---------- Search by Name ----------
void searchReservation() {
    char searchName[50];
    printf("Enter guest name to search: ");
    getchar();
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < totalBookings; i++) {
        if (strcasecmp(bookings[i].name, searchName) == 0) {
            printf("\nBooking found:\n");
            printf("ID: %d, Name: %s, Phone: %s, Room: %d, Days: %d, Bill: %.2f\n",
                   bookings[i].id, bookings[i].name, bookings[i].phone,
                   bookings[i].roomNumber, bookings[i].days, bookings[i].bill);
            printf("Facilities: %s\n", bookings[i].facilities); // <-- New line
            found = 1;
            break;
        }
    }

    if (!found) printf("No reservation found for %s\n", searchName);
}

// ---------- Cancel Reservation ----------
void cancelReservation() {
    int id;
    printf("Enter booking ID to cancel: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < totalBookings; i++) {
        if (bookings[i].id == id) {
            for (int j = i; j < totalBookings - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            totalBookings--;
            printf("Reservation ID %d canceled.\n", id);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No booking found with ID %d\n", id);
    }
}

// ---------- Save to File ----------
void saveData() {
    FILE *fp = fopen("bookings.txt", "w");
    if (fp == NULL) return;

    for (int i = 0; i < totalBookings; i++) {
        fprintf(fp, "%d|%s|%s|%d|%s|%d|%.2f|%s\n",
                bookings[i].id, bookings[i].name, bookings[i].phone,
                bookings[i].roomNumber, bookings[i].roomType,
                bookings[i].days, bookings[i].bill,
                bookings[i].facilities); // <-- New field saved
    }
    fclose(fp);
}

// ---------- Load from File ----------
void loadData() {
    FILE *fp = fopen("bookings.txt", "r");
    if (fp == NULL) return;

    while (fscanf(fp, "%d|%[^|]|%[^|]|%d|%[^|]|%d|%f|%[^|\n]\n",
                  &bookings[totalBookings].id,
                  bookings[totalBookings].name,
                  bookings[totalBookings].phone,
                  &bookings[totalBookings].roomNumber,
                  bookings[totalBookings].roomType,
                  &bookings[totalBookings].days,
                  &bookings[totalBookings].bill,
                  bookings[totalBookings].facilities) != EOF) {
        totalBookings++;
    }
    fclose(fp);
}
