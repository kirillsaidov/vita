#include "vita/math/math.h"

float isqrt(float num) {
    float numHalf = 0.5f*num;
    int i = *(int*)&num;                // get bits for floating value
    
    i = 0x5fe6eb50c7b537a9 - (i >> 1);  // gives initial guess
    num = *(float*)&i;                  // convert bits back to float
    num *= (1.5 - (numHalf*num*num));   // Newton step, repeating increases accuracy
    num *= (1.5 - (numHalf*num*num));   // Newton step, repeating increases accuracy
    num *= (1.5 - (numHalf*num*num));   // Newton step, repeating increases accuracy

    return num;
}





