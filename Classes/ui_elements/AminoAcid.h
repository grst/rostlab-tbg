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
	// ISO C++ forbids in-class initialization of non-static member
    bool toDelete;
    char type;
    long created; //timestamp
public:
    char getType();
    static AminoAcid* createRandom();
    static AminoAcid* create(char type);
    long getCreatedTime();
    void flagForDelete();
    bool isFlaggedForDelete();
};


#endif
