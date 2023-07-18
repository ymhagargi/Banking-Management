**Project Description: Online Banking Management System**

The "Online Banking Management System" is a comprehensive and user-friendly project aimed at providing a seamless banking experience with multi-functional capabilities. The primary objective of this project is to develop a secure and efficient online banking system that allows customers to perform various banking transactions conveniently. The project implements a range of features, including account management, user login systems, administrative access control, and multi-threaded concurrent access.

**Key Features:**
1. **Account and Customer Information Management:** All customer information and account transactional details are securely stored in files, ensuring data integrity and confidentiality.

2. **Login System:** Account holders are required to log in with their credentials to access their accounts, providing an added layer of security.

3. **Administrative Access Control:** The application incorporates password-protected administrative access to safeguard the entire management system from unauthorized access.

4. **Multiple Types of Logins:** The system supports three types of logins - normal user, joint account user, and administrator, offering specific privileges based on user roles.

5. **Administrator Functions:** Upon logging in as an administrator, authorized personnel can perform account-related operations, such as adding, deleting, modifying, and searching for specific account details.

6. **Interactive User Menu:** Users are presented with an interactive menu upon connecting to the server. The menu allows customers to choose from various banking functionalities like deposit, withdraw, balance inquiry, password change, view details, and exit.

7. **File Locking Mechanism:** The system employs proper file locking techniques, specifically read and write locks, to ensure data consistency and protect critical data sections for joint account holders during simultaneous operations.

8. **Socket Programming:** The server-client communication is achieved using socket programming. The server maintains the central database and services multiple clients concurrently, allowing customers to connect and access their specific account details securely.

9. **System Calls:** Throughout the project, system calls are utilized instead of library functions where possible, to enhance efficiency and optimize process management, file handling, file locking, multi-threading, and inter-process communication mechanisms.

# Instructions to use

Step 1: Begin by executing the dataop.c program and provide the required registration information.

`$ gcc dataop.c -o data`

`$ ./data`

Step 2: Start the Server

`$ gcc -pthread server.c -o server`

`$ ./server`

Step 3: Launch the Client

`$ gcc client.c -o client`

`$ ./client`
