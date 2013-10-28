//
//  AminoAcid.h
//  tbg
//
//  Created by Gregor Sturm on 9/8/13.
//
//
#ifndef AMINOACID_H_
#define AMINOACID_H_
#include "cocos2d.h"
#include "../helper/HelperFunctions.h"

class AminoAcid: public cocos2d::CCSprite {
private:
    bool toDelete = false;
    char type;
public:
    char getType();
    static AminoAcid* create();
    static AminoAcid* create(char type);
    void flagForDelete();
    bool isFlaggedForDelete();
};


#endif
