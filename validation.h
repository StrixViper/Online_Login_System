#ifndef VALIDATION_H_INCLUDED
#define VALIDATION_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>


#define MAX_NAME_LENGTH 50
#define MAX_COUNTRIES 200
#define MAX_CITIES 1700
#define MAX_PASSWORD_LENGTH 20
#define MAX_EMAIL_LENGTH 30
#define MAX_PHONE_LENGTH 15


// Function Declarations


bool isValidPassword(const char *password);
bool isValidAge(int age);
bool isValidUserName(const char *username);
bool isValidName(const char *name);
bool isValidEmail(const char *email);
bool isValidCountry(const char* input, const char countries[][MAX_NAME_LENGTH], int num_countries);
bool isValidPhone(const char* PhoneNumber,const char* Country);
bool isValidCity(const char* input, const char cities[][MAX_NAME_LENGTH], int num_cities);
bool isValidMonth(const int month);
bool isValidYear(const int year);
bool isValidDay(const int day, const int month, const int year);
bool isValidDate(const int year,const int month, const int day);
bool isValidGender(int gender);
bool isAgeVaildBasedOnBirth(int userAge, int day, int month,int year);


int generatedCode;
time_t codeGenerationTime;


void SendVarificationCode();
bool isVarificationOK();


//Error printing function declaration

void printPasswordError(const char *password);
void printAgeError(int age);
void printUserNameError(const char *username);
void printNameError(const char *name);
void printEmailError(const char *email);


//helping functions

bool isLeapYear(int year);
void toLowerCase(char* input);
void trimWhitespace(char* str);
void ClearConsole();
void sleep_in_seconds(int seconds);
int calculateAge(int birth_day,int birth_month, int birth_year);

// Function Implementations

bool isValidPassword(const char *password) {
    int length = strlen(password);

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    bool isValid = true;

    // Check password length
    if (length < 8) {
        isValid = false;
        return false;
    } else if (length > 20) {
        isValid = false;
        return false;
    }

    // Check for character types
    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) hasUpper = true;
        if (islower(password[i])) hasLower = true;
        if (isdigit(password[i])) hasDigit = true;
        if (ispunct(password[i])) hasSpecial = true;
    }

    // Provide feedback for missing criteria
    if (!hasUpper || !hasLower || !hasDigit || !hasSpecial) {
        isValid = false;
        return false;
    }

    // Check for consecutive identical characters
    for (int i = 0; i < length - 1; i++) {
        if (password[i] == password[i + 1]) {
            isValid = false;
            return false;
            break;
        }
    }

    return isValid;
}

bool isValidAge(int age) {
    // Check if the age is within the valid range
    return (age >= 12 && age <= 120);
}

bool isValidUserName(const char *username) {
    int length = strlen(username);

    // Check if the length is within the valid range
    if (length < 2 || length > 20) {
        return false;
    }

    // Check if the username starts with a letter
    if (!isalpha(username[0])) {
        return false;
    }

    // Check if all characters are alphanumeric and there are no spaces
    for (int i = 0; i < length; i++) {
        if (!isalnum(username[i])) {
            return false;
        }
    }

    return true;
}

bool isValidGender(int gender) {

    if(gender != 0 && gender !=1 && gender !=2)
        return false;

    return true;
}

bool isValidName(const char *name) {
    int length = strlen(name);

    // Check if the length is within the valid range
    if (length < 2 || length > 20) {
        return false;
    }

    // Check if the name contains only alphabetic characters and spaces
    for (int i = 0; i < length; i++) {
        if (!(isalpha(name[i]) || name[i] == ' ')) {
            return false;
        }
    }

    return true;
}

bool isValidEmail(const char *email) {
    int len = strlen(email);

    // Length check (should be between 12 and 20 characters)
    if (len < 12 || len > 30) {
        return false;
    }

    // Check for the presence of exactly one '@' character
    int atCount = 0;
    int atIndex = -1;
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            atCount++;
            atIndex = i;
        }
    }

    // More than one '@' or no '@' at all is invalid
    if (atCount != 1) {
        return false;
    }

    // '@' should not be the first or last character
    if (atIndex == 0 || atIndex == len - 1) {
        return false;
    }

    // Check if there's a period ('.') after '@'
    int periodFound = 0;
    for (int i = atIndex + 1; i < len; i++) {
        if (email[i] == '.') {
            periodFound = 1;
            break;
        }
    }

    if (!periodFound) {
        return false;
    }

    // Check if all characters are valid (alphanumeric, '@', '.', '-', '_')
    for (int i = 0; i < len; i++) {
        if (!isalnum(email[i]) && email[i] != '@' && email[i] != '.' && email[i] != '-' && email[i] != '_') {
            return false;
        }
    }

    return true;
}

