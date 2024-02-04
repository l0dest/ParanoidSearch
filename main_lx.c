#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NAMES 70
#define CREW_MEMBERS 7
#define DAYS_TO_WORK 7
#define LOOT_TO_STEAL 20
#define MAX_BULLETS 2

// Define the Crew Stats
typedef struct
{
    char name[15];
    float mood;
    bool alive;
}
innocent;

// innocent crew_members[CREW_MEMBERS];
innocent crew_members[CREW_MEMBERS];

// functions
int prompt_int(char * message);
void number_pressed(char * message, int required_number);

int main(void)
{
    char buffer[1000];
    char *data;
    bool ending = false, win = false, lose = false; // To check game finished state
    int WorkDay = 0; // Days worked
    int loot = 0; // Loot stealed

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

    number_pressed("> Type 1 to beggin: ", 1);

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
        crew_members[i].mood = 100.0;
        //printf("crew %i: %s\n",i+1, crew_members[i].name);
    }

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
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive) printf("- %s - %i\n", crew_members[i].name, (int)crew_members[i].mood);
            else printf("- %s -> DEATH\n", crew_members[i].name);
        }
        printf("\n");

        // Gun ammo
        int ammo = (rand() % MAX_BULLETS) + 1;
        printf("- You have recieved a Knife and a Gun with %i shots\n", ammo);

        number_pressed("\n> Type 1 to read the facility's Blueprints: ", 1);
        ending = true;
        // Print Movements Remainig


        // Calculate Random House Position for the Crews and Store in an Struct with the name of the Room
        // Calculate Random Object Displacement for the House and Store in a Struct with the name of the Room

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
