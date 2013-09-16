#include <string>
#include <iostream>
#include <list>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include "loader.h"

#define foreach( a, b, c )\
   for( a::iterator c = b.begin(); c != b.end(); c++ ) 

Loader::Loader() {
   isInitialized = true;
}

Loader::~Loader() {
   if( !isInitialized )
     return;
   
   if( !m_plugins.size() )
     return;
   
   foreach( std::list<plugin_t *>, m_plugins, x ) {
      plugin_t *t = *x;
      t->handle.unload();
   }
   std::list<plugin_t *>t = m_plugins;
   foreach( std::list<plugin_t *>, t, x ) {
      m_plugins.remove( (*x) );
   }
}
   
std::list<void *> Loader::plugins() {
   std::list<void *> a;
   foreach( std::list<plugin_t *>, m_plugins, x ) {
      a.push_back( (*x)->instance );
   }   
   return a;
}

bool Loader::load( boost::filesystem::path &fileName ) {
   typedef void *(*vp_f)();
   plugin_t *p = new plugin_t;
   ;
   if( !p->handle.load( fileName ) ) {
      std::cerr << "Unable to load " << fileName << " ( " << p->handle.error() << " )" << std::endl;
      delete p;
      return false;
   }
   vp_f temp;
   temp = (vp_f) p->handle.findSym( "init_plugin" );
   if( !temp ) {
      std::cerr << fileName << " is not a plugin (?) : " << p->handle.error() << std::endl;
      p->handle.unload();
      delete p;
      return false;
   }
   p->instance = temp();
   p->filename = fileName.string();
   m_plugins.push_back( p );
   return true;
}
   
bool Loader::load( std::string fileName ) {
   typedef void *(*vp_f)();
   plugin_t *p = new plugin_t;
   if( !p->handle.load( fileName ) ) {
      std::cerr << "Unable to load " << fileName << " ( " << p->handle.error() << " )" << std::endl;
      delete p;
      return false;
   }

   vp_f temp;
   temp = (vp_f) p->handle.findSym( std::string( "init_plugin" ) );
   if( !temp ) {
      std::cerr << fileName << " is not a plugin (?) : " << p->handle.error() << std::endl;
      p->handle.unload();
      delete p;
      return false;
   }
   p->instance = temp();
   p->filename = fileName;
   m_plugins.push_back( p );
   return true;
}

bool Loader::unload( std::string fileName) {
   foreach( std::list<plugin_t *>, m_plugins, x ) {
      if( (*x)->filename == fileName ) {
	 m_plugins.remove((*x));
	 (*x)->instance = NULL;
	 (*x)->handle.unload();
	 return true;
      }
   }
   std::cerr << "Plugin " << fileName << " not found in list - was it loaded?" << std::endl;
   return false;
}

void Loader::loadDir( std::string direct ) {
   std::string fN = direct;
   boost::filesystem::path full_path( boost::filesystem::system_complete( boost::filesystem::path( direct ) ) );
   
   if( !boost::filesystem::exists( full_path ) ) {
      std::cout << direct << " (" << full_path.filename().string() << ") does not exist" << std::endl;
      abort();
   }
   
   if( !boost::filesystem::is_directory( full_path ) ) {
      std::cout << direct << " (" << full_path.string() << ") is not a directory" << std::endl;
      abort();
   }
   boost::filesystem::directory_iterator end_iter;
   for( boost::filesystem::directory_iterator dir_itr( full_path ); dir_itr != end_iter; ++dir_itr ) {
      if( boost::filesystem::is_regular( dir_itr->status() ) ) {
	 boost::regex re( "^.*\\.so$" );
	 if( boost::regex_match( dir_itr->path().leaf().string(), re ) ) {
	    boost::filesystem::path p(dir_itr->path());
	    load( p.normalize() );
	 }
      }
   }
}

void Loader::unloadDir( std::string direct ) {
   foreach( std::list<plugin_t *>, m_plugins, x ) {
      if( (*x)->filename.substr( 0, direct.length() ) == direct )
	unload( (*x)->filename );
   }
}
