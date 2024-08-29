///////////////////////////////////////////////////////////////////////////////
//
//  Mp3TagReader.cpp
//
//  Copyright © Pete Isensee (PKIsensee@msn.com).
//  All rights reserved worldwide.
//
//  Permission to copy, modify, reproduce or redistribute this source code is
//  granted provided the above copyright notice is retained in the resulting 
//  source code.
// 
//  This software is provided "as is" and without any express or implied
//  warranties.
//
///////////////////////////////////////////////////////////////////////////////

#include <filesystem>
#include <iostream>

#include "Log.h"
#include "Mp3TagData.h"

using namespace PKIsensee;
namespace fs = std::filesystem;

bool DisplayTagData( const fs::path& mp3File )
{
  Mp3TagData tagData;
  if( !tagData.LoadTagData( mp3File ) )
  {
    PKLOG_WARN( "Input file %s doesn't appear to be MP3 format (err: %u)\n", 
                mp3File.c_str(), Util::GetLastError());
  }
  std::cout << tagData;
  return true;
}

int __cdecl main( int argc, char** argv )
{
  if( argc != 2 )
  {
    PKLOG_WARN( "Usage: %s %s\n", "Mp3TagReader.exe", "<mp3FileOrFolder>");
    return -1;
  }

  fs::path mp3File( argv[1] );
  if( fs::is_directory( mp3File ) )
  {
    bool foundMp3File = false;
    for( const auto& entry : fs::recursive_directory_iterator( mp3File ) )
    {
      fs::path path = entry.path();
      std::string ext = path.extension().string();
      StrUtil::ToLower( ext );
      if( ext == ".mp3" )
      {
        foundMp3File = true;
        if( !DisplayTagData( path ) )
          return -3;
      }
    }
    if( !foundMp3File )
    {
      PKLOG_WARN( "No MP3 files found\n" );
      return -4;
    }
    return 0;
  }

  // Single file
  else if( !fs::exists( mp3File ) )
  {
    PKLOG_WARN( "Input file %s not found (err: %u)\n", argv[1], Util::GetLastError() );
    return -2;
  }
  if( !DisplayTagData( mp3File ) )
    return -3;

  return 0;
}
