#include <queue>
#include <list>
#include <map>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <getopt.h>
#include "key.h"
#include "section.h"
#include "config.h"

using namespace std;
using namespace boost::filesystem;

// Constructor :)
ConfigData::ConfigData() {};


bool ConfigData::load( std::string filename ) {
   return Parse( filename );
}

bool ConfigData::load( boost::filesystem::path filename ) {
   return Parse( filename );
}

void ConfigData::addKey( std::string name ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, true );
      else {
	 ConfigSection d;
	 d.addKey( c, true );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(true);
}
   
void ConfigData::addKey( std::string name, std::string val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}
	
void ConfigData::addKey( std::string name, bool val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}
   
void ConfigData::addKey( std::string name, int val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}

bool ConfigData::hasKey( std::string name ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( m_sections.count(b) )
	return m_sections[b].hasKey( c );
      else
	return false;
   } else
     return !!m_keys.count(name);
}
   
void ConfigData::addOption( c_l_arg& arg ) {
   m_options.push_back(arg);
}

void ConfigData::setCommandLine( std::list<std::string> d_cl ) {
   for( list<string>::iterator x = d_cl.begin(); x != d_cl.end(); x++ )
     m_commandline.push_back( (*x) );
}

void ConfigData::setCommandLine( int argc, char *argv[] ) {
   for( int x = 1; x < argc; x++ )
     m_commandline.push_back( string( argv[x] ) );
}

void ConfigData::parseCommandLine(){
   int opt, ind, argc;
   list<struct option> opts;
   string optstring = "";
   char **argv;
   
   argc = 0;
   for( list<string>::iterator x = m_commandline.begin(); x != m_commandline.end(); x++ )
     argc++;
   
   argv = new char * [argc+1];
   argc = 1;
   argv[0] = (char *)"program name should be here";
   for( list<string>::iterator x = m_commandline.begin(); x != m_commandline.end(); x++ )
     argv[argc++] = (char *)(*x).c_str();
   
   for( list<c_l_arg>::iterator x = m_options.begin(); x != m_options.end(); x++ ) {
      struct option y;
      optstring += (*x).short_opt;
      if( (*x).hasParameter )
	optstring += ":";
      y.name = (*x).long_opt.c_str();
      y.has_arg = !!(*x).hasParameter;
      y.flag = 0;
      y.val = (*x).short_opt;
      opts.push_back( y );
   }
   
   struct option *long_opts = new struct option [opts.size() + 1];
   memset( long_opts,0, sizeof(struct option) * (opts.size() + 1) );
   int i = 0;
   for( list<struct option>::iterator x = opts.begin() ; x != opts.end() ; x++ ) {
      long_opts[i] = *x;
      i++;
   }
   
   while( (opt = getopt_long( argc, argv, optstring.c_str(), long_opts, &ind )) != -1 ) {
      for( list<c_l_arg>::iterator x = m_options.begin(); x != m_options.end(); x++ ) {
	 if( (*x).short_opt == (char)opt ) {
	    if( (*x).hasParameter )
	      addKey( (*x).option_keyname, string( optarg ) );
	    else
	      addKey( (*x).option_keyname, "true" );
	 } else if( opt == '?' ) {
	    int k = optind;
	    addKey( "HELP", "true" );
	 }
      }
   }
   delete long_opts;
   delete argv;
}

void ConfigData::saveConfig( std::string filename ) {
   string fname;
   string home_dir = getenv("HOME");
   const char *replace = string( home_dir + "$1" ).c_str();
   boost::regex bm("^\\s*~.*$");
   boost::regex rm("^\\s*~(.*)$");
   if( boost::regex_match( filename, bm ) ) {
      ostringstream ot(ios::out|ios::binary);
      ostream_iterator<char,char> oi(ot);
      boost::regex_replace( oi, filename.begin(), filename.end(), 
			    rm, replace, boost::match_default );
      fname = ot.str();
   } else {
      fname = filename;
   }
   path a( fname );
   path b( a.normalize() );
   saveConfig( b );
}
   
void ConfigData::saveConfig( boost::filesystem::path filename ) {
   fstream ofile;
   ofile.open( filename.string().c_str(), ios::out );
   
   for( map<string,ConfigSection>::iterator x = m_sections.begin(); x != m_sections.end(); x++ ) {
      ofile << "section " << (*x).first << endl;
      (*x).second.save( ofile );
      ofile << "endsection" << endl;
   }
   ofile << endl;
   ofile.close();
}

