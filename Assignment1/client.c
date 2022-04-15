#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, message; 
    struct sockaddr_in serverAdd; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serverAdd, '0', sizeof(serverAdd)); 
   
    serverAdd.sin_family = AF_INET; 
    serverAdd.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serverAdd.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serverAdd, sizeof(serverAdd)) < 0)
    { 
        printf("\nConnection Failed\n"); 
        return -1; 
    }

    send(sock, hello, strlen(hello), 0 ); 
    printf("Hello message sent\n"); 
    message = read(sock, buffer, 1024); 
    printf("Read %d bytes: %s\n", message, buffer);

    return 0; 
} 