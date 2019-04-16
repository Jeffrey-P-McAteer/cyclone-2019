
#include <iostream>

#include "cyclone/cyclone.h"
#include "cyclone/collision.h"

using namespace cyclone;

CollisionRegistry::CollisionRegistry():
registry(),
resolver(10)
{
  
}

void CollisionRegistry::add(CollisionPrimitive* a, CollisionPrimitive* b) {
  CollisionRegistration registration(a,b);
  this->registry.push_back(registration);
}

void CollisionRegistry::generateCollisionForces(real duration) {
  Registry::iterator i = registry.begin();
  for (; i != registry.end(); i++) {
    if (i->isColliding()) {
      std::cout << "\nCollision detected!\n" << std::endl;
      i->generateCorrectiveForceData();
      i->applyCorrectionForces(duration, &resolver);
    }
  }
}

CollisionRegistry::CollisionRegistration::CollisionRegistration(CollisionPrimitive* a, CollisionPrimitive* b) {
  this->a = a;
  this->b = b;
  this->data = new CollisionData();
}

bool CollisionRegistry::CollisionRegistration::isColliding() {
  if (CollisionBox* a_cb = dynamic_cast<CollisionBox*>(this->a)) {
    if (CollisionBox* b_cb = dynamic_cast<CollisionBox*>(this->b)) {
      return IntersectionTests::boxAndBox(*a_cb, *b_cb);
    }
  }
  if (CollisionSphere* a_cb = dynamic_cast<CollisionSphere*>(this->a)) {
    if (CollisionSphere* b_cb = dynamic_cast<CollisionSphere*>(this->b)) {
      return IntersectionTests::sphereAndSphere(*a_cb, *b_cb);
    }
  }
  if (CollisionSOLID* a_cb = dynamic_cast<CollisionSOLID*>(this->a)) {
    if (CollisionSOLID* b_cb = dynamic_cast<CollisionSOLID*>(this->b)) {
      return CollisionSOLID::testIsColliding(*a_cb, *b_cb);
    }
  }
  std::cerr << "We don't know how to compare these two collision primitives!" << std::endl;
  return false;
}

void CollisionRegistry::CollisionRegistration::generateCorrectiveForceData() {
  if (CollisionBox* a_cb = dynamic_cast<CollisionBox*>(this->a)) {
    if (CollisionBox* b_cb = dynamic_cast<CollisionBox*>(this->b)) {
      CollisionDetector::boxAndBox(*a_cb, *b_cb, this->data);
      return;
    }
  }
  if (CollisionSphere* a_cb = dynamic_cast<CollisionSphere*>(this->a)) {
    if (CollisionSphere* b_cb = dynamic_cast<CollisionSphere*>(this->b)) {
      CollisionDetector::sphereAndSphere(*a_cb, *b_cb, this->data);
      return;
    }
  }
  if (CollisionSOLID* a_cb = dynamic_cast<CollisionSOLID*>(this->a)) {
    if (CollisionSOLID* b_cb = dynamic_cast<CollisionSOLID*>(this->b)) {
      CollisionSOLID::correctCollision(*a_cb, *b_cb, *this->data);
      return;
    }
  }
  std::cerr << "We don't know how to correct these two collision primitives!" << std::endl;
}

void CollisionRegistry::CollisionRegistration::applyCorrectionForces(real duration, ContactResolver* resolver) {
  if (!this->data->hasMoreContacts()) {
    return; // no contacts, do nothing
  }
  // Resolve a contacts using given resolver
  resolver->resolveContacts(this->data->contactArray, this->data->contactCount, duration);
}

CollisionSOLID::CollisionSOLID(real x1, real y1, real z1, real x2, real y2, real z2):
  x1(x1),
  y1(y1),
  z1(z1),
  x2(x2),
  y2(y2),
  z2(z2)
{
  
}

bool is_colliding_axis(real a1, real a2, real b1, real b2) {
  return (a2 > b1 && a2 < b2) || (b2 > a1 && b2 < a2);
}

bool CollisionSOLID::testIsColliding(CollisionSOLID& a, CollisionSOLID& b) {
  Vector3 apos = a.body->getPosition();
  Vector3 bpos = b.body->getPosition();
  return is_colliding_axis(apos.x + a.x1, apos.x + a.x2,   bpos.x + b.x1, bpos.x + b.x2) &&
         is_colliding_axis(apos.y + a.y1, apos.y + a.y2,   bpos.y + b.y1, bpos.y + b.y2) &&
         is_colliding_axis(apos.z + a.z1, apos.z + a.z2,   bpos.z + b.z1, bpos.z + b.z2);
}

// See cyclone-headers/cyclone/collide_fine.h:184:
void CollisionSOLID::correctCollision(CollisionSOLID& a, CollisionSOLID& b, CollisionData& data) {
  Vector3 apos;
  a.body->getPosition(&apos);
  Vector3 bpos;
  b.body->getPosition(&bpos);
  
  // A has fallen on B from the top;
  if (a.getWorldz1() < b.getWorldz2()) {
    apos.z = b.getWorldz2() - a.z1;
  }
  
  
  a.body->setPosition(apos.x, apos.y, apos.z);
  b.body->setPosition(bpos.x, bpos.y, bpos.z);
  
}
