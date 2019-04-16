#ifndef COLLISION_H
#define COLLISION_H

namespace cyclone {
  
  /**
   * An very simple collision registry. 
   */
  class CollisionRegistry {
  public:
    class CollisionRegistration {
    public:
      CollisionPrimitive *a;
      CollisionPrimitive *b;
      // Defined cyclone-headers/cyclone/collide_fine.h:183:
      CollisionData *data;
      CollisionRegistration(CollisionPrimitive *a, CollisionPrimitive *b);
      bool isColliding();
      void generateCorrectiveForceData();
      void applyCorrectionForces(real duration, ContactResolver* resolver);
    };
    
    typedef std::vector<CollisionRegistration> Registry;
    
    Registry registry;
    
    ContactResolver resolver;
    
    CollisionRegistry();
    
    // Add two objects which will be compared when generateCollisionForces() is called.
    void add(CollisionPrimitive *a, CollisionPrimitive *b);
    
    // Generates forces on CollisionPrimitive.body which will correct for the collision
    void generateCollisionForces(real duration);
    
  };
  
};

#endif