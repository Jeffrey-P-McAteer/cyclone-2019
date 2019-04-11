
#include <iostream>

// OpenGL
#include "ogl_headers.h"
// Cyclone
#include "cyclone/cyclone.h"

// Holds applicationLauncher()
#include "cycloneutil.h"

// Define our own sub-class of Application from cyclone/app.h

class MyApp : public Application {
public:
    MyApp() {
        
    }
    
    virtual const char* getTitle() {
        return "My Win Title!";
    }
    
    virtual void update() {
        std::cout << "Update called" << std::endl;
    }
    
    virtual void display() {
        
    }
    
    virtual void mouse(int button, int state, int x, int y) {
        
    }
    
    virtual void key(unsigned char key) {
        
    }
    
    
};

int main() {
    std::cout << "Hello Cyclone!" << std::endl;
    
    Application* my_app = new MyApp();
    applicationLauncher(my_app);
    
    return 0;
}


