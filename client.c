    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <conio.h>
    #include "validation.h"
    #include "security.h"
    #include "loading.h"
    #include <winsock2.h>
    //#include "dbms.h"
    #include "input_from_user.h"

    #pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

    #define SERVER_IP "127.0.0.1"  // Change this to the server's IP address
    #define SERVER_PORT 8080
    #define MAX_BUFFER_SIZE 512
    #define MAX_WIDTH 60 // Maximum width of the text box

    const char* users_file = "users.txt";
    const char* userName_file = "users_name_file";
    bool isRegisterSuccessfully = false;
    bool isLoggedIn = false;
    char serverRequestResponse[256] = ""; // Global variable with proper memory allocation

    typedef enum {
    USER_EXISTS,
    SERVER_ERROR,
    CONNECTION_FAILED,
    INITIALIZATION_FAILED,
    SOCKET_CREATION_FAILED,
    REGISTER_OK,
    LOGIN_OK,
    NO_RESPONSE,
    LOGIN_WRONG,
    REGISTER_WRONG,
    DELETE_OK,
    DELETE_WRONG,
    CHANGE_OK,
    CHANGE_WRONG
}ServerResponse;

    //--------------------------------------------//

    char countries[MAX_COUNTRIES][MAX_NAME_LENGTH];
    char cities[MAX_CITIES][MAX_NAME_LENGTH];


    void Login(User *user);
    void Register(User* user);
    bool IsUserExistInSystem(const char *username);
    void DeleteAccount(User *user);
    void ChangePassword(User* user);
    void LogOut(User* user);

    //------------------------------------------//

    void ViewMenu();
    void PrivacyPolicy();


    void InputFirstName(User *user);
    void InputLastName(User *user);
    void InputAge(User *user);
    void InputGender(User *user);
    void InputPassword(User *user);
    void InputBirthDetails(User *user);
    void InputLocationDetails(User *user);
    void InputEmail(User *user);
    void InputPhoneNumber(User *user);
    void InputUserName(User *user);
    void initializeUser(User *user);
    void PrintUserDetails(const User *user);

    void printLine(char c, int length);
    void write_username_to_file(const char* username); //don't neccesay
    bool is_username_exists_in_file(const char* username); // don't neccesary
    void initializeUser(User *user);
    void PrintUserDetails(const User *user);

    ServerResponse send_user_to_server(User *user);
    bool find_user_from_server(const char* username);
    bool send_login_to_server(const char* username, const char* password);

    void clearInputBuffer();

    //-----------------------------------------//

int main() {

    display_menu();

    return 0;
}

void display_menu() {
    int choice;

    while (1) {
        // Display the menu
        printf("\n");
        printf("********************************************\n");
        printf("*                                          *\n");
        printf("*           WELCOME TO LOGIN SYSTEM        *\n");
        printf("*                                          *\n");
        printf("********************************************\n");
        printf("*                                          *\n");
        printf("*   1. Login                               *\n");
        printf("*   2. Register                            *\n");
        printf("*   3. Exit                                *\n");
        printf("*                                          *\n");
        printf("********************************************\n");
        printf("\n");
        printf("Please select an option (1-3): ");

        // Get the user's choice
        if (scanf("%d", &choice) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            continue;
        }
        User user;
        // Handle the user's choice
        switch (choice) {
            case 1:
                Login(&user);
                if(isLoggedIn)
                    {
                        display_loggedIn_menu(&user);
                    }
                break;
            case 2:
                Register(&user);
                break;
            case 3:
                printf("Thank you for using the login system. Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Please select a number between 1 and 4.\n");
        }
    }
}

void display_loggedIn_menu(User *user) {
    int choice;

    while (1) {
        // Display the menu
        printf("\n");
        printf("********************************************\n");
        printf("*                                          *\n");
        printf("*           WELCOME %s         *\n",user->userName);
        printf("*                                          *\n");
        printf("********************************************\n");
        printf("*                                          *\n");
        printf("*   1. Delete Account                      *\n");
        printf("*   2. Change Password                     *\n");
        printf("*   3. Log out                             *\n");
        printf("*                                          *\n");
        printf("********************************************\n");
        printf("\n");
        printf("Please select an option (1-3): ");

        // Get the user's choice
        if (scanf("%d", &choice) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            continue;
        }

        // Handle the user's choice
        switch (choice) {
            case 1:
                DeleteAccount(user);
                break;
            case 2:
                ChangePassword(user);
                break;
            case 3:
                printf("Thank you for using the login system. Goodbye!\n");
                LogOut(user);
                return;
            default:
                printf("Invalid choice. Please select a number between 1 and 4.\n");
        }
    }
}

