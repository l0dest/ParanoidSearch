#include <stdio.h>
#include <stdlib.h>
#include <commands.h>
#include <time.h>

//Print Help
void help(void)
{
    printf("===================================================================================\n");
    printf(" Enterprise Macro Systems - CLMRS version 6.7 - [MendedSun]\n\n");
    printf(" help : muestra esta lista.\n\n");
    printf(" move : permite moverte a otras habitaciones, selecciona el numero de la elegida\n"
    " (cuesta 1 movimiento).\n\n");
    printf(" roominfo : muetra el numero de personas, objetos y chances de se atrapado si\n robas en la habitacion actual.\n\n");
    printf(" steal : una vez en una habitacion, si hay objetos, robas uno de ellos.\n (cuesta 1 movimiento"
    " y puedes ser atrapado por colegas segun su estado de ánimo).\n\n");
    printf(" kill : selecciona para matar a un colega, usar la pistola da un 100%% de chances de\n exito, "
    "usar el cuchillo da un 20%% de chances de éxito. Si alguien te ve, eres\n descubierto y debes eliminarlos.\n\n");
    printf(" people : muesta el estado de tus colegas.\n\n rooms : muestra las habitaciones de la casa.\n\n");
    printf(" clear : limpia la consola de comandos.\n");
    printf("===================================================================================\n");
}

// Movement Mechanics
void move(int *floor, int *movements, innocent crew_members[], room rooms_list[])
{
    printf("===================================================================================\n");
    printf(" Escribe el numero de la habitación a la que quieras ir: \n\n");
    int location = 100; //default
    switch (*floor)
    {
        case 0:

            for (int i = 0, floor1 = (ROOMS_NUM / 2); i < floor1; i++)
            {
                printf(" %i- %s\n",i + 1, rooms_list[i].name);
            }
            printf(" 0- Segundo Piso\n");
            printf("\n Escribe 101 para cancelar\n");
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
                printf("Ahora estas en el 2do piso | %i Movimimientos Restantes\n", *movements);
                break;
            }

            if (location == 101) break;

            *movements = *movements - 1;
            // Reset Player Room Position
            for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
            rooms_list[location - 1].player_in = true; // Set position tu current room
            printf("Ahora estas en %s | %i Movimientos Restantes\n", rooms_list[location - 1].name, *movements);
            break;

        case 1:
            for (int i = (ROOMS_NUM / 2); i < ROOMS_NUM; i++)
            {
                printf(" %i- %s\n",i + 1, rooms_list[i].name);
            }
            printf(" 0- Primer Piso\n");
            printf("\n Escribe 101 para cancelar\n");
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
                printf("Ahora estas en el 1er piso | %i Movimientos Restantes\n", *movements);
                break;
            }

            *movements = *movements - 1;
            // Reset Player Room Position
            for (int i = 0; i < ROOMS_NUM; i++) rooms_list[i].player_in = 0;
            rooms_list[location - 1].player_in = true; // Set position to current room
            printf("Ahora estas en %s | %i Movimientos Restantes\n", rooms_list[location - 1].name, *movements);   

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
            printf("> %s tiene %i objetos y %i personas\n", rooms_list[i].name, rooms_list[i].items,
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
            

            printf(" Robar te da un %i%% chances de fallar\n", chance_to_fail);
            }
            printf("===================================================================================\n");
            return;
        }
    }
    printf("No estás en una habitacion. Escribe \"move\" y ve a una para consultar su informacion.\n");
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
                if (mood_average < 1) mood_average = 1;
                // numPeople * 5 / average_mood * 100
                chance_to_fail = (((rooms_list[i].people_in_it) * 5) / (mood_average)) * 100;
                random_num = (rand() % 100) + 1;

                if (chance_to_fail < 0) chance_to_fail = 0;
                if (chance_to_fail > 100) chance_to_fail = 100;
                
                if (random_num <= chance_to_fail) //Discovered
                {
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
                    printf("Tomaste un objeto | %i/%i Objetos | %i Movimientos Restantes\n", *loot, LOOT_TO_STEAL, *movements);
                    mood_steal(i, crew_members, rooms_list);
                    printf("===================================================================================\n");
                }
            }
            // If there are no objects there
            else
            {
                printf("No hay objetos para robar aqui.\n");
                printf("===================================================================================\n");
            }
        }
    }
    // If PLayer is not in a Room
    if (player_is_in_room == 0)
    {
        printf("No estás en una habitacion, escribe \"move\", selecciona una y escribe\n\"roominfo\" para ver sus items.\n");
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
                printf("No hay nadie a quien matar aqui\n");
                break;
            }
            printf("===================================================================================\n");
            printf(" Selecciona el numero correspondiente al miembro:\n\n");
            for (int i = 0; i < CREW_MEMBERS; i++)
            {
                if (crew_members[i].room == current_room && crew_members[i].alive == true)
                {
                    printf(" %i-%s\n", iterations + 1, crew_members[i].name);
                    iterations++;
                }
            }
            printf("\n Escribe 101 para cancelar\n");
            printf("===================================================================================\n");
            // if (iterations == 0)
            // {
            //     printf("No hay nadie a quien matar aquí\n");
            //     break;
            // }
            while (victim < 0 || victim > rooms_list[current_room].people_in_it)
            {
                victim = prompt_int("/kill/> ");
                if (victim == 101) break;
            }

            if (victim == 101) break;
            
            printf("===================================================================================\n");
            printf(" Elije el numero del arma que quieras usar:\n\n");
            printf(" 1- Pistola (%i Balas Restantes)\n 2- Cuchillo\n\n", *gun);
            printf(" Escribe 101 para cancelar\n");
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
                        printf(" FALLASTE! No tienes balas.\n");
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
                                printf(" > Escribe el numero - Matar a [%s]?\n\n", crew_members[i].name);
                                printf(" 1- Si\n 2- No\n\n");
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
                                printf(" >> %s ha muerto <<\n", crew_members[i].name);

                                if(*gun == 0)
                                {
                                    printf(" >> Municion Agotada <<\n"); // Referencias ;)
                                }
                                else{
                                    printf(" >> [%i] Balas Restantes <<\n", *gun);
                                }
                                printf("===================================================================================\n");
                                number_pressed("> Escribe 1 para continuar: ", 1);
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
                                printf(" > Escribe el número - Matar a [%s]?\n\n", crew_members[i].name);
                                printf(" > Nota: Usar el cuchillo como arma ofensiva primaria es altamente arriesgado.\n\n");
                                printf(" 1- Si\n 2- No\n\n");
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
                                    printf(" >> %s ha muerto <<\n", crew_members[i].name); 
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
        printf("No estás en una habitación. Escribe \"move\" y busca a alguien si quieres eliminarlo\n");
    }
    printf("===================================================================================\n");
}

