#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <commands.h>

#define NAMES 71
#define MAX_BULLETS 2
#define MOVEMENTS_PER_ROUND 8

// List to use
char *facility_rooms[] = {"Computers Area", "Laboratory", "Storage Room", "Armory", "Offices", "Basement Access",
                            "Atic Access", "Bedrooms", "Bathrooms", "Bar", "Kitchen", "Library"};


char* commands[] = {"help", "move", "steal", "kill", "clear", "people", "rooms", "roominfo", "info"};

// innocent crew_members[CREW_MEMBERS];
innocent crew_members[CREW_MEMBERS];
room rooms_list[ROOMS_NUM] = {0};

// functions
int prompt_int(char * message);
void number_pressed(char * message, int required_number);
int command_prompt(char *prompt);
void increse_paranoia_if_kill(innocent crew_members[]);

int main(void)
{
    char buffer[1000];
    char *data;
    bool ending = false; // To check game finished state
    int WorkDay = 0; // Days worked
    int loot = 0; // Loot stealed
    int room_num_location;
    int alive_members = CREW_MEMBERS;
    int caught = 0;


    FILE *mercstats = fopen("mercstats.csv", "r");
    if (mercstats == NULL)
    {
        printf("Couldn't find the Names file. Please execute the program in the same folder with it.\n");
        exit(0);
    }

    // Welcome message

    printf("\n===================================================================================\n");
    printf("\t\t\t\tMESSAGE RECIEVED\n");
    printf("\n Welcome once more and possibly last to MendedSun\n We are in the climax of the war with our"
    " enemy faction BrokenMoon\n It's vital to us that you get to infiltrate in one of their many bases and "
    "STEAL\n as much information and valuable objects as you can.\n\n");
    printf(" Be careful though, some of your \"colleagues\" may be either aware or alert for you\n");
    printf("\n===================================================================================\n");
    
    number_pressed("> Type 1 to continue: ", 1);

    printf("===================================================================================\n");
    printf("\n You will be in that facility exactly one week\n At the begginng of each day I would be capable of sending you a" 
    " report with\n valuable information regarding the people arround you. Think your actions twice\n Because they are limited."
    " Good luck and try to not be killed.\n\n -1NF=)>=|)\n");
    printf("\n===================================================================================\n");

    number_pressed("> Type 1 to read the facility's Blueprints: ", 1);

    system("clear");  // Clear Console  pd:In windows change to "cls"

    // End of Welcome message

    // Choose a random number from zero to NAMES and saves them in a list size CREW_MEMBERS 
    srand(time(0));
    int randomness[CREW_MEMBERS]; 
    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        randomness[i] = (rand() % NAMES) + 2;
    }
    printf("\n");
    

   // Assign a random name for the crew members
    int counter = 0;
    do
    {
        for (int i = 0; i < randomness[counter]; i++)
        {    
            fgets(buffer, sizeof(buffer), mercstats);
        }
        data = strtok(buffer, ",");
        strcpy(crew_members[counter].name, data);

        FILE *stream1 = mercstats;
        mercstats = freopen("mercstats.csv", "r", stream1);
        counter++;

    } while(counter < CREW_MEMBERS);

    // Add String Terminator to All
    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        int length = strlen(crew_members[i].name);
        crew_members[i].name[length] = '\0';
    }
    // Set all then ALIVE and with MAX mood and Print the Crew Members
    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        crew_members[i].alive = true;
        crew_members[i].room = 0;
        crew_members[i].mood = 100.0;
        crew_members[i].discovered_player = false;
        //printf("crew %i: %s\n",i+1, crew_members[i].name);
    }

    // Set all rooms
    for (int i = 0; i < ROOMS_NUM; i++)
    {
        strcpy(rooms_list[i].name, facility_rooms[i]);
    }

    // Print House's Blueprints
    printf("\n===================================================================================\n");
    printf(" This facility has 2 floors. Each of them have 6 rooms in them. All faction members\n "
    "meet througt each day in the Main Room," 
    " located in the first floor. (Objects can't\n appear in here). You can see this info later.\n\n");

    rooms(rooms_list); // Print The Facility Rooms and their Floors

    printf("===================================================================================\n");
    number_pressed("> Type 1 to Begginn Day: ", 1);
    system("clear"); // CHANGE TO cls FOR WINDOWS

    // Main Loop
    while (!ending)
    {
        int floor = 0;
        int movements = MOVEMENTS_PER_ROUND;
        int room_discovered;
        int failing_chance_steal = 0;

        for (int i = 0; i < ROOMS_NUM; i++)
        {
            rooms_list[i].people_in_it = 0;
        }

        // See and Print the day number
        printf("\n===================================================================================\n");
        
        //if (WorkDay == DAYS_TO_WORK && loot < LOOT_TO_STEAL)
        if (WorkDay == DAYS_TO_WORK && loot != LOOT_TO_STEAL)
        {
            bad_quota_ending();
        }
        
        // If all objects have been collected no matter the day
        if (loot >= LOOT_TO_STEAL)
        {
            good_quota_ending();
        }

        bool all_death = false;
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == true)
            {
                all_death = false;
                break;
            }
            else{
                all_death = true;
            }
        }

        if (all_death)
        {
            all_death_ending();
        }
    
        printf(" Day %i of %i || ", WorkDay + 1, DAYS_TO_WORK );
        // Print Remaining Objects to Steal
        printf("%i/%i Objects || ", loot, LOOT_TO_STEAL);
        printf("FACTION MEMBERS' STATUS -> Name - Mood");
        printf("\n===================================================================================\n");

        // Print Crew Members and their Status
        people(CREW_MEMBERS, crew_members);
        
        printf("\n");
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == false)
            {
                printf("* %s didn't came to work.\n", crew_members[i].name);
            }
        }
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == false)
            {
                printf("\n- Everyone is a little bit more nervous\n\n");
                break;
            }
        }
        // Ammunition Given
        int ammo = (rand() % MAX_BULLETS) + 1;
        printf("- You have received a knife and a gun with %i shots", ammo);
        printf("\n===================================================================================\n");
        number_pressed("> Type 1 to Continue: ", 1);
        // ending = true;
        // Print Movements Remainig

        // Calculate Random House Position for the Crews and Store in an Struct with the name of the Room
        for (int i = 0; i < ROOMS_NUM; i++)
        {
            if (crew_members[i].alive)
            {
                room_num_location = rand() % ROOMS_NUM;
                crew_members[i].room = room_num_location;
                rooms_list[crew_members[i].room].people[i] = crew_members[i];
                rooms_list[crew_members[i].room].people_in_it++;
            } 
        } 

        //This just prints where is every crew, delete later
        /*for (int i = 0; i < CREW_MEMBERS; i++)
        {
            printf("%s is in %s\n", crew_members[i].name, rooms_list[crew_members[i].room].name);
        }*/
        
        // Calculate Random Object Displacement for the House and Store in a Struct with the name of each the Room
        for (int i = 0; i < ROOMS_NUM; i++)
        {
            int loot_probability = (rand() % 10) + 1;
            // ITEMS PROBABILTY: 10% none | 90% some --> 20% -> three | 30 % -> two | 50% -> one
            if (loot_probability > 9) // No obejcts
            {
                rooms_list[i].items = 0;
            }
            else
            {
                loot_probability = (rand() % 10) + 1; // re-roll

                if (loot_probability <= 2) rooms_list[i].items = 3;

                else if (loot_probability <= 5 && loot_probability > 2) rooms_list[i].items = 2;

                else rooms_list[i].items = 1;
            }
        }
        // This also just prints the room name, how many items has and how many peoples are in it
        /*printf("\n");
        for (int i = 0; i < ROOMS_NUM; i++)
        {
            printf("%s has %i items and %i people in it\n", rooms_list[i].name, rooms_list[i].items, rooms_list[i].people_in_it);
        }*/

        // Command Line Message
        printf("===================================================================================\n");
        printf(" Welcome to the Command Line Movement Report System (CLMRS)\n\n In MendedSun we appreciate data" 
        " collection. You'll report every movement you make\n through the day. Use the commands"
        " to tell us every action you do. Thank you for\n letting us employ you.");
        printf("\n\n YOU HAVE %i MOVEMENTS LEFT", movements);
        printf("\n===================================================================================\n");
        printf("> Type \"help\" to see the commands\n");

        // Loop Trough Day
        int end_day = 0;
        char prompt[20];
        int place_message = 0;
        while (end_day == 0)
        {
            int lineRead = 0;
            // Print > withe the name of the current room
            for (int i = 0; i < ROOMS_NUM; i++)
            {
                if (rooms_list[i].player_in)
                {
                    printf("/%s/> ", rooms_list[i].name);
                    place_message = 1;
                    break;
                }
                else place_message = 0;
            }
            if (place_message == 0) printf("> ");

            lineRead = scanf("%19[^\n]%*c", prompt);
            while(lineRead != 1) // Avoid Newline Loop Error
            {
                printf("That is not a command, type \"help\" to see them\n");
                while (getchar() != '\n');
                printf("> ");
                lineRead = scanf("%19[^\n]%*c", prompt);
            }            
            int result = command_prompt(prompt);
            
            if (caught == 0)
            {
                switch (result)
                {
                    case 0:
                        help();
                        break;
                    case 1:
                        move(&floor, &movements, crew_members, rooms_list);
                        break;
                    case 2:
                        steal(&loot, &movements, crew_members, rooms_list, &ammo, &caught,&end_day, &room_discovered);
                        break;
                    case 3:
                        kill(crew_members, rooms_list, &movements, &ammo, &caught, &end_day, &room_discovered);
                        break;
                    case 4:
                        clear();
                        break;
                    case 5:
                        printf("===================================================================================\n");
                        people(CREW_MEMBERS, crew_members);
                        printf("===================================================================================\n");
                        break;
                    case 6:
                        printf("===================================================================================\n");
                        rooms(rooms_list);
                        printf("===================================================================================\n");
                        break;
                    case 7:
                        roominfo(crew_members, rooms_list);
                        break;
                    case 8:
                        info(&WorkDay, &loot);
                        break;
                }
            }
            if (caught == 1) 
            {
                discovered(crew_members, rooms_list, room_discovered, &caught, &ammo);
            }
            if (movements == 0) 
            {
                for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
                printf(" 0 MOVEMENTS LEFT - SHIFT COMPLETED\n");
                printf("===================================================================================\n");
                number_pressed("> Type 1 to continue: ", 1);
                system("clear");
                end_day = 1;
            }
        }
        // End Loop Trough Day

        // Increase Crew paranoia proportionally to the number of death crew_members
        increse_paranoia_if_kill(crew_members);
        WorkDay++;

        int death_members = 0;
        printf("===================================================================================\n");
        printf(" The shift has ended:\n\n");
        printf("- %i/%i Objects\n", loot, LOOT_TO_STEAL);
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == false)
            {
                death_members++;
            }
        }
        printf("- %i People died until now\n\n", death_members);
        printf("===================================================================================\n");
        number_pressed("> Type 1 to beggin the next day: ", 1);
        system("clear");
    }
    // Close the Names file
    fclose(mercstats);

}
// Get an Integer from the User.
int prompt_int(char * message)
{
    int number, numRead;
    printf("%s", message);
    numRead = scanf("%d", &number);
    while (numRead != 1)
    {
        printf("%s", message);
        scanf("%*[^\n]");
        numRead = scanf("%d", &number);
    }
    while (getchar() != '\n');  // Empty stdin
    return number;
}

// Makes a loop that breaks only when certain number is typed.
void number_pressed(char * message, int required_number)
{
    int num; 
    do
    {
        num = prompt_int(message);
    } while (num != required_number);
}

int command_prompt(char *prompt)
{
    int result = COMMANDS + 1;
    for (int i = 0; i < COMMANDS; i++)
    {
        if (strcmp(prompt, commands[i]) == 0) result = i;
    }

    if (result == COMMANDS + 1)
    {
        printf("That is not a command, type \"help\" to see them\n");
    }

    return result;
}

void increse_paranoia_if_kill(innocent crew_members[])
{
    int death_members = 0;
    for (int i = 0; i < CREW_MEMBERS; i++)
    {   
        if (crew_members[i].alive == false)
        {
            death_members++;
        }
    }

    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        if (crew_members[i].alive)
        {
            crew_members[i].mood = crew_members[i].mood - ((rand() % 10) + 5) * death_members;
            
            if (crew_members[i].mood < 0)
            {
                crew_members[i].mood = 0;
            } 
        }
    }
}