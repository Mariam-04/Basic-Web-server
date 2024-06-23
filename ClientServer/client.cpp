// client.cpp

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main() {
  int request_fd = open("request_pipe", O_WRONLY);
  int response_fd = open("response_pipe", O_RDONLY);

  // Ask the user for the file they want to request
  cout << "Enter the file you want to request: ";
  string filename;
  getline(cin, filename);

  // Prepare the request
  string request = filename;
  write(request_fd, request.c_str(), request.size());

  // Receive and display response
  char buffer[1024];
  ssize_t bytes_read = read(response_fd, buffer, sizeof(buffer));
  buffer[bytes_read] = '\0'; // Null terminate the string

  // Display response
  cout << "Response from server:\n" << buffer << endl;

  close(request_fd);
  close(response_fd);

  return 0;
}

