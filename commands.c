#include <stdio.h>
#include <stdlib.h>
#include <commands.h>
#include <time.h>

//Print Help
void help(void)
{
    printf("===================================================================================\n");
    printf(" Enterprise Macro Systems - CLMRS version 6.7\n\n");
    printf(" help : prints this list.\n\n");
    printf(" move [location] : if you have remaining movements, you will move to the written\n room. (cost 1 movement).\n\n");
    printf(" steal : once in a room, if there are objects, steal one of them. (cost 1 movement\n"
    " and can get caught by alert collegues).\n\n");
    printf(" kill [object_to_use] [name] : select to kill a collegue, using the gun gives you\n 100%% chances of success, "
    "using the knife may fail based on the victim paranoia.\n (cost 1 movement).\n\n");
    printf(" people : prints your colleagues stats.\n\n rooms : prints the rooms of the facility.\n\n");
    printf(" clear : clears console.\n");
    printf("===================================================================================\n");
}

// Movement Mechanics
void move(int *floor, int *movements, innocent crew_members[], room rooms_list[])
{
    printf("===================================================================================\n");
    printf(" Type the number where you want to go\n\n");
    int location = 100; //default
    switch (*floor)
    {
        case 0:

            for (int i = 0, floor1 = (ROOMS_NUM / 2); i < floor1; i++)
            {
                printf(" %i- %s\n",i + 1, rooms_list[i].name);
            }
            printf(" 0- Second Floor\n");
            printf("\n Type 101 to cancel\n");
            printf("===================================================================================\n");

            while (location < 0 || location > 6)
            {
                location = prompt_int("/move/first_floor> ");
                if (location == 101) break;
            }

            if (location == 0)
            {
                for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
                *floor = 1;
                *movements = *movements - 1;
                printf("You are now in the second floor | %i Movements Left\n", *movements);
                break;
            }

            if (location == 101) break;

            *movements = *movements - 1;
            // Reset Player Room Position
            for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
            rooms_list[location - 1].player_in = true; // Set position tu current room
            printf("You are now in %s | %i Movements Left\n", rooms_list[location - 1].name, *movements);
            break;

        case 1:
            for (int i = (ROOMS_NUM / 2); i < ROOMS_NUM; i++)
            {
                printf(" %i- %s\n",i + 1, rooms_list[i].name);
            }
            printf(" 0- First Floor\n");
            printf("\n Type 101 to cancel\n");
            printf("===================================================================================\n");

            while (location < 7 || location > 12)
            {
                location = prompt_int("/move/second_floor> ");
                if (location == 0 || location == 101) break;
            }

            if (location == 101) break;
            if (location == 0)
            {
                for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
                *floor = 0;
                *movements = *movements - 1;
                printf("You are now in the first floor | %i Movements Left\n", *movements);
                break;
            }

            *movements = *movements - 1;
            // Reset Player Room Position
            for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
            rooms_list[location - 1].player_in = true; // Set position to current room
            printf("You are now in %s | %i Movements Left\n", rooms_list[location - 1].name, *movements);   

            break;
    }
    printf("===================================================================================\n");
    
}

// Prints Room Info: People and Items to Steal
void roominfo(innocent crew_members[], room rooms_list[])
{
    for (int i = 0; i < ROOMS_NUM; i++)
    {
        if (rooms_list[i].player_in)
        {
            printf("> %s has %i items in here an %i other people\n", rooms_list[i].name, rooms_list[i].items,
            rooms_list[i].people_in_it);
            
            for (int j = 0; j < CREW_MEMBERS; j++)
            {
                if (crew_members[j].room == i && crew_members[j].alive == 1)
                {
                    printf("- %s\n", crew_members[j].name);
                }
            }
            printf("===================================================================================\n");
            return;
        }
    }
    printf("You are not in a Room right now. Type \"move\" and go to one to consult information.\n");
}

