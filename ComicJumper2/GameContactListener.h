//
//  GameContactListener.h
//  ComicCloudJumer
//
//  Created by Bai Ryan on 13-6-2.
//
//

#ifndef __ComicCloudJumer__GameContactListener__
#define __ComicCloudJumer__GameContactListener__

#include "Box2D.h"
#include <vector.h>
#include <algorithm>

struct GameContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const GameContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};



class GameContactListener : public b2ContactListener
{
public:
    
    std::vector<GameContact>_contacts;
    GameContactListener();
    ~GameContactListener();

    
    virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    

};


#endif /* defined(__ComicCloudJumer__GameContactListener__) */
