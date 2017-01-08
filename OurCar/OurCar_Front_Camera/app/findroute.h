#ifndef __FIND_ROUTE_H_
#define __FIND_ROUTE_H_
#include <stdint.h>
#include <stdbool.h>

typedef struct p{
    float x;
    float y;
}Pos;


void Init_routedata(void);
void Find_Route(void);
void Route_Init(void);
void Path_Finding(void);
void Path_Runing(void);
void Path_Circling(void);
void Check_Obstacle(void);
#endif

