# unix-pipe-ping-pong

Ping-pong. Two processes will play the ping-pong game.

The first process will generate a random number between 5000 and 15000 that will be send to the other process.

This process will subtract a random value (between 50 and 1000) and will send the number back,

The chat between the processes will be implemented using pipe channels.

The game ends when the value is below zero.

Each process will print the received value. 
