#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

// User structure definition
struct user {
    char name[50];
    char account_number[50];
    char phone[50];
    char password[50];
    int balance;
};

// Pending deletion request structure
struct deletion_request {
    char account_number[50];
    char phone[50];
};

// Function declarations
void main_menu();
void admin_menu();
void register_user();
void login_user();
void admin_login();
void view_all_users();
void approve_deletion_requests();
void deposit_money(struct user current_user);
void withdraw_money(struct user current_user);
void account_details(struct user current_user);
void request_account_deletion(struct user current_user);
void update_account_details(struct user current_user);
void save_user_data(struct user current_user);
void update_users_dat(struct user updated_user);
void load_user_data(char phone[], struct user *current_user);
void save_deletion_request(struct deletion_request request);
void exit_program();
void divider();

// Function to display a divider line
void divider() {
    printf("\n-------------------------------------------------------------\n");
}

// Main function
int main() {
    system("color F1");
    main_menu();
    return 0;
}

// Main menu
void main_menu() {
    int choice;

    while (1) {
        system("cls");
        printf("\n\t\t\t*** Digital Money Management System ***\n");
        divider();
        printf("1. Register Account\n");
        printf("2. Login to Account\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        divider();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                login_user();
                break;
            case 3:
                admin_login();
                break;
            case 4:
                exit_program();
                return;
            default:
                printf("\nInvalid choice. Try again.\n");
                getch();
        }
    }
}

// Admin login
void admin_login() {
    char username[50], password[50];
    system("cls");
    printf("\n*** Admin Login ***\n");
    divider();

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        admin_menu();
    } else {
        printf("\nInvalid credentials.\n");
        getch();
    }
}

// Admin menu
void admin_menu() {
    int choice;

    while (1) {
        system("cls");
        printf("\n*** Admin Dashboard ***\n");
        divider();
        printf("1. View All Users\n");
        printf("2. Approve Account Deletion Requests\n");
        printf("3. Logout\n");
        divider();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_all_users();
                break;
            case 2:
                approve_deletion_requests();
                break;
            case 3:
                return;
            default:
                printf("\nInvalid choice. Try again.\n");
                getch();
        }
    }
}

// View all users
void view_all_users() {
    FILE *fp;
    struct user user_data;

    fp = fopen("users.dat", "rb");
    if (fp == NULL) {
        printf("\nNo users found.\n");
        getch();
        return;
    }

    printf("\n*** All Registered Users ***\n");
    divider();
    while (fread(&user_data, sizeof(struct user), 1, fp)) {
        printf("Name: %s | Phone: %s | Balance: %d\n", user_data.name, user_data.phone, user_data.balance);
    }
    fclose(fp);
    getch();
}

// Register a new user
void register_user() {
    struct user new_user;
    FILE *fp;

    system("cls");
    printf("\n*** Register New Account ***\n");
    divider();

    printf("Enter your name: ");
    scanf("%s", new_user.name);
    printf("Enter your phone number: ");
    scanf("%s", new_user.phone);
    printf("Enter a password: ");
    scanf("%s", new_user.password);

    strcpy(new_user.account_number, new_user.phone);  // Phone number as account number
    new_user.balance = 0;

    fp = fopen(strcat(new_user.phone, ".dat"), "wb");
    if (fp != NULL) {
        fwrite(&new_user, sizeof(struct user), 1, fp);
        fclose(fp);
        printf("\nAccount successfully registered!\n");
    } else {
        printf("\nError in registration. Try again.\n");
    }

    update_users_dat(new_user);
    getch();
}

// Login user
void login_user() {
    struct user current_user;
    char phone[50], password[50];

    system("cls");
    printf("\n*** Login to Account ***\n");
    divider();

    printf("Enter phone number: ");
    scanf("%s", phone);
    printf("Enter password: ");
    scanf("%s", password);

    load_user_data(phone, &current_user);

    if (strcmp(password, current_user.password) == 0) {
        int choice;
        while (1) {
            system("cls");
            printf("\n*** Welcome, %s ***\n", current_user.name);
            divider();
            printf("1. Deposit Money\n");
            printf("2. Withdraw Money\n");
            printf("3. View Account Details\n");
            printf("4. Request Account Deletion\n");
            printf("5. Update Account Details\n");
            printf("6. Logout\n");
            divider();
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    deposit_money(current_user);
                    break;
                case 2:
                    withdraw_money(current_user);
                    break;
                case 3:
                    account_details(current_user);
                    break;
                case 4:
                    request_account_deletion(current_user);
                    break;
                case 5:
                    update_account_details(current_user);
                    break;
                case 6:
                    save_user_data(current_user);
                    return;
                default:
                    printf("\nInvalid choice. Try again.\n");
                    getch();
            }
        }
    } else {
        printf("\nInvalid password. Try again.\n");
        getch();
    }
}

