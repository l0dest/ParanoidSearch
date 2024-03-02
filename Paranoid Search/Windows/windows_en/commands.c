#include <stdio.h>
#include <stdlib.h>
#include <commands.h>
#include <time.h>

//Print Help
void help(void)
{
    printf("===================================================================================\n");
    printf(" Enterprise Macro Systems - CLMRS version 6.7 - [MendedSun]\n\n");
    printf(" help : prints this list.\n\n");
    printf(" move : let you move trough rooms, select the number of the choosen one"
    " (cost 1\n movement).\n\n");
    printf(" roominfo : prints the number of people, objects and chances to get caught if you\n steal in the current room.\n\n");
    printf(" steal : once in a room, if there are objects, steal one of them. (cost 1 movement\n"
    " and can get caught by colleagues based on their mood).\n\n");
    printf(" kill : select to kill a collegue, using the gun gives you 100%% chances of success,\n "
    "using the knife gives you a 20%% chances of success. If someone sees you, you get\n discovered and must kill them.\n\n");
    printf(" people : prints your colleagues stats.\n\n rooms : prints the rooms of the facility.\n\n");
    printf(" info : prints the day and objects left to steal\n\n");
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
    int chance_to_fail = 0;
    float mood_average = 0;
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

            if (rooms_list[i].items > 0)
            {
            for (int ii = 0; ii < CREW_MEMBERS; ii++)
            {
                if (crew_members[ii].room == i && crew_members[ii].alive == true)
                {
                    mood_average += crew_members[ii].mood;
                }
            }
            mood_average = mood_average / rooms_list[i].people_in_it;
            if (mood_average < 1) mood_average = 1;
            // numPeople * 5 / average_mood * 100
            chance_to_fail = (((rooms_list[i].people_in_it) * 5) / (mood_average)) * 100;
            if (chance_to_fail < 0) chance_to_fail = 0;
            if (chance_to_fail > 100) chance_to_fail = 100;
            

            printf(" Stealing gives you a %i%% chance of failing\n", chance_to_fail);
            }
            printf("===================================================================================\n");
            return;
        }
    }
    printf("You are not in a Room right now. Type \"move\" and go to one to consult information.\n");
    printf("===================================================================================\n");
}

// Steal Mechanic and its Consequences
void steal(int *loot, int *movements, innocent crew_members[], room rooms_list[], int *gun, int *caught, int *end_day, int *room_discovered)
{
    int player_is_in_room = 0, random_num = 0;
    int chance_to_fail = 0;
    float mood_average = 0;
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
                if (mood_average < 1) mood_average = 1;
                chance_to_fail = (((rooms_list[i].people_in_it) * 5) / (mood_average)) * 100;
                random_num = (rand() % 100) + 1;

                if (chance_to_fail < 0) chance_to_fail = 0;
                if (chance_to_fail > 100) chance_to_fail = 100;
                
                if (random_num <= chance_to_fail) //Discovered
                {
                    printf("room people %i\n", rooms_list[i].people_in_it);
                    *room_discovered = i;
                    *caught = 1;
                    discovered(crew_members, rooms_list, i, caught, gun);
                }
                else // Steal Done Successufully
                {
                    rooms_list[i].items--;
                    *loot = *loot + 1;
                    *movements = *movements - 1;
                    steal_done = 1;
                    // printf("chance to fail %i, mood avg %f, random num %i\n", chance_to_fail, mood_average, random_num);
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
        printf("You are not in a Room. Type \"move\", pick one and type \"roominfo\" to see its items.\n");
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
            if (crew_members[i].mood < 0)
            {
                crew_members[i].mood = 0;
            } 
        }
    }
}

