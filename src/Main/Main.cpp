#include <iostream>
using namespace std;

#include "../Main/Globals.hpp"
#include "../Debug/console.h"

int main(int argc, char* argv[]) {
  LOG("%s: %s, version %s %s, built: %s...", 
    argv[0],
    Globals::getProjectName().c_str(),
    Globals::getGitVersion().c_str(),
    Globals::getBuildType().c_str(),
    Globals::getBuildDate().c_str()
  );
  DEBUG_A("Debug: ON");

  LOG("Finished.");
  return 0;
}
