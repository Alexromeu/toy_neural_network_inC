#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

#define BUF_SIZE 1024
#define PORT "2000"

char* get_time() {
    time_t timep;
    timep = time(NULL);
    char* tim_str = ctime(&timep);

    return tim_str;
} 

char* generate_response() {
    char* time_resp = get_time();
    int timelen = strlen(time_resp);
    char* new_respond = malloc(BUF_SIZE);
    snprintf(new_respond, BUF_SIZE, "HTTP/1.1 200 OK\r\n" "Content-Type: text/plain\r\n" "Content-Length: %d\r\n" "\r\n"
        "%s", timelen, time_resp);
    return new_respond;
}

int create_server(char* port) {
    int                      sfd, s, acc_sck;
    char                     buf[BUF_SIZE];
    socklen_t                peer_addrlen;
    struct addrinfo          hints;
    struct addrinfo          *result, *rp;
    struct sockaddr_storage  peer_addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    
    s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  

        close(sfd);
    }

    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        return -1;
    }
    freeaddrinfo(result);

    if (listen(sfd, 10) < 0) {
        perror("listen");
        return -1;
    }

    peer_addrlen = sizeof(peer_addr);
    printf("Waiting...\n");

    if ((acc_sck = accept(sfd, (struct sockaddr*)&peer_addr, &peer_addrlen)) < 0) {
        perror("accept");
        return -1;
    }

    return acc_sck;
}

void bounce_data(int sck_fd, char* buf, int buf_size) {
    ssize_t nread;
    while (nread = recv(sck_fd, buf, BUF_SIZE - 1, 0)) {
        buf[BUF_SIZE] = '\0';

        printf("%s\n", buf);
        if (strstr(buf, "\r\n\r\n")) {
            break;
        }
    }

    char* res = generate_response();

    printf("date-> %s", res);
    ssize_t ss;
    while (ss = send(sck_fd, res, BUF_SIZE - 1, 0)) {
        ;
    }
}

int main() {
    // while (1) {
    //     fd_set  readFDs;
    //     FD_ZERO(&readFDs);

    //     int fd;
    //     for (fd = minFD; fd < maxFD; fd++) {
    //         FD_SET(fd, &readFDs);
    //     }
    // }

    int connected_socket = create_server(PORT);
    char buf[BUF_SIZE];

    if (connected_socket < 0) {
        return -1;
    }

    bounce_data(connected_socket, buf, BUF_SIZE);
    return 0;
}