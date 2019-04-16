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
  
  /**
   * A SOLID collision detection and correction algorithm
   */
  class CollisionSOLID: public CollisionPrimitive {
  private:
      // "lower-bottom-leftmost" (lowest value corner)
      real x1;
      real y1;
      real z1;
      
      // "upper-top-rightmost" (highest value corner)
      real x2;
      real y2;
      real z2;
  public:
      CollisionSOLID(real x1, real y1, real z1, real x2, real y2, real z2);
      
      real getWorldx1() {
        return this->body->getPosition().x + this->x1;
      }
      
      real getWorldx2() {
        return this->body->getPosition().x + this->x2;
      }
      
      real getWorldy1() {
        return this->body->getPosition().y + this->y1;
      }
      
      real getWorldy2() {
        return this->body->getPosition().y + this->y2;
      }
      
      real getWorldz1() {
        return this->body->getPosition().z + this->z1;
      }
      
      real getWorldz2() {
        return this->body->getPosition().z + this->z2;
      }
      
      static bool testIsColliding(CollisionSOLID& a, CollisionSOLID& b);
      static void correctCollision(CollisionSOLID& a, CollisionSOLID& b, CollisionData& data);
      
  };
  
};

#endif