bool isValidCountry(const char* input, const char countries[][MAX_NAME_LENGTH], int num_countries) {
    // Copy the input to a modifiable string (to avoid modifying the original input)
    char country_input[MAX_NAME_LENGTH];
    strncpy(country_input, input, MAX_NAME_LENGTH);
    country_input[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    // Trim any leading/trailing whitespace from the input
    trimWhitespace(country_input);

    // Convert the input string to lowercase
    toLowerCase(country_input);

    // Compare with the countries
    for (int i = 0; i < num_countries; i++) {
        char country[MAX_NAME_LENGTH];
        strncpy(country, countries[i], MAX_NAME_LENGTH);
        country[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

        // Trim and convert country names from file to lowercase
        trimWhitespace(country);
        toLowerCase(country);

        if (strcmp(country_input, country) == 0) {
            return true;  // Country found in the list
        }
    }

    return false;  // Country not found
}

bool isValidPhone(const char* PhoneNumber, const char* Country) {
    if (PhoneNumber == NULL || Country == NULL) {
        return false; // Invalid input
    }

    toLowerCase(Country);
    // Check for Israel's country code (+972 or local format 0)
    if (strcmp(Country, "israel") == 0) {
        // Check for international format (+972)
        if (strncmp(PhoneNumber, "+972", 4) == 0 && strlen(PhoneNumber) == 13) {
            return true; // Valid phone number for Israel (international format)
        }
        // Check for local format (0)
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for Israel (local format)
        }
    }

    // Check for USA's country code (+1 or local format 1)
    if (strcmp(Country, "usa") == 0) {
        if (strncmp(PhoneNumber, "+1", 2) == 0 && strlen(PhoneNumber) == 11) {
            return true; // Valid phone number for USA (international format)
        }
        else if (PhoneNumber[0] == '1' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for USA (local format)
        }
    }

    // Check for France's country code (+33 or local format 0)
    if (strcmp(Country, "france") == 0) {
        if (strncmp(PhoneNumber, "+33", 3) == 0 && strlen(PhoneNumber) == 12) {
            return true; // Valid phone number for France (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for France (local format)
        }
    }

    // Check for Germany's country code (+49 or local format 0)
    if (strcmp(Country, "germany") == 0) {
        if (strncmp(PhoneNumber, "+49", 3) == 0 && strlen(PhoneNumber) == 13) {
            return true; // Valid phone number for Germany (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 11) {
            return true; // Valid phone number for Germany (local format)
        }
    }

    // Check for United Kingdom's country code (+44 or local format 0)
    if (strcmp(Country, "united kingdom") == 0) {
        if (strncmp(PhoneNumber, "+44", 3) == 0 && strlen(PhoneNumber) == 13) {
            return true; // Valid phone number for United Kingdom (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 11) {
            return true; // Valid phone number for United Kingdom (local format)
        }
    }

    // Check for India's country code (+91 or local format 0)
    if (strcmp(Country, "india") == 0) {
        if (strncmp(PhoneNumber, "+91", 3) == 0 && strlen(PhoneNumber) == 13) {
            return true; // Valid phone number for India (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for India (local format)
        }
    }

        // Check for Australia's country code (+61 or local format 0)
    if (strcmp(Country, "australia") == 0) {
        if (strncmp(PhoneNumber, "+61", 3) == 0 && strlen(PhoneNumber) == 12) {
            return true; // Valid phone number for Australia (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for Australia (local format)
        }
    }

    // Check for Canada's country code (+1 or local format 1)
    if (strcmp(Country, "canada") == 0) {
        if (strncmp(PhoneNumber, "+1", 2) == 0 && strlen(PhoneNumber) == 12) {
            return true; // Valid phone number for Canada (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for Canada (local format)
        }
    }

    // Check for Brazil's country code (+55 or local format 0)
    if (strcmp(Country, "brazil") == 0) {
        if (strncmp(PhoneNumber, "+55", 3) == 0 && strlen(PhoneNumber) == 14) {
            return true; // Valid phone number for Brazil (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 12) {
            return true; // Valid phone number for Brazil (local format)
        }
    }

    // Check for Mexico's country code (+52 or local format 0)
    if (strcmp(Country, "mexico") == 0) {
        if (strncmp(PhoneNumber, "+52", 3) == 0 && strlen(PhoneNumber) == 13) {
            return true; // Valid phone number for Mexico (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for Mexico (local format)
        }
    }

    // Check for South Africa's country code (+27 or local format 0)
    if (strcmp(Country, "south africa") == 0) {
        if (strncmp(PhoneNumber, "+27", 3) == 0 && strlen(PhoneNumber) == 12) {
            return true; // Valid phone number for South Africa (international format)
        }
        else if (PhoneNumber[0] == '0' && strlen(PhoneNumber) == 10) {
            return true; // Valid phone number for South Africa (local format)
        }
    }

    // Check for Russia's country code (+7 or local format 7)
    if (strcmp(Country, "russia") == 0) {
    // Check for international format (+7)
        if (strncmp(PhoneNumber, "+7", 2) == 0 && strlen(PhoneNumber) == 12) {
        return true; // Valid phone number for Russia (international format)
        }
        // Check for local format (7)
        else if (PhoneNumber[0] == '7' && strlen(PhoneNumber) == 11) {
        return true; // Valid phone number for Russia (local format)
        }
    }



    printf("The Phone number you insert is invalid for my DB, if you continue i wont be able to send you massages. ");

    //The Country Is Not Support My DB , then return false.



    // If the country doesn't match or the number doesn't start with the country code
    return false;
}

bool isValidCity(const char* input, const char cities[][MAX_NAME_LENGTH], int num_cities) {
    // Copy the input to a modifiable string (to avoid modifying the original input)
    char city_input[MAX_NAME_LENGTH];
    strncpy(city_input, input, MAX_NAME_LENGTH);
    city_input[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    // Trim any leading/trailing whitespace from the input
    trimWhitespace(city_input);

    // Convert the input string to lowercase
    toLowerCase(city_input);

    // Compare with the cities
    for (int i = 0; i < num_cities; i++) {
        char city[MAX_NAME_LENGTH];
        strncpy(city, cities[i], MAX_NAME_LENGTH);
        city[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

        // Trim and convert city names from file to lowercase
        trimWhitespace(city);
        toLowerCase(city);

        if (strcmp(city_input, city) == 0) {
            return true;  // City found in the list
        }
    }

    return false;  // City not found
}

bool isValidMonth(const int month) {
    return month >= 1 && month <= 12;
}

bool isValidYear(const int year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int currentYear = tm.tm_year + 1900; // tm_year is years since 1900
    return year >= 1900 && year <= currentYear;
}

bool isLeapYear(int year) {
    // Leap year rule: divisible by 4, but not divisible by 100, except when divisible by 400
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool isValidDay(const int day, const int month, const int year) {
    if (day < 1) {
        return false; // Day must be at least 1
    }

    if(!isValidMonth(month))
    {
        return false;
    }

    // Days in each month (non-leap year)
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for February in leap years
    if (month == 2 && isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    return day <= daysInMonth[month - 1]; // month - 1 because array is 0-based
}

bool isValidDate(const int year,const int month, const int day) {

    bool isYearOk = isValidYear(year);
    bool isMonthOk = isValidMonth(month);
    bool isDayOk = isValidDay(day,month,year);

    return isYearOk && isMonthOk && isDayOk;

}

bool isAgeVaildBasedOnBirth(int userAge, int day,int month, int year) {

    int age = calculateAge(day,month,year);

    return age == userAge;
}

// Code Validation Functions

void SendVarificationCode() {
    // Generate a 6-digit random number
    srand(time(NULL)); // Seed the random number generator
    generatedCode = rand() % 900000 + 100000; // Random 6-digit code

    // Get the current time when the code is generated
    codeGenerationTime = time(NULL);

    // Create a temporary file to store the verification code
    FILE *file = fopen("verification_code.txt", "w");
    if (file == NULL) {
        printf("Failed to create verification code file.\n");
        return;
    }
    fprintf(file, "Verification Code: %d\n", generatedCode);
    fclose(file);

    // Open Notepad with the file containing the verification code
    ShellExecute(NULL, "open", "notepad.exe", "verification_code.txt", NULL, SW_SHOWNORMAL);
}

bool isVarificationOK() {
    int userCode;

    // Prompt user to enter the verification code
    printf("Please enter the verification code shown in Notepad: ");
    scanf("%d", &userCode);

    time_t currentTime = time(NULL);  // Get the current time

    // Check if more than 5 seconds have passed
    if (currentTime - codeGenerationTime >= 10) {
        // More than 5 seconds have passed, close Notepad and clear the generated code
        remove("verification_code.txt");
        system("taskkill /f /im notepad.exe");

        ClearConsole();

        // Clear the generated code for security reasons
        generatedCode = 0;

        printf("Verification failed: Time limit exceeded.\n");
        return false;
    }

    // Compare the user-entered code with the generated code
    if (userCode == generatedCode) {
        return true;  // Code is correct
    } else {
        printf("Code is Incorrect!\n");
        return false;  // Code is incorrect
    }


}


// Error printing functions

void printPasswordError(const char *password) {
    int length = strlen(password);

    // Check password length
    if (length < 8) {
        printf("Your password must be at least 8 characters long.\n");
    } else if (length > 128) {
        printf("Your password must be no longer than 128 characters.\n");
    }

    // Check for character types
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) hasUpper = true;
        if (islower(password[i])) hasLower = true;
        if (isdigit(password[i])) hasDigit = true;
        if (ispunct(password[i])) hasSpecial = true;
    }

    // Provide feedback for missing criteria
    if (!hasUpper) {
        printf("Your password must include at least one uppercase letter.\n");
    }
    if (!hasLower) {
        printf("Your password must include at least one lowercase letter.\n");
    }
    if (!hasDigit) {
        printf("Your password must include at least one digit.\n");
    }
    if (!hasSpecial) {
        printf("Your password must include at least one special character (e.g., !, @, #, $, etc.).\n");
    }

    // Check for consecutive identical characters
    for (int i = 0; i < length - 1; i++) {
        if (password[i] == password[i + 1]) {
            printf("Avoid using consecutive identical characters in your password.\n");
            break;
        }
    }
}

void printAgeError(int age) {
    if (age < 12 || age > 120) {
        printf("The age is out of range. Please enter an age between 12 and 120.\n");
    }
}

void printUserNameError(const char *username) {
    int length = strlen(username);

    if (length < 3 || length > 20) {
        printf("Username must be between 3 and 20 characters long.\n");
    }

    if (!isalpha(username[0])) {
        printf("Username must start with a letter.\n");
    }

    for (int i = 0; i < length; i++) {
        if (!isalnum(username[i])) {
            printf("Username can only contain letters and numbers (no spaces or special characters).\n");
            break;
        }
    }
}

void printNameError(const char *name) {
    int length = strlen(name);

    if (length < 2 || length > 50) {
        printf("Name must be between 2 and 50 characters long.\n");
    }

    for (int i = 0; i < length; i++) {
        if (!(isalpha(name[i]) || name[i] == ' ')) {
            printf("Name can only contain alphabetic characters and spaces.\n");
            break;
        }
    }
}

void printEmailError(const char *email) {
    int len = strlen(email);

    if (len < 12 || len > 20) {
        printf("Error: Email must be between 12 and 20 characters long.\n");
        return;
    }

    int atCount = 0;
    int atIndex = -1;
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            atCount++;
            atIndex = i;
        }
    }

    if (atCount != 1) {
        printf("Error: Email must contain exactly one '@' symbol.\n");
        return;
    }

    if (atIndex == 0 || atIndex == len - 1) {
        printf("Error: '@' cannot be the first or last character.\n");
        return;
    }

    int periodFound = 0;
    for (int i = atIndex + 1; i < len; i++) {
        if (email[i] == '.') {
            periodFound = 1;
            break;
        }
    }

    if (!periodFound) {
        printf("Error: Domain must contain a period ('.') after '@'.\n");
        return;
    }

    for (int i = 0; i < len; i++) {
        if (!isalnum(email[i]) && email[i] != '@' && email[i] != '.' && email[i] != '-' && email[i] != '_') {
            printf("Error: Invalid character '%c' in email.\n", email[i]);
            return;
        }
    }
}


//Helping Functions

void toLowerCase(char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        input[i] = tolower((unsigned char)input[i]);
    }
}

