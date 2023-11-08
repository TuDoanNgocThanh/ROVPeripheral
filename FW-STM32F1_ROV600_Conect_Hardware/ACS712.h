#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"

#include "IC_MUX.h"
#include "peripheral.h"
#include "variable.h"

void ACS712_getADCvalue();
void ACS712_setADCzero();
void Caculate_ACS712(void);