void kill(innocent crew_members[], room rooms_list[], int *movements, int *gun, int *caught, int *end_day, int *room_discovered)
{
    int player_is_in_room = 0, iterations = 0, weapon = 100, victim = 100;
    int proceed = 0, knife_probability = 0;

    for (int current_room = 0; current_room < ROOMS_NUM; current_room++)
    {
        if (rooms_list[current_room].player_in)
        {
            player_is_in_room = 1;

            if (rooms_list[current_room].people_in_it == 0)
            {
                printf("There are no people here, unable to kill\n");
                break;
            }
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
                weapon = prompt_int("/kill/weapon/> ");
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
                                // Kill Confirmation
                                printf("===================================================================================\n");
                                printf(" > Type the number - Proceed on killing [%s]?\n\n", crew_members[i].name);
                                printf(" 1- Yes\n 2- No\n\n");
                                printf("===================================================================================\n");
                                
                                while (proceed < 1 || proceed > 2)
                                {
                                    proceed = prompt_int("/kill/gun/confirm/> ");
                                }
                                if (proceed == 2) break; // Kill Canceled
                                printf("===================================================================================\n");
                                crew_members[i].alive = false; // Kill Done Successfully
                                rooms_list[current_room].people_in_it--;
                                *gun = *gun - 1;
                                printf(" >> %s is dead <<\n", crew_members[i].name);

                                if(*gun == 0)
                                {
                                    printf(" >> Ammunition Depleted <<\n"); // Referencias ;)
                                }
                                else{
                                    printf(" >> [%i] Bullets Left <<\n", *gun);
                                }
                                printf("===================================================================================\n");
                                number_pressed("> Type 1 to continue: ", 1);
                                // If there's someone else in the room --> DISCOVERED
                                if(rooms_list[current_room].people_in_it > 0)
                                {
                                    *room_discovered = current_room;
                                    *caught = 1;
                                    discovered(crew_members, rooms_list, current_room, caught, gun);
                                }
                                break;
                            }
                        }
                    }
                    break;
                case 2:
                    for (int i = 0; i < CREW_MEMBERS; i++)
                    {
                        if (crew_members[i].room == current_room && crew_members[i].alive == true)
                        {
                            if (victim != kill_iteration) kill_iteration++;
                            if (victim == kill_iteration)
                            {
                                // Kill Confirmation
                                printf("===================================================================================\n");
                                printf(" > Type the number - Proceed on killing [%s]?\n", crew_members[i].name);
                                printf(" > Note: Using the Knife as a Primary Offensive Weapon is highly unrecommended.\n\n");
                                printf(" 1- Yes\n 2- No\n\n");
                                printf("===================================================================================\n");
                                
                                while (proceed < 1 || proceed > 2)
                                {
                                    proceed = prompt_int("/kill/knife/confirm/> ");
                                }
                                if (proceed == 2) break; // Kill Canceled
                                printf("===================================================================================\n");

                                knife_probability = (rand() % 100) + 1;
                                if (knife_probability > 75) // 60 | 40 --> FAILURE|SUCCESS
                                {
                                    crew_members[i].alive = false; // Kill Done Successfully
                                    rooms_list[current_room].people_in_it--;
                                    printf(" >> %s is dead <<\n", crew_members[i].name); 
                                }
                                else
                                {
                                    *room_discovered = current_room;
                                    *caught = 1;
                                    discovered(crew_members, rooms_list, current_room, caught, gun);
                                }
                                break;
                            }
                        }
                    }
            }
        }
    }

    if (player_is_in_room == 0)
    {
        printf("You are not in a Room. Type \"move\" and find someone if you want to kill him.\n");
    }
    printf("===================================================================================\n");
}   

