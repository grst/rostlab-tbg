//
//  Contact.cpp
//  HelloWorld
//
//  Created by Gavin Thornton on 11-08-18.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ContactListener.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CContactListener::CContactListener(): _contacts() 
{
}

CContactListener::~CContactListener() 
{
}

void CContactListener::BeginContact(b2Contact* contact) 
{
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    ContactData Contact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(Contact);
}

void CContactListener::EndContact(b2Contact* contact) 
{
    ContactData Contact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<ContactData>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), Contact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
}

void CContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) 
{
}

void CContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
{
}