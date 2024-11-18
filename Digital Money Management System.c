#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

// User structure definition
struct user
{
    char name[50];
    char account_number[50];
    char phone[50];
    char password[50];
    int balance;
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
void search_account();
void delete_account();
void approve_deletion_requests();
void view_all_users();
void update_account();
void deposit_money(struct user *current_user);
void withdraw_money(struct user *current_user);
void account_details(const struct user *current_user);
void save_user_data(const char *filename, const struct user *current_user);
void save_deletion_request(const struct deletion_request *request);
void exit_program();
void divider();

// Function to display a divider line
void divider()
{
    printf("\n-------------------------------------------------------------\n");
}

// Main function
int main()
{
    system("color f1");
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

    // Replace these with actual credentials
    if (strcmp(username, "idba") == 0 && strcmp(password, "1111") == 0)
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
        printf("2. Approve Account Deletion Requests\n");
        printf("3. Logout\n");
        divider();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
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

// Register a new user
void register_user()
{
    struct user new_user;
    char filename[50];
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

    strcpy(new_user.account_number, new_user.phone); // Use phone number as account number
    new_user.balance = 0;

    // Save to individual file
    strcpy(filename, new_user.phone);
    strcat(filename, ".dat");
    fp = fopen(filename, "wb");
    if (fp != NULL)
    {
        fwrite(&new_user, sizeof(struct user), 1, fp);
        fclose(fp);
        printf("\nAccount successfully registered!\n");
    }
    else
    {
        printf("\nError in registration. Try again.\n");
    }

    // Save to master file
    fp = fopen("users.dat", "ab");
    if (fp != NULL)
    {
        fwrite(&new_user, sizeof(struct user), 1, fp);
        fclose(fp);
    }
    getch();
}

// Login user
void login_user()
{
    struct user user_data;
    char filename[50], phone[50], password[50];
    FILE *fp;

    system("cls");
    printf("\n*** Login to Account ***\n");
    divider();

    printf("Enter phone number: ");
    scanf("%s", phone);
    printf("Enter password: ");
    scanf("%s", password);

    strcpy(filename, phone);
    strcat(filename, ".dat");
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nAccount not found.\n");
        getch();
        return;
    }

    fread(&user_data, sizeof(struct user), 1, fp);
    fclose(fp);

    if (strcmp(user_data.password, password) != 0)
    {
        printf("\nInvalid password.\n");
        getch();
        return;
    }

    int choice;
    while (1)
    {
        system("cls");
        printf("\n*** Welcome, %s ***\n", user_data.name);
        divider();
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. View Account Details\n");
        printf("4. Request Account Deletion\n");
        printf("5. Logout\n");
        divider();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            deposit_money(&user_data);
            save_user_data(filename, &user_data);
            break;
        case 2:
            withdraw_money(&user_data);
            save_user_data(filename, &user_data);
            break;
        case 3:
            account_details(&user_data);
            break;
        case 4:
        {
            struct deletion_request request;
            strcpy(request.account_number, user_data.account_number);
            strcpy(request.phone, user_data.phone);
            save_deletion_request(&request);
            printf("\nAccount deletion request submitted for approval.\n");
            getch();
            break;
        }
        case 5:
            return;
        default:
            printf("\nInvalid choice. Try again.\n");
        }
    }
}

// Save deletion request
void save_deletion_request(const struct deletion_request *request)
{
    FILE *fp = fopen("pending_requests.dat", "ab");
    if (fp != NULL)
    {
        fwrite(request, sizeof(struct deletion_request), 1, fp);
        fclose(fp);
    }
    else
    {
        printf("\nError saving deletion request.\n");
    }
}

// Approve account deletion requests
void approve_deletion_requests()
{
    FILE *fp, *temp_fp, *users_fp, *temp_users_fp;
    struct deletion_request request;
    struct user user_data;
    char filename[50];
    int found;

    fp = fopen("pending_requests.dat", "rb");
    if (fp == NULL)
    {
        printf("\nNo pending deletion requests.\n");
        getch();
        return;
    }

    temp_fp = fopen("temp_requests.dat", "wb");
    users_fp = fopen("users.dat", "rb");
    temp_users_fp = fopen("temp_users.dat", "wb");

    if (users_fp == NULL || temp_users_fp == NULL)
    {
        printf("\nError: Unable to process user data.\n");
        fclose(fp);
        fclose(temp_fp);
        return;
    }

    printf("\n*** Pending Deletion Requests ***\n");
    divider();

    while (fread(&request, sizeof(struct deletion_request), 1, fp))
    {
        printf("Account: %s | Phone: %s\n", request.account_number, request.phone);
        printf("Approve? (y/n): ");
        char choice;
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            // Delete user file
            strcpy(filename, request.phone);
            strcat(filename, ".dat");
            remove(filename);
            printf("\nAccount deleted successfully.\n");

            // Remove the account from users.dat
            rewind(users_fp);
            while (fread(&user_data, sizeof(struct user), 1, users_fp))
            {
                if (strcmp(user_data.account_number, request.account_number) != 0)
                {
                    fwrite(&user_data, sizeof(struct user), 1, temp_users_fp);
                }
                else
                {
                    printf("Removed from registered users: %s\n", user_data.name);
                }
            }
        }
        else
        {
            fwrite(&request, sizeof(struct deletion_request), 1, temp_fp);
        }
    }

    fclose(fp);
    fclose(temp_fp);
    fclose(users_fp);
    fclose(temp_users_fp);

    // Replace files
    remove("pending_requests.dat");
    rename("temp_requests.dat", "pending_requests.dat");
    remove("users.dat");
    rename("temp_users.dat", "users.dat");

    printf("\nAll requests processed.\n");
    getch();
}

