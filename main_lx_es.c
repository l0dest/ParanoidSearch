#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <commands.h>

#define NAMES 71
#define DAYS_TO_WORK 7
#define MAX_BULLETS 2
#define MOVEMENTS_PER_ROUND 10

// List to use
char *facility_rooms[] = {"Area de Computadoras", "Laboratorio", "Depósito", "Armeria", "Ofinas", "Acceso al Sótano",
                            "Acceso al Ático", "Habitaciones", "Baños", "Bar", "Cocina", "Biblioteca"};


char* commands[] = {"help", "move", "steal", "kill", "clear", "people", "rooms", "roominfo"};

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
        printf("NO se pudo encontrar el archivo con los nombres, asegurate de ejecutar el juego en la misma carpeta que este\n");
        exit(0);
    }

    // Welcome message

    printf("\n===================================================================================\n");
    printf("\t\t\t\tMENSAJE RECIBIDO\n");
    printf("\n Bienvenido otra y posiblemente ultima vez a MendedSun\n Estamos en el clímax de la guerra con"
    " nuestra facción enemiga BrokenMoon\n Es vital para nosotros que te infiltres en una de sus bases y "
    "ROBES\n tanta información y objetos valiosos como puedas.\n\n");
    printf(" Ten cuidado alguno de tus \"colegas\" podria estar alerta de tí.\n");
    printf("\n===================================================================================\n");
    
    number_pressed("> Escribe 1 para continuar: ", 1);

    printf("===================================================================================\n");
    printf("\n Estaras en sus instalaciones una semana\n Al principio de cada día sere capaz de enviarte un" 
    " reporte con\n informació valiosa acerca de tus compañeros. Piensa dos veces tus acciones\n Porque son limitadas."
    " Buena suerte y trata de no ser descubierto.\n\n -1NF=)>=|)\n");
    printf("\n===================================================================================\n");

    number_pressed("> Escribe 1 para leer los planos de las instalaciones: ", 1);

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
    printf(" Esta instalación tiene 2 pisos. Cada uno tiene 6 habitaciones en él. Todos los miembros\n "
    "se encuentran tras cada día en Main Room," 
    " alocada en el primer piso. (Los objetos\n no aparecen allí). Puedes consultar esta información luego.\n\n");

    rooms(rooms_list); // Print The Facility Rooms and their Floors

    printf("===================================================================================\n");
    number_pressed("> Escribe 1 para empezar: ", 1);
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
    
        printf(" Día %i de %i || ", WorkDay + 1, DAYS_TO_WORK);
        // Print Remaining Objects to Steal
        printf("%i/%i Objetos|| ", loot, LOOT_TO_STEAL);
        printf("Estado de los Miembros -> Nombre - Ánimo");
        printf("\n===================================================================================\n");

        // Print Crew Members and their Status
        people(CREW_MEMBERS, crew_members);
        
        printf("\n");
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == false)
            {
                printf("* %s no vino al trabajo.\n", crew_members[i].name);
            }
        }
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == false)
            {
                printf("\n- Todos están un poco más nerviosos\n\n");
                break;
            }
        }
        // Ammunition Given
        int ammo = (rand() % MAX_BULLETS) + 1;
        printf("- Recibiste un cuchillo y un arma con %i disparos", ammo);
        printf("\n===================================================================================\n");
        number_pressed("> Escribe 1 para Continuar: ", 1);
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
        printf(" _Bienvenido al Sistema de Reporte de Movimientos de la Linea de Comandos (CLMRS)_\n\n" 
        " En MendedSun apreciamos la recolección de datos. Reportaras cada movimiento que\n hagas en el día." 
        " Usa los comandos proporcionados para informarnos todo.\n Gracias por dejarnos emplearte.");
        printf("\n\n TIENES %i MOVIMIENTOS", movements);
        printf("\n===================================================================================\n");
        printf("> Escribe \"help\" para ver los comandos\n");

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
                printf("Ese no es un comando, escribe \"help\" para verlos\n");
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
                }
            }
            if (caught == 1) 
            {
                discovered(crew_members, rooms_list, room_discovered, &caught, &ammo);
            }
            if (movements == 0) 
            {
                for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
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
        printf(" La jornada terminó:\n\n");
        printf("- %i/%i Objetos\n", loot, LOOT_TO_STEAL);
        for (int i = 0; i < CREW_MEMBERS; i++)
        {
            if (crew_members[i].alive == false)
            {
                death_members++;
            }
        }
        printf("- %i Personas murieron hasta el momento\n\n", death_members);
        printf("===================================================================================\n");
        number_pressed("> Escribe 1 para empezar el siguiente día: ", 1);
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
        printf("Ese no es un comando, escribe \"help\" para verlos\n");
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
            crew_members[i].mood = crew_members[i].mood - ((rand() % 20) + 15) * death_members;
        }
    }
}