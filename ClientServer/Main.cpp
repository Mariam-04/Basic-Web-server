// main.cpp

#include "header.h"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main() {
  mkfifo("request_pipe", 0666);
  mkfifo("response_pipe", 0666);

  int x = 1;
  while (x != -1) {
    system("./server &");

    system("./client");
    cout << "enter -1 to exit else 1" << endl;
    cin >> x;
  }
  return 0;
}
