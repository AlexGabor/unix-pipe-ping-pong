#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int p2c[2], c2p[2];

int hit(char direction) {
  int *in, *out; // pointers to pipes
  int ball, sub; 
  unsigned char byte; // buffer for reading
  int numberOfBytes = 0; 
  
  if (direction == 'p') { // if hit was called by parent
    in = &c2p[0]; // will read from c2p[0]
    out = &p2c[1]; // will write to p2c[1]
  } else { // if hit was called by child 
    in = &p2c[0]; // will read from p2c[0]
    out = &c2p[1]; // will write to c2p[1]
  }
  ball = 0;
  while (numberOfBytes < sizeof(ball) && read(*in, &byte, 1) > 0) { // reads one byte at a time
    ball |= byte << (numberOfBytes*8); // puts the current byte in it's corresponding place
    numberOfBytes++;
  }
  
  sub = rand()%51+50; // generates a number between [50, 100]
  ball -= sub; 
  write(*out, &ball, sizeof(ball)); // writes the new value in pipe for the other process
  if (ball < 0) { // if the value became negative closes the pipes and exits
    close(*in);
    close(*out);
    if (direction == 'p') // the parent will wait for the child
      wait(NULL); 
    exit(0);
  }
  return ball;
}

int main () {

  int ball; // curent number
  srand(time(NULL)); // seeds the number generator

  if (pipe(p2c) == -1) { // creates parent to child pipe and checks for error
    perror("pipe");
    exit(1); // terminates with status 1
  }
  if (pipe(c2p) == -1) { // creates child to parent pipe and checks for error
    perror("pipe");
    exit(1);
  }

  int pid = fork(); // creates a child process
  if (pid < 0) { // checks for error
    perror("fork");
    exit(1);
  }
  ball = rand()%10001+5000; // generates a random number between [5000, 15000]
  if (pid > 0) { // parent process  
    while (1) { // keeps hitting the ball if the value is positive 
      ball = hit('p'); 
      printf("Parrent: %d\n", ball);
    }
  } else {
    write(c2p[1], &ball, sizeof(ball)); // writes the initial value for the parent process to begin with
    while (1) { // keeps hitting the ball if the value is positive 
      ball = hit('c');
      printf("Child: %d\n", ball);
    }
  }
  

  return 0;
}
