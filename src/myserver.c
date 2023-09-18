#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT_NUMBER 8080

int main () {
    // get server config
    int port_number;
    if (!getenv("PORT_NUMBER")) {
        port_number = DEFAULT_PORT_NUMBER;
    } else {
        port_number = atoi(getenv("PORT_NUMBER"));
    }

    // server socket file descriptor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { printf("Could not create socket\n"); exit(2); }

    // Populate server structure
    struct sockaddr_in server = {
        .sin_family      = AF_INET,
        .sin_port        = htons(port_number),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
    };

    // bind to address and start listener
    int err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0) { printf("Could not bind socket\n"); exit(2); }

    err = listen(server_fd, 128);
    if (err < 0) { printf("Could not listen on socket\n"); exit(2); }

    printf("Server is listening on %d\n", port_number);

    struct sockaddr_in client;
    char buf[BUFFER_SIZE]; // buffer for echo data
    // server receive and send infinitive loop
    while (1) {
        socklen_t client_len = sizeof(client);

        // accept will block until client connection
        int client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);
        if (client_fd < 0) printf("Could not establish new connection\n");

        // receive data from client
        ssize_t read_err = recv(client_fd, buf, BUFFER_SIZE, 0);
        if (read_err < 0) printf("Client read failed\n");

        // send data back
        ssize_t send_err = send(client_fd, buf, read_err, 0);
        if (send_err < 0) printf("Client write failed\n");

        close(client_fd);
    }
}