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
    int serverFd, newSocket, message;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(serverFd, (struct sockaddr *)&address,
	sizeof(address)) < 0)
    {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    if ((newSocket = accept(serverFd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    pid_t childProcessID,pid;
    childProcessID = fork();
    struct passwd* pwd;
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
            message = read(newSocket, buffer, 1024);
            printf("Read %d bytes: %s\n", message , buffer);
            send(newSocket, hello, strlen(hello), 0);
            printf("Hello Message Sent\n");
    }

    wait();
    return 0;
}