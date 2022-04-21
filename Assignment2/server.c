// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int serverFd, newSock, message;
    struct sockaddr_in address;
    struct passwd* pwd;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello From Server";
     //compare if the running one is child process
     
     if(strcmp(argv[0],"socket")==0){
     	printf("\nRunning in Child Process");
     	int newSocket = atoi(argv[1]);
     	message = read(newSocket, buffer, 1024);
        printf("%s\n", buffer);
        send(newSocket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        exit(0);
     }
    
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(serverFd, (struct sockaddr *)&address,sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((newSock = accept(serverFd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
        
    pid_t childProcessID,pid;
    childProcessID = fork();
    int status;
    
    if(childProcessID ==0){
        printf("\nFork Sucessful \n");
        pwd = getpwnam("nobody");
        pid = setuid(pwd->pw_uid);
        if(pid==0){
            status = 1;
        }
        status = 0;
    } 

    if(status){
            message = read(newSock, buffer, 1024);
            printf("Read %d bytes: %s\n", message, buffer);
            send(newSock, hello, strlen(hello), 0);
            printf("Hello message sent\n");
            int CopyParent = dup(newSock);

            if(CopyParent==-1){
                perror("socket dup function failed");
            }
            char stringSocket[10];
            sprintf(stringSocket,"%d",CopyParent);
            char *args[] = {"socket",stringSocket,NULL};
            execvp(argv[0],args);
    }

    wait();
    return 0;
}
