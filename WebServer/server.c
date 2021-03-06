/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>

#include "server.h"

#define BUFSIZE 8096
#define IMGSIZE 10000000

struct {
    char *ext;
    char *filetype;
} extensions [] = {
    {"gif", "image/gif" },
    {"jpg", "image/jpeg"},
    {"jpeg","image/jpeg"},
    {"png", "image/png" },
    {"zip", "image/zip" },
    {"gz",  "image/gz"  },
    {"tar", "image/tar" },
    {"htm", "text/html" },
    {"html","text/html" },
    {"php", "image/php" },
    {"cgi", "text/cgi"  },
    {"asp","text/asp"   },
    {"jsp", "image/jsp" },
    {"xml", "text/xml"  },
    {"js","text/js"     },
    {"css","test/css"   },

    {0,0} };

/* HTTP response and header for a successful request.  */

static char* ok_response =
  "HTTP/1.0 200 OK\n"
  "Content-type: text/html\n"
  "\n";

static char* jpg_response =
  "HTTP/1.0 200 OK\n"
  "Content-type: image/jpeg\n"
  "Connection: Keep-Alive\n"
  "\n";

static char* gif_response =
  "HTTP/1.0 200 OK\n"
  "Content-type: image/gif\n"
  "Connection: Keep-Alive\n"
  "\n";

/* HTTP response, header, and body indicating that the we didn't
   understand the request.  */

static char* bad_request_response =
  "HTTP/1.0 400 Bad Request\n"
  "Content-type: text/html\n"
  "\n"
  "<html>\n"
  " <body>\n"
  "  <h1>400 Bad Request</h1>\n"
  "  <p>This server did not understand your request.</p>\n"
  " </body>\n"
  "</html>\n";

/* HTTP response, header, and body template indicating that the
   requested document was not found.  */

static char* not_found_response_template =
  "HTTP/1.0 404 Not Found\n"
  "Content-type: text/html\n"
  "\n"
  "<html>\n"
  " <body>\n"
  "  <h1>404 Not Found</h1>\n"
  "  <p>The requested URL %s was not found on this server.</p>\n"
  " </body>\n"
  "</html>\n";

/* HTTP response, header, and body template indicating that the
   method was not understood.  */

static char* bad_method_response_template =
  "HTTP/1.0 501 Method Not Implemented\n"
  "Content-type: text/html\n"
  "\n"
  "<html>\n"
  " <body>\n"
  "  <h1>501 Method Not Implemented</h1>\n"
  "  <p>The method %s is not implemented by this server.</p>\n"
  " </body>\n"
  "</html>\n";

/* Handler for SIGCHLD, to clean up child processes that have
   terminated.  */

static void clean_up_child_process (int signal_number)
{
  int status;
  wait (&status);
}

/* Process an HTTP "GET" request for PAGE, and send the results to the
   file descriptor CONNECTION_FD.  */

