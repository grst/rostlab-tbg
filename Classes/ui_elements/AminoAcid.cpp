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


char AminoAcid::getType() {
    return this->type;
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
        pobSprite->setTag(1);
        pobSprite->setZOrder(3);
        pobSprite->autorelease();
        
        return pobSprite;
    } else {
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }    
}

/**
 * flag the target to be deleted.
 */
void AminoAcid::flagForDelete() {
    this->toDelete = true;
}

bool AminoAcid::isFlaggedForDelete() {
    return this->toDelete;
}
