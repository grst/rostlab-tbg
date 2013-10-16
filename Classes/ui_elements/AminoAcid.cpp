//
//  AminoAcid.m
//  tbg
//
//  Created by rootmac on 9/8/13.
//
//

#include "AminoAcid.h"
#include "../helper/MatrixHelper.h"
  
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
////type is a valid one-letter IUPAC AminoAcid code
//void AminoAcid::setType(char t){
//    this->type = t;
//    //TODO: also change sprite image
//}

/**
 * @return Score according to Scoring-Matrix (e.g. BLOSUM62)
 */
int AminoAcid::getScore(AminoAcid* other){
    //TODO apply Matrix score
    return 1;
}

/**
 * creates an AminoAcidSprite of a random AminoAcidType
 */
AminoAcid* AminoAcid::create() {
    return AminoAcid::create(MatrixHelper::getRandomAminoAcid());
}

/**
 * creates an AminoAcidSprite of a specific AminoAcidType
 * @param type valid IUPAC one-letter-code
 */
AminoAcid* AminoAcid::create(char type){
    AminoAcid *pobSprite = new AminoAcid();
    if (pobSprite && pobSprite->initWithFile(MatrixHelper::getImagePathForAcid(type))) {
        pobSprite->type = type;
        pobSprite->direction = -1; //negative default value.
        pobSprite->setTag(1);
        pobSprite->setZOrder(3);
        pobSprite->autorelease();
        
        return pobSprite;
    } else {
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }    
}

