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

// Function declarations
void deposit_money(struct user *current_user);
void withdraw_money(struct user *current_user);
void account_details(const struct user *current_user);
void exit_program();
void divider();
void register_user();
void login_user();
void search_account();
void delete_account();
void update_account();
void save_user_data(const char *filename, const struct user *current_user);

// Function to print divider line
void divider()
{
    printf("\n\t\t\t\t\t");
    for (int i = 0; i < 50; i++)
    {
        printf("-");
    }
    printf("\n");
}

int main()
{
    int opt;
    system("color f1");

    // Main menu loop
    while (1)
    {
        system("cls");
        printf("\n\n\n\n\t\t\t\t\t\t\t*Digital Money Management*\t\t\t\t\t\n\n");

        printf("\n\n\n\t\t\t\t 1. Login your account\t\t\t\t");
        printf("\n\n\n\t\t\t\t 2. Register one account\t\t\t\t");
        printf("\n\n\n\t\t\t\t 3. Search your account by phone number\t\t");
        printf("\n\n\n\t\t\t\t 4. Delete an account\n");
        printf("\n\n\n\t\t\t\t 5. Update account details\n\n\n");
        printf("\n\n\n\t\t\t\t 6. Exit\n\n\n");
        printf("\n\t\t\t\t\tEnter your choice: ");
        scanf("%d", &opt);

        // Menu options with corresponding functions
        switch (opt)
        {
        case 1:
            login_user();
            break;
        case 2:
            register_user();
            break;
        case 3:
            search_account();
            break;
        case 4:
            delete_account();
            break;
        case 5:
            update_account();
            break;
        case 6:
            exit_program();
            return 0;
        default:
            printf("\n\t\t\t\t\tInvalid choice. Try again.\n");
            getch();
        }
    }
    return 0;
}

// Function to register a new user account
void register_user()
{
    struct user new_user;
    char filename[50];
    FILE *fp;

    system("cls");
    printf("\n\n\n\n\t\t\t\t\t\t\t*Digital Money Management*\t\t\t\t\t\n\n\n");

    printf("\n\t\t\t\t\tEnter your name:\t\t");
    scanf("%s", new_user.name);
    printf("\n\t\t\t\t\tEnter your phone number:\t");
    scanf("%s", new_user.phone);
    printf("\n\t\t\t\t\tEnter your phone number again:\t");
    scanf("%s", new_user.account_number);
    printf("\n\t\t\t\t\tEnter your new password:\t");
    scanf("%s", new_user.password);
    printf("\n\n\t\t\t\t\tYour phone number is your new account number : %s\t\n\n", new_user.account_number);
    new_user.balance = 0;

    strcpy(filename, new_user.phone);
    fp = fopen(strcat(filename, ".dat"), "wb");
    if (fp != NULL)
    {
        fwrite(&new_user, sizeof(struct user), 1, fp);
        fclose(fp);
        printf("\n\t\t\t\t\tSuccessfully registered\n\n");
    }
    else
    {
        printf("\n\t\t\t\t\tError in registration\n\n");
    }
    getch();
}

// Function to login an existing user
void login_user()
{
    struct user user;
    char filename[50], phone[50], password[50];
    FILE *fp;

    system("cls");
    printf("\n\n\n\n\t\t\t\t\t\t\t*Digital Money Management*\t\t\t\t\t\n\n\n");

    printf("\n\n\t\t\t\t\tPhone No.:\t");
    scanf("%s", phone);
    printf("\n\t\t\t\t\tPassword:\t");
    scanf("%s", password);

    strcpy(filename, phone);
    fp = fopen(strcat(filename, ".dat"), "rb");

    if (fp == NULL)
    {
        printf("\n\t\t\t\t\tAccount number not registered\n");
        getch();
        return;
    }

    fread(&user, sizeof(struct user), 1, fp);
    fclose(fp);

    if (strcmp(password, user.password) != 0)
    {
        printf("\n\t\t\t\t\tInvalid password\n");
        getch();
        return;
    }

    char cont = 'y';
    while (cont == 'y')
    {
        system("cls");
        printf("\n\t\t\t\t\tMENU\n");
        divider();
        printf("\n\t\t\t\t\t1. Deposit Money\n");
        printf("\n\t\t\t\t\t2. Withdraw Money\n");
        printf("\n\t\t\t\t\t3. Account details\n");
        printf("\n\t\t\t\t\t4. Exit\n");
        printf("\n\t\t\t\t\tEnter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("cls");
            deposit_money(&user);
            save_user_data(filename, &user);
            break;
        case 2:
            system("cls");
            withdraw_money(&user);
            save_user_data(filename, &user);
            break;
        case 3:
            system("cls");
            account_details(&user);
            break;
        case 4:
            system("cls");
            exit_program();
            return;
        default:
            printf("\n\t\t\t\t\t*Invalid choice*\n");
        }
        printf("\n\n\t\t\t\t\tDo you want to continue? [y/n]: ");
        scanf(" %c", &cont);
    }
    printf("\n\n\t\t\t\t\t*Thank you for using the system*\n\n");
    getch();
}

