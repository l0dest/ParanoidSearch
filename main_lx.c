#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NAMES 70
#define CREW_MEMBERS 7

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

    FILE *mercstats = fopen("mercstats.csv", "r");

    // Welcome message
    printf("\n==================================================================================\n");
    printf("\t\t\t\tMESSAGE RECIEVED\n");
    printf("\nWelcome once more and possibly last to [INSERT NAME]\nWe are in the climax of the war with our enemy faction"
    " [INSERT NAME]\nIt's vital to us that you get to infiltrate in one of their many bases and STEAL\nas much information"
    " and valuable objects as you can.\n\n");
    printf("Be careful though, some of your \"colleagues\" may be either aware or alert for you\n");
    printf("\n==================================================================================\n");
    
    number_pressed("Type 1 to continue: ", 1);

    printf("==================================================================================\n");
    printf("\nYou will be in that facility exactly one week\nAt the begginng of each day I would be capable of sending you a" 
    " report with\nvaluable information regarding the people arround you. Think you actions twice\nBecause they are limited."
    " Good luck and try to not be killed.\n\n-1NF=)>=|)\n");
    printf("\n==================================================================================\n");

    number_pressed("Type 1 to beggin: ", 1);

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
        printf("crew %i: %s\n",i+1, crew_members[i].name);
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