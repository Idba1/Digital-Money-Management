#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

// User structure definition
struct user
{
    char name[50];
    char account_number[50];
    char phone[50];
    char password[50];
    int balance;
};

// Transaction structure
struct transaction
{
    char type[10];
    int amount;
    char date[20];
};

// Pending deletion request structure
struct deletion_request
{
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
void search_account();
void approve_deletion_requests();
struct user deposit_money(struct user current_user);
struct user withdraw_money(struct user current_user);
void account_details(struct user current_user);
void request_account_deletion(struct user current_user);
struct user update_account_details(struct user current_user);
void view_transaction_report(struct user current_user, char report_type[]);
void record_transaction(struct user current_user, char type[], int amount);
void save_user_data(struct user current_user);
struct user load_user_data(char phone[]);
void save_deletion_request(struct deletion_request request);
void view_all_transactions(struct user current_user);
void exit_program();
void divider();

// Divider function
void divider()
{
    printf("\n-------------------------------------------------------------\n");
}

// Main function
int main()
{
    system("color F1");
    main_menu();
    return 0;
}

// Main menu
void main_menu()
{
    int choice;

    while (1)
    {
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

        switch (choice)
        {
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
void admin_login()
{
    char username[50], password[50];
    system("cls");
    printf("\n*** Admin Login ***\n");
    divider();

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0)
    {
        admin_menu();
    }
    else
    {
        printf("\nInvalid credentials.\n");
        getch();
    }
}

// Admin menu
void admin_menu()
{
    int choice;

    while (1)
    {
        system("cls");
        printf("\n*** Admin Dashboard ***\n");
        divider();
        printf("1. View All Users\n");
        printf("2. search account\n");
        printf("3. Approve Account Deletion Requests\n");
        printf("4. Logout\n");
        divider();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            view_all_users();
            break;
        case 2:
            search_account();
            break;
        case 3:
            approve_deletion_requests();
            break;
        case 4:
            return;
        default:
            printf("\nInvalid choice. Try again.\n");
            getch();
        }
    }
}

// View all users
void view_all_users()
{
    FILE *fp;
    struct user user_data;

    fp = fopen("users.dat", "rb");
    if (fp == NULL)
    {
        printf("\nNo users found.\n");
        getch();
        return;
    }

    printf("\n*** All Registered Users ***\n");
    divider();
    while (fread(&user_data, sizeof(struct user), 1, fp))
    {
        printf("Name: %s | Phone: %s | Balance: %d\n", user_data.name, user_data.phone, user_data.balance);
    }
    fclose(fp);
    getch();
}

// Search account function for the admin panel
void search_account()
{
    struct user user_data;
    char phone[50], filename[50];
    FILE *fp;

    system("cls");
    printf("\n\t\t\t*** Account Search ***\n");
    divider();

    printf("Enter the phone number to search: ");
    scanf("%s", phone);

    // Construct the filename
    strcpy(filename, phone);
    strcat(filename, ".dat");

    // Open the user file
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nNo account found with the phone number: %s\n", phone);
        getch();
        return;
    }

    // Read and display user data
    fread(&user_data, sizeof(struct user), 1, fp);
    fclose(fp);

    printf("\n*** Account Found ***\n");
    divider();
    printf("Name           : %s\n", user_data.name);
    printf("Account Number : %s\n", user_data.account_number);
    printf("Phone Number   : %s\n", user_data.phone);
    printf("Balance        : %d\n", user_data.balance);
    divider();

    getch();
}

// Approve deletion requests
void approve_deletion_requests()
{
    FILE *fp = fopen("pending_requests.dat", "rb");
    FILE *temp_fp = fopen("temp_pending_requests.dat", "wb");
    struct deletion_request request;

    if (fp == NULL)
    {
        printf("\nNo deletion requests found.\n");
        getch();
        return;
    }

    if (temp_fp == NULL)
    {
        fclose(fp);
        printf("\nError processing requests.\n");
        getch();
        return;
    }

    while (fread(&request, sizeof(struct deletion_request), 1, fp))
    {
        printf("Approve deletion for Account: %s | Phone: %s? (y/n): ", request.account_number, request.phone);
        char choice;
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            char user_file[50];
            strcpy(user_file, request.phone);
            strcat(user_file, ".dat");
            if (remove(user_file) == 0)
            {
                printf("\nAccount with phone %s deleted successfully.\n", request.phone);
            }
        }
        else
        {
            fwrite(&request, sizeof(struct deletion_request), 1, temp_fp);
        }
    }

    fclose(fp);
    fclose(temp_fp);
    remove("pending_requests.dat");
    rename("temp_pending_requests.dat", "pending_requests.dat");

