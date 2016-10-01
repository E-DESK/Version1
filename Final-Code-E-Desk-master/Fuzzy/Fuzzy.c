#include "Fuzzy.h"

float percentMonth(float month) {
  if ((month >= 2) && (month <= 8)) {
    return 100 - ((month - 2) / (8 - 2)) * 100;
  } else {
    if (month <= 2) {
      month = month + 12;
    }
    return ((month - 8) / (14 - 8)) * 100;
  }
}

float percentDay(float hour) {
  if ((hour >= 0) && (hour <= 12)) {
    // return rightall(hour, 100, 100);
    return (((hour - 0) / (12 - 0)) * 100);
  } else {
    // return leftall(hour, 100, 0);
    return (100 - ((hour - 12) / (12 - 0)) * 100);
  }
}

float MIN5(float num1, float num2, float num3, float num4, float num5) {
  return MIN(num1, MIN(num2, MIN(num3, MIN(num4, num5))));
}

/************************************************************************************/
float MAX5(float num1, float num2, float num3, float num4, float num5) {
  return MAX(num1, MAX(num2, MAX(num3, MAX(num4, num5))));
}


/************************************************************************************/
void fuzzy_init_lwf(FUZ_SYS_LWF *fuzzy_system_lwf) {
  /* Define the input and output membership functions. */
  int i = 0;
  int sizeof_IN_MEM = sizeof(IN_MEM);
  int sizeof_OUT_MEM = sizeof(OUT_MEM);
  /* Allocate memory for membership functions. */
  if (!(fuzzy_system_lwf->tem_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->hum_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->presdown_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->rateYear_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->rateDay_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }

  if (!(fuzzy_system_lwf->out_mem = (OUT_MEM *)malloc(sizeof(OUT_MEM)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }

  if (!(fuzzy_system_lwf->tem_mem->center =
            (float *)malloc(3 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->tem_mem->dom = (float *)malloc(3 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->hum_mem->center =
            (float *)malloc(3 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->hum_mem->dom = (float *)malloc(3 * sizeof(float)))) {
    printf("Error allocating memory.\n");
  }
  if (!(fuzzy_system_lwf->presdown_mem->center =
            (float *)malloc(2 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->presdown_mem->dom =
            (float *)malloc(2 * sizeof(float)))) {
    printf("Error allocating memory.\n");
  }

  if (!(fuzzy_system_lwf->rateYear_mem->center =
            (float *)malloc(2 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->rateYear_mem->dom =
            (float *)malloc(2 * sizeof(float)))) {
    printf("Error allocating memory.\n");
  }
  if (!(fuzzy_system_lwf->rateDay_mem->center =
            (float *)malloc(2 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->rateDay_mem->dom =
            (float *)malloc(2 * sizeof(float)))) {
    printf("Error allocating memory.\n");
  }

  if (!(fuzzy_system_lwf->out_mem->center =
            (float *)malloc(5 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // exit(1);
  }
  if (!(fuzzy_system_lwf->out_mem->dom = (float *)malloc(5 * sizeof(float)))) {
    printf("Error allocating memory.\n");
    // TM_DISCO_LedToggle(LED_RED);
    i = 1;
  }

  /* Initialize for local weather forecast. */
  fuzzy_system_lwf->tem_mem->width = TEMP_WITCH;
  fuzzy_system_lwf->hum_mem->width = HUM_WITCH;
  fuzzy_system_lwf->presdown_mem->width = PRES_WIDTH;
  fuzzy_system_lwf->rateYear_mem->width = RATE_YEAR_WIDTH;
  fuzzy_system_lwf->rateDay_mem->width = RATE_DAY_WIDTH;
  fuzzy_system_lwf->out_mem->width = OUT_PUT_WIDTH;

  fuzzy_system_lwf->tem_mem->center[0] = TEMP_LEFT;
  fuzzy_system_lwf->tem_mem->center[1] = TEMP_CENTER;
  fuzzy_system_lwf->tem_mem->center[2] = TEMP_RIGHT;

  fuzzy_system_lwf->hum_mem->center[0] = HUM_LEFT;
  fuzzy_system_lwf->hum_mem->center[1] = HUM_CENTER;
  fuzzy_system_lwf->hum_mem->center[2] = HUM_RIGHT;

  fuzzy_system_lwf->presdown_mem->center[0] = PRES_LEFT;
  fuzzy_system_lwf->presdown_mem->center[1] = PRES_RIGHT;

  fuzzy_system_lwf->rateYear_mem->center[0] = RATE_YEAR_LEFT;
  fuzzy_system_lwf->rateYear_mem->center[1] = RATE_YEAR_RIGHT;

  fuzzy_system_lwf->rateDay_mem->center[0] = RATE_DAY_LEFT;
  fuzzy_system_lwf->rateDay_mem->center[1] = RATE_DAY_RIGHT;
  for (i = 0; i < 5; i++)
  {
    fuzzy_system_lwf->out_mem->center[i] = i * OUT_PUT_WIDTH;
    fuzzy_system_lwf->out_mem->dom[i] = 0;
    //		std::cout << "outmem center :" <<
    //fuzzy_system_lwf->out_mem->center[i]<<std::endl;
  }
}

/************************************************************************************/

void fuzzy_free(FUZ_SYS_LWF *fuzzy_system_lwf) {
  /* Free memory allocated in fuzzy_init(). */
  free(fuzzy_system_lwf->tem_mem->center);
  free(fuzzy_system_lwf->tem_mem->dom);

  free(fuzzy_system_lwf->hum_mem->center);
  free(fuzzy_system_lwf->hum_mem->dom);

  free(fuzzy_system_lwf->presdown_mem->center);
  free(fuzzy_system_lwf->presdown_mem->dom);

  free(fuzzy_system_lwf->rateYear_mem->center);
  free(fuzzy_system_lwf->rateYear_mem->dom);

  free(fuzzy_system_lwf->rateDay_mem->center);
  free(fuzzy_system_lwf->rateDay_mem->dom);

  free(fuzzy_system_lwf->out_mem->center);
  free(fuzzy_system_lwf->out_mem->dom);

  free(fuzzy_system_lwf->tem_mem);
  free(fuzzy_system_lwf->hum_mem);
  free(fuzzy_system_lwf->presdown_mem);
  free(fuzzy_system_lwf->rateYear_mem);
  free(fuzzy_system_lwf->rateDay_mem);
  free(fuzzy_system_lwf->out_mem);
}

/************************************************************************************/
float fuzzy_control_lwf(float tem, float hum, float presDown, float rateYear,
                        float rateDay, FUZ_SYS_LWF *fuzzy_system_lwf) {

  /* Given crisp inputs e and edot, determine the crisp output u. */

  // int pos[5];

  fuzzyify_three(tem, fuzzy_system_lwf->tem_mem);
  fuzzyify_three(hum, fuzzy_system_lwf->hum_mem);
  fuzzyify_two(presDown, fuzzy_system_lwf->presdown_mem);
  fuzzyify_two(rateYear, fuzzy_system_lwf->rateYear_mem);
  fuzzyify_two(rateDay, fuzzy_system_lwf->rateDay_mem);

  return inf_defuzz_lwf2(
      fuzzy_system_lwf->tem_mem, fuzzy_system_lwf->hum_mem,
      fuzzy_system_lwf->presdown_mem, fuzzy_system_lwf->rateYear_mem,
      fuzzy_system_lwf->rateDay_mem, fuzzy_system_lwf->out_mem);
}

/************************************************************************************/
int fuzzyify_three(float u, IN_MEM *mem) {

  /* Assumes 3 membership functions, with first and last membership
  functions leftall and rightall respectively.  Center membership functions are
  triangular. */
  mem->dom[0] = leftall(u, mem->width, mem->center[0]);
  mem->dom[1] = triangle(u, mem->width, mem->center[1]);
  mem->dom[2] = rightall(u, mem->width, mem->center[2]);
  return 1;
}

/************************************************************************************/
int fuzzyify_two(float u, IN_MEM *mem) {

  /* Assumes 2 membership functions, leftall and rightall respectively. */
  mem->dom[0] = leftall(u, mem->width, mem->center[0]);
  mem->dom[1] = rightall(u, mem->width, mem->center[1]);
  return 1;
}

/************************************************************************************/
float leftall(float u, float w, float c)
/* Determine degree of membership for a leftall membership function.
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
  if (u < c)
    return 1.0;
  else
    return MAX(0, (1 - (u - c) / w));
}

/************************************************************************************/

float rightall(float u, float w, float c)
/* Determine degree of membership for a RIGHTALL membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
  if (u >= c)
    return 1.0;
  else
    return MAX(0, (1 - (c - u) / w));
}

/************************************************************************************/

float triangle(float u, float w, float c)

/* Determine degree of membership for a TRIANGLE membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
  if (u >= c)
    return MAX(0, (1 - (u - c) / w));
  else
    return MAX(0, (1 - (c - u) / w));
}

/*defuzzi using Sugeno*/

void findMaxMin(
	const IN_MEM *tem_mem,
	const IN_MEM *hum_mem,
	const IN_MEM *presDown_mem,
	const IN_MEM *rateYear_mem,
	const IN_MEM *rateDay_mem,
	OUT_MEM *out_mem)
{
	float tmp = 0;
	int i=0,j=0,k=0,m=0,n=0;
	int index = 0;// chay tu 0->80 
	float outDOM[5] = { 0, 0, 0, 0, 0 };
	int uFuzzy[72];
	for (i = 0; i < 72; i++)
	{
		uFuzzy[i] = LWF_Fuzzy[i];
	}
	//             
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			for (m = 0; m < 2; m++)
				for (n = 0; n < 2; n++)
					for (k = 0; k < 2; k++)
					{
						if ((tem_mem->dom[i] != 0.0)
							&& (hum_mem->dom[j] != 0.0)
							&& (presDown_mem->dom[m] != 0.0)
							&& (rateYear_mem->dom[n] != 0.0)
							&& (rateDay_mem->dom[k] != 0.0))
						{

							tmp = MIN5(tem_mem->dom[i],
								hum_mem->dom[j],
								presDown_mem->dom[m],
								rateYear_mem->dom[n],
								rateDay_mem->dom[k]);
							
							outDOM[uFuzzy[index]] = MAX(outDOM[uFuzzy[index]], tmp);
							//std::cout << "DOM::::::::::" << tmp << std::endl;
						}
						index++;
					}

	}// End loop for  

	for (i = 0; i<5; i++) {
		out_mem->dom[i] = outDOM[i];
	}
}

float inf_defuzz_lwf2(IN_MEM *tem_mem, IN_MEM *hum_mem, IN_MEM *presDown_mem,
                      IN_MEM *rateYear_mem, IN_MEM *rateDay_mem,
                      OUT_MEM *out_mem) {
  float WAtot = 0, Atot = 0;
  int i = 0;
  findMaxMin(tem_mem, hum_mem, presDown_mem, rateYear_mem, rateDay_mem,
             out_mem);

  // Processing defuzzification
  for (i = 0; i < 5; i++) {
    WAtot += out_mem->center[i] * out_mem->dom[i];
    Atot += out_mem->dom[i];
    out_mem->dom[i] = 0;
  }
  
  if(Atot == 0)
  {
      return 1;
  }
  else
  {
    return WAtot / Atot;
  }
}
