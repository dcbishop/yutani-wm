#include <iostream>
using namespace std;

#include "../Main/Globals.hpp"
#include "../Debug/console.h"

int main(int argc, char* argv[]) {
   LOG("Starting %s, version %s, built: %s...", argv[0], g.getGitVersion().c_str(), g.getBuildDate().c_str());

   LOG("Finished.");
   return 0;
}