// Function to deposit money
void deposit_money(struct user *current_user)
{
    int dip_amt;
    printf("\n\t\t\t\t\t*DEPOSITING MONEY*\n");
    divider();
    printf("\n\n\t\t\t\t\tEnter the amount you want to deposit: ");
    scanf("%d", &dip_amt);
    current_user->balance += dip_amt;
    printf("\n\t\t\t\t\t**Money Deposited\n");
    printf("\n\t\t\t\t\tCurrent balance: %d\n", current_user->balance);
    printf("\n\t\t\t\t\tPress any key....\n");
    getch();
}

// Function to withdraw money
void withdraw_money(struct user *current_user)
{
    int with_amt;
    printf("\n\t\t\t\t\t*WITHDRAWING MONEY*\n");
    divider();
    printf("\n\n\t\t\t\t\tEnter the amount you want to withdraw: ");
    scanf("%d", &with_amt);
    if (current_user->balance < with_amt)
    {
        printf("\n\t\t\t\t\t*Insufficient balance*\n");
    }
    else
    {
        current_user->balance -= with_amt;
        printf("\n\t\t\t\t\t*Money Withdrawn*\n");
        printf("\n\t\t\t\t\tCurrent balance: %d\n", current_user->balance);
    }
    printf("\n\t\t\t\t\tPress any key....\n");
    getch();
}

// Function to display account details
void account_details(const struct user *current_user)
{
    printf("\n\t\t\t\t\tACCOUNT DETAILS\n");
    divider();
    printf("\n\n\t\t\t\t\tName: %s\n", current_user->name);
    printf("\n\t\t\t\t\tAccount No.: %s\n", current_user->account_number);
    printf("\n\t\t\t\t\tPhone No.: %s\n", current_user->phone);
    printf("\n\t\t\t\t\tTotal balance: %d\n", current_user->balance);
    printf("\n\n\t\t\t\t\tPress any key to continue...\n");
    getch();
}

// Function to exit program
void exit_program()
{
    printf("\n\n\t\t\t\t\tThank you for using Digital Money Management System.\n");
    printf("\n\t\t\t\t\tPress any key to exit.");
    getch();
}

// Function to search for an account by phone number
void search_account()
{
    struct user user_data;
    char search_input[50];
    int found = 0;

    printf("\n\t\t\tEnter your phone number to search: ");
    scanf("%s", search_input);

    FILE *fp = fopen(strcat(search_input, ".dat"), "rb");
    if (fp != NULL)
    {
        fread(&user_data, sizeof(struct user), 1, fp);
        fclose(fp);
        printf("\n\t\t\tAccount found:\n");
        printf("\n\t\t\tName: %s\n", user_data.name);
        printf("\t\t\tAccount Number: %s\n", user_data.account_number);
        printf("\t\t\tPhone: %s\n", user_data.phone);
        printf("\t\t\tBalance: %d\n", user_data.balance);
        found = 1;
    }

    if (!found)
    {
        printf("\n\t\t\tAccount '%s' not found.\n", search_input);
    }
    printf("\n\t\t\tPress any key to return to main menu...");
    getch();
}

// Function to delete an account
void delete_account()
{
    char phone[50];
    char filename[50];

    printf("\n\t\t\tEnter the phone number of the account to delete: ");
    scanf("%s", phone);

    strcpy(filename, phone);
    strcat(filename, ".dat");

    if (remove(filename) == 0)
    {
        printf("\n\t\t\tAccount deleted successfully.\n");
    }
    else
    {
        printf("\n\t\t\tError: Account not found or unable to delete.\n");
    }

    getch();
}

// Function to update account details
void update_account()
{
    struct user user_data;
    char filename[50], phone[50];
    FILE *fp;

    printf("\n\t\t\tEnter the phone number of the account to update: ");
    scanf("%s", phone);

    strcpy(filename, phone);
    strcat(filename, ".dat");

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\n\t\t\tAccount not found.\n");
        getch();
        return;
    }

    fread(&user_data, sizeof(struct user), 1, fp);
    fclose(fp);

    int choice;
    printf("\n\t\t\tWhat would you like to update?\n");
    printf("\n\t\t\t1. Phone Number\n");
    printf("\n\t\t\t2. Password\n");
    printf("\n\t\t\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("\n\t\t\tEnter new phone number: ");
        scanf("%s", user_data.phone);
        strcpy(user_data.account_number, user_data.phone);
        printf("\n\t\t\tPhone number updated.\n");
        break;

    case 2:
        printf("\n\t\t\tEnter new password: ");
        scanf("%s", user_data.password);
        printf("\n\t\t\tPassword updated.\n");
        break;

    default:
        printf("\n\t\t\tInvalid choice.\n");
        return;
    }

    remove(filename); // Delete old file
    strcpy(filename, user_data.phone);
    strcat(filename, ".dat");

    fp = fopen(filename, "wb");
    if (fp != NULL)
    {
        fwrite(&user_data, sizeof(struct user), 1, fp);
        fclose(fp);
        printf("\n\t\t\tAccount updated successfully.\n");
    }
    else
    {
        printf("\n\t\t\tError updating account.\n");
    }

    getch();
}

// Function to save user data to a file
void save_user_data(const char *filename, const struct user *current_user)
{
    FILE *fp = fopen(filename, "wb");

    if (fp != NULL)
    {
        fwrite(current_user, sizeof(struct user), 1, fp);
        fclose(fp);
    }
    else
    {
        printf("\n\t\t\tError updating user data\n");
    }
}
