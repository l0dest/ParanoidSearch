#include <stdbool.h>

#define CREW_MEMBERS 9 // People with you in the game (you lost btw)
#define COMMANDS 8 // Number of Console Commands (The functions below)
#define ROOMS_NUM 12
#define LOOT_TO_STEAL 20

// Innocent Struct
typedef struct
{
    char name[20];
    float mood;
    bool alive;
    int room;
    bool discovered_player;
} innocent;

// Room Struct
typedef struct
{
    char name[20];
    innocent people[CREW_MEMBERS];
    int people_in_it;
    bool previously_visited;
    int items;
    bool player_in;   
} room;
// User can use these

void help(void);
void move(int *floor, int *movements, innocent crew_members[], room rooms_list[]);
void kill(innocent crew_members[], room rooms_list[], int *movements, int *gun);
void steal(int *loot, int *movements, innocent crew_members[], room rooms_list[]);
void clear(void);
void people(int people_number, innocent crew_members[]);
void rooms(room rooms_list[]);
void roominfo(innocent crew_members[], room rooms_list[]);

// User cannot use these

void discovered(void);
int prompt_int(char * message);
void mood_steal(int iterations_to_room, innocent crew_members[], room rooms_list[]);

