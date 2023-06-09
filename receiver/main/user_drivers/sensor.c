#include "./include/sensor.h"

/*
Function:sensor_by_slope
Return:
    1 : sensoring a motion change
    0 : no motion change
*/
int sensor_by_slope(int last_vlt, int vlt)
{
    int slope;
    slope = vlt - last_vlt;
    //square wave slope only can be 4095, 0, -4095
    if(!(slope == 0 || slope == 4095 || slope == -4095))
    {
        //communication quality is abnormal 
        return 1;
    }
    return 0;
}