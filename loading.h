#ifndef LOADING_H_INCLUDED
#define LOADING_H_INCLUDED

#include "validation.h"
#include "security.h"
#include "dbms.h"

#define MAX_COUNTRY_NAME_LENGTH 50
#define FILENAME "users.dat"
#define USERS_FILE_SIZE 10000

    char* country_file_name = "countries.txt";


//Function Declaration

    int load_countries_from_file(char countries[][MAX_COUNTRY_NAME_LENGTH]);
    int load_cities_from_file(const char* country, char cities[][MAX_COUNTRY_NAME_LENGTH]);
    void loadDatabaseFromFile(const char *fileName);
    int load_user(const char* username, const char* password, User* user_out);
    void initialize_file();


//Function Implamantation

int load_countries_from_file(char countries[][50]) {
    // Prepend the folder path to the file name
    const char *folder_path = "countries_data/";
    char filepath[MAX_NAME_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s%s", folder_path, country_file_name);

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", filepath);
        return 0;
    }

    int i = 0;
    while (fgets(countries[i], MAX_NAME_LENGTH, file) != NULL) {
        // Remove newline character if present
        countries[i][strcspn(countries[i], "\n")] = '\0';
        i++;
    }

    fclose(file);
    return i; // Return the number of countries read
}

int load_cities_from_file(const char *country, char cities[][MAX_COUNTRY_NAME_LENGTH]) {
    // Prepend the folder path to the file name
    const char *folder_path = "countries_data/";
    char filename[MAX_NAME_LENGTH];
    snprintf(filename, sizeof(filename), "%s%s.txt", folder_path, country);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        return 0;
    }

    int i = 0;
    while (fgets(cities[i], MAX_NAME_LENGTH, file) != NULL) {
        // Remove newline character if present
        cities[i][strcspn(cities[i], "\n")] = '\0';
        i++;
    }

    fclose(file);
    return i; // Return the number of cities read
}

void loadDatabaseFromFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", fileName);
        return;
    }

    char line[1024];
    Table *currentTable = NULL;

    while (fgets(line, sizeof(line), file)) {
        // Remove the trailing newline character
        line[strcspn(line, "\n")] = 0;

        // Check for the start of a table
        if (strncmp(line, "TABLE", 5) == 0) {
            char tableName[MAX_NAME_LENGTH];
            sscanf(line, "TABLE %s", tableName);

            // Find or create a new table
            for (int t = 0; t < MAX_TABLES; t++) {
                if (strcmp(database[t].tableName, tableName) == 0 || database[t].rowCount == 0) {
                    currentTable = &database[t];
                    strcpy(currentTable->tableName, tableName);
                    currentTable->rowCount = 0; // Reset row count
                    break;
                }
            }
        } else if (strncmp(line, "END_TABLE", 9) == 0) {
            currentTable = NULL; // End of the current table
        } else if (currentTable) {
            // Parse a row of data
            Row newRow = {0};
            char *token = strtok(line, ",");
            int column = 0;

            while (token && column < MAX_COLUMNS) {
                strcpy(newRow.values[column++], token);
                token = strtok(NULL, ",");
            }

            // Add the row to the current table
            if (currentTable->rowCount < MAX_ROWS) {
                currentTable->rows[currentTable->rowCount++] = newRow;
            } else {
                printf("Error: Table %s is full.\n", currentTable->tableName);
            }
        }
    }

    fclose(file);
    printf("Database loaded from file: %s\n", fileName);
}


int load_user(const char* username, const char* password, User* user_out) {
    FILE* file = fopen(FILENAME, "rb"); // Open file for reading in binary mode
    if (!file) {
        perror("Failed to open file");
        return 0; // Return 0 if the file cannot be opened
    }

    uint32_t offset = hash_username(username) * sizeof(User); // Calculate the offset based on the hashed username
    fseek(file, offset, SEEK_SET); // Move to the calculated offset

    User found_user;
    fread(&found_user, sizeof(User), 1, file); // Read the User struct from the file
    fclose(file);

    // Validate the username and password
    if (strcmp(found_user.userName, username) == 0 && strcmp(found_user.password, password) == 0) {
        if (user_out) {
            *user_out = found_user; // Copy the user data to the output struct
        }
        return 1; // Return 1 to indicate success
    }

    return 0; // Return 0 if the user is not found or credentials are invalid
}

void initialize_file() {
    FILE* file = fopen(FILENAME, "wb"); // Open file for writing in binary mode
    if (!file) {
        perror("Failed to open file");
        return;
    }

    User empty_user;
    initialize_empty_user(&empty_user); // Initialize an empty user structure

    for (int i = 0; i < USERS_FILE_SIZE; i++) {
        fwrite(&empty_user, sizeof(User), 1, file); // Write empty user structs to the file
    }

    fclose(file);
    printf("File initialized with empty user structures.\n");
}


#endif // LOADING_H_INCLUDED