void clear(void)
{
    system("clear");
    printf("> Escribe \"help\" para ver los comandos\n");
}

// Print People's Name and their Status
void people(int people_number, innocent crew_members[])
{
    for (int i = 0; i < people_number; i++)
    {
        if (crew_members[i].alive) printf("- %s - %i\n", crew_members[i].name, (int)crew_members[i].mood);
        else printf("- %s -> MUERTO\n", crew_members[i].name);
    }
}

// Print The Facility Rooms and their Floors
void rooms(room rooms_list[])
{
    printf(" Primer Piso:\n");
    for (int i = 0, floor1 = (ROOMS_NUM / 2); i < floor1; i++)
    {
        printf("- %s\n", rooms_list[i].name);
    }

    printf("\n Segundo Piso:\n");
    for (int i = (ROOMS_NUM / 2); i < ROOMS_NUM; i++)
    {
        printf("- %s\n", rooms_list[i].name);
    }
}

void discovered(innocent crew_members[], room rooms_list[], int room_num, int *caught, int *ammo)
{
    while (*caught == 1){
    
    system("clear"); //CHANGE TO cls FOR WINDOWS
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
            printf("TE TENGO! Maldita serpiente.\n");
            break;
        case 2:
            printf("QUIETO AHI!\n");
            break;
        case 3:
            printf("Rata sucia. QUIETO!!\n");
            break;
        case 4:
            printf("Te enviare al infierno. Reptil!\n");
            break;
        case 5:
            printf("VEN AQUI, COBARDE!!\n");
            break;
    }
    printf("===================================================================================\n");
    printf(" Fuiste descubierto, elimina a todos los testigos o muere:\n\n");

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
    printf("Elije un arma: \n\n");
    printf(" 1- Pistola (%i Balas restantes)\n 2- Cuchillo\n\n", *ammo);
    printf("===================================================================================\n");

    while (weapon_choosen != 2)
    {
        weapon_choosen = prompt_int(" /discovered/kill/weapon/> ");
        if (weapon_choosen == 1 && *ammo == 0)
        {
            printf(" FALLASTE! No tienes balas.\n");
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
                    printf(" >> %s ha muerto <<\n", crew_members[i].name);

                    if(*ammo == 0)
                    {
                        printf(" >> Municion Agotada <<\n"); // Referencias ;)
                    }
                    else{
                        printf(" >> [%i] Balas Restantes <<\n", *ammo);
                    }

                    printf("===================================================================================\n");

                    number_pressed(" > Escribe 1 para continuar: ", 1);
                    
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
                        printf(" >> %s ha muerto <<\n", crew_members[i].name);
                        printf("===================================================================================\n");
                        number_pressed("> Escribe 1 para continuar: ", 1);
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
        printf(" Bien hecho, todos los testigos han muerto\n");
        *caught = 0;
    }
}
}

