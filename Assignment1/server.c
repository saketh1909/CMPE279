#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

// int dropp_privilages(){
    
//     struct passwd* pwd;
    
//     pid_t childProcessID,pid;

//     childProcessID = fork();

//     if(childProcessID ==0){
//         //success
//         printf("\n fork sucessful \n");
//         //getting user id with lessed privilages
//         pwd = getpwnam("nobody");
//         pid = setuid(pwd->pw_uid);
//         if(pid==0){
//             return 1;
//         }

//         return 0;

//     }


// }

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, message;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    struct passwd* passPtr;


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
	sizeof(address)) < 0)
    {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    pid_t childPId;
    childPId = fork();
    int status = 0;

    if(childPId == -1){
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }

    if(childPId == 0){
        passPtr = getpwnam("nobody");
        printf("ID %d", setuid(passPtr->pw_uid));
        if(setuid(passPtr->pw_uid) != 0){
        // pid=setuid(passPtr->pw_uid);
        // printf("%d",pid);
        // if(pid!=0){
            perror("Failed Setting Child process Id");
            exit(EXIT_FAILURE);
        }else{
            message = read(new_socket, buffer, 1024);
            printf("Read %d bytes: %s\n", message , buffer);
            send(new_socket, hello, strlen(hello), 0);
            printf("Hello Message sent\n");
        }
    }else{
        while((childPId = wait(&status))>0);
    }
    // //drop privilages 
    // if(dropp_privilages()){
    //      //message processing
    //         message = read(new_socket, buffer, 1024);
    //         printf("Read %d bytes: %s\n", message , buffer);
    //         send(new_socket, hello, strlen(hello), 0);
    //         printf("Hello Message Sent\n");
            

    // }

    // wait();
    return 0;
}