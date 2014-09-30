////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
// (C) Juanmi Huertas Delgado 2014-2015
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Text overlay
//

// enable Erwin's marvelous Bullet Physics.
#define OCTET_BULLET 1

#include "../../octet.h"

#include "PanchitoCannon.h"

/// Create a box with octet
int main(int argc, char **argv) {
  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::PanchitoCannon app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}


