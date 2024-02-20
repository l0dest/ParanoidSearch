#include <stdio.h>
#include <stdlib.h>
#include <commands.h>

void help(void)
{
    printf(" Enterprise Macro Systems - CLMRS version 6.7\n\n");
    printf(" help : prints this list.\n\n");
    printf(" move [location] : if you have remaining movements, you will move to the written\n room (cost 1 movement).\n\n");
    printf(" steal : once in a room, if there are objects, steal them (can get caught by alert collegues)\n\n");
    printf(" kill [object_to_use] [name] : select to kill a collegue, using the gun gives you 100%% chances of succes,\n while "
    "using the knife may fail based on the victim paranoia\n");
}

void move(void)
{
    printf("You choose move\n");
}

void steal(void)
{
    printf("you choose steal\n");
}

void kill(void)
{
    printf("you choose kill\n");
}

void clear(void)
{
    system("clear");
    printf("> Type \"help\" to see the commands\n");
}

// Print People's Name and their Status
void people(int people_number, innocent crew_members[])
{
    for (int i = 0; i < people_number; i++)
    {
        if (crew_members[i].alive) printf("- %s - %i\n", crew_members[i].name, (int)crew_members[i].mood);
        else printf("- %s -> DEATH\n", crew_members[i].name);
    }
}

// Print The Facility Rooms and their Floors
void rooms(room rooms_list[])
{
    printf(" The first floor's rooms are:\n");
    for (int i = 0, floor1 = (ROOMS_NUM / 2); i < floor1; i++)
    {
        printf("- %s\n", rooms_list[i].name);
    }

    printf("\n The second floor's rooms are:\n");
    for (int i = (ROOMS_NUM / 2); i < ROOMS_NUM; i++)
    {
        printf("- %s\n", rooms_list[i].name);
    }
}