static void handle_get (int connection_fd, const char* page)
{
  struct server_module* module = NULL;

  /* Check to see if HTML or module request*/
  if(strstr(page, ".html")!= NULL){
    long ret;
    int file_fd;
    char buffer[BUFSIZE+1];
    char  * filename = malloc(strlen(page)+2);

    strcat(filename, ".");
    strcat(filename, page);
    printf("%s\n", filename);

    // Try to open html file
    if (file_fd = open(filename, O_RDONLY) == -1)
    {
      // 404 not found
      char response[1024];
      snprintf (response, sizeof (response), not_found_response_template, page);
      write (connection_fd, response, strlen (response));
      return;
    }

    while((ret=read(file_fd, buffer, BUFSIZE))>0){

    }

    char * response = malloc(strlen(ok_response)+strlen(buffer)+1);
    strcat(response, ok_response);
    strcat(response, buffer);

    write(connection_fd, response, strlen(response));
  }
  /* jpg support */
  else if (strstr(page, ".jpeg") != NULL) {
    long ret;
    int file_fd;
    char  * filename = malloc(strlen(page)+2);

    strcat(filename, ".");
    strcat(filename, page);

    printf("%s\n", filename);

    FILE *file;
    long fileLen;

    file = fopen(filename, "rb+");
    if (!file)
    {
      char response[1024];
      snprintf (response, sizeof (response), not_found_response_template, page);
      write (connection_fd, response, strlen (response));
      return;
    }

    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    rewind(file);

    char * response = malloc(strlen(jpg_response) + fileLen + 1);
    int i = 0;

    for (i = 0; i < strlen(jpg_response); i++) {
      response[i] = jpg_response[i];
    }

    for(; i < fileLen; i++) {
      response[i] = fgetc(file);
    }

    // printf("%s\n", response);

    write(connection_fd, response, strlen(jpg_response) + fileLen + 1);
    fclose(file);
  }
  else if (strstr(page, ".jpg") != NULL) {
    long ret;
    int file_fd;
    char  * filename = malloc(strlen(page)+2);

    strcat(filename, ".");
    strcat(filename, page);

    printf("%s\n", filename);

    FILE *file;
    long fileLen;

    file = fopen(filename, "rb+");
    if (!file)
    {
      char response[1024];
      snprintf (response, sizeof (response), not_found_response_template, page);
      write (connection_fd, response, strlen (response));
      return;
    }

    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    rewind(file);

    char * response = malloc(strlen(jpg_response) + fileLen + 1);
    int i = 0;

    for (i = 0; i < strlen(jpg_response); i++) {
      response[i] = jpg_response[i];
    }

    for(; i < fileLen; i++) {
      response[i] = fgetc(file);
    }

    // printf("%s\n", response);

    write(connection_fd, response, strlen(jpg_response) + fileLen + 1);
    fclose(file);
  }
  else if (strstr(page, ".gif") != NULL) {
    long ret;
    int file_fd;
    char  * filename = malloc(strlen(page)+2);

    strcat(filename, ".");
    strcat(filename, page);

    printf("%s\n", filename);

    FILE *file;
    long fileLen;

    file = fopen(filename, "rb+");
    if (!file)
    {
      char response[1024];
      snprintf (response, sizeof (response), not_found_response_template, page);
      write (connection_fd, response, strlen (response));
      return;
    }

    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    rewind(file);

    char * response = malloc(strlen(jpg_response) + fileLen + 1);
    int i = 0;

    for (i = 0; i < strlen(gif_response); i++) {
      response[i] = gif_response[i];
    }

    for(; i < fileLen; i++) {
      response[i] = fgetc(file);
    }

    // printf("%s\n", response);

    write(connection_fd, response, strlen(jpg_response) + fileLen + 1);
    fclose(file);
  }
  else if (strstr(page, ".cgi") != NULL) {
    char* buffer[1000];

    char * cmd = malloc(strlen(page)+10);
    char * filename = malloc(strlen(page)+2);
    strcat(filename, ".");
    strcat(filename, page);
    strcat(cmd, "/bin/sh ");
    strcat(cmd, filename);

    if(open(filename, O_RDONLY) == -1)
    {
      // 404 not found
      char response[1024];
      snprintf (response, sizeof (response), not_found_response_template, page);
      write (connection_fd, response, strlen (response));
      return;
    }

    FILE * file = popen(cmd, "r");
    if (file == NULL) {
      char response[1024];
      snprintf (response, sizeof (response), not_found_response_template, page);
      write (connection_fd, response, strlen (response));
      return;
    }

    if (strstr(page, "histogram.cgi")!=NULL) {
      FILE * file2 = popen("/bin/sh ./searchscript", "r");
      if (file2 == NULL) {
        char response[1024];
        snprintf (response, sizeof (response), not_found_response_template, page);
        write (connection_fd, response, strlen (response));
        return;
      }

    }

    while (fgets(buffer, sizeof(buffer)-1, file) != NULL) {
      printf("%s", buffer);
    }

    char * response = malloc(strlen(ok_response)+1000000+1);
    strcat(response, ok_response);

    write(connection_fd, response, strlen(response));
    pclose(file);
  }
  else if (strstr(page,"arduino.run") != NULL) {
    int fd = -1;
    int baudrate = 9600;
    char eolchar = '\n';
    int timeout = 5000;
    char * buf = "ahm";
    int rc,n;
    char * serialport = "/dev/tty.wchusbserial1410";

    fd = serialport_init(serialport, baudrate);
    printf("opened port %s\n",serialport);
    serialport_flush(fd);
    serialport_write(fd, buf);
  }
  // else if () {
  //
  // }
  // 404
  else {
    char response[1024];
    snprintf (response, sizeof (response), not_found_response_template, page);
    write (connection_fd, response, strlen (response));
    return;
  }
}


/* Handle a client connection on the file descriptor CONNECTION_FD.  */

