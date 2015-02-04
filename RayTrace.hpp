/* ----------------------------------------------------------------
   name:           RayTrace.hpp
   version:	   
   TODO:           
   author:         
   ------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>
#include "Ray.h"

#include "glm/glm.hpp"

namespace Common{

  void loadShaders();
  // common keyboard callback
  void keyPressed(unsigned char key, int x, int y);
};

void createPrimaryRays(float windowX, float windowY, float samplingRate);

namespace RayTrace{
  
  extern int menuOptions[];
  extern std::string menuText[];
  extern glm::vec2 previousMouse; // previous mouse position

  extern std::vector<Ray> rays;

  void display(void);
  void reshape(int width, int height);
  void mousePressed(int button, int state, int x, int y);
  void mouseDragged(int x, int y);
  void mouseMoved(int x, int y);
  void menu(int value);
};

namespace World{

  extern int menuOptions[];
  extern std::string menuText[];
  extern int numOptions;
  extern glm::vec2 previousMouse; // previous mouse position
  
  void display(void);
  void reshape(int width, int height);
  void mousePressed(int button, int state, int x, int y);
  void mouseDragged(int x, int y);
  void menu(int value);

  void setLighting();
  void setMaterial();
};