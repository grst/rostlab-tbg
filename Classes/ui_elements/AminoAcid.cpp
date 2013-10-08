//
//  AminoAcid.m
//  tbg
//
//  Created by rootmac on 9/8/13.
//
//

#include "AminoAcid.h"
#include "MatrixHelper.h"
  
USING_NS_CC;


int AminoAcid::getDirection() {
    return this->direction;
}
char AminoAcid::getType() {
    return this->type;
}
//d is an angle between 0 and 360 degree
void AminoAcid::setDirection(int d){
    this->direction = HelperFunctions::mod(d, 360);
}
//type is a valid one-letter IUPAC AminoAcid code
void AminoAcid::setType(char t){
    this->type = t;
}

/**
 * @return Score according to Scoring-Matrix (e.g. BLOSUM62)
 */
int AminoAcid::getScore(AminoAcid* other){
    //TODO apply Matrix score
    return 1;
}

AminoAcid* AminoAcid::create(char type){
    AminoAcid *pobSprite = new AminoAcid();
    if (pobSprite && pobSprite->initWithFile(
                                        MatrixHelper::getImagePathForAcid(type),
                                        CCRectMake(0, 0, 50, 50)))
    {
        pobSprite->setType(type);
        pobSprite->setTag(1);
        pobSprite->setZOrder(3);
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
}