void printLine(char c, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", c);
    }
    printf("\n");
}

void PrivacyPolicy() {

    char *rules[15] = {
        "1. Information Collection: We collect personal data such as your name, email, and posts.",
        "2. Use of Information: We use your information to enhance your experience on the platform.",
        "3. Data Sharing: Your data is not shared with third parties, unless required by law.",
        "4. Security: We implement measures to protect your personal data from unauthorized access.",
        "5. Cookies: We may use cookies to personalize your experience. You can disable them.",
        "6. Data Retention: We retain your data for as long as necessary to provide our services.",
        "7. User Rights: You can access, modify, or delete your personal information at any time.",
        "8. Third-Party Links: Our platform may contain links to third-party websites, which have their own privacy policies.",
        "9. Marketing: We may send promotional emails, but you can opt-out at any time.",
        "10. Account Deletion: You can request to delete your account and all associated data.",
        "11. Age Restrictions: Our platform is for users 13 and older. We do not knowingly collect data from minors.",
        "12. Consent: By using our platform, you agree to the terms of this Privacy Policy.",
        "13. Changes to Policy: We may update this policy. All changes will be posted on this page.",
        "14. Data Breaches: In case of a breach, we will notify affected users in a timely manner.",
        "15. Contact Us: For any privacy-related questions, please contact us at support@example.com."
    };

    // Print the header with a border
    printLine('*', MAX_WIDTH);
    printf("*%*sPrivacy Policy%*s*\n", (MAX_WIDTH - 15) / 2, "", (MAX_WIDTH - 15) / 2, "");
    printLine('*', MAX_WIDTH);

    // Print each rule with proper formatting
    for (int i = 0; i < 15; i++) {
        int ruleLength = strlen(rules[i]);
        if (ruleLength > MAX_WIDTH - 4) {
            // If the rule text is too long, split it into multiple lines
            char line[MAX_WIDTH - 4];
            int startPos = 0;
            while (startPos < ruleLength) {
                int len = (ruleLength - startPos > MAX_WIDTH - 4) ? MAX_WIDTH - 4 : ruleLength - startPos;
                strncpy(line, &rules[i][startPos], len);
                line[len] = '\0';
                printf("* %-*s *\n", MAX_WIDTH - 4, line);
                startPos += len;
            }
        } else {
            printf("* %-*s *\n", MAX_WIDTH - 4, rules[i]);
        }
    }

    // Print the footer with a border
    printLine('*', MAX_WIDTH);
}

bool IsUserExistInSystem(const char *username) {

    return is_username_exists_in_file(username);
}

void write_username_to_file(const char* username) {
    // Open the file in append mode to add new usernames at the end
    FILE *file = fopen("usernames.txt", "a");

    if (file == NULL) {
        // Error handling: if the file can't be opened
        perror("Error opening file");
        return;
    }

    // Write the username to the file followed by a newline
    fprintf(file, "%s\n", username);

    // Close the file
    fclose(file);
}

bool is_username_exists_in_file(const char* username) {
    FILE *file = fopen("usernames.txt", "r");

    if (file == NULL) {
        // Error handling: if the file can't be opened
        perror("Error opening file");
        return false; // Return 0 to indicate that the file couldn't be read
    }

    char line[256];  // Buffer to store each line from the file

    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character from the line, if it exists
        line[strcspn(line, "\n")] = 0;

        // Compare the username with the current line from the file
        if (strcmp(username, line) == 0) {
            fclose(file);
            return true;  // Username found
        }
    }

    fclose(file);
    return false;  // Username not found
}

void clearInputBuffer() {
    while (getchar() != '\n' && getchar() != EOF);  // Clears the input buffer
}

void InputFirstName(User *user) {

    clearInputBuffer();  // Clear any stray newlines or input


    while (1) {
        printf("Enter First Name: ");
        fgets(user->FirstName, MAX_NAME_LENGTH, stdin);
        user->FirstName[strcspn(user->FirstName, "\n")] = '\0'; // Remove newline

        if (isValidName(user->FirstName)) {
            break;
        }
        printNameError(user->FirstName);
        strcpy(user->FirstName, ""); // Clear invalid input
    }
}

void InputLastName(User *user) {

    while (1) {
        printf("Enter Last Name: ");
        fgets(user->LastName, MAX_NAME_LENGTH, stdin);
        user->LastName[strcspn(user->LastName, "\n")] = '\0';

        if (isValidName(user->LastName)) {
            break;
        }
        printNameError(user->LastName);
        strcpy(user->LastName, ""); // Clear invalid input
    }
}

