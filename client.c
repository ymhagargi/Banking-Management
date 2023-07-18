#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

typedef struct normal_user{
	int userID;
	char name[30];
	char password[10];
	int account_num;
	float acc_balance;
	char status[20];
}normal_user;

typedef struct joint_user{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_num;
	float acc_balance;
	char status[20];
}joint_user;

typedef struct administrator{
	int userID;
	char username[30];
	char password[10];
}administrator;

void attempt_normal_user_login(int sock_desc);
void attempt_joint_user_login(int sock_desc);
void attempt_admin_login(int sock_desc);
void show_menu(int sock_desc);
void deposit(int sock_desc);
void withdraw(int sock_desc);
void balance_enquiry(int sock_desc);
void password_change(int sock_desc);
void view_details(int sock_desc);
void add_account(int sock_desc);
void delete_account(int sock_desc);
void modify_account(int sock_desc);
void search_account_details(int sock_desc);
int choice,current_user_ID;

void selectChoice(int sock_desc){ //3 types of login to choose from: normal user, joint account user; administrator 
	while(1){
		write(1,"1 : Normal User\n", sizeof("1 : Normal User\n"));
		write(1,"2 : Joint Account User\n", sizeof("2 : Joint Account User\n"));
		write(1,"3 : Administrator\n", sizeof("3 : Administrator\n"));
		write(1,"Select choice : ", sizeof("Select choice : "));
		scanf("%d", &choice);
		printf("choice : %d\n",choice);
		if(choice == 1){
			attempt_normal_user_login(sock_desc); 
			break;
		}
		else if(choice == 2){
			attempt_joint_user_login(sock_desc);
			break;
		}
		else if(choice == 3){
			attempt_admin_login(sock_desc);
			break;
		}
		else write(1,"Invalid choice\n\n",sizeof("Invalid choice\n\n"));
	}
	return;
}

/** If normal user or joint account user option is chosen,
 * one can deposit money; withdraw money; check account balance;
 * view account details; exit from menu
 */ 
void show_menu(int sock_desc){  
	int select_choice;
	if(choice == 1 || choice == 2){
		write(1, "1 : Deposit\n", sizeof("1 : Deposit\n"));
		write(1, "2 : Withdraw\n", sizeof("2 : Withdraw\n"));
		write(1, "3 : Balance Enquiry\n", sizeof("3 : Balance Enquiry\n"));
		write(1, "4 : Password Change\n", sizeof("4 : Password Change\n"));
		write(1, "5 : View Details\n", sizeof("5 : View Details\n"));
		write(1, "6 : Exit\n", sizeof("6 : Exit\n"));
		write(1, "Choose an choice : ", sizeof("Choose an choice : "));
		scanf("%d", &select_choice);
		printf("Choice : %d\n", select_choice);
		switch(select_choice){
		case 1 :
			deposit(sock_desc);
			break;
		case 2 :
			withdraw(sock_desc);
			break;
		case 3 :
			balance_enquiry(sock_desc);
			break;
		case 4 :
			password_change(sock_desc);
			break;
		case 5 :
			view_details(sock_desc);
			break;
		case 6 :
			write(sock_desc, &select, sizeof(int));
			write(1, "Thank you\n", sizeof("Thank you\n"));
			exit(0);
		default :
			write(1, "Invalid choice\n\n", sizeof("Invalid choice\n\n"));
			show_menu(sock_desc);
			break;
		}
	}
	/** If administrator option is chosen,
	 * one can add an account; delete an account; modify an account;
	 * search for account details; exit from menu
 	 */ 
	else if(choice == 3){
		write(1,"1 : Add Account\n", sizeof("1 : Add Account\n"));
		write(1,"2 : Delete Account\n", sizeof("2 : Delete Account\n"));
		write(1,"3 : Modify Account\n", sizeof("3 : Modify Account\n"));
		write(1,"4 : Search Account Details\n", sizeof("4 : Search Account Details\n"));
		write(1,"5 : Exit\n", sizeof("6 : Exit\n"));
		write(1,"Choose an choice : ", sizeof("Choose an choice : "));
		scanf("%d", &select_choice);
		printf("Choice : %d\n", select_choice);
		switch(select_choice){
		case 1 :
			add_account(sock_desc);
			break;
		case 2 :
			delete_account(sock_desc);
			break;
		case 3 :
			modify_account(sock_desc);
			break;
		case 4 :
			search_account_details(sock_desc);
			break;
		case 5 :
			write(sock_desc,&select,sizeof(int));
			write(1,"Thank you\n",sizeof("Thank you\n"));
			exit(0);
		default :
			write(1,"Invalid choice\n\n",sizeof("Invalid choice\n\n"));
			show_menu(sock_desc);
			break;
		}
	}
}

