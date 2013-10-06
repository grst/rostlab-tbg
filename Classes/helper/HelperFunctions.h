//
//  HelperFunctions.h
//  tbg
//
//  Created by rootmac on 9/8/13.
//
//

#ifndef __tbg__HelperFunctions__
#define __tbg__HelperFunctions__

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include "math.h";

class HelperFunctions {
public:
    static int mod(int a, int b);
    static float randomValueBetween(float low, float high);
    static double radian(double degrees);
    static std::vector<std::string> split(const std::string &s, char delim);
};
#endif /* defined(__tbg__HelperFunctions__) */