    printf("\nAll deletion requests processed.\n");
    getch();
}

// Register a new user
void register_user()
{
    struct user new_user;

    system("cls");
    printf("\n*** Register New Account ***\n");
    divider();

    printf("Enter your name: ");
    scanf("%s", new_user.name);
    printf("Enter your phone number: ");
    scanf("%s", new_user.phone);
    printf("Enter a password: ");
    scanf("%s", new_user.password);

    strcpy(new_user.account_number, new_user.phone);
    new_user.balance = 0;

    save_user_data(new_user);
    printf("\nAccount successfully registered!\n");
    getch();
}

// Login user
void login_user()
{
    struct user current_user;
    char phone[50], password[50];

    system("cls");
    printf("\n*** Login to Account ***\n");
    divider();

    printf("Enter phone number: ");
    scanf("%s", phone);
    printf("Enter password: ");
    scanf("%s", password);

    current_user = load_user_data(phone);

    if (strcmp(password, current_user.password) == 0)
    {
        int choice;
        while (1)
        {
            system("cls");
            printf("\n*** Welcome, %s ***\n", current_user.name);
            divider();
            printf("1. Deposit Money\n");
            printf("2. Withdraw Money\n");
            printf("3. View Account Details\n");
            printf("4. Request Account Deletion\n");
            printf("5. Update Account Details\n");
            printf("6. View All Transactions\n");
            printf("7. Logout\n");
            divider();
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                current_user = deposit_money(current_user);
                break;
            case 2:
                current_user = withdraw_money(current_user);
                break;
            case 3:
                account_details(current_user);
                break;
            case 4:
                request_account_deletion(current_user);
                break;
            case 5:
                current_user = update_account_details(current_user);
                break;
            case 6:
                view_all_transactions(current_user);
                break;
            case 7:
                save_user_data(current_user);
                return;
            default:
                printf("\nInvalid choice. Try again.\n");
                getch();
            }
        }
    }
    else
    {
        printf("\nInvalid password. Try again.\n");
        getch();
    }
}

// Deposit money
struct user deposit_money(struct user current_user)
{
    int amount;
    printf("\nEnter amount to deposit: ");
    scanf("%d", &amount);

    if (amount > 0)
    {
        current_user.balance += amount;
        printf("\nDeposit successful. New balance: %d\n", current_user.balance);
        save_user_data(current_user);

        // Record the transaction
        record_transaction(current_user, "Deposit", amount);
    }
    else
    {
        printf("\nInvalid amount.\n");
    }
    getch();
    return current_user;
}

// Withdraw money
struct user withdraw_money(struct user current_user)
{
    int amount;
    printf("\nEnter amount to withdraw: ");
    scanf("%d", &amount);

    if (amount > current_user.balance)
    {
        printf("\nInsufficient balance.\n");
    }
    else
    {
        current_user.balance -= amount;
        printf("\nWithdrawal successful. Remaining balance: %d\n", current_user.balance);
        save_user_data(current_user);

        // Record the transaction
        record_transaction(current_user, "Withdraw", amount);
    }
    getch();
    return current_user;
}

// View account details
void account_details(struct user current_user)
{
    printf("\n*** Account Details ***\n");
    divider();
    printf("Name: %s\n", current_user.name);
    printf("Phone: %s\n", current_user.phone);
    printf("Balance: %d\n", current_user.balance);
    getch();
}

// Request account deletion
void request_account_deletion(struct user current_user)
{
    struct deletion_request request;
    strcpy(request.account_number, current_user.account_number);
    strcpy(request.phone, current_user.phone);

    save_deletion_request(request);

    printf("\nAccount deletion request submitted.\n");
    getch();
}

// Update account details
struct user update_account_details(struct user current_user)
{
    printf("\nEnter new name: ");
    scanf("%s", current_user.name);

    printf("\nEnter new password: ");
    scanf("%s", current_user.password);

    // Save the updated details back into the user's file
    save_user_data(current_user);

    // Update user data in users.dat (overwrite if phone number changed)
    printf("\nAccount details updated successfully.\n");
    getch();
    return current_user;
}

