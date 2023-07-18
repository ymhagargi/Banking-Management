#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

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

normal_user getNormalUser(int ID){
	int i = ID - 1000;
	normal_user current_user;
	int fd = open("Normal_User_file", O_RDONLY, 0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(normal_user);
	lock.l_len = sizeof(normal_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
	read(fd, &current_user, sizeof(normal_user));
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return current_user;
}

joint_user getJointUser(int ID){
	int i = ID - 1000;
	joint_user current_user;
	int fd = open("Joint_User_file", O_RDONLY, 0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(joint_user);
	lock.l_len = sizeof(joint_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
	read(fd, &current_user, sizeof(joint_user));
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return current_user;
}

administrator getAdmin(int ID){
	int i = ID-1000;
	administrator current_user;
	int fd = open("Admin_file", O_RDONLY, 0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(administrator);
	lock.l_len = sizeof(administrator);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd,(i)*sizeof(administrator), SEEK_SET);
	read(fd, &current_user, sizeof(administrator));
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return current_user;
}
/**
 * Checks if normal user account exists by verifying password and account status 
 */
bool check_normal_user(normal_user current_user){
	int i = current_user.userID - 1000;
	int fd = open("Normal_User_file", O_RDONLY, 0744);
	bool result;
	normal_user temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(normal_user);
	lock.l_len = sizeof(normal_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
	read(fd, &temp, sizeof(normal_user));
	if(!strcmp(temp.password, current_user.password) && !strcmp(temp.status, "ACTIVE"))	
		result = true;
	else						
		result = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;
}
/**
 * Checks if joint user account exists by verifying password and account status 
 */
bool check_joint_user(joint_user current_user){
	int i = current_user.userID - 1000;
	int fd = open("Joint_User_file", O_RDONLY, 0744);
	bool result;
	joint_user temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(joint_user);
	lock.l_len = sizeof(joint_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
	read(fd, &temp, sizeof(joint_user));
	if(!strcmp(temp.password, current_user.password) && !strcmp(temp.status, "ACTIVE"))	
		result = true;
	else						
		result = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;
}
/**
 * Checks if administrator exists by verifying password
 */
bool check_admin(administrator current_user){
	int i = current_user.userID - 1000;
	int fd = open("Admin_file", O_RDONLY, 0744);
	bool result;
	administrator temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(administrator);
	lock.l_len = sizeof(administrator);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i)*sizeof(administrator), SEEK_SET);
	read(fd, &temp, sizeof(administrator));
	if(!strcmp(temp.password, current_user.password))	
		result = true;
	else						
		result = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;
}
/**
 * Money will be deposited only if account status is active
 */
bool deposit_money(int account_type, int ID, float amt){
	int i = ID - 1000;
	if(account_type == 1){ //type 1 refers to normal user account
		int fd = open("Normal_User_file", O_RDWR, 0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(normal_user);
		lock.l_len = sizeof(normal_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		normal_user current_user;
		lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
		read(fd, &current_user, sizeof(normal_user));
		if(!strcmp(current_user.status, "ACTIVE")){
			current_user.acc_balance += amt;
			lseek(fd, sizeof(normal_user)*(-1), SEEK_CUR);
			write(fd, &current_user, sizeof(normal_user));
			result = true;
		}
		else	
			result =false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);

		close(fd);
		return result;		
	}
	else if(account_type == 2){ //type 2 refers to joint user account
		int fd = open("Joint_User_file", O_RDWR, 0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(joint_user);
		lock.l_len = sizeof(joint_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		joint_user current_user;
		lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
		read(fd, &current_user, sizeof(joint_user));
		if(!strcmp(current_user.status, "ACTIVE")){
			current_user.acc_balance += amt;
			lseek(fd, sizeof(joint_user)*(-1), SEEK_CUR);
			write(fd, &current_user, sizeof(joint_user));
			result = true;
		}
		else	
			result = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;	
	}
	return false;
}
/**
 * Money can be withdrawn only if account is still active and current account balance exceeds the amount to be withdrawn
 */
bool withdraw_money(int account_type, int ID, float amt){
	int i = ID - 1000;
	if(account_type == 1){ //type 1 refers to normal user account
		int fd = open("Normal_User_file", O_RDWR, 0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(normal_user);
		lock.l_len = sizeof(normal_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		normal_user current_user;
		lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
		read(fd, &current_user, sizeof(normal_user));
		if(!strcmp(current_user.status, "ACTIVE") && current_user.acc_balance >= amt){
			current_user.acc_balance -= amt;
			lseek(fd, sizeof(normal_user)*(-1), SEEK_CUR);
			write(fd, &current_user, sizeof(normal_user));
			result = true;
		}
		else	
			result = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;	
	}
	else if(account_type == 2){ //type 2 refers to joint user account
		int fd = open("Joint_User_file", O_RDWR, 0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(joint_user);
		lock.l_len = sizeof(joint_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		joint_user current_user;
		lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
		read(fd, &current_user, sizeof(joint_user));
		if(!strcmp(current_user.status, "ACTIVE") && current_user.acc_balance >= amt){
			current_user.acc_balance -= amt;
			lseek(fd, sizeof(joint_user)*(-1), SEEK_CUR);
			write(fd, &current_user, sizeof(joint_user));
			result = true;
		}
		else	
			result = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;
	}
	return false;
}
/**
 * Account balance can be checked only if account is still active
 */
float get_account_balance(int account_type, int ID){
	int i = ID - 1000;
	float result;
	if(account_type == 1){ //type 1 refers to normal user account
		int i = ID - 1000;
		int fd = open("Normal_User_file", O_RDONLY, 0744);
		normal_user temp;
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(normal_user);
		lock.l_len = sizeof(normal_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd,F_SETLKW,&lock);
		lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
		read(fd, &temp, sizeof(normal_user));
		if(!strcmp(temp.status, "ACTIVE"))	
			result = temp.acc_balance;
		else					
			result = 0;

		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;
	}
	else if(account_type == 2){ //type 2 refers to joint user account
		int i = ID - 1000;
		int fd = open("Joint_User_file", O_RDONLY, 0744);
		joint_user temp;
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(joint_user);
		lock.l_len = sizeof(joint_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
		read(fd, &temp, sizeof(joint_user));
		if(!strcmp(temp.status, "ACTIVE"))	
			result = temp.acc_balance;
		else					
			result = 0;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;
	}
	return 0;
}
/**
 * Account password can be changed only if account is active 
 */
bool change_password(int account_type, int ID, char new_passwrd[10]){
	int i = ID - 1000;
	if(account_type == 1){ //type 1 refers to normal user account
		int fd = open("Normal_User_file", O_RDWR, 0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(normal_user);
		lock.l_len = sizeof(normal_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		normal_user current_user;
		lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
		read(fd, &current_user, sizeof(normal_user));
		if(!strcmp(current_user.status, "ACTIVE")){
			strcpy(current_user.password, new_passwrd);
			lseek(fd, sizeof(normal_user)*(-1), SEEK_CUR);
			write(fd, &current_user, sizeof(normal_user));
			result = true;
		}
		else	
			result = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;
	}
	else if(account_type == 2){ //type 2 refers to joint user account
		int fd = open("Joint_User_file", O_RDWR, 0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i)*sizeof(joint_user);
		lock.l_len = sizeof(joint_user);
		lock.l_pid = getpid();
		fl1 = fcntl(fd, F_SETLKW, &lock);
		joint_user current_user;
		lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
		read(fd, &current_user, sizeof(joint_user));
		if(!strcmp(current_user.status, "ACTIVE")){
			strcpy(current_user.password, new_passwrd);
			lseek(fd, sizeof(joint_user)*(-1), SEEK_CUR);
			write(fd, &current_user, sizeof(joint_user));
			result = true;
		}
		else	
			result = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return result;
	}
	return false;
}
/**
 * Administrator can add a normal user account 
 */
bool addNormalUser(normal_user record){
	int fd = open("Normal_User_file", O_RDWR, 0744);
	bool result;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_END;
	lock.l_start = (-1)*sizeof(normal_user);
	lock.l_len = sizeof(normal_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	normal_user endUser;
	lseek(fd, (-1)*sizeof(normal_user), SEEK_END);
	read(fd, &endUser, sizeof(normal_user));
	record.userID = endUser.userID + 1;
	record.account_num = endUser.account_num + 1;
	strcpy(record.status, "ACTIVE");
	int j = write(fd, &record, sizeof(normal_user));
	if(j!=0)	
		result = true;
	else	
		result = false;
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;	
}
/**
 * Administrator can add joint user account
 */
bool addJointUser(joint_user record){
	int fd = open("Joint_User_file", O_RDWR, 0744);
	bool result;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_END;
	lock.l_start = (-1)*sizeof(joint_user);
	lock.l_len = sizeof(joint_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	joint_user endUser;
	lseek(fd, (-1)*sizeof(joint_user), SEEK_END);
	read(fd, &endUser, sizeof(joint_user));
	record.userID = endUser.userID + 1;
	record.account_num = endUser.account_num + 1;
	strcpy(record.status, "ACTIVE");
	int j = write(fd, &record, sizeof(joint_user));
	if(j!=0)	
		result = true;
	else	
		result = false;
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;	
}
/**
 * Administrator can delete a normal user account
 */
bool deleteNormalUser(int ID){
	int i = ID - 1000;
	int fd = open("Normal_User_file", O_RDWR, 0744);
	bool result;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(normal_user);
	lock.l_len = sizeof(normal_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	normal_user current_user;
	lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
	read(fd, &current_user, sizeof(normal_user));
	if(!strcmp(current_user.status, "ACTIVE")){	
		strcpy(current_user.status, "CLOSED");
		current_user.acc_balance = 0;
		lseek(fd, (-1)*sizeof(normal_user), SEEK_CUR); 
		int j = write(fd, &current_user, sizeof(normal_user));
		if(j!=0)	
			result = true;
		else		
			result = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;	
}
/**
 * Administrator can delete a joint user account
 */
bool delete_joint_user(int ID){
	int i = ID - 1000;
	int fd = open("Joint_User_file", O_RDWR, 0744);
	bool result;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(joint_user);
	lock.l_len = sizeof(joint_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	joint_user current_user;
	lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
	read(fd, &current_user, sizeof(joint_user));
	if(!strcmp(current_user.status, "ACTIVE")){	
		strcpy(current_user.status, "CLOSED");
		current_user.acc_balance = 0;
		lseek(fd, (-1)*sizeof(joint_user), SEEK_CUR); 
		int j = write(fd, &current_user, sizeof(joint_user));
		if(j!=0)	
			result = true;
		else		
			result = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;	
}
/**
 * Administrator a normal user account only if it's active and account number matches(modified one and current)
 */
bool modify_normal_user(normal_user modi_user){
	int i = modi_user.userID - 1000;
	int fd = open("Normal_User_file", O_RDWR, 0744);
	bool result = false;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(normal_user);
	lock.l_len = sizeof(normal_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	normal_user current_user;
	lseek(fd, (i)*sizeof(normal_user), SEEK_SET);
	read(fd, &current_user, sizeof(normal_user));
	if(!strcmp(current_user.status, "ACTIVE") && (modi_user.account_num == current_user.account_num)){	
		strcpy(modi_user.status, "ACTIVE");
		lseek(fd, (-1)*sizeof(normal_user), SEEK_CUR); 
		int j = write(fd, &modi_user, sizeof(normal_user));
		if(j!=0)	
			result = true;
		else		
			result = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;	
}
/**
 * Administrator a joint user account only if it's active and account number matches(modified one and current)
 */
bool modify_joint_user(joint_user modi_user){
	int i = modi_user.userID - 1000;
	int fd = open("Joint_User_file", O_RDWR, 0744);
	bool result = false;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i)*sizeof(joint_user);
	lock.l_len = sizeof(joint_user);
	lock.l_pid = getpid();
	fl1 = fcntl(fd, F_SETLKW, &lock);
	joint_user current_user;
	lseek(fd, (i)*sizeof(joint_user), SEEK_SET);
	read(fd, &current_user, sizeof(joint_user));
	if(!strcmp(current_user.status, "ACTIVE")  && (modi_user.account_num == current_user.account_num)){	
		strcpy(modi_user.status, "ACTIVE");
		lseek(fd, (-1)*sizeof(joint_user), SEEK_CUR); 
		int j = write(fd, &modi_user, sizeof(joint_user));
		if(j!=0)	
			result = true;
		else		
			result = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;	
}

void serverTask(int new_server_desc){
	int msgLength, select, type, option, account_type, current_user_ID;
	bool result;
	while(1){
		read(new_server_desc, &option, sizeof(option));
		printf("Option : %d\n", option);
		if(option == 1){ //option 1 refers to normal user
			normal_user currUser1;
			account_type = 1;
			msgLength = read(new_server_desc, &currUser1, sizeof(normal_user));
			printf("Username: %d\n", currUser1.userID);
			printf("Password: %s\n", currUser1.password);
			current_user_ID = currUser1.userID;
			result = check_normal_user(currUser1);
			write(new_server_desc, &result, sizeof(result));
		}
		else if(option == 2){ //option 2 refers to joint user account
			joint_user currUser2;
			account_type = 2;
			msgLength = read(new_server_desc, &currUser2, sizeof(joint_user));
			current_user_ID = currUser2.userID;
			printf("Username: %d\n", currUser2.userID);
			printf("Password: %s\n", currUser2.password);
			result = check_joint_user(currUser2);
			write(new_server_desc, &result, sizeof(result));
		}
		else if(option == 3){ //option 3 refers to administrator
			administrator currUser3;
			account_type = 3;
			msgLength = read(new_server_desc, &currUser3, sizeof(administrator));
			current_user_ID = currUser3.userID;
			printf("Username: %d\n", currUser3.userID);
			printf("Password: %s\n", currUser3.password);
			result = check_admin(currUser3);
			write(new_server_desc, &result, sizeof(result));
		}
		else{
			result = false;
			write(new_server_desc, &result, sizeof(result));
		}
		if(result)	
			break;		
	}
	while(1){
		read(new_server_desc, &select, sizeof(int));
		if(option == 1 || option == 2){
			if(select == 1){ //menu to choose what to be done, deposit; withdraw; password change; account details
				float amt;
				read(new_server_desc, &amt, sizeof(float));
				result = deposit_money(account_type, current_user_ID, amt);
				write(new_server_desc, &result, sizeof(result));
			}
			else if(select == 2){
				float amt;
				read(new_server_desc, &amt, sizeof(float));
				result = withdraw_money(account_type, current_user_ID, amt);
				write(new_server_desc, &result, sizeof(result));
			}
			else if(select == 3){
				float amt;
				amt = get_account_balance(account_type, current_user_ID);
				write(new_server_desc, &amt, sizeof(float));
			}
			else if(select == 4){
				char pwd[10];
				read(new_server_desc, pwd, sizeof(pwd));
				result = change_password(account_type, current_user_ID, pwd);
				write(new_server_desc, &result, sizeof(result));
			}
			else if(select == 5){
				if(option == 1){
					normal_user user1 = getNormalUser(current_user_ID);
					write(new_server_desc, &user1, sizeof(normal_user));
				}
				else if(option == 2){
					joint_user user2 = getJointUser(current_user_ID);
					write(new_server_desc, &user2, sizeof(joint_user));
				}
			}
			else if(select == 6)	
				break;
		}
		else if(option == 3){
			read(new_server_desc, &type, sizeof(int));
			if(select == 1){
				if(type == 1){ //menu for administrator to choose from, adding, modifying, deleting users or searching for specific account details
					normal_user newUser1;
					read(new_server_desc, &newUser1, sizeof(normal_user));
					result = addNormalUser(newUser1);
					write(new_server_desc, &result, sizeof(result));
				}
				else if(type == 2){
					joint_user newUser2;
					read(new_server_desc, &newUser2, sizeof(joint_user));
					result = addJointUser(newUser2);
					write(new_server_desc, &result, sizeof(result));
				}
			}
			else if(select == 2){
				if(type == 1){
					int delUserID1;
					read(new_server_desc, &delUserID1, sizeof(int));
					result = deleteNormalUser(delUserID1);
					write(new_server_desc, &result, sizeof(result));
				}
				else if(type == 2){
					int delUserID2;
					read(new_server_desc, &delUserID2, sizeof(int));
					result = delete_joint_user(delUserID2);
					write(new_server_desc, &result, sizeof(result));
				}
			}
			else if(select == 3){
				if(type == 1){
					normal_user modUser1;
					read(new_server_desc, &modUser1, sizeof(normal_user));
					result = modify_normal_user(modUser1);
					write(new_server_desc, &result, sizeof(result));
				}
				else if(type == 2){
					joint_user modUser2;
					read(new_server_desc, &modUser2, sizeof(joint_user));
					result = modify_joint_user(modUser2);
					write(new_server_desc, &result, sizeof(result));
				}
			}
			else if(select == 4){
				if(type == 1){
					normal_user searchUser1;
					int userID1;
					read(new_server_desc, &userID1, sizeof(int));
					searchUser1 = getNormalUser(userID1);
					write(new_server_desc, &searchUser1, sizeof(normal_user));
				}
				else if(type == 2){
					joint_user searchUser2;
					int userID2;
					read(new_server_desc, &userID2, sizeof(int));
					searchUser2 = getJointUser(userID2);
					write(new_server_desc, &searchUser2, sizeof(joint_user));
				}
			}
			else if(select == 5)	
				break;
		}
	}
	close(new_server_desc);
	write(1, "Client session over\n", sizeof("Client session over\n"));
	return;
}

void *connection_handler(void *new_server_desc) {
	int nsfd = *(int*)new_server_desc;
	serverTask(nsfd);
}

int main(){
	struct sockaddr_in server,client;
	int server_desc, new_server_desc, clientLen;
	pthread_t threads;
	bool result;
	server_desc = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);
	bind(server_desc, (struct sockaddr *)&server, sizeof(server));
	listen(server_desc, 5);
	write(1, "Waiting for client...\n", sizeof("Waiting for client...\n"));
	while(1){
		clientLen = sizeof(client);
		new_server_desc = accept(server_desc, (struct sockaddr *)&client, &clientLen);
		write(1, "Connected to client!\n", sizeof("Connected to client!\n"));
		if(pthread_create(&threads, NULL, connection_handler, (void*) &new_server_desc)<0){
			perror("Error! Couldn't create thread");
			return 1;
		}	
	}
	pthread_exit(NULL);
	close(server_desc);
	return 0;
}