void attempt_normal_user_login(int sock_desc){
	bool result;
	normal_user current_user;
	write(1, "User ID : ", sizeof("User ID : "));
	scanf("%d", &current_user.userID);
	current_user_ID = current_user.userID;
	write(1, "Password : ", sizeof("Password : "));
	scanf("%s", current_user.password);
	write(sock_desc, &choice, sizeof(int));
	write(sock_desc, &current_user, sizeof(normal_user));
	read(sock_desc, &result, sizeof(result));
	if(!result){
		write(1, "Invalid login\n\n", sizeof("Invalid login\n\n"));
		selectChoice(sock_desc);
	}
	else{
		write(1, "Succesful login\n\n", sizeof("Succesful login\n\n"));
	}
	return;
}

void attempt_joint_user_login(int sock_desc){
	bool result;
	joint_user current_user;
	write(1, "User ID : ", sizeof("User ID : "));
	scanf("%d", &current_user.userID);
	current_user_ID = current_user.userID;
	write(1, "Password : ", sizeof("Password : "));
	scanf("%s", current_user.password);
	write(sock_desc, &choice, sizeof(int));
	write(sock_desc, &current_user, sizeof(joint_user));
	read(sock_desc, &result, sizeof(result));
	if(!result){
		write(1, "Invalid login\n\n", sizeof("Invalid login\n\n"));
		selectChoice(sock_desc);
	}
	else{
		write(1, "Successful login\n\n",sizeof("Successful login\n\n"));
	}
	return;
}

void attempt_admin_login(int sock_desc){
	bool result;
	administrator current_user;
	write(1, "User ID : ", sizeof("User ID : "));
	scanf("%d", &current_user.userID);
	current_user_ID = current_user.userID;
	write(1, "Password : ", sizeof("Password : "));
	scanf("%s", current_user.password);
	write(sock_desc, &choice, sizeof(int));
	write(sock_desc, &current_user, sizeof(administrator));
	read(sock_desc, &result, sizeof(result));
	if(!result){
		write(1, "Invalid login\n\n", sizeof("Invalid login\n\n"));
		selectChoice(sock_desc);
	}
	else{
		write(1, "Successful login\n\n", sizeof("Successful login\n\n"));
	}
	return;
}
/**
 * To enter amount to be deposited by customer into account 
 */
void deposit(int sock_desc){
	float amt;
	int select = 1;
	bool result;
	write(1, "Enter amount to be deposited : Rs.", sizeof("Enter amount to be deposited : Rs."));
	scanf("%f", &amt);
	while(amt <= 0){
		printf("Invalid amount\n");
		write(1, "Enter amount to be deposited : Rs.", sizeof("Enter amount to be deposited : Rs."));
		scanf("%f", &amt);
	}
	write(sock_desc, &select, sizeof(int));
	write(sock_desc, &amt, sizeof(float));
	read(sock_desc, &result, sizeof(result));
	if(!result){
		write(1, "Could not deposit money\n\n", sizeof("Could not deposit money\n\n"));
	}
	else{
		write(1, "Succesfully deposited\n\n", sizeof("Succesfully deposited\n\n"));
	}
	show_menu(sock_desc);
	return;
}
/**
 * To withdraw desired amount from account 
 */
