# PONG - Mini Project

`` 
The following project is a colaberation with Alex Lindberg & Lucas Larsson
``

This project was done as a part of the IS1200 course at KTH. The project fulfills the **advanced**
project requirements.

###### Compiling

The project uses the MCB32 toolchain for compilation.
If you can't compile the game, please make sure that the makefile is set up correctly for your machine.
This project was originally compiled on Windows and uses serial port *ttyS3*. If an error occurs, check
which serial port your board is connected to. 
Another error could stem from not having the *math.h* header available when using the makefile provided
in the labs. Use our makefile or link the header yourself.

## Features

Below is a list of currently supported featuers.
    
- Display:
    - Drawing sigle pixels
    - Drawing lines
    - Drawing empty boxes
    - Drawing filled boxes
    - Drawing text

- Game State:
    - Menu
    - Two player mode
    - A.I. mode
    - Highscore List

- Game:
    - PVP
    - PVM
    - Score keeping
    - Pausing

- I/O:
    - Button support
    - Switch support
    - Two player support


## Future Work

Below is a list of further improvements that can be made to the application.

- Refactoring
    - Main loop
    - score.c
    - etc.

- Game:
    - Better score logging
        - Scores for Player vs. Player matches
        - Persistent highscore list using the I2C protocol.

    - A.I.
        - Refactoring and bugfixing
            The A.I. is by no means perfect (or good for that matter).
            It can be trapped in the corner fairly easily on level 3, which
            allows the player to farm points for as long as they want.
        - A.I. vs A.I. mode
        - A.I. Level 4