// Steal Mechanic and its Consequences
void steal(int *loot, int *movements, innocent crew_members[], room rooms_list[])
{
    int player_is_in_room = 0, random_num = 0;
    float chance_to_fail = 0, mood_average = 0;
    bool steal_done = 0;

    for (int i = 0; i < ROOMS_NUM; i++)
    {
        if(rooms_list[i].player_in)
        {
            player_is_in_room = 1;
            if (rooms_list[i].items > 0)
            {
                for (int j = 0; j < CREW_MEMBERS; j++)
                {
                    if (crew_members[j].room == i && crew_members[j].alive == true)
                    {
                        mood_average += crew_members[j].mood;
                    }
                }
                mood_average = mood_average / rooms_list[i].people_in_it;
                // numPeople * 5 / average_mood * 100
                chance_to_fail = (((rooms_list[i].people_in_it) * 5) / (mood_average)) * 100;
                random_num = (rand() % 100) + 1;
                
                if (random_num <= chance_to_fail) //Discovered
                {
                    printf("chance to fail %f, mood avg %f, random num %i\n", chance_to_fail, mood_average, random_num);
                    printf("room people %i\n", rooms_list[i].people_in_it);
                    discovered();
                }
                else // Steal Done Successufully
                {
                    rooms_list[i].items--;
                    *loot = *loot + 1;
                    *movements = *movements - 1;
                    steal_done = 1;
                    printf("chance to fail %f, mood avg %f, random num %i\n", chance_to_fail, mood_average, random_num);
                    printf("You took and Object | %i/%i Objects | %i Movements Left\n", *loot, LOOT_TO_STEAL, *movements);
                    mood_steal(i, crew_members, rooms_list);
                    printf("===================================================================================\n");
                }
            }
            // If there are no objects there
            else
            {
                printf("There are no objects to steal here.\n");
                printf("===================================================================================\n");
            }
        }
    }
    // If PLayer is not in a Room
    if (player_is_in_room == 0)
    {
        printf("You are not in a Room, type \"move\", pick one and type \"roominfo\" to see its items.\n");
        printf("===================================================================================\n");
    }
}

// Decrease Mood if you Steal in Front of Someone
void mood_steal(int iterations_to_room, innocent crew_members[], room rooms_list[])
{
    int decrease_rate; 
    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        if (crew_members[i].alive == true && crew_members[i].room == iterations_to_room)
        {
            decrease_rate = (rand() % 15) + 1;
            crew_members[i].mood = crew_members[i].mood - decrease_rate; 
        }
    }
}

void kill(innocent crew_members[], room rooms_list[], int *movements, int *gun)
{
    int player_is_in_room = 0, iterations = 0, weapon = 100, victim = 100;
    int lineRead = 0, proceed = 0;
    char answer_buffer[20];

    for (int current_room = 0; current_room < ROOMS_NUM; current_room++)
    {
        if (rooms_list[current_room].player_in)
        {
            player_is_in_room = 1;
            printf("===================================================================================\n");
            printf(" Type the Number Corresponding at the member:\n\n");
            for (int i = 0; i < CREW_MEMBERS; i++)
            {
                if (crew_members[i].room == current_room && crew_members[i].alive == true)
                {
                    printf(" %i-%s\n", iterations + 1, crew_members[i].name);
                    iterations++;
                }
            }
            printf("\n Type 101 to cancel\n");
            printf("===================================================================================\n");
            if (iterations == 0)
            {
                printf("There are no people here, unable to kill\n");
                break;
            }
            while (victim < 0 || victim > rooms_list[current_room].people_in_it)
            {
                victim = prompt_int("/kill/> ");
                if (victim == 101) break;
            }

            if (victim == 101) break;
            
            printf("===================================================================================\n");
            printf(" Type the number of the weapon you want to use:\n\n");
            printf(" 1- Gun (%i Bullets Left)\n 2- Knife\n\n", *gun);
            printf(" Type 101 to cancel\n");
            printf("===================================================================================\n");
            while (weapon < 1 || weapon > 2)
            {
                weapon = prompt_int("/kill/innocent/> ");
                if (weapon == 101) break;
            }
            if (weapon == 101) break;

            int kill_iteration = 0;
            switch (weapon)
            {
                case 1:
                    if (*gun <= 0)
                    {
                        printf("FAILED! You don't have bullets.\n");
                        printf("===================================================================================\n");
                        return;
                    }
                    for (int i = 0; i < CREW_MEMBERS; i++)
                    {
                        if (crew_members[i].room == current_room && crew_members[i].alive == true)
                        {
                            if (victim != kill_iteration) kill_iteration++;
                            if (victim == kill_iteration)
                            {
                                printf("===================================================================================\n");
                                printf(" > Type the number - Proceed on killing [%s]?\n\n", crew_members[i].name);
                                printf(" 1- Yes\n 2- No\n\n");
                                printf("===================================================================================\n");
                                
                                while (proceed < 1 || proceed > 2)
                                {
                                    proceed = prompt_int("/kill/gun/confirm/> ");
                                }
                                if (proceed == 2) 
                                {
                                    printf("FAILED! Cancelled.\n");
                                    break;
                                }
                                crew_members[i].alive = false;
                                *gun = *gun - 1;
                                printf(" > %s is dead.\n", crew_members[i].name);

                                if(*gun == 0)
                                {
                                    printf("Ammunition Depleted\n");
                                }
                                break;
                            }
                        }
                    }
                    break;
                case 2:
                    printf("Elegiste el Cuchillo\n");
            }
            


        }

    }
    printf("===================================================================================\n");
    if (player_is_in_room == 0)
    {
        printf("You are not in a  Room\n");
    }
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

void discovered(void)
{
    printf("Patata\n");
}