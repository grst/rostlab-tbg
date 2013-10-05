//
//  ContactListener.h
//  HelloWorld
//
//  Created by Gavin Thornton on 11-08-18.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _ContactListener_H_
#define _ContactListener_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Box2D.h"


#define PTM_RATIO 32.0

////////////////////////////////////////////////////////////////////////////////////////////////////
struct ContactData {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const ContactData& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class CContactListener : public b2ContactListener {

public:
    std::vector<ContactData>_contacts;
    
    CContactListener();
    ~CContactListener();
    
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