void InputAge(User *user) {
    while (1) {
        printf("Enter Age: ");
        if (scanf("%d", &user->age) == 1 && isValidAge(user->age)) {
            getchar(); // Clear the newline character from the input buffer
            break;
        }
        getchar(); // Clear invalid input
        printAgeError(&user->age);
        user->age = -1; // Set to invalid default
    }
}

void InputGender(User *user) {

    char genderStr[MAX_NAME_LENGTH];
    while (1) {
        printf("Enter Gender (Male, Female, Other): ");
        fgets(genderStr, MAX_NAME_LENGTH, stdin);
        genderStr[strcspn(genderStr, "\n")] = '\0'; // Remove newline

        Gender userGender = turnCharIntoGender(genderStr);
        if (isValidGender(userGender)) {
            user->userGender = userGender;
            break;
        }
        printf("Invalid Gender. Gender must be Male, Female, or Other.\n");
    }
}

void InputPassword(User *user) {

    while (1) {
        printf("Enter Password: ");
        fgets(user->password, MAX_PASSWORD_LENGTH, stdin);
        user->password[strcspn(user->password, "\n")] = '\0';

        if (isValidPassword(user->password)) {
            char* encryptedPassword = encrypt_password(user->password);
            strcpy(user->password,encryptedPassword);
            return;
        }
        printPasswordError(user->password);
        strcpy(user->password, ""); // Clear invalid input
    }
}

void InputBirthDetails(User *user) {

    while (1) {

        printf("Enter Birth Year: ");
        scanf("%d", &user->UserBirthDetails.YearOfBirth);
        getchar(); // Clear newline

        if(!isValidYear(user->UserBirthDetails.YearOfBirth))
        {
            printf("invalid year!\n");
            user->UserBirthDetails.YearOfBirth = 0;
        }
        else
        {
            break;
        }
    }

    while(1) {

        printf("Enter Birth Month (1-12): ");
        scanf("%d", &user->UserBirthDetails.MonthOfBirth);
        getchar();

        if(!isValidMonth(user->UserBirthDetails.MonthOfBirth))
        {
            printf("invalid month!\n");
            user->UserBirthDetails.MonthOfBirth = 0;
        }
        else
        {
            break;
        }
    }

    while(1) {

        printf("Enter Birth Day (1-31): ");
        scanf("%d", &user->UserBirthDetails.DayOfBirth);
        getchar();

        if(!isValidDay(user->UserBirthDetails.DayOfBirth,user->UserBirthDetails.MonthOfBirth,user->UserBirthDetails.YearOfBirth))
        {
            printf("invalid day!\n");
            user->UserBirthDetails.DayOfBirth = 0;
        }
        else
        {
            break;
        }

    }

    int userAge = calculateAge(user->UserBirthDetails.DayOfBirth, user->UserBirthDetails.MonthOfBirth, user->UserBirthDetails.YearOfBirth);
    if(isValidAge(userAge))
            {
                user->age = userAge;
            }
        else
            printAgeError(userAge);
}

void InputLocationDetails(User *user) {
    char country[MAX_NAME_LENGTH];

    while (1) {
        // Load countries from the file
        int num_countries = load_countries_from_file(countries);
        if (num_countries == 0) {
            printf("No countries loaded.\n");
            return;
        }

        printf("Enter Country: ");
        fgets(country, MAX_NAME_LENGTH, stdin);
        country[strcspn(country, "\n")] = '\0'; // Remove newline character

        if (!isValidCountry(country, countries, MAX_COUNTRIES)) {
            printf("Invalid country!\n");
            continue;
        }

        toLowerCase(user->UserLocationDetails.country);

        // Save valid country to user struct
        strcpy(user->UserLocationDetails.country, country);

        // Load cities for the given country
        int num_cities = load_cities_from_file(country, cities);
        if (num_cities == 0) {
            printf("No cities loaded for the given country.\n");
            return;
        }
        break;
    }

    while(1)
    {
        printf("Enter City: ");
        fgets(user->UserLocationDetails.city, MAX_NAME_LENGTH, stdin);
        user->UserLocationDetails.city[strcspn(user->UserLocationDetails.city, "\n")] = '\0';

        if (!isValidCity(user->UserLocationDetails.city, cities, MAX_CITIES)) {
            printf("Invalid city!\n");
            continue;
        }

        toLowerCase(user->UserLocationDetails.city);

        printf("Enter ZipCode: ");
        fgets(user->UserLocationDetails.zipcode, MAX_NAME_LENGTH, stdin);
        user->UserLocationDetails.zipcode[strcspn(user->UserLocationDetails.zipcode, "\n")] = '\0';

        break; // Exit loop once all inputs are valid
    }
}