// Save user data
void save_user_data(struct user current_user)
{
    char filename[50];
    strcpy(filename, current_user.phone);
    strcat(filename, ".dat");

    // Save to individual user file
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL)
    {
        fwrite(&current_user, sizeof(struct user), 1, fp);
        fclose(fp);
    }
    else
    {
        printf("\nError: Failed to save user data to file.\n");
    }

    // Update global users.dat
    FILE *users_fp = fopen("users.dat", "rb");
    FILE *temp_fp = fopen("temp_users.dat", "wb");
    struct user temp_user;
    int found = 0;

    if (users_fp != NULL && temp_fp != NULL)
    {
        while (fread(&temp_user, sizeof(struct user), 1, users_fp))
        {
            if (strcmp(temp_user.phone, current_user.phone) == 0)
            {
                fwrite(&current_user, sizeof(struct user), 1, temp_fp);
                found = 1;
            }
            else
            {
                fwrite(&temp_user, sizeof(struct user), 1, temp_fp);
            }
        }
        fclose(users_fp);
    }

    // If phone number changed, rename the file and update users.dat
    if (found && strcmp(temp_user.phone, current_user.phone) != 0)
    {
        // If phone number is updated, remove old file and rename the new one
        char old_filename[50];
        strcpy(old_filename, temp_user.phone);
        strcat(old_filename, ".dat");

        // Remove the old file and rename the new file
        remove(old_filename);
        rename(filename, old_filename);
    }

    if (!found && temp_fp != NULL)
    {
        fwrite(&current_user, sizeof(struct user), 1, temp_fp);
    }

    fclose(temp_fp);
    remove("users.dat");
    rename("temp_users.dat", "users.dat");
}

// Load user data
struct user load_user_data(char phone[])
{
    struct user current_user = {0};
    char filename[50];
    strcpy(filename, phone);
    strcat(filename, ".dat");

    FILE *fp = fopen(filename, "rb");
    if (fp != NULL)
    {
        fread(&current_user, sizeof(struct user), 1, fp);
        fclose(fp);
    }
    else
    {
        printf("\nError: Account not found or failed to load user data.\n");
        getch();
    }
    return current_user;
}

// Save deletion request
void save_deletion_request(struct deletion_request request)
{
    FILE *fp = fopen("pending_requests.dat", "ab");
    if (fp != NULL)
    {
        fwrite(&request, sizeof(struct deletion_request), 1, fp);
        fclose(fp);
    }
}

// Record a transaction in the file
void record_transaction(struct user current_user, char type[], int amount)
{
    char filename[50];
    strcpy(filename, current_user.phone);
    strcat(filename, "_transactions.dat");

    FILE *fp = fopen(filename, "ab");
    if (fp != NULL)
    {
        struct transaction new_transaction;
        strcpy(new_transaction.type, type);
        new_transaction.amount = amount;

        // Get current date
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(new_transaction.date, sizeof(new_transaction.date), "%Y-%m-%d", tm);

        fwrite(&new_transaction, sizeof(struct transaction), 1, fp);
        fclose(fp);
    }
}

// Check if a transaction date is within the report range
int is_within_report_range(char transaction_date[], char report_type[])
{
    time_t t = time(NULL);
    struct tm *current_tm = localtime(&t);

    // Parse transaction date
    struct tm transaction_tm = {0};
    sscanf(transaction_date, "%d-%d-%d", &transaction_tm.tm_year, &transaction_tm.tm_mon, &transaction_tm.tm_mday);
    transaction_tm.tm_year -= 1900; // tm_year is years since 1900
    transaction_tm.tm_mon -= 1;     // tm_mon is 0-based

    time_t transaction_time = mktime(&transaction_tm);
    if (transaction_time == -1)
    {
        // Invalid transaction date
        return 0;
    }

    // Calculate time difference in days
    double days_difference = difftime(t, transaction_time) / (60 * 60 * 24);

    if (strcmp(report_type, "Daily") == 0 && days_difference <= 1 && current_tm->tm_mday == transaction_tm.tm_mday)
    {
        return 1;
    }
    else if (strcmp(report_type, "Weekly") == 0 && days_difference <= 7)
    {
        return 1;
    }
    else if (strcmp(report_type, "Monthly") == 0 && days_difference <= 30)
    {
        return 1;
    }
    else if (strcmp(report_type, "Yearly") == 0 && days_difference <= 365)
    {
        return 1;
    }

    return 0;
}

// View transaction report based on the type
void view_all_transactions(struct user current_user)
{
    char filename[50];
    strcpy(filename, current_user.phone);
    strcat(filename, "_transactions.dat");

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nNo transaction history found.\n");
        getch();
        return;
    }

    struct transaction record;
    printf("\n*** All Transactions for %s ***\n", current_user.name);
    divider();

    while (fread(&record, sizeof(struct transaction), 1, fp))
    {
        printf("Date: %s | Type: %s | Amount: %d\n", record.date, record.type, record.amount);
    }

    fclose(fp);
    getch();
}

// Exit program
void exit_program()
{
    printf("\nThank you for using Digital Money Management System. Goodbye!\n");
    getch();
}