void trimWhitespace(char* str) {
    int start = 0;
    int end = strlen(str) - 1;

    // Remove leading whitespace
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }

    // Remove trailing whitespace
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // Shift the string to the beginning and null-terminate
    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }
    str[end - start + 1] = '\0';  // Null-terminate the string
}

void ClearConsole() {
    #if defined(_WIN32)
        system("cls"); // For Windows
    #else
        system("clear"); // For Linux and macOS
    #endif
}

void sleep_in_seconds(int seconds) {
    #ifdef _WIN32
        Sleep(seconds * 1000);  // Sleep expects milliseconds on Windows
    #elif __linux__ || __APPLE__
        sleep(seconds);         // Sleep expects seconds on Linux/macOS
    #else
        printf("Unsupported OS\n");
    #endif
}

int calculateAge(int birth_day, int birth_month, int birth_year) {
    // Get current date
    time_t t = time(NULL);
    struct tm current_date = *localtime(&t);

    // Correct calculation for current year
    int current_year = current_date.tm_year + 1900; // tm_year gives years since 1900

    // Calculate age
    int age = current_year - birth_year;

    // Check if the birthday has passed this year
    if (current_date.tm_mon + 1 < birth_month ||
        (current_date.tm_mon + 1 == birth_month && current_date.tm_mday < birth_day)) {
        age--; // Subtract one year if the birthday hasn't occurred yet
    }

    return age;
}


#endif // VALIDATION_H_INCLUDED
