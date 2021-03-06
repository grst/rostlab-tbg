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
AminoAcid* AminoAcid::createRandom() {
    return AminoAcid::create(MatrixHelper::getRandomAminoAcid());
}

/**
 * creates an AminoAcidSprite of a specific AminoAcidType
 * @param type valid IUPAC one-letter-code
 */
AminoAcid* AminoAcid::create(char type){
    AminoAcid *pobSprite = new AminoAcid();
    pobSprite->created = HelperFunctions::nowInMilliSeconds();
    const char *path = MatrixHelper::getImagePathForAcid(type);
    CCLog("Create AA: %s", path);
    if (pobSprite && pobSprite->initWithFile(path)) {
        pobSprite->type = type;
        pobSprite->setTag(1);
        pobSprite->setZOrder(3);
        pobSprite->autorelease();
        
        return pobSprite;
    } else {
    	CCLog("[Error]: no file found for AA %s", path);
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

long AminoAcid::getCreatedTime() {
    return this->created;
}
