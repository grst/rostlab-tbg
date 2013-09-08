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

class AminoAcid: public cocos2d::CCSprite {
private:
    //default value: negative
	int direction = -1;
    char type;
public:
    void setDirection(int d);
    int getDirection();
    void setType(char t);
    char getType();
    static AminoAcid* create(char type);
};


#endif