void clear(void)
{
    system("cls");
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

void discovered(innocent crew_members[], room rooms_list[], int room_num, int *caught, int *ammo)
{
    while (*caught == 1){
    
    system("cls"); //CHANGE TO cls FOR WINDOWS
    printf("===================================================================================\n");
    int messages = (rand() % 5) + 1;

    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        if (crew_members[i].room == room_num && crew_members[i].alive == true)
        {
        printf(" %s: ", crew_members[i].name);
        break;
        }
    }
    
    switch (messages)
    {
        case 1:
            printf("GOT YOU! You damn snake.\n");
            break;
        case 2:
            printf("STOP RIGHT THERE!\n");
            break;
        case 3:
            printf("You Filthy Rat. FREEZE!!\n");
            break;
        case 4:
            printf("I'll send you to hell. You filthy rat!\n");
            break;
        case 5:
            printf("COME RIGHT HERE, COWARD!!\n");
            break;
    }
    printf("===================================================================================\n");
    printf(" You have been discovered, kill all the witnesses or die:\n\n");

    int list_num = 0; // Numerical list of players in room
    int choose_to_kill = 0; // Who you want to kill?
    int weapon_choosen = 0; // What weapon do you want to use
    // Print Kill Selection
    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        if (crew_members[i].room == room_num && crew_members[i].alive == true)
        {
            printf(" %i-%s\n", list_num + 1, crew_members[i].name);
            list_num++;
        }
    }
    printf("===================================================================================\n");

    while (choose_to_kill < 1 || choose_to_kill > rooms_list[room_num].people_in_it)
    {
        choose_to_kill = prompt_int(" /discovered/kill/> ");
    }
    
    printf("===================================================================================\n");
    printf("Choose your weapon: \n\n");
    printf(" 1- Gun (%i Bullets left)\n 2- Knife\n\n", *ammo);
    printf("===================================================================================\n");

    while (weapon_choosen != 2)
    {
        weapon_choosen = prompt_int(" /discovered/kill/weapon/> ");
        if (weapon_choosen == 1 && *ammo == 0)
        {
            printf(" FAILED! You don't have bullets left\n");
            printf("===================================================================================\n");
            
        }
        if (weapon_choosen == 1 && *ammo != 0) break;
    }
    
    int kill_iteration = 0;
    switch (weapon_choosen)
    {
    case 1: // GUN
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].room == room_num && crew_members[i].alive == true)
            {
                if (choose_to_kill != kill_iteration) kill_iteration++;
                if (choose_to_kill == kill_iteration)
                {
                    crew_members[i].alive = false;
                    *ammo = *ammo - 1;
                    rooms_list[room_num].people_in_it--;
                    printf(" >> %s is dead <<\n", crew_members[i].name);

                    if(*ammo == 0)
                    {
                        printf(" >> Ammunition Depleted <<\n"); // Referencias ;)
                    }
                    else{
                        printf(" >> [%i] Bullets Left <<\n", *ammo);
                    }

                    printf("===================================================================================\n");

                    number_pressed(" > Type 1 to continue: ", 1);
                    
                    break;
                }
            }
        }
        break;

    case 2: // KNIFE
        int knife_probability = 0;
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].room == room_num && crew_members[i].alive == true)
            {
                if (choose_to_kill != kill_iteration) kill_iteration++;
                if (choose_to_kill == kill_iteration)
                {
                    knife_probability = (rand() % 100) + 1;
                    if (knife_probability > 75)
                    {
                        crew_members[i].alive = false;
                        rooms_list[room_num].people_in_it--;
                        printf(" >> %s is dead <<\n", crew_members[i].name);
                        printf("===================================================================================\n");
                        number_pressed("> Type 1 to continue: ", 1);
                        break;
                    }

                    else
                    {
                        discovered_fail();
                    }
                }
            }
        }
        break;
    }
    if (rooms_list[room_num].people_in_it == 0) 
    {
        printf(" Well done, all witnesses were killed\n");
        *caught = 0;
    }
}
}

void discovered_fail(void)
{
    printf("===================================================================================\n");
    printf(" YOU'VE FAILED\n\n You have been discovered, now you're going to be executed.\n\n THE END.\n");
    printf("===================================================================================\n");
    number_pressed("> Type 1 to exit: ", 1);
    exit(0);
}
void bad_quota_ending(void)
{
    system("cls");
    printf("===================================================================================\n");
    printf("\t\t\t\tMESSAGE RECIEVED\n\n");
    printf(" The deadline is over, you couldn't get enought valuable objects and information\n"
    " Our enemy intelligence is a force to be considered, and they surely managed to get\n something from us.\n\n"
    " This war took an unexpected twist against us, we cannot see a victorious future\n anymore. Neither you do.\n\n\n");
    printf(" -Connection Terminated.\n");
    printf("===================================================================================\n");
    number_pressed("> Type 1 to exit: ", 1);
    exit(0);
}

void good_quota_ending(void)
{
    system("cls");
    printf("===================================================================================\n");
    printf("\t\t\t\tMESSAGE RECIEVED\n\n");
    printf(" Agent, you have successfully completed the objects required.\n Thanks to your services we can finally"
    " set an endpoint to this pointless war.\n Peace will bloom everywhere and all thanks to you. We'll be in contact\n\n");
    printf("-Connection Terminated.\n");
    printf("===================================================================================\n");
    number_pressed("> Thanks for playing. Type 1 to celerabte: ", 1);

    printf("===================================================================================\n");
    printf(" * You sit and drink a cup of coffee *\n\n");
    printf(" Thanks for Playing\n");
    printf("\t\t-Lodest\n");
    printf("===================================================================================\n");
    number_pressed("> Type 1 to exit", 1);
    exit(0);
}

// If all die
void all_death_ending(void)
{
    system("cls");
    printf("===================================================================================\n");
    printf("\t\t\t\tMESSAGE RECIEVED\n\n");
    printf(" You kill all the facilty members without stealing everything.\n If we needed an assassin we could contract one."
    " After any response, BrokenMoon's\n agents are coming right now to your location. It is impossible to escape alive\n from there.\n\n"
    "-Connection Terminated\n");
    printf("===================================================================================\n");
    number_pressed("> Type 1 to exit: ", 1);
    exit(0);
}

void info(int *WorkDay, int*loot)
{
    printf("===================================================================================\n");
    printf(" Day %i of %i | You have %i/%i Objects\n", *WorkDay + 1, DAYS_TO_WORK, *loot, LOOT_TO_STEAL);
    printf("===================================================================================\n");
}