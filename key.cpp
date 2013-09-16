#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <boost/regex.hpp>
#include <iostream>
#include "key.h"

using namespace std;

ConfigKey::ConfigKey() {
   typeStored = CK_TYPE_BAD_TYPE;
};

ConfigKey::ConfigKey( string val ) {
   string temp = string(val);
   transform( temp.begin(), temp.end(), temp.begin(), ::tolower );
   boost::regex isFloat("^\\d*\\.\\d+$", boost::regex::perl );
   boost::regex isInt("^\\d+$", boost::regex::perl );
   if( temp == "true" || temp == "yes" ) {
      typeStored = CK_TYPE_BOOL;
      bdata = true;
   } else if( temp == "false" || temp == "no" ) {
      typeStored = CK_TYPE_BOOL;
      bdata = false;
   } else if( boost::regex_match( val, isFloat ) ) {
      typeStored = CK_TYPE_FLOAT;
      fdata = atof( val.c_str() );
   } else if( boost::regex_match( val, isInt ) ) {
      typeStored = CK_TYPE_INT;
      idata = atoi( val.c_str() );
   } else {
      typeStored = CK_TYPE_STRING;
      sdata = val;
   }
};

ConfigKey::ConfigKey( bool val ) {
   typeStored = CK_TYPE_BOOL;
   bdata = val;
};

ConfigKey::ConfigKey( int val ) {
   typeStored = CK_TYPE_INT;
   idata = val;
};

ConfigKey::ConfigKey( float val ) {
   typeStored = CK_TYPE_FLOAT;
   fdata = val;
};

string ConfigKey::toString() {
   switch(typeStored) {
    case CK_TYPE_BOOL:
      return string( (bdata?"TRUE":"FALSE") );
      break;
    case CK_TYPE_INT: 
	{
	   stringstream temp;
	   temp << idata;
	   return temp.str();
	}
      break;
    case CK_TYPE_STRING:
      return string( sdata );
      break;
    case CK_TYPE_FLOAT: 
	{
	   stringstream temp;
	   temp << idata;
	   return temp.str();
	}
      break;
    case CK_TYPE_BAD_TYPE:
    default:
      return string( "POSSIBLE ERROR - VALUE NOT SET (?)" );
   }
   return string( "" ); // control should never reach this point
};

bool ConfigKey::toBool() {
   switch( typeStored ) {
    case CK_TYPE_BOOL:
      return bdata;
      break;
    case CK_TYPE_INT:
      return !!idata;
      break;
    case CK_TYPE_STRING: 
	{
	   string temp = sdata;
	   transform( temp.begin(), temp.end(), temp.begin(), ::tolower );
	   if( temp == "yes" || temp == "true" || temp == "y" || temp == "1" || temp == "t" )
	     return true;
	   else
	     return false;
	}
      break;
    case CK_TYPE_FLOAT:
      return !!fdata;
    case CK_TYPE_BAD_TYPE:
    default:
      return false;
   }
   return false; // control should never reach here
};

float ConfigKey::toFloat() {
   switch( typeStored ) {
    case CK_TYPE_BOOL:
      return (bdata?1.0f:0.0f);
      break;
    case CK_TYPE_INT:
      return (float)idata;
      break;
    case CK_TYPE_STRING:
      return atof( sdata.c_str() );
      break;
    case CK_TYPE_FLOAT:
      return fdata;
      break;
    case CK_TYPE_BAD_TYPE:
    default:
      return false;
   }
   return false; // control should never reach here
};

int ConfigKey::toInt() {
   switch( typeStored ) {
    case CK_TYPE_BOOL:
      return (bdata?1:0);
      break;
    case CK_TYPE_INT:
      return idata;
      break;
    case CK_TYPE_STRING:
      return atoi( sdata.c_str() );
      break;
    case CK_TYPE_FLOAT:
      return (int)fdata;
      break;
    case CK_TYPE_BAD_TYPE:
    default:
      return 0;
   }
   return 0;
};
