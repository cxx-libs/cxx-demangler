//===-- Demangle.cpp - Common demangling functions ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains definitions of common demangling functions.
///
//===----------------------------------------------------------------------===//

#include "Demangle.h"

#include "StringViewExtras.h"

#include <cstdlib>
#include <string_view>

std::string cxx::demangler::backend::clang::demangle( const std::string_view MangledName )
{
  std::string Result;

  if( nonMicrosoftDemangle( MangledName, Result ) ) return Result;

  if( cxx::demangler::backend::clang::starts_with( MangledName, '_' ) && nonMicrosoftDemangle( MangledName.substr( 1 ), Result, /*CanHaveLeadingDot=*/false ) ) return Result;

  if( char* Demangled = cxx::demangler::backend::clang::microsoftDemangle( MangledName, nullptr, nullptr ) )
  {
    Result = Demangled;
    std::free( Demangled );
  }
  else
  {
    Result = MangledName;
  }
  return Result;
}

static bool isItaniumEncoding( const std::string_view S )
{
  // Itanium demangler supports prefixes with 1-4 underscores.
  const size_t Pos = S.find_first_not_of( '_' );
  return Pos > 0 && Pos <= 4 && S[Pos] == 'Z';
}

bool cxx::demangler::backend::clang::nonMicrosoftDemangle( std::string_view MangledName, std::string& Result, const bool CanHaveLeadingDot, const bool ParseParams )
{
  char* Demangled{ nullptr };

  // Do not consider the dot prefix as part of the demangled symbol name.
  if( CanHaveLeadingDot && MangledName.size() > 0 && MangledName[0] == '.' )
  {
    MangledName.remove_prefix( 1 );
    Result = ".";
  }

  if( isItaniumEncoding( MangledName ) ) Demangled = itaniumDemangle( MangledName, ParseParams );

  if( !Demangled ) return false;

  Result += Demangled;
  std::free( Demangled );
  return true;
}