void InputEmail(User *user) {

    while (1) {
        printf("Enter Email: ");
        fgets(user->email, MAX_EMAIL_LENGTH, stdin);
        user->email[strcspn(user->email, "\n")] = '\0';

        if (isValidEmail(user->email)) {
            break;
        }
        printEmailError(user->email);
        strcpy(user->email, ""); // Clear invalid input
    }
}

void InputPhoneNumber(User *user) {

    while (1) {
        printf("Enter Phone Number: ");
        fgets(user->phoneNumber, MAX_PHONE_LENGTH, stdin);
        user->phoneNumber[strcspn(user->phoneNumber, "\n")] = '\0';

        if (isValidPhone(user->phoneNumber, user->UserLocationDetails.country)) {
            break;
        }
        printf("Invalid Phone Number\n");
        strcpy(user->phoneNumber, ""); // Clear invalid input
    }
}

void InputUserName(User *user) {

    while (1) {
        printf("Enter Username: ");
        fgets(user->userName, MAX_NAME_LENGTH, stdin);
        user->userName[strcspn(user->userName, "\n")] = '\0';

        if (isValidUserName(user->userName)) {
            break;
        }
        printUserNameError(user->userName);
        strcpy(user->userName, ""); // Clear invalid input
    }
}

void initializeUser(User *user) {
    InputFirstName(user);
    InputLastName(user);
    InputGender(user);
    InputPassword(user);
    InputBirthDetails(user);
    InputLocationDetails(user);
    InputEmail(user);
    InputUserName(user);
    InputPhoneNumber(user);

    //printf("FINISH THE INITIALLIZE PROCCESS\n");
}

void PrintUserDetails(const User *user) {
    printf("\n======================== USER DETAILS ========================\n");
    printf("First Name       : %s\n", user->FirstName);
    printf("Last Name        : %s\n", user->LastName);
    printf("Age              : %d\n", user->age);
    printf("Gender           : %s\n", turnGenderToString(user->userGender)); // Assuming a function to convert Gender to string
    printf("Username         : %s\n", user->userName);
    printf("Email            : %s\n", user->email);
    printf("Passowrd         : %s\n",user->password);
    printf("Phone Number     : %s\n", user->phoneNumber);
    printf("Location         : %s, %s, %s\n",
           user->UserLocationDetails.city,
           user->UserLocationDetails.country,
           user->UserLocationDetails.zipcode);
    printf("Date of Birth    : %02d/%02d/%04d\n",
           user->UserBirthDetails.DayOfBirth,
           user->UserBirthDetails.MonthOfBirth,
           user->UserBirthDetails.YearOfBirth);
    printf("=============================================================\n");
}

void Register(User *user) {
    initializeUser(user);
    user->userRequest = REGISTER;

    ServerResponse response = send_user_to_server(user);
    if (response == REGISTER_OK) {
        printf("User registered successfully and saved to the server!\n");
        isRegisterSuccessfully = true;
    } else if (response == USER_EXISTS) {
        printf("This User Exists In The System. Want To Log-In: 1:yes / 0:no ");
        int choice;
        while (1) {
            scanf("%d", &choice);
            if (choice == 1) {
                Login(user);
                break;
            } else if (choice == 0) {
                printf("OK! The program will close.\n");
                return;
            } else {
                printf("Invalid choice, try again: ");
            }
        }
    } else {
        printf("Failed to save user to the server. Please try again later.\n");
    }
}

