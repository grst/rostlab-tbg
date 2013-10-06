//
//  HelperFunctions.cpp
//  tbg
//
//  Created by rootmac on 9/8/13.
//
//

#include "HelperFunctions.h"

//real modulo with positive result
int HelperFunctions::mod(int a, int b) {
    return ((a%b) + b) %b;
}

float HelperFunctions::randomValueBetween(float low, float high) {
    return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}
