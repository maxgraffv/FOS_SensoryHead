#include "CameraStereo.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void take_and_send(CameraStereo*);
void send(std::vector<uint8_t>&);

int sockfd;
struct sockaddr_in server_addr;
uint8_t *buffer_ptr;
int buffer_size{0};



/*
    MAIN
*/
int main(int argv, char** argc)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);  // ten sam port co serwer

    // TU ustawiamy adres serwera = 192.168.1.13
    if (inet_pton(AF_INET, "192.168.2.80", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Klient (192.168.1.11): połączono z 192.168.1.13:5000\n");

    CameraStereo *stereoCam = new CameraStereo();
    stereoCam->init();
    stereoCam->start();

    for(int i = 0; i < 1; i++)
    {
        take_and_send(stereoCam);
    }
    
    stereoCam->stop();
    free(buffer_ptr);
    return 0;
}





/*
    TAKING SENDING
*/
void take_and_send(CameraStereo *cam)
{
    void *left = cam->getCamLeft()->frameMemory;
    int leftSize = cam->getCamLeft()->getFrameSize();

    void *right = cam->getCamRight()->frameMemory;
    int rightSize = cam->getCamLeft()->getFrameSize();

    std::cout << "Frame Size: " << leftSize << std::endl;
    buffer_size = leftSize;
    buffer_ptr = (uint8_t*)malloc(sizeof(uint8_t)*leftSize);

    // send(left, leftSize);

}

void send(void* mem, int len)
{
    send(sockfd, mem, len, 0);

    ssize_t n = recv(sockfd, buffer_ptr, (sizeof(uint8_t)*buffer_size) - 1, 0);
    if (n >= 0) {
        buffer_ptr[n] = '\0';
        printf("Klient: odpowiedź serwera: %s\n", *buffer_ptr);
    }
}