#include <iostream>
using namespace std;

#include "../Main/Globals.hpp"
#include "../Debug/console.h"
#include "../Backends/XCBBackend.hpp"

int main(int argc, char* argv[]) {
  LOG("%s: %s, version %s %s, built: %s...",
      argv[0],
      Globals::getProjectName().c_str(),
      Globals::getGitVersion().c_str(),
      Globals::getBuildType().c_str(),
      Globals::getBuildDate().c_str());
  DEBUG_A("Debug: ON");

  try {
    XCBBackend backend;
    backend.eventLoop();
  } catch(...) {
    ERROR("Fatal error");
    return 1;
  }

  LOG("Finished.");
  return 0;
}
