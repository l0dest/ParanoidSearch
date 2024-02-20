#include <stdbool.h>

#define CREW_MEMBERS 9 // People with you in the game (you lost btw)
#define COMMANDS 7 // Number of Console Commands (The functions below)
#define ROOMS_NUM 12

// Innocent Struct
typedef struct
{
    char name[20];
    float mood;
    bool alive;
    int room;
} innocent;
// Room Struct
typedef struct
{
    char name[20];
    innocent people[CREW_MEMBERS];
    int people_in_it;
    bool previously_visited;
    int items;   
} room;

void help(void);
void move(void);
void kill(void);
void steal(void);
void clear(void);
void people(int people_number, innocent crew_members[]);
void rooms(room rooms_list[]);
