#ifndef _NEW_CONFIG_H_
#include <list>
#include <queue>
#include <map>
#include <string>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include "key.h"
#include "section.h"

typedef enum option_type_e
{
   TYPE_BOOL = 0,
     TYPE_INT,
     TYPE_STRING,
     TYPE_FLOAT
} option_type_t;
   
typedef struct c_l_arg {
   std::string long_opt;
   const char short_opt;
   bool hasParameter;
   std::string option_keyname;
   std::string option_help;
   option_type_t option_type;
} c_l_arg;

class ConfigData {
 public:
   ConfigData();
   
   bool load( std::string );

   bool load( boost::filesystem::path );
   void addKey( std::string );
   void addKey( std::string, std::string );
   void addKey( std::string, bool );
   void addKey( std::string, int );
   void addKey( std::string, float );
   bool hasKey( std::string );
   void addOption( c_l_arg& );
   void setCommandLine( std::list<std::string> );
   void setCommandLine( int, char ** );
   void parseCommandLine();
   void saveConfig( std::string );
   void saveConfig( boost::filesystem::path );
   void dumpConfig();
   std::string getString(std::string);
   float getFloat(std::string);
   bool getBool(std::string);
   int getInt(std::string);
   std::list<c_l_arg> getOptions();
   
 private:
   std::list<std::string> m_commandline;
   std::list<c_l_arg> m_options;
   // truthfully m_sections is the only one that is ever saved
   // because m_keys is for the top-level keys like "LIST" and "HELP"
   std::map<std::string,ConfigSection> m_sections;
   std::map<std::string,ConfigKey> m_keys;
   bool Parse( std::string );
   bool Parse( boost::filesystem::path );
   void internalParse( std::queue<std::string> );
};

#define _NEW_CONFIG_H_
#endif