// Deposit money
void deposit_money(struct user *current_user)
{
    int amount;
    printf("\nEnter amount to deposit: ");
    scanf("%d", &amount);

    if (amount > 0)
    {
        current_user->balance += amount;
        printf("\nDeposit successful. New balance: %d\n", current_user->balance);

        // Save updated balance to users.dat and individual file
        save_user_data(current_user->phone, current_user);
    }
    else
    {
        printf("\nInvalid amount.\n");
    }
    getch();
}

// Withdraw money
void withdraw_money(struct user *current_user)
{
    int amount;
    printf("\nEnter amount to withdraw: ");
    scanf("%d", &amount);

    if (amount > current_user->balance)
    {
        printf("\nInsufficient balance.\n");
    }
    else
    {
        current_user->balance -= amount;
        printf("\nWithdrawal successful. Remaining balance: %d\n", current_user->balance);

        // Save updated balance to users.dat and individual file
        save_user_data(current_user->phone, current_user);
    }
    getch();
}

// View account details
void account_details(const struct user *current_user)
{
    printf("\n*** Account Details ***\n");
    divider();
    printf("Name: %s\n", current_user->name);
    printf("Account Number: %s\n", current_user->account_number);
    printf("Phone: %s\n", current_user->phone);
    printf("Balance: %d\n", current_user->balance);
    getch();
}

// Search for an account
void search_account()
{
    struct user user_data;
    char phone[50], filename[50];
    FILE *fp;

    printf("\nEnter phone number to search: ");
    scanf("%s", phone);

    strcpy(filename, phone);
    strcat(filename, ".dat");
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nAccount not found.\n");
        getch();
        return;
    }

    fread(&user_data, sizeof(struct user), 1, fp);
    fclose(fp);

    printf("\nAccount found:\n");
    printf("Name: %s\n", user_data.name);
    printf("Phone: %s\n", user_data.phone);
    printf("Balance: %d\n", user_data.balance);
    getch();
}

// Delete an account
void delete_account()
{
    char phone[50], filename[50];

    printf("\nEnter phone number to delete account: ");
    scanf("%s", phone);

    strcpy(filename, phone);
    strcat(filename, ".dat");

    if (remove(filename) == 0)
    {
        printf("\nAccount successfully deleted.\n");
    }
    else
    {
        printf("\nError deleting account. Ensure the account exists.\n");
    }
    getch();
}

// Update account details
void update_account()
{
    struct user user_data;
    char phone[50], filename[50];
    FILE *fp;

    printf("\nEnter phone number to update account: ");
    scanf("%s", phone);

    strcpy(filename, phone);
    strcat(filename, ".dat");

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nAccount not found.\n");
        getch();
        return;
    }

    fread(&user_data, sizeof(struct user), 1, fp);
    fclose(fp);

    int choice;
    printf("\n1. Update Phone Number\n");
    printf("2. Update Password\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter new phone number: ");
        scanf("%s", user_data.phone);
        strcpy(user_data.account_number, user_data.phone);
    }
    else if (choice == 2)
    {
        printf("Enter new password: ");
        scanf("%s", user_data.password);
    }
    else
    {
        printf("\nInvalid choice.\n");
        return;
    }

    remove(filename);
    strcpy(filename, user_data.phone);
    strcat(filename, ".dat");

    fp = fopen(filename, "wb");
    if (fp != NULL)
    {
        fwrite(&user_data, sizeof(struct user), 1, fp);
        fclose(fp);
        printf("\nAccount updated successfully.\n");
    }
    else
    {
        printf("\nError updating account.\n");
    }
    getch();
}

// Save user data
void save_user_data(const char *filename, const struct user *current_user)
{
    FILE *fp = fopen(filename, "wb");
    FILE *users_fp = fopen("users.dat", "rb");
    FILE *temp_users_fp = fopen("temp_users.dat", "wb");
    struct user user_data;

    if (fp != NULL)
    {
        fwrite(current_user, sizeof(struct user), 1, fp);
        fclose(fp);
    }
    else
    {
        printf("\nError saving data to individual file.\n");
    }

    if (users_fp == NULL || temp_users_fp == NULL)
    {
        printf("\nError updating users.dat.\n");
        return;
    }

    // Update the specific user's details in the users.dat file
    while (fread(&user_data, sizeof(struct user), 1, users_fp))
    {
        if (strcmp(user_data.account_number, current_user->account_number) == 0)
        {
            fwrite(current_user, sizeof(struct user), 1, temp_users_fp);
        }
        else
        {
            fwrite(&user_data, sizeof(struct user), 1, temp_users_fp);
        }
    }

    fclose(users_fp);
    fclose(temp_users_fp);

    remove("users.dat");
    rename("temp_users.dat", "users.dat");
}

// Exit program
void exit_program()
{
    printf("\nThank you for using Digital Money Management System. Goodbye!\n");
    getch();
}
