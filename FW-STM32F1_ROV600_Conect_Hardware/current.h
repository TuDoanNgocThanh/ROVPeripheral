#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"

#include "peripheral.h"
#include "variable.h"

float ACS712_getADCvalue(void);
float ACS712_mean(void);
void ACS712_Cab(void);