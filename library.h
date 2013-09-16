#ifndef _LIBRARY_H_
#include <dlfcn.h>
#include <boost/filesystem.hpp>
#include <string>

class Library
{
 public:
   Library();
   ~Library();
   
   bool load( boost::filesystem::path );
   bool load( std::string );
   void *findSym( std::string );
   void unload();
   std::string libname();
   std::string error();
   
 private:
   void *handle;
   std::string name;
};

#define _LIBRARY_H_
#endif
