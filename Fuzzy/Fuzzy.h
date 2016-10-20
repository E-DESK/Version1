/*
Program:     fuzzy.c
Written by:  Scott Brown

2 input fuzzy controller to control inverted pendulum system.  Controller has
5 membship functions for each input and 5 membership functions for the output.
Center-of-gravity is used for defuzzification.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUZZY_H_
#define __FUZZY_H_

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "nrf24l01.h"

#define IS_TEST_MODE 1

#define MAX(A,B)  ((A) > (B) ? (A) : (B))
#define MIN(A,B)  ((A) < (B) ? (A) : (B))
#define PI 3.14159265359

#define TEMP_LEFT 10
#define TEMP_RIGHT 40
#define TEMP_CENTER ((TEMP_RIGHT + TEMP_LEFT)/2)
#define TEMP_WITCH ((TEMP_RIGHT-TEMP_LEFT)/2)

#define HUM_LEFT 20
#define HUM_RIGHT 80
#define HUM_CENTER 50;//((HUM_RIGHT + HUM_RIGHT)/2)
#define HUM_WITCH 30;//(HUM_RIGHT-HUM_LEFT)/2

#define PRES_LEFT -200
#define PRES_RIGHT 200 
#define PRES_WIDTH 400;//PRES_RIGHT*2

#define RATE_YEAR_LEFT 0
#define RATE_YEAR_RIGHT 100
#define RATE_YEAR_WIDTH RATE_YEAR_RIGHT

#define RATE_DAY_LEFT 0
#define RATE_DAY_RIGHT 100
#define RATE_DAY_WIDTH RATE_DAY_RIGHT

#define OUT_PUT_WIDTH 0.25

//using namespace std;
/************************************************************************************/

enum OUTPUT5_
{
	RT_5,
	T_5,
	V_5,
	C_5,
	RC_5
};
enum OUTPUT3_
{
	T_3,
	V_3,
	C_3,
};
static int LWF_Fuzzy[72] = {
};


typedef struct in_mem {
	float width;         /* Input membership function width (1/2 of triangle base).  */
	float *center;       /* Center of each input membership function.                */
	float *dom;          /* Degree of membership for each membership function.       */
} IN_MEM;

typedef struct out_mem {
	float width;         /* Output membership function width (1/2 of triangle base). */
	float *center;       /* Center of each output membership function.               */
	float *dom;
} OUT_MEM;


typedef struct fuz_sys_2 {
	IN_MEM  *in_mem1;        /* Groups all fuzzy system parameters in a single variable. */
	IN_MEM  *in_mem2;
	OUT_MEM *out_mem;
} FUZ_SYS_2;

/*============================================================================*/
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCTION PROTOTYPES*/
/*============================================================================*/
/* Memory locate*/
void    fuzzy_init_2    (FUZ_SYS_2 *fuzzy_system_2);
void    fuzzy_free_2      (FUZ_SYS_2 *fuzzy_system_2);
/*Fuzzy control*/
float   fuzzy_control_2 (float vInMem1,float vInMem2,FUZ_SYS_2 *fuzzy_system_2);
/*Defuzzy*/
float   inf_defuzz_2    (IN_MEM *in_mem1, IN_MEM *in_mem2, OUT_MEM *out_mem);
void    findMaxMin_2    (const IN_MEM *in_mem1,const IN_MEM *in_mem2);
/*internal functions*/
int fuzzyify_three(float u, IN_MEM *mem);
int fuzzyify_two(float u, IN_MEM *mem);
float leftall(float u, float w, float c);
float rightall(float u, float w, float c);
float triangle(float u, float w, float c);
/*============================================================================*/
/*/////////////////////////////////////////////////////////FUNCTION PROTOTYPES*/
/*============================================================================*/
#endif /*_FUZZY_H*/
