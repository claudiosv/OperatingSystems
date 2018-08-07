
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2017
// server.h header file
// Homework 1
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and
// then binds it to the specified port_number
// for incoming client connections
//
//
// Arguments:   port_number = port number the server
//              socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port(unsigned int port_number)
{

    // -------------------------------------
    // NOTHING TODO HERE :)
    // -------------------------------------
    // Please do not modify

    int socket_fd;
    int set_option = 1;

    struct sockaddr_in server_address;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof(set_option));

    if (socket_fd < 0)
        return FAIL;

    bzero((char*)&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == 0) {

        return socket_fd;
    }
    else {

        return FAIL;
    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
//
//
// Arguments:   server file descriptor
//
// Return:      Termination status of client
//              ( 0 = No Errors, -1 = Error )
//
int accept_client(int server_socket_fd)
{

    int exit_status = OK;

    int client_socket_fd = -1;

    socklen_t client_length;

    struct sockaddr_in client_address;

    char request[512];

    client_length = sizeof(client_address);

    client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_address, &client_length);


    if (client_socket_fd >= 0) {
        int pid = fork();
        if (pid == 0) {
            //i'm the child :D

            bzero(request, 512);

            read(client_socket_fd, request, 511);

            if (DEBUG)
                printf("Here is the http message:\n%s\n\n", request);

            char entity_body[4096];

            if (strncmp(request, "GET ", 4) == 0) {
                char* form_name = "/SimplePost.html";

                unsigned int form_name_length = strlen(form_name);
                char* url_offset = request + 4;

                if (strncmp(url_offset, form_name, form_name_length) == 0) {
                    //This code is from the class c example 1
                    FILE* fhnd;

                    fhnd = fopen("SimplePost.html", "r");

                    char line[50];

                    if (fhnd != NULL) {

                        while (fgets(line, sizeof(line), fhnd) != NULL) {

                            strcat(entity_body, line);
                        }
                    }

                    fclose(fhnd);
                }
                else {
                    char entities[1024];

                    strcpy(entity_body, "<html><body><h1>GET Operation</h1><table cellpadding=5 cellspacing=5 border=1>");
                    
                    //parse get variables
                    char* entities_ptr = find_entities(request);
                    strcpy(entities, entities_ptr);

                    char table_buffer[1024];
                    char* table_content = parse_print_entities(entities, table_buffer);

                    strcat(entity_body, table_content);

                    strcat(entity_body, "<table></body></html>");
                }
            }
            else if (strncmp(request, "POST ", 5) == 0) {
                char entities[1024];
                char* search_ptr;
                char* found_ptr;

                strcpy(entity_body, "<html><body><h1>POST Operation</h1><table cellpadding=5 cellspacing=5 border=1>");

                size_t req_len = strlen(request);

                search_ptr = request;

                unsigned long int i = 0;

                for (i = 0; i < req_len; i++) {

                    if (strncmp(search_ptr, "\r\n\r\n ", 4) == 0) {
                        search_ptr = search_ptr + 4;
                        found_ptr = search_ptr;
                    }
                    else {
                        search_ptr++;
                    }
                }

                


                char* entities_ptr = find_entities(found_ptr);

                strcpy(entities, entities_ptr);
                char table_buffer[1024];
                char* table_content = parse_print_entities(entities, table_buffer);

                strcat(entity_body, table_content);

                strcat(entity_body, "<table></body></html>");
            }
            else {
                //error, http function not supported!
                strcpy(entity_body, "Only GET and POST operations are supported on this webserver.");
                char response[512];
                sprintf(response, "HTTP/1.1 501 Not Implemented\r\nContent-Length: %d\r\n\r\n%s", (int)strlen(entity_body), entity_body);

                if (DEBUG)
                    printf("%s\n", response);

                write(client_socket_fd, response, strlen(response));

                close(client_socket_fd);
                return exit_status;
            }

            char response[512];
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen(entity_body), entity_body);

            if (DEBUG)
                printf("%s\n", response);

            write(client_socket_fd, response, strlen(response));

            close(client_socket_fd);
            exit(0);
        }
        else if (pid > 0) {
            //we're the parent :)
            
        }
        else {
            //fork fail
        }
    }
    else {

        exit_status = FAIL;
    }

    if (DEBUG)
        printf("Exit status = %d\n", exit_status);

    return exit_status;

} // end accept_client function

char* find_entities(char* request)
{
    char* search_ptr = request;
                    char* result_ptr = request;


                    size_t str_len = strlen(request);
                    unsigned long int i = 0;

                    for (i = 0; i < str_len; i++) {

                        if ((*search_ptr) == '?') {
                            result_ptr = ++search_ptr;
                        }
                        else if ((*search_ptr) == ' ') {
                            (*search_ptr) = '\0';
                        }
                        else {
                            search_ptr++;
                        }
                    }
                    return result_ptr;
}

char* parse_print_entities(char* entities, char* buff)
{
    size_t entity_length = strlen(entities);
    unsigned int j = 0;
                    char* entity_start = entities;
                    char* entity_end = entities;
                    char entity_buffer[1024];
                    
                    for (j = 0; j <= entity_length; j++) {
                        if ((*entity_end) == '=') {
                            (*entity_end) = '\0';
                            sprintf(entity_buffer, "<tr><td><b>%s</b></td>", entity_start);
                            entity_start = ++entity_end;
                            strcat(buff, entity_buffer);
                        }
                        else if ((*entity_end) == '&') {
                            (*entity_end) = '\0';
                            sprintf(entity_buffer, "<td>%s</td></tr>\n", entity_start);
                            entity_start = ++entity_end;
                            strcat(buff, entity_buffer);
                        }
                        else if ((*entity_end) == ' ' || (*entity_end) == '\0') {
                            sprintf(entity_buffer, "<td>%s</td></tr>\n", entity_start);
                            strcat(buff, entity_buffer);
                        }
                        else {
                            entity_end++;
                        }
                    }

                    return buff;
}