void withdraw(int sock_desc){
	float amt;
	int select = 2;
	bool result;
	write(1, "Enter amount to be withdrawn : Rs.", sizeof("Enter amount to be withdrawn : Rs."));
	scanf("%f", &amt);
	while(amt <= 0){
		printf("Invalid amount\n");
		write(1, "Enter amount to be withdrawn : Rs.", sizeof("Enter amount to be withdrawn : Rs."));
		scanf("%f", &amt);
	}
	write(sock_desc,&select,sizeof(int));
	write(sock_desc,&amt,sizeof(float));
	read(sock_desc,&result,sizeof(result));
	if(!result){
		write(1, "Error withdrawing money, please check your account balance\n\n", sizeof("Error withdrawing money, please check your account balance\n\n"));
	}
	else{
		write(1,"Successfully withdrew money\n\n",sizeof("Succesfully withdrew money\n\n"));
	}
	show_menu(sock_desc);
	return;
}
/**
 * To check how much money is there in the account 
 */
void balance_enquiry(int sock_desc){
	float amt;
	int select = 3;
	int len;
	write(sock_desc, &select, sizeof(int));
	len = read(sock_desc, &amt, sizeof(float));
	write(1, "Account Balance: Rs.", sizeof("Account Balance: Rs."));
	printf("%0.2f\n\n", amt);
	show_menu(sock_desc);
	return;
}
/**
 * To change the account password 
 */
void password_change(int sock_desc){
	int select=4;
	char nu_password[10];
	bool result;
	write(1, "Enter new password(Limit: 10 characters): ",sizeof("Enter new password(Limit: 10 characters): "));
	scanf("%s", nu_password);
	write(sock_desc, &select, sizeof(int));
	write(sock_desc, nu_password, sizeof(nu_password));
	read(sock_desc, &result, sizeof(result));
	if(!result){
		write(1, "Error! Cannot change password\n\n", sizeof("Error! Cannot change password\n\n"));
	}
	else{
		write(1,"Password succesfully changed\n\n",sizeof("Password succesfully changed\n\n"));
	}
	show_menu(sock_desc);
	return;
}
/**
 * To view account details such as: User ID; Name of account holder; Account number;
 * Account balance; Status of the account 
 */
void view_details(int sock_desc){
	int select = 5;
	write(sock_desc, &select, sizeof(int));
	if(choice == 1){ //normal user option
		normal_user current_user1;
		read(sock_desc, &current_user1, sizeof(normal_user));
		printf("User ID : %d\n", current_user1.userID);
		printf("Name : %s\n", current_user1.name);
		printf("Account Number : %d\n", current_user1.account_num);
		printf("Amount Balance : Rs.%0.2f\n", current_user1.acc_balance);
		printf("Account Status : %s\n\n", current_user1.status);
	}
	else if(choice == 2){ //joint user account option
		joint_user current_user2;
		read(sock_desc,&current_user2,sizeof(joint_user));
		printf("User ID : %d\n", current_user2.userID);
		printf("Main Account Holder's Name : %s\n", current_user2.name1);
		printf("Other Account Holder's Name : %s\n", current_user2.name2);
		printf("Account Number : %d\n", current_user2.account_num);
		printf("Available Balance : Rs.%0.2f\n", current_user2.acc_balance);
		printf("Status : %s\n\n", current_user2.status);
	}
	show_menu(sock_desc);
	return;
}
/**
 * Administrator can add account
 */
