//
//  AcidCounter.cpp
//  TBG
//
//  Created by rootmac on 12/20/13.
//
//

#include "AcidCounter.h"
#include "MatrixHelper.h"

USING_NS_CC;

void AcidCounter::decCounter(char c) {
    decCounter(MatrixHelper::getAcidInt(c));
}

void AcidCounter::incCounter(char c) {
    incCounter(MatrixHelper::getAcidInt(c));
}

bool AcidCounter::hasAcid(char c) {
    return hasAcid(MatrixHelper::getAcidInt(c));
}

void AcidCounter::decCounter(int i) {
    if (i < 0 || i >= 20) {
        return;
    }
    counter[i]--;
}

void AcidCounter::incCounter(int i) {
    if (i < 0 || i >= 20) {
        return;
    }
    counter[i]++;
}

bool AcidCounter::hasAcid(int i) {
    if (i < 0 || i >= 20) {
        return false;
    }
    return (counter[i] > 0);

}