#include "stm32f10x.h"
#include <stdio.h>
#include "math.h"
#include "string.h"
#include "stdlib.h"

#include "variable.h"
#include "peripheral.h"

void Config_Gripper(void);
void Config_PWMGripper(int duty1,int duty2);
void Caculate_IGripper(void);
void Receive_Gripper(void);
void	Gripper(void);
