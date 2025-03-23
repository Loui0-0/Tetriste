#include <math.h>
#include "mathFunc.h"

float distance(float x1, float y1, float x2, float y2){
    /*distance euclidienne entre 2 point sur le plan*/
	float a = (x2 - x1) * (x2 - x1);
    float b = (y2 - y1) * (y2 - y1);
    return sqrt(a + b);
}