void discovered_fail(void)
{
    printf("===================================================================================\n");
    printf(" FALLASTE\n\n Fuiste descubierto, ahora seras ejecutado.\n\n EL FIN.\n");
    printf("===================================================================================\n");
    number_pressed("> Escribe 1 para salir: ", 1);
    exit(0);
}
void bad_quota_ending(void)
{
    system("clear");
    printf("===================================================================================\n");
    printf("\t\t\t\tMENSAJE RECIBIDO\n\n");
    printf(" La fecha limite ha legado, no pudiste colectar la cantidad suficiente de\n infomacion y objetos."
    " La inteligencia de nuestro enemigo es una fuerza a ser\n reconocida. Muy posiblemente consiguieron algo de nosotros.\n\n"
    " Esta guerra tomo un giro inesperado en nuestra contra, no vemos un futuro exitoso\n ni para nosotros. Ni para ti.\n\n\n");
    printf(" -Conexion Terminada.\n");
    printf("===================================================================================\n");
    number_pressed("> Escribe 1 para salir: ", 1);
    exit(0);
}

void good_quota_ending(void)
{
    system("clear");
    printf("===================================================================================\n");
    printf("\t\t\t\tMENSAJE RECIBIDO\n\n");
    printf(" Agente, exitosamente ha conseguido todos los objetos necesarios.\n Gracias a sus servicios finalmente podemos"
    " marcar un punto de cierre a esta\n guerra sin sentido. La paz florecera en todos lados gracias a usted.\n" 
    " Estaremos en contacto.\n\n");
    printf("-Conexión Terminada.\n");
    printf("===================================================================================\n");
    number_pressed("> Escribe 1 para Celebrar: ", 1);

    printf("===================================================================================\n");
    printf(" * Te sientas, miras al horizonte y tomas un poco de cafe *\n\n");
    printf(" Gracias por Jugar ;)\n");
    printf("\t\t-Lodest\n");
    printf("===================================================================================\n");
    number_pressed("> Escribe 1 para salir:", 1);
    exit(0);
}

// If all die
void all_death_ending(void)
{
    system("clear");
    printf("===================================================================================\n");
    printf("\t\t\t\tMENSAJE RECIBIDO\n\n");
    printf(" Has eliminado a todos los miembros sin recolectar todos los objetos.\n Si hubiesemos necesitado un asesino,"
    "hubiesemos contratado uno.\n Despues de no\n recibir respuesta, agentes de BrokenMoon se dirigen a tu locacion ahora mismo\n" " Es imposible escapar vivo de alli.\n\n"
    "-Conexion Terminada\n");
    printf("===================================================================================\n");
    number_pressed("> Escribe 1 para salir ", 1);
    exit(0);
}
