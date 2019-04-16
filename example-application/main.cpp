
#include <iostream>

// OpenGL
#include "ogl_headers.h"
// Cyclone
#include "cyclone/cyclone.h"

// Holds applicationLauncher()
#include "cycloneutil.h"

// Helper functions to draw complex shapes
static void draw_floor() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(10.0f, 10000.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
}
static void draw_red_box() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Define our own sub-class of Application from cyclone/app.h

class MyApp : public Application {
private:
    cyclone::ForceRegistry force_registry;
    cyclone::CollisionRegistry collision_registry;
    
    cyclone::CollisionBox red_box;
    
    cyclone::CollisionBox floor;
    
    cyclone::Gravity gravity;
    
    float camera_offset_x = 0.0;
    float camera_offset_z = 5.0;
    
public:
    MyApp():
        Application(),
        force_registry(),
        collision_registry(),
        red_box(),
        floor(),
        gravity(cyclone::Vector3(0.0, 0.0, -16.0))
    {
        red_box.body = new cyclone::RigidBody();
        
        red_box.body->setMass(6.0);
        cyclone::Matrix3 it;
        it.setBlockInertiaTensor(cyclone::Vector3(1,1,1), 1);
        red_box.body->setInertiaTensor(it);
        red_box.body->setDamping(0.9f, 0.9f);
        red_box.body->setCanSleep(true); /* not controlled by user */
        
        red_box.body->setPosition(0, 0, 3);
        red_box.body->setOrientation(1, 0, 0, 0);
        red_box.body->setRotation(0, 0, 0);
        red_box.body->setVelocity(0, 0, 0);
        
        floor.body = new cyclone::RigidBody();
        floor.body->setPosition(0, 0, 0);
        floor.body->setOrientation(1, 0, 0, 0);
        floor.body->setRotation(0, 0, 0);
        floor.body->setVelocity(0, 0, 0);
        
        // Setup forces
        force_registry.add(red_box.body, &gravity);
        collision_registry.add(&red_box, &floor);
        
    }
    
    virtual const char* getTitle() {
        return "My Win Title!";
    }
    
    virtual void update() {
        // Find the duration of the last frame in seconds
        float duration = (float) TimingData::get().lastFrameDuration * 0.001f;
        if (duration <= 0.0f) {
            return;
        }
        
        // Start with no forces or acceleration.
        red_box.body->clearAccumulators();
        
        // Add the forces acting on the box
        force_registry.updateForces(duration);
        
        // Update physics acting on red_box.body
        red_box.body->integrate(duration);
        
        // Perform one round of collision detection
        collision_registry.generateCollisionForces(duration);
        
        // Do a very basic collision detection and response with the ground.
        cyclone::Vector3 pos = red_box.body->getPosition();
        std::cout << "pos  = [" << pos.x << ", " << pos.y << ", " << pos.z << "]" << std::endl;
        
        if (pos.z < 0.25f) {
            pos.z = 0.25f;
            red_box.body->setPosition(pos);
        }
        
        Application::update();
        
    }
    
    virtual void display() {
        // Clear the view port and set the camera direction
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        gluLookAt(-10.0, this->camera_offset_x, this->camera_offset_z, // where is the eye?
                  0.0, 0.0, 0.0,  // What is it looking at
                  0.0, 0.0, 1.0); // Z == up direction
        
        // Paint Floor
        glColor3f(0.8f, 0.8f, 0.8f);
        glPushMatrix();
        glTranslatef(0.0, 0.0, 0.0);
        glScalef(1.0f, 0.001f, 1.0f);
        draw_floor();
        glPopMatrix();
        
        // Get position matrix for red_box.body
        cyclone::Matrix4 transform = red_box.body->getTransform();
        GLfloat gl_transform[16];
        transform.fillGLArray(gl_transform);
        glPushMatrix();
        glMultMatrixf(gl_transform);
        cyclone::Vector3 box_pos = red_box.body->getPosition();
        glTranslatef(box_pos.x, box_pos.y, box_pos.z);
        glColor3f(1.0,0.0,0.0);
        draw_red_box();
        glPopMatrix();
        
    }
    
    virtual void mouse(int button, int state, int x, int y) {
        
    }
    
    virtual void key(unsigned char key) {
        if (key == 'a') {
            this->camera_offset_x -= 0.1;
        }
        else if (key == 'd') {
            this->camera_offset_x += 0.1;
        }
        else if (key == 'w') {
            this->camera_offset_z += 0.1;
        }
        else if (key == 's') {
            this->camera_offset_z -= 0.1;
        }
        else if (key == ' ') {
            // Apply a momentary up force
            this->red_box.body->setVelocity(0, 0, 24);
            // Also a minor rotation
            cyclone::Vector3 box_rot = this->red_box.body->getRotation();
            red_box.body->setRotation(box_rot.x + 0.5, box_rot.y, box_rot.z);
        }
    }
    
};

int main() {
    std::cout << "Hello Cyclone!" << std::endl;
    
    Application* my_app = new MyApp();
    applicationLauncher(my_app);
    
    return 0;
}


