#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <commands.h>

#define NAMES 70
#define DAYS_TO_WORK 7
#define LOOT_TO_STEAL 20
#define MAX_BULLETS 2

// List to use
char *facility_rooms[] = {"Computers Area", "Laboratory", "Storage Room", "Armory", "Offices", "Basement Access",
                            "Atic Access", "Bedrooms", "Bathrooms", "Bar", "Kitchen", "Library"};


char* commands[] = {"help", "move", "steal", "kill", "clear", "people", "rooms"};

// innocent crew_members[CREW_MEMBERS];
innocent crew_members[CREW_MEMBERS];
room rooms_list[ROOMS_NUM] = {0};

// functions
int prompt_int(char * message);
void number_pressed(char * message, int required_number);
void command_prompt(char *prompt);
void choose_command_function(int result);

int main(void)
{
    char buffer[1000];
    char *data;
    bool ending = false, win = false, lose = false; // To check game finished state
    int WorkDay = 0; // Days worked
    int loot = 0; // Loot stealed
    int room_num_location;

    FILE *mercstats = fopen("mercstats.csv", "r");

    // Welcome message

    printf("\n===================================================================================\n");
    printf("\t\t\t\tMESSAGE RECIEVED\n");
    printf("\n Welcome once more and possibly last to [INSERTAR NOMBRE TO EPICO]\n We are in the climax of the war with our"
    " enemy faction [INSERTAR NOMBRE TO EPICO]\n It's vital to us that you get to infiltrate in one of their many bases and "
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

    // Set all then ALIVE and with MAX mood and Print the Crew Members
    for (int i = 0; i < CREW_MEMBERS; i++)
    {
        crew_members[i].alive = true;
        crew_members[i].room = 0;
        crew_members[i].mood = 100.0;
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
        // See and Print the day number
        printf("\n===================================================================================\n");
        printf(" Day %i: %i Days Remaining || ", WorkDay + 1, DAYS_TO_WORK - WorkDay);
        
        // If deathline is over and not all objects have been collected
            // Lose -> END
        /*if (WorkDay == DAYS_TO_WORK && loot < LOOT_TO_STEAL)
        {
            bad_noquota_ending();
        }
        // If all objects have been collected no matter the day
            // Win -> END
        if (loot >= LOOT_TO_STEAL)
        {
            good_quota_ending();
        }*/

        // Print Remaining Objects to Steal
        printf("%i/%i Objects || ", loot, LOOT_TO_STEAL);
        printf("FACTION MEMBERS' STATUS -> Name - Mood");
        printf("\n===================================================================================\n");

        // Print Crew Members and their Status
        people(CREW_MEMBERS, crew_members);
        
        printf("\n");

        // Ammunition Given
        int ammo = (rand() % MAX_BULLETS) + 1;
        printf("- You have recieved a Knife and a Gun with %i shots", ammo);
        printf("\n===================================================================================\n");
        number_pressed("> Type 1 to Continue: ", 1);
        // ending = true;
        // Print Movements Remainig

        // Calculate Random House Position for the Crews and Store in an Struct with the name of the Room
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            room_num_location = rand() % 12;
            crew_members[i].room = room_num_location;
            rooms_list[crew_members[i].room].people[i] = crew_members[i];
            rooms_list[crew_members[i].room].people_in_it++; 
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
        printf(" __Welcome to the Command Line Movement Report System (CLMRS)__\n\n In [NAME] we appreciate data" 
        " collection. You'll report every movement you make\n through the day. Use written commands"
        " to tell us every action you do\n Thank you for letting us use you.");
        printf("\n\n YOU HAVE 7 MOVEMENTS LEFT");
        printf("\n===================================================================================\n");
        printf("> Type \"help\" to see the commands\n");
        
        int end_day = 0;
        char prompt[20];
        while (end_day == 0)
        {
            int lineRead = 0;
            printf("> ");
            lineRead = scanf("%19[^\n]%*c", prompt);
            while(lineRead != 1) // Avoid Newline Loop Error
            {
                printf("That is not a command, type \"help\" to see them\n");
                while (getchar() != '\n');
                printf("> ");
                lineRead = scanf("%19[^\n]%*c", prompt);
            }            
            command_prompt(prompt);
        }

        ending = true;
        // While Player has movements Remaning
            // If Player Moves to N room, print Objects and People in there -> Movements Remaining --
            // Player Inputs his action 
            //If STEAL
                // Check CREWs in the room stats and Calculate the chances of success
                    // If success
                        // Objects to Steal --
                        // Crews in the room Paranoia ++
                        // Movements Remaining --
                    // If discovered and Bullets != 0 and Crew memebers in the room < 3
                        // Print "discovered", use as many bullets as crews in the room
                        // Bullets--
                        // Movements Remaining --
                    // Else
                        // Lose -> END
            //If Kill
                // Ask to use knife or Gun
                // If GUN
                    // Kill Crew Member
                    // Check if someone else saw it
                    // If discovered and Bullets != 0 and Crew memebers in the room < 3
                        // Print "discovered", use as many bullets as crews in the room
                        // Bullets--
                        // Movements Remaining --
                    // Else
                        // Lose -> END

            // Increase Crew paranoia proportionally to the number of death crew_members
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

void command_prompt(char *prompt)
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

    choose_command_function(result);
}

void choose_command_function(int result)
{
    switch (result)
    {
        case 0:
            help();
            break;
        case 1:
            move();
            break;
        case 2:
            steal();
            break;
        case 3:
            kill();
            break;
        case 4:
            clear();
            break;
        case 5:
            people(CREW_MEMBERS, crew_members);
            break;
        case 6:
            rooms(rooms_list);
            break;
    }
}
