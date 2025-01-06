#ifndef INPUT_FROM_USER_H_INCLUDED
#define INPUT_FROM_USER_H_INCLUDED


    typedef struct
    {
        int YearOfBirth;
        int MonthOfBirth;
        int DayOfBirth;

    }BirthDetails;

    typedef struct
    {
        char zipcode[MAX_NAME_LENGTH];
        char country[MAX_NAME_LENGTH];
        char city[MAX_NAME_LENGTH];

    }LocationDetails;

    typedef enum
    {
        MALE, //0
        FEMALE, //1
        OTHER, //2
        UNKNOWN_GENDER //3
    }Gender;

    typedef enum
    {
        ONLINE,
        OFFLINE,
        SUSPENDED
    }Status;

    typedef enum
    {
        REGISTER,
        LOGIN,
        DELETE_ACCOUNT,
        CHANGE_PASSWORD
    }UserRequest;

    #pragma pack(1)
    typedef struct
    {
        char FirstName[MAX_NAME_LENGTH];
        char LastName[MAX_NAME_LENGTH];
        int age;
        Gender userGender;
        Status userStatus;
        char password[65];
        BirthDetails UserBirthDetails;
        LocationDetails UserLocationDetails;
        char email[MAX_EMAIL_LENGTH];
        char phoneNumber[MAX_PHONE_LENGTH];
        char userName[MAX_NAME_LENGTH];
        UserRequest userRequest;

    }User;
#pragma pack()



    const char* turnGenderToString(Gender gender);
    const char* turnStatusToString(Status status);
    Gender turnCharIntoGender(const char* gender);
    void initialize_empty_user(User *user);

    const char* turnGenderToString(Gender gender) {
    switch (gender) {
        case MALE:
            return "Male";
        case FEMALE:
            return "Female";
        case OTHER:
            return "Other";
        default:
            return "Unknown";
    }
}

    const char* turnStatusToString(Status status) {
    switch (status) {
        case ONLINE:
            return "Online";
        case OFFLINE:
            return "Offline";
        case SUSPENDED:
            return "Suspended";
        default:
            return "\0";
    }
}

Gender turnCharIntoGender(const char* gender) {
    // Make a copy of the input string to avoid modifying the original const string
    char mutableInput[MAX_NAME_LENGTH];
    strncpy(mutableInput, gender, MAX_NAME_LENGTH - 1); // Ensure it doesn't overflow
    mutableInput[MAX_NAME_LENGTH - 1] = '\0';  // Null-terminate the string

    // Now pass the copy of the string to toLowerCase
    toLowerCase(mutableInput);

    if (strcmp(mutableInput, "male") == 0) {
        return MALE;
    } else if (strcmp(mutableInput, "female") == 0) {
        return FEMALE;
    } else if (strcmp(mutableInput, "other") == 0) {
        return OTHER;
    } else {
        printf("Invalid gender\n");
        return UNKNOWN_GENDER;
    }
}

void initialize_empty_user(User *user) {
    // Initialize strings to empty
    memset(user->FirstName, 0, MAX_NAME_LENGTH);
    memset(user->LastName, 0, MAX_NAME_LENGTH);
    memset(user->password, 0, MAX_PASSWORD_LENGTH);
    memset(user->email, 0, MAX_EMAIL_LENGTH);
    memset(user->phoneNumber, 0, MAX_PHONE_LENGTH);
    memset(user->userName, 0, MAX_NAME_LENGTH);

    // Initialize integers to 0
    user->age = 0;
    user->UserBirthDetails.YearOfBirth = 0;
    user->UserBirthDetails.MonthOfBirth = 0;
    user->UserBirthDetails.DayOfBirth = 0;

    // Initialize enums to default values
    user->userGender = UNKNOWN_GENDER;
    user->userStatus = OFFLINE;

    // Initialize LocationDetails
    memset(user->UserLocationDetails.zipcode, 0, MAX_NAME_LENGTH);
    memset(user->UserLocationDetails.country, 0, MAX_NAME_LENGTH);
    memset(user->UserLocationDetails.city, 0, MAX_NAME_LENGTH);
}

#endif // INPUT_FROM_USER_H_INCLUDED
