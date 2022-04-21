// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

int dropp_privilages(){
    
    struct passwd* pwd;
    
    pid_t childProcessID,pid;

    childProcessID = fork();

    if(childProcessID ==0){
        //success
        printf("\n fork sucessful \n");
        //getting user id with lessed privilages
        pwd = getpwnam("nobody");
        pid = setuid(pwd->pw_uid);
        if(pid==0){
            return 1;
        }

        return 0;

    }


}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";


     //compare if the running one is child process
     
     if(strcmp(argv[0],"socket")==0)
     {
     	
     	printf("\n Running in child process");
     	//taking new socket
     	int newSocket = atoi(argv[1]);
     	
     	valread = read(newSocket, buffer, 1024);
        printf("%s\n", buffer);
        send(newSocket, hello, strlen(hello), 0);
        
        printf("Hello message sent\n");
        exit(0);
     	
     
     }


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
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
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //drop privilages 
    if(dropp_privilages()){
         //message processing
            valread = read(new_socket, buffer, 1024);
            printf("Read %d bytes: %s\n", valread, buffer);
            send(new_socket, hello, strlen(hello), 0);
            printf("Hello message sent\n");
            
            //code for second assignemnt 
            
            int CopyParent = dup(new_socket);
            
            if(CopyParent==-1)
           		perror("socket dup function failed");
           
           
            //convert socket to string
            
            char stringSocket[10];
            
            sprintf(stringSocket,"%d",CopyParent);
            
            char *args[] = {"socket",stringSocket,NULL};
            
            execvp(argv[0],args);
            
            
    }

    wait();
    return 0;
}
