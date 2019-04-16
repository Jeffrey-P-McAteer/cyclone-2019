
#include "cyclone/cyclone.h"
#include "cyclone/collision.h"

using namespace cyclone;

CollisionRegistry::CollisionRegistry():
registry(),
resolver(3)
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
  // We don't know how to compare these two collision primitives
  return false;
}

void CollisionRegistry::CollisionRegistration::generateCorrectiveForceData() {
  if (CollisionBox* a_cb = dynamic_cast<CollisionBox*>(this->a)) {
    if (CollisionBox* b_cb = dynamic_cast<CollisionBox*>(this->b)) {
      CollisionDetector::boxAndBox(*a_cb, *b_cb, this->data);
    }
  }
  if (CollisionSphere* a_cb = dynamic_cast<CollisionSphere*>(this->a)) {
    if (CollisionSphere* b_cb = dynamic_cast<CollisionSphere*>(this->b)) {
      CollisionDetector::sphereAndSphere(*a_cb, *b_cb, this->data);
    }
  }
}

void CollisionRegistry::CollisionRegistration::applyCorrectionForces(real duration, ContactResolver* resolver) {
  if (!this->data->hasMoreContacts()) {
    return; // no contacts, do nothing
  }
  // Resolve a contacts using given resolver
  resolver->resolveContacts(this->data->contactArray, this->data->contactCount, duration);
}


