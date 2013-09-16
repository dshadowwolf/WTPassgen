#ifndef _CONFIG_SECTION_H_
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "key.h"

class ConfigSection {
 public:
   ConfigSection();
   
   bool hasKey( std::string );
   void addKey( std::string);
   void addKey( std::string, std::string );
   void addKey( std::string, bool );
   void addKey( std::string, int );
   void addKey( std::string, float );
   std::string getString( std::string );
   float getFloat( std::string );
   bool getBool( std::string );
   int getInt( std::string );
   void dump();
   void save( std::fstream & );
 private:
   std::map<std::string,ConfigKey> m_keys;
   std::map<std::string,ConfigSection> m_sections;
   std::string m_name;
};

#define _CONFIG_SECTION_H_
#endif