void add_account(int sock_desc){
	int select = 1;
	int account_type;
	bool result;
	write(sock_desc,&select,sizeof(int));
	write(1,"Enter the account type to be added(1: Normal Account 2: Joint Account): ", sizeof("Enter the account type to be added(1: Normal Account 2: Joint Account): "));
	scanf("%d", &account_type);
	write(sock_desc, &account_type, sizeof(int));
	if(account_type == 1){ //normal user option
		normal_user new_normal_user;
		write(1, "Account holder name: ", sizeof("Account holder name: "));
		scanf(" %[^\n]", new_normal_user.name);
		write(1, "Password(Limit: 10 characters) : ", sizeof("Password(Limit: 10 characters) : "));
		scanf("%s", new_normal_user.password);
		write(1, "Initial deposit: Rs.", sizeof("Initial deposit: Rs."));
		scanf("%f", &new_normal_user.acc_balance);
		write(sock_desc,&new_normal_user, sizeof(normal_user));
	}
	if(account_type == 2){ //joint user account option
		joint_user new_jacc_user;
		write(1,"Primary account holder name: ", sizeof("Primary account holder name: "));
		scanf(" %[^\n]", new_jacc_user.name1);
		write(1,"Secondary account holder name: ", sizeof("Secondary account holder name: "));
		scanf(" %[^\n]", new_jacc_user.name2);
		write(1,"Password(Limit: 10 characters) : ", sizeof("Password(Limit: 10 characters) : "));
		scanf("%s", new_jacc_user.password);
		write(1,"Initial deposit: Rs.",sizeof("Initial deposit: Rs."));
		scanf("%f", &new_jacc_user.acc_balance);
		write(sock_desc, &new_jacc_user, sizeof(joint_user));
	}
	read(sock_desc,&result,sizeof(result));
	if(!result){
		write(1, "Error! Could not add account\n\n", sizeof("Error! Could not add account\n\n"));
	}
	else{
		write(1, "Account added succesfully\n\n", sizeof("Account added succesfully\n\n"));
	}
	show_menu(sock_desc);
	return;
}
/**
 * Administrator can delete account
 */
void delete_account(int sock_desc){
	int select = 2;
	int type, userID;
	bool result;
	write(sock_desc, &select, sizeof(int));
	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d", &type);
	write(sock_desc, &type, sizeof(int));
	write(1,"User ID : ", sizeof("User ID : "));
	scanf("%d", &userID);
	write(sock_desc, &userID, sizeof(int));
	read(sock_desc, &result, sizeof(result)); //from the server
	if(!result){
		write(1, "Error! Cannot delete account ,enter valid User ID\n\n", sizeof("Error! Cannot delete account ,enter valid User ID\n\n"));
	}
	else{
		write(1, "Account deleted succesfully\n\n", sizeof("Account deleted succesfully\n\n"));
	}
	show_menu(sock_desc);
	return;
}
/**
 * Administrator can modify account details such as: User ID; Account number;
 * Name of account holder(both names if joint account); Account password; Change account balance
 */
