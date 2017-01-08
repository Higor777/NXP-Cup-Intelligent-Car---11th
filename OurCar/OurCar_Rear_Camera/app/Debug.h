#ifndef __DEBUG_H_
#define	__DEBUG_H_

#include <stdint.h>
#include <stdbool.h>

#define ParameterName_Size 15

typedef struct tagParameterNode_Type
{
	float *exterdata;
	float step;
	char name[ParameterName_Size];
	struct tagParameterNode_Type *prepara;
	struct tagParameterNode_Type *nextpara;
}ParameterNode_Type, *ParameterList_Type;


static void Show_Parameter(ParameterNode_Type *para, uint8_t config);
static void Insert_ParameterNode(ParameterNode_Type *insert_node, ParameterList_Type *insert_place, ParameterNode_Type *blank_node);

void ParameterList_Init(void);
void Data_Init(void);
void Parameter_Change(void);
void Car_Protect(void);

#endif
