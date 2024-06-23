////server

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Struct to hold file descriptors
struct ThreadArgs {
  int request_fd;
  int response_fd;
};

// Function to handle each client request
void *handleRequest(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;

  int request_fd = args->request_fd;
  int response_fd = args->response_fd;

  char buffer[1024];
  ssize_t bytes_read;

  while (true) {
    // Read the filename sent by the client
    bytes_read = read(request_fd, buffer, sizeof(buffer));
    if (bytes_read == 0) {
      break; // No more requests, exit loop
    }
    buffer[bytes_read] = '\0'; // Null terminate the string

    // Open the requested file
    int file_fd = open(buffer, O_RDONLY);
    if (file_fd == -1) {
      const char *response =
          "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\nFile not found";
      write(response_fd, response, strlen(response));
      continue; // Continue to next request
    }

    // Read the content of the file
    ssize_t file_bytes_read;
    while ((file_bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
      // Write the content to the response pipe
      write(response_fd, buffer, file_bytes_read);
    }

    // Close the file
    close(file_fd);
  }

  // Close the pipes
  close(request_fd);
  close(response_fd);

  return nullptr;
}

int main() {
  // Open request and response pipes for each client request
  int request_fd = open("request_pipe", O_RDONLY);
  int response_fd = open("response_pipe", O_WRONLY);

  // Create a struct to hold file descriptors
  ThreadArgs args;
  args.request_fd = request_fd;
  args.response_fd = response_fd;

  // Create a thread to handle the request
  pthread_t t1;
  pthread_create(&t1, NULL, handleRequest, &args);

  // Wait for the thread to finish
  pthread_join(t1, NULL);

  return 0;
}
