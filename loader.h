#ifndef _LOADER_H_
#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include "library.h"

typedef struct plugin_t {
   void *instance;
   Library handle;
   std::string filename;
} plugin_t;

class Loader {
 public:
   Loader();
   ~Loader();
   
   std::list<void *> plugins();
   bool load( std::string );
   bool load( boost::filesystem::path &);
   bool unload( std::string );
   void loadDir( std::string );
   void unloadDir( std::string );
 private:
   bool isInitialized;
   std::list<plugin_t *> m_plugins;
};

#define _LOADER_H_
#endif

	
