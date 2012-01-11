#include <iostream>
using namespace std;

#include "../Main/Globals.hpp"
#include "../Debug/console.h"

int main(int argc, char* argv[]) {
   LOG("Starting %s, version %s %s, built: %s...", 
   	argv[0],
   	globals.getGitVersion().c_str(),
   	globals.getBuildType().c_str(),
   	globals.getBuildDate().c_str()
   );

   DEBUG_A("Debug: ON");

   LOG("Finished.");
   return 0;
}
