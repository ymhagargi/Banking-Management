#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct Normal_User{
	int userID;
	char name[30];
	char password[10];
	int account_number;
	float acc_balance;
	char status[20];
}Normal_User;

typedef struct jointUser{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_number;
	float acc_balance;
	char status[20];
}jointUser;

typedef struct admin{
	int userID;
	char username[30];
	char password[10];
}admin;

int getNewUserID1();
int getNewUserID2();
int getNewUserID3();

int getNewUserID1(){
	int fd = open("Normal_user_file", O_RDONLY, 0744);
	Normal_User record;
	lseek(fd,-sizeof(Normal_User), SEEK_END);
	read(fd,&record,sizeof(Normal_User));
	close(fd);
	return record.userID+1;
}

int getNewUserID2(){
	int fd = open("Joint_user_file", O_RDONLY, 0744);
	jointUser record;
	lseek(fd, -sizeof(jointUser), SEEK_END);
	read(fd, &record, sizeof(jointUser));
	close(fd);
	return record.userID+1;
}

int getNewUserID3(){
	int fd = open("Admin_file", O_RDONLY, 0744);
	admin record;
	lseek(fd, -sizeof(admin), SEEK_END);
	read(fd, &record, sizeof(admin));
	close(fd);
	return record.userID+1;
}

int main(){
	int fd = open("Admin_file", O_RDWR | O_CREAT, 0744);
	int ch = 0;
	admin newAdmin;
	printf("Enter admin name: ");
	scanf(" %s", newAdmin.username);
	printf("Enter password(Limit: 10 characters): ");
	scanf(" %s", newAdmin.password);
	newAdmin.userID = 1000;
	printf("Your userID is : %d\n", newAdmin.userID);
	write(fd, &newAdmin, sizeof(newAdmin));
	printf("Do you want to continue(Enter either 0 or 1)? ");
	scanf("%d", &ch);
	while(ch){
		printf("Enter admin name: ");
		scanf(" %[^\n]", newAdmin.username);
		printf("Enter password(Limit: 10 characters): ");
		scanf(" %[^\n]", newAdmin.password);
		newAdmin.userID = getNewUserID3();
		printf("Your userID is : %d\n", newAdmin.userID);
		write(fd, &newAdmin, sizeof(admin));
		printf("Do you want to continue(Enter either 0 or 1)? ");
		scanf("%d", &ch);
	}
	close(fd);

	fd = open("Normal_user_file", O_RDWR | O_CREAT, 0744);
	ch = 1;
	Normal_User new_normal_user_var;
	printf("Enter user name: ");
	scanf(" %[^\n]",new_normal_user_var.name);
	printf("Enter password(Limit: 10 characters): ");
	scanf(" %[^\n]", new_normal_user_var.password);
	new_normal_user_var.userID = 1000;
	new_normal_user_var.acc_balance = 1000;
	new_normal_user_var.account_number = (new_normal_user_var.userID-1000)+100000;
	printf("Your userID is : %d\n", new_normal_user_var.userID);
	strcpy(new_normal_user_var.status, "ACTIVE");
	write(fd,&new_normal_user_var, sizeof(Normal_User));
	printf("Do you want to continue(Enter either 0 or 1)? ");
	scanf("%d", &ch);
	while(ch){
		printf("Enter user name: ");
		scanf(" %[^\n]", new_normal_user_var.name);
		printf("Enter password(Limit: 10 characters): ");
		scanf(" %[^\n]", new_normal_user_var.password);
		new_normal_user_var.userID = getNewUserID1();
		new_normal_user_var.acc_balance = 1000;
		new_normal_user_var.account_number = (new_normal_user_var.userID-1000)+100000;
		printf("Your userID is : %d\n", new_normal_user_var.userID);
		strcpy(new_normal_user_var.status, "ACTIVE");
		write(fd, &new_normal_user_var, sizeof(Normal_User));
		printf("Do you want to continue(Enter either 0 or 1)? ");
		scanf("%d", &ch);
	}
	close(fd);

	fd = open("Joint_user_file", O_RDWR | O_CREAT, 0744);
	ch = 1;
	jointUser new_joint_user;
	printf("Enter main name of joint account: ");
	scanf(" %[^\n]", new_joint_user.name1);
	printf("Enter second name of joint account: ");
	scanf(" %[^\n]", new_joint_user.name2);
	printf("Enter password(Limit: 10 characters): ");
	scanf(" %[^\n]", new_joint_user.password);
	new_joint_user.userID = 1000;
	new_joint_user.acc_balance = 1000;
	new_joint_user.account_number = (new_joint_user.userID-1000)+100000;
	printf("Your userID is : %d\n", new_joint_user.userID);
	strcpy(new_joint_user.status, "ACTIVE");
	write(fd, &new_joint_user, sizeof(jointUser));
	printf("Do you want to continue(Either 0 or 1)? ");
	scanf("%d", &ch);
	while(ch){
		printf("Enter the main name of the joint user: ");
		scanf(" %[^\n]", new_joint_user.name1);
		printf("Enter the second name of the joint user: ");
		scanf(" %[^\n]", new_joint_user.name2);
		printf("Enter password(Limit: 10 characters): ");
		scanf(" %[^\n]", new_joint_user.password);
		new_joint_user.userID = getNewUserID2();
		new_joint_user.acc_balance = 1000;
		new_joint_user.account_number = (new_joint_user.userID-1000)+100000;
		printf("Your userID is : %d\n", new_joint_user.userID);
		strcpy(new_joint_user.status, "ACTIVE");
		write(fd, &new_joint_user, sizeof(jointUser));
		printf("Do you want to continue(Either 0 or 1)? ");
		scanf("%d", &ch);
	}
	close(fd);
	return 0;
}