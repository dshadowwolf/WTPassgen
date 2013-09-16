#include "dict_plugin.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

dict_plugin::dict_plugin() 
{
   fs::path dictionary_path( "/usr/share/dict/words" );
   fs::path alternate_path( "/usr/share/dict/linux.words" );
   fs::ifstream dict_file();
   
   bool use_alternate = 0;
   
   if( !fs::is_symlink( dictionary_path ) ) {
      std::cerr << "\033[22;32msystem dictionary should be a link\033[0m" << std::endl;
      if( !fs::is_regular_file( dictionary_path ) ) {
	 std::cout << "\033[21;31mDictionary not at " << dictionary_path.string() << "! Checking alternate\033[0m" << std::endl;
	 if( !fs::is_regular_file( alternate_path ) ) {
	    std::cout << "\033[21;31mAlternate dictionary file " << alternate_path.string() << " not found! Aborting Load." << std::endl;
	    abort( "Unable to load dictionary!" );
	 } else {
	    use_alternate = 1;
	 }
      }
   }
   
   if( use_alternate ) {
      dict_file.open( alternate_path );
   } else {
      dict_file.open( dictionary_path );
   }
   
   while( dict_file ) {
      std::string line;
      std::getline( dict_file, line );
      dictionary.push_back( line );
   }
}
   