void modify_account(int sock_desc){
	int select = 3;
	int type;
	bool result;
	write(sock_desc, &select, sizeof(int));
	write(1, "Enter account type(1: Normal Account 2: Joint Account): ", sizeof("Enter account type(1: Normal Account 2: Joint Account): "));
	scanf("%d",&type);
	write(sock_desc,&type,sizeof(int));
	if(type == 1){ //normal user option
		normal_user modify_normal_user;
		write(1, "User ID: ", sizeof("User ID: "));
		scanf("%d", &modify_normal_user.userID);
		write(1, "Account number: ", sizeof("Account number: "));
		scanf("%d", &modify_normal_user.account_num);
		write(1,"New account holder name: ",sizeof("New account holder name: "));
		scanf(" %[^\n]", modify_normal_user.name);
		write(1,"New password(Limit: 10 characters): ",sizeof("New password(Limit: 10 characters): "));
		scanf("%s", modify_normal_user.password);
		write(1,"New account balance: ",sizeof("New account balance: "));
		scanf("%f", &modify_normal_user.acc_balance);
		write(sock_desc, &modify_normal_user, sizeof(normal_user));
	}
	if(type == 2){ //joint user account option
		joint_user modify_jacc_user;
		write(1, "User ID: ", sizeof("User ID: "));
		scanf("%d", &modify_jacc_user.userID);
		write(1, "Account number: ",sizeof("Account number: "));
		scanf("%d", &modify_jacc_user.account_num);
		write(1, "New primary account holder name: ", sizeof("New primary account holder name: "));
		scanf(" %[^\n]", modify_jacc_user.name1);
		write(1, "New secondary account holder name: ", sizeof("New secondary account holder name: "));
		scanf(" %[^\n]", modify_jacc_user.name2);
		write(1, "New password(Limit: 10 characters): ", sizeof("New password(Limit: 10 characters): "));
		scanf("%s", modify_jacc_user.password);
		write(1, "New account balance: ",sizeof("New account balance: "));
		scanf("%f", &modify_jacc_user.acc_balance);
		write(sock_desc, &modify_jacc_user, sizeof(joint_user));
	}
	read(sock_desc, &result, sizeof(result));
	if(!result){
		write(1, "Error! Cannot modify account ,enter valid User ID and Account number\n\n", sizeof("Error! Cannot modify account ,enter valid User ID and Account number\n\n"));
	}
	else{
		write(1, "Account modified succesfully\n\n", sizeof("Account modified succesfully\n\n"));
	}
	show_menu(sock_desc);
	return;
}
/**
 * Administrator can search for specific account details such as: 
 * User ID; Name of account holder(both if joint account);
 * Account number; Account balance; Account status
 */
void search_account_details(int sock_desc){
	int select = 4;
	int accoun_ttype, len;
	bool result;
	write(sock_desc, &select, sizeof(int));
	write(1, "Enter account type(1: Normal Account 2: Joint Account): ", sizeof("Enter account type(1: Normal Account 2: Joint Account): "));
	scanf("%d", &accoun_ttype);
	write(sock_desc, &accoun_ttype, sizeof(int));
	if(accoun_ttype == 1){ //normal account option
		normal_user search_normal_user;
		int normal_userID;
		write(1,"User ID: ", sizeof("User ID: "));
		scanf("%d", &normal_userID);
		write(sock_desc, &normal_userID, sizeof(int));
		len = read(sock_desc,&search_normal_user, sizeof(normal_user));
		if(len == 0){
			write(1, "Enter valid User ID\n\n", sizeof("Enter valid User ID\n\n"));
		}
		else{
			printf("User ID : %d\n", search_normal_user.userID);
			printf("Name : %s\n", search_normal_user.name);
			printf("Account number : %d\n", search_normal_user.account_num);
			printf("Account balance : Rs.%0.2f\n", search_normal_user.acc_balance);
			printf("Account Status : %s\n\n", search_normal_user.status);
		}
	}
	if(accoun_ttype == 2){ //joint user account option
		joint_user search_jacc_user;
		int jacc_userID;
		write(1, "User ID: ", sizeof("User ID: "));
		scanf("%d", &jacc_userID);
		write(sock_desc, &jacc_userID, sizeof(int));
		len = read(sock_desc, &search_jacc_user, sizeof(joint_user));
		if(len == 0){
			write(1, "Enter valid User ID\n\n", sizeof("Enter valid User ID\n\n"));
		}
		else{
			printf("User ID: %d\n", search_jacc_user.userID);
			printf("Primary holder name: %s\n", search_jacc_user.name1);
			printf("Secondary holder name: %s\n", search_jacc_user.name2);
			printf("Account number: %d\n", search_jacc_user.account_num);
			printf("Account balance: Rs.%0.2f\n", search_jacc_user.acc_balance);
			printf("Account status: %s\n\n", search_jacc_user.status);
		}
	}
	show_menu(sock_desc);
	return;
}

int main(){
	struct sockaddr_in server;
	int sock_desc, msgLength;
	char buff[50];
	char result;
	//connection establishment
	sock_desc = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5555);
	connect(sock_desc,(struct sockaddr *)&server,sizeof(server));
	selectChoice(sock_desc);
	show_menu(sock_desc);	
	close(sock_desc);
	return 0;
}