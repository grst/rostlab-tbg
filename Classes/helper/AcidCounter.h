//
//  AcidCounter.h
//  TBG
//
//  Created by rootmac on 12/20/13.
//
//

#ifndef __TBG__AcidCounter__
#define __TBG__AcidCounter__

#include <iostream>
#include "cocos2d.h"

class AcidCounter {
private:
    int counter [20];
public:
    void incCounter(char c);
    void decCounter(char c);
    bool hasAcid(char c);
    void incCounter(int i);
    void decCounter(int i);
    bool hasAcid(int i);
};

#endif /* defined(__TBG__AcidCounter__) */

