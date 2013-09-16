#include <dlfcn.h>
#include <boost/filesystem.hpp>
#include <string>
#include "library.h"

using namespace std;
Library::Library() {
};

Library::~Library() {
   if( handle )
     dlclose( handle );
}

bool Library::load( boost::filesystem::path lib ) {
   handle = dlopen( lib.string().c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND );
   if( !handle )
      return false;
   
   name = string( lib.string() );
   return true;
};

bool Library::load( std::string lib ) {
   handle = dlopen( lib.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND );
   if( !handle )
      return false;
   
   name = string( lib );
   return true;
};

void *Library::findSym( std::string sym ) {
   return dlsym( handle, sym.c_str() );
};

void Library::unload() {
   dlclose( handle );
   name = string("");
};

std::string Library::libname() {
   return string( name );
};

std::string Library::error() 
{
   return string( dlerror() );
};
