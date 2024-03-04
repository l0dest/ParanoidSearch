# Paranoid Search - C Console Game

 You're playing as a **Spy** who infiltrates an enemy base and needs to *steal* objects from it. You should take care because you have other 9 people arround you who can catch you if you don't play wisely.

All input an output information are given in the command line and you'll use certains commands to interact with you environment.

## Downloading and Executing
In the folder downloads you'll find folders for **Windows** and **Linux**, inside of them are two zip files, one for the **ENGLISH** version of the game and other for the **SPANISH**. Unzipping them gives you the codes, a makefile, a csv file used for the different names and a executable file.

Once you are in the desired folder you can either run the executable or get access to the code if you want to compile it yourself. The code is free to see and download for everyone who want it. This game is mainly a test for me.

## Game Mechanics
#### Objective and Game Set
Your mission is to steal 30 objects from the base, you have a deadline of 7 days and you'll spend these in a base with 12 rooms, around 9 people. Each day objects can appear in all rooms and these 9 people go to random rooms.

- Each day you are granted with 8 moves, spending all of them means the end of a day.
- All people have a "Mood Level", that determines it's sanity. All start with 100 wich means they are all calm.
- Each day, you receive a Knife and a Gun with one or two bullets.

#### Command Line System
You can do things interacting with a command_line-like system made in the game, the commands used here are:

- help : to print these list.
- move : to move trough the rooms (Cost 1 movement).
- steal : if there are objects, steal one of them (Cost 1 movement).
- roominfo : prints how many people are in the room and the probability of getting caught if you choose to steal there.
- kill : to eliminate one of your colleagues. (Using the Gun gives you 100% chances of killing while the knife has only a 20% chance of succces).
- people : prints the list of people and their mood.
- rooms : prints the list of rooms in the house.
- info : prints the current day and objects left to steal.

### Mood Level Decrease
The mood of you colleagues affects if they discover you when you steal in front of them. Mood levels can decrease if:
- You steal an object in the same room as other colleague.
- Another collegue don't come to work (You killed one of them).

## Technical Things
The program is written in the C programming lenguage. Has two files with code one ("main_lx.c") for the game itself and the other ("commands.c") for the different functions, it also contains a header file ("commands.h") with the definitions of the functions, libraries, structs, etc.
A csv file is within the files and it contains the possibly names of the people, you should execute the program in the same folder as this csv file or it won't work, because its necessary to it.

## Feedback
If you want to submit any kind of feedback I would be very pleased to read it.