void ConfigData::dumpConfig() {
   for( map<string,ConfigSection>::iterator x = m_sections.begin(); x != m_sections.end(); x++ ) {
      cout << "section " << (*x).first << endl;
      (*x).second.dump();
      cout << "endsection" << endl << endl;
   }
   cout << endl;
   for( map<string,ConfigKey>::iterator x = m_keys.begin(); x != m_keys.end(); x++ ) {
      cout << (*x).first << " == " << (*x).second.toString() << endl;
   }
   cout << endl;
}

std::string ConfigData::getString(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) ) {
	   return m_sections[b].getString(c);
	 } else {
	   return string("");
	 }
      }
   } else {
      return string("");
   }
   return string("There is an error");
}
	
float ConfigData::getFloat(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) )
	   return m_sections[b].getFloat(c);
	 else
	   return 0.0f;
      }
   } else {
      return 0.0f;
   }
   return -0.0f;
}

bool ConfigData::getBool(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) )
	   return m_sections[b].getBool(c);
	 else
	   return false;
      }
   } else {
      return false;
   }
   return false;
}

int ConfigData::getInt(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) )
	   return m_sections[b].getInt(c);
	 else
	   return 0;
      }
   } else {
      return 0;
   }
   return -1;
}

bool ConfigData::Parse( string filename ) {
   string fname;
   string home_dir = getenv("HOME");
   const char *replace = string( home_dir + "$1" ).c_str();
   boost::regex bm("^\\s*~.*$");
   boost::regex rm("^\\s*~(.*)$");
   if( boost::regex_match( filename, bm ) ) {
      ostringstream ot(ios::out|ios::binary);
      ostream_iterator<char,char> oi(ot);
      boost::regex_replace( oi, filename.begin(), filename.end(), 
			    rm, replace, boost::match_default );
      fname = ot.str();
   } else {
      fname = filename;
   }
   path a( fname );
   path b( a.normalize() );
   return Parse( b );
}

bool ConfigData::Parse( boost::filesystem::path filename ) {
   queue<string> data;
   if( !exists( filename ) )
     return false;
   
   if( !is_regular( filename ) )
     return false;
   
   fstream cfile;
   cfile.open( filename.string().c_str(), ios::in );
   while( !cfile.eof() ) {
      char b[655356];
      memset( b, 0, 65536 );
      cfile.getline( b, 65535 );
      data.push( string(b) );
   }
   cfile.close();
}

void ConfigData::internalParse( std::queue<std::string> data ) {
   list<string> stack;
   boost::regex comment( "^\\s*#.*$", boost::regex::perl );
   boost::regex section( "^\\s*section\\s+(\\w+)(:?\\s*#?.*)?$", boost::regex::perl|boost::regex::icase );
   boost::regex section_end( "^\\s*endsection(:?\\s*#?.*)?$", boost::regex::perl|boost::regex::icase );
   boost::regex kv_pair( "^\\s*([\\w\\d]+)\\s+([\\w\\d]+)(:?\\s*#?.*)?$", boost::regex::perl );
   boost::regex empty("^\\s*$", boost::regex::perl);
   boost::smatch matches;
   while( !data.empty() ) {
      string x = data.front();
      data.pop();
      if( !boost::regex_match( x, comment ) &&
	  !boost::regex_match( x, empty ) &&
	  x.length() > 0 ) {
	 if( boost::regex_match( x, matches, section ) ) {
	    stack.push_back( string( matches[1].first, matches[1].second) );
	 } else if( boost::regex_match( x, section_end ) ) {
	    if( !!stack.size() )
	       stack.pop_back();
		 } else if( ( boost::regex_match( x, matches, kv_pair ) ) &&
		    !!stack.size() ) {
	    string full_key_name = "";
	    for( list<string>::iterator y = stack.begin(); y != stack.end(); y++ ) {
	       if( full_key_name.length() == 0 )
		 full_key_name += (*y);
	       else
		 full_key_name += "." + (*y);
	    }
	    full_key_name += "." + string( matches[1].first, matches[1].second );
	    addKey( full_key_name, string( matches[2].first, matches[2].second ) );
	 } else if( boost::regex_match( x, kv_pair ) && !(!!stack.size()) ) {
	    cerr << "key/value pair " << x << " seen outside of a section." << endl;
	 }
      } 
   }
}

list<c_l_arg> ConfigData::getOptions() 
{
   list<c_l_arg> temp;
   for( list<c_l_arg>::iterator x = m_options.begin(); x != m_options.end(); x++ )
     temp.push_back( (*x) );
   return temp;
}