// Deposit money
void deposit_money(struct user current_user) {
    int amount;
    printf("\nEnter amount to deposit: ");
    scanf("%d", &amount);

    if (amount > 0) {
        current_user.balance += amount;
        printf("\nDeposit successful. New balance: %d\n", current_user.balance);
    } else {
        printf("\nInvalid amount.\n");
    }
    getch();
}

// Withdraw money
void withdraw_money(struct user current_user) {
    int amount;
    printf("\nEnter amount to withdraw: ");
    scanf("%d", &amount);

    if (amount > current_user.balance) {
        printf("\nInsufficient balance.\n");
    } else {
        current_user.balance -= amount;
        printf("\nWithdrawal successful. Remaining balance: %d\n", current_user.balance);
    }
    getch();
}

// Account details
void account_details(struct user current_user) {
    printf("\n*** Account Details ***\n");
    divider();
    printf("Name: %s\n", current_user.name);
    printf("Phone: %s\n", current_user.phone);
    printf("Balance: %d\n", current_user.balance);
    getch();
}

// Request account deletion
void request_account_deletion(struct user current_user) {
    struct deletion_request request;
    strcpy(request.account_number, current_user.account_number);
    strcpy(request.phone, current_user.phone);

    save_deletion_request(request);

    printf("\nAccount deletion request submitted.\n");
    getch();
}

// Update account details
void update_account_details(struct user current_user) {
    printf("\nEnter new name: ");
    scanf("%s", current_user.name);

    printf("\nAccount details updated successfully.\n");
    save_user_data(current_user);
    getch();
}

// Save user data
void save_user_data(struct user current_user) {
    FILE *fp = fopen(strcat(current_user.phone, ".dat"), "wb");
    if (fp != NULL) {
        fwrite(&current_user, sizeof(struct user), 1, fp);
        fclose(fp);
    }
    update_users_dat(current_user);
}

// Update users.dat with user details
void update_users_dat(struct user updated_user) {
    FILE *fp = fopen("users.dat", "rb");
    FILE *temp_fp = fopen("temp_users.dat", "wb");
    struct user temp_user;

    if (fp == NULL || temp_fp == NULL) {
        if (temp_fp) fclose(temp_fp);
        return;
    }

    int found = 0;
    while (fread(&temp_user, sizeof(struct user), 1, fp)) {
        if (strcmp(temp_user.phone, updated_user.phone) == 0) {
            fwrite(&updated_user, sizeof(struct user), 1, temp_fp);
            found = 1;
        } else {
            fwrite(&temp_user, sizeof(struct user), 1, temp_fp);
        }
    }

    if (!found) fwrite(&updated_user, sizeof(struct user), 1, temp_fp);

    fclose(fp);
    fclose(temp_fp);
    remove("users.dat");
    rename("temp_users.dat", "users.dat");
}

// Load user data
void load_user_data(char phone[], struct user *current_user) {
    FILE *fp = fopen(strcat(phone, ".dat"), "rb");
    if (fp != NULL) {
        fread(current_user, sizeof(struct user), 1, fp);
        fclose(fp);
    } else {
        memset(current_user, 0, sizeof(struct user));
        printf("\nAccount not found.\n");
        getch();
    }
}

// Save deletion request
void save_deletion_request(struct deletion_request request) {
    FILE *fp = fopen("pending_requests.dat", "ab");
    if (fp != NULL) {
        fwrite(&request, sizeof(struct deletion_request), 1, fp);
        fclose(fp);
    }
}

// Exit program
void exit_program() {
    printf("\nThank you for using Digital Money Management System. Goodbye!\n");
    getch();
}