static void handle_connection (int connection_fd)
{
    char buffer[256];
    ssize_t bytes_read;

    /* Read some data from the client.  */
    bytes_read = read (connection_fd, buffer, sizeof (buffer) - 1);
    if (bytes_read > 0) {
        char method[sizeof (buffer)];
        char url[sizeof (buffer)];
        char protocol[sizeof (buffer)];

        /* Some data was read successfully.  NUL-terminate the buffer so
         we can use string operations on it.  */
        buffer[bytes_read] = '\0';
        /* The first line the client sends is the HTTP request, which is
         composed of a method, the requested page, and the protocol
         version.  */
        sscanf (buffer, "%s %s %s", method, url, protocol);
        /* The client may send various header information following the
         request.  For this HTTP implementation, we don't care about it.
         However, we need to read any data the client tries to send.  Keep
         on reading data until we get to the end of the header, which is
         delimited by a blank line.  HTTP specifies CR/LF as the line
         delimiter.  */
        while (strstr (buffer, "\r\n\r\n") == NULL)
            bytes_read = read (connection_fd, buffer, sizeof (buffer));
        /* Make sure the last read didn't fail.  If it did, there's a
         problem with the connection, so give up.  */
        if (bytes_read == -1) {
            close (connection_fd);
            return;
        }
        /* Check the protocol field.  We understand HTTP versions 1.0 and
         1.1.  */
        if (strcmp (protocol, "HTTP/1.0") && strcmp (protocol, "HTTP/1.1")) {
            /* We don't understand this protocol.  Report a bad response.  */
            write (connection_fd, bad_request_response,
                   sizeof (bad_request_response));
        }
        else if (strcmp (method, "GET")) {
            /* This server only implements the GET method.  The client
             specified some other method, so report the failure.  */
            char response[1024];

            snprintf (response, sizeof (response),
                      bad_method_response_template, method);
            write (connection_fd, response, strlen (response));
        }
        else
        /* A valid request.  Process it.  */
            handle_get (connection_fd, url);
    }
    else if (bytes_read == 0)
    /* The client closed the connection before sending any data.
     Nothing to do.  */
        ;
    else
    /* The call to read failed.  */
        system_error ("read");
}


void server_run (struct in_addr local_address, uint16_t port)
{
  struct sockaddr_in socket_address;
  int rval;
  struct sigaction sigchld_action;
  int server_socket;

  /* Install a handler for SIGCHLD that cleans up child processes that
     have terminated.  */
  memset (&sigchld_action, 0, sizeof (sigchld_action));
  sigchld_action.sa_handler = &clean_up_child_process;
  sigaction (SIGCHLD, &sigchld_action, NULL);

  /* Create a TCP socket.  */
  server_socket = socket (PF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
    system_error ("socket");
  /* Construct a socket address structure for the local address on
     which we want to listen for connections.  */
  memset (&socket_address, 0, sizeof (socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = port;
  socket_address.sin_addr = local_address;
  /* Bind the socket to that address.  */
  rval = bind (server_socket, &socket_address, sizeof (socket_address));
  if (rval != 0)
    system_error ("bind");
  /*  Instruct the socket to accept connections.  */
  rval = listen (server_socket, 10);
  if (rval != 0)
    system_error ("listen");

  if (verbose) {
    /* In verbose mode, display the local address and port number
       we're listening on.  */
    socklen_t address_length;

    /* Find the socket's local address.  */
    address_length = sizeof (socket_address);
    rval = getsockname (server_socket, &socket_address, &address_length);
    assert (rval == 0);
    /* Print a message.  The port number needs to be converted from
       network byte order (big endian) to host byte order.  */
    printf ("server listening on %s:%d\n",
	    inet_ntoa (socket_address.sin_addr),
	    (int) ntohs (socket_address.sin_port));
  }

  /* Loop forever, handling connections.  */
  while (1) {
    struct sockaddr_in remote_address;
    socklen_t address_length;
    int connection;
    pid_t child_pid;

    /* Accept a connection.  This call blocks until a connection is
       ready.  */
    address_length = sizeof (remote_address);
    connection = accept (server_socket, &remote_address, &address_length);
    if (connection == -1) {
      /* The call to accept failed.  */
      if (errno == EINTR)
	/* The call was interrupted by a signal.  Try again.  */
	continue;
      else
	/* Something else went wrong.  */
	system_error ("accept");
    }

    /* We have a connection.  Print a message if we're running in
       verbose mode.  */
    if (verbose) {
      socklen_t address_length;

      /* Get the remote address of the connection.  */
      address_length = sizeof (socket_address);
      rval = getpeername (connection, &socket_address, &address_length);
      assert (rval == 0);
      /* Print a message.  */
      printf ("connection accepted from %s\n",
	      inet_ntoa (socket_address.sin_addr));
    }

    /* Fork a child process to handle the connection.  */
    child_pid = fork ();
    if (child_pid == 0) {
      /* This is the child process.  It shouldn't use stdin or stdout,
	 so close them.  */
      close (STDIN_FILENO);
      // close (STDOUT_FILENO);
      /* Also this child process shouldn't do anything with the
	 listening socket.  */
      close (server_socket);
      /* Handle a request from the connection.  We have our own copy
	 of the connected socket descriptor.  */
      handle_connection (connection);
      /* All done; close the connection socket, and end the child
	 process.  */
      close (connection);
      exit (0);
    }
    else if (child_pid > 0) {
      /* This is the parent process.  The child process handles the
	 connection, so we don't need our copy of the connected socket
	 descriptor.  Close it.  Then continue with the loop and
	 accept another connection.  */
      close (connection);
    }
    else
      /* Call to fork failed.  */
      system_error ("fork");
  }
}