ServerResponse send_user_to_server(User *user) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock initialization failed.\n");
        return INITIALIZATION_FAILED;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return SOCKET_CREATION_FAILED;
    }

    // Define the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT); // Replace SERVER_PORT with your server's port number
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // Replace SERVER_IP with "127.0.0.1" for local testing

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to connect to the server.\n");
        closesocket(clientSocket);
        WSACleanup();
        return CONNECTION_FAILED;
    }

    // Send the User struct to the server
    int bytesSent = send(clientSocket, (char*)user, sizeof(User), 0);
    if (bytesSent == SOCKET_ERROR) {
        printf("Failed to send data to the server.\n");
        closesocket(clientSocket);
        WSACleanup();
        return SERVER_ERROR;
    }

    // Receive response from the server
    char serverResponse[256];
    int responseLen = recv(clientSocket, serverResponse, sizeof(serverResponse) - 1, 0);
    if (responseLen > 0) {
        serverResponse[responseLen] = '\0'; // Null-terminate the response
        strcpy(serverRequestResponse,serverResponse);

        if (strcmp(serverResponse, "RegisterOK") == 0) {
            closesocket(clientSocket);
            WSACleanup();
            return REGISTER_OK;
        }
        else if(strcmp(serverResponse, "RegisterError") == 0)
        {
            printf("failed to register the user, try again later...\n");
            return REGISTER_WRONG;
        }
        else if(strcmp(serverResponse, "UserExists") == 0)
        {
            printf("User %s already exists\n",user->userName);
            return USER_EXISTS;
        }
        else if(strcmp(serverResponse, "LogInOK") == 0)
        {
            printf("User %s log-in successfully\n",user->userName);
            return LOGIN_OK;
        }
        else if(strcmp(serverResponse,"LogInOK") == 0)
        {
            printf("user could not log in. \n");
            return LOGIN_WRONG;
        }
        else if(strcmp(serverResponse,"DeleteOk") == 0)
        {
            printf("user delete account was success!\n");
            return DELETE_OK;
        }
        else if(strcmp(serverResponse,"DeleteError") == 0)
        {
            printf("user delete account FAILED!\n");
            return DELETE_WRONG;
        }
        else if(strcmp(serverResponse,"ChangeOk") == 0)
        {
            printf("user update password was success!\n");
            return CHANGE_OK;
        }
        else if(strcmp(serverResponse,"ChangeError") == 0)
        {
            printf("user update password FAILED!\n");
            return CHANGE_WRONG;
        }
         else {
            printf("Server error: %s\n", serverResponse);
            return SERVER_ERROR;
        }
    } else {
        printf("No response from the server.\n");
        return NO_RESPONSE;
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    return false;
}

void initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(EXIT_FAILURE);
    }
}

void Login(User *user) {
    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    // Clear the input buffer in case of leftover characters
    while (getchar() != '\n' && !feof(stdin));

    // Prompt for username
    printf("Enter username: ");
    if (!fgets(username, MAX_NAME_LENGTH, stdin)) {
        printf("Error reading username. Please try again.\n");
        return;
    }
    username[strcspn(username, "\n")] = 0; // Remove newline character

    // Prompt for password
    printf("Enter password: ");
    if (!fgets(password, MAX_PASSWORD_LENGTH, stdin)) {
        printf("Error reading password. Please try again.\n");
        return;
    }
    password[strcspn(password, "\n")] = 0; // Remove newline character

    // Copy the username and password into the user struct
    strcpy(user->userName, username);
    strcpy(user->password, password);

    // Set the user's request type to LOGIN
    user->userRequest = LOGIN;

    // Send the login request to the server
    ServerResponse response = send_user_to_server(user);
    if (response == LOGIN_OK) {
        // Send a verification code
        SendVarificationCode();

        // Check if the verification is successful
        if (isVarificationOK()) {
            printf("Login successful!\n");
            isLoggedIn = true;
        } else {
            printf("Verification failed. Please try again.\n");
        }
    } else {
        printf("Login failed. Please check your username and password and try again.\n");
    }
}

void LogOut(User *user) {

    isLoggedIn = false;
}

void DeleteAccount(User *user) {

    user->userRequest = DELETE_ACCOUNT;

    ServerResponse response = send_user_to_server(user);

    if(response == DELETE_OK)
    {
        printf("User Account was successfully deleted. \n");
        return;
    }
    else if(response == DELETE_WRONG)
    {
        printf("User Account Deletion failed , try again later. \n");
        return;
    }
    else
    {
        printf("Error Occured , Try again later\n");
        return;
    }
}

void ChangePassword(User *user) {

    char PrevPassword[MAX_PASSWORD_LENGTH];

    // Clear the input buffer to avoid leftover characters
    while (getchar() != '\n' && !feof(stdin));

    // Prompt for the current password
    printf("Enter your current password: ");
    if (!fgets(PrevPassword, MAX_PASSWORD_LENGTH, stdin)) {
        printf("Error reading input. Please try again.\n");
        return;
    }
    PrevPassword[strcspn(PrevPassword, "\n")] = 0; // Remove newline
    // Encrypt and verify the current password
    char *encrypted = encrypt_password(PrevPassword);

    if (strcmp(user->password, PrevPassword) == 0) {
        // Prompt for the new password
        printf("insert your new password\n");
        InputPassword(user);
        user->userRequest = CHANGE_PASSWORD;


        // Send the updated user to the server
        ServerResponse response = send_user_to_server(user);

        // Handle server response
        if (response == CHANGE_OK) {
            printf("Your password was updated successfully.\n");
        } else if (response == CHANGE_WRONG) {
            printf("Failed to update your password.\n");
        } else {
            printf("An error occurred. Please try again later.\n");
        }
    } else {
        printf("The current password you entered is incorrect.\n");
    }
}



