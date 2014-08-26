//
//  GameContactListener.cpp
//  ComicCloudJumer
//
//  Created by Bai Ryan on 13-6-2.
//
//

#include "GameContactListener.h"


GameContactListener::GameContactListener() : _contacts()
{
    
}

GameContactListener::~GameContactListener()
{

}


void GameContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    GameContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(myContact);
    
}

void GameContactListener::EndContact(b2Contact* contact) {
    GameContact Contact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<GameContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), Contact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }}

void GameContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}

void GameContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}

