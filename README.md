# Online Login System

This project implements a basic **Online Login System** in C. The system allows users to register, log in, change their password, and delete their account. Passwords are encrypted for security purposes. The system consists of two main components: **client** and **server**.

## Features

- **User Registration:** Users can create a new account by providing a username and password.
- **Login:** Users can log in using their credentials. Passwords are encrypted for secure authentication.
- **Change Password:** Logged-in users can change their password after verifying their current password.
- **Delete Account:** Users can delete their account from the system.

## Prerequisites

- C compiler (e.g., GCC)
- Basic knowledge of C programming
- Encryption library (if applicable for your encryption method)

## Installation

To set up and run the project, follow these steps:

1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/online-login-system.git
    ```

2. **Navigate to the project directory:**
    ```bash
    cd online-login-system
    ```

3. **Compile the project:**
    You need to compile `server.c` and `client.c` together. Run:
    ```bash
    gcc -o login_system client.c server.c -lssl -lcrypto
    ```

4. **Run the server and client:**
   - Open two terminal windows:
   - In the first terminal, run the **server**:
     ```bash
     ./server
     ```
   - In the second terminal, run the **client**:
     ```bash
     ./client
     ```

   The client will interact with the server, allowing you to log in, register, change passwords, and delete accounts.

## Usage

### Client Interaction

Upon starting the client, the following options are available:

1. **Login:** Enter your username and password to log in.
2. **Register:** Create a new account by entering a username and password.
3. **Exit:** Exit the system.

### Logged-In User Menu

Once logged in, the following options are available:

1. **Change Password:** Change your password after verifying your current password.
2. **Delete Account:** Delete your account from the system.
3. **Log Out:** Log out of the system.

### Server Interaction

- The **server** listens for requests from the **client**.
- The server processes user registration, login attempts, password changes, and account deletions.

## Functions

### Client-Side Functions

- `Login(User *user)`: Authenticates the user by verifying their username and password.
- `Register(User *user)`: Registers a new user with a username and password.
- `ChangePassword(User *user)`: Allows the user to change their password after authentication.
- `DeleteAccount(User *user)`: Deletes the user's account from the system.

### Server-Side Functions

- `Update_Password(User *user)`: Updates the user's password on the server.
- `Delete_User(User *user)`: Deletes the user's account from the server.

## Security

- **Password Encryption:** Passwords are encrypted using a secure method before they are stored.
- **Secure Authentication:** Login and password change operations are handled securely by comparing encrypted passwords.
