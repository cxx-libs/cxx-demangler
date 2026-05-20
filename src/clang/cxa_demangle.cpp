//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// FIXME: (possibly) incomplete list of features that clang mangles that this
// file does not yet support:
//   - C++ modules TS

#include "DemangleConfig.h"
#include "ItaniumDemangle.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <functional>
#include <numeric>
#include <string_view>
#include <utility>

using namespace llvm::itanium_demangle;

namespace
{

class BumpPointerAllocator
{
  struct BlockMeta
  {
    BlockMeta* Next{ nullptr };
    size_t     Current{ 0 };
  };

  static constexpr std::size_t AllocSize{ 4096 };
  static constexpr std::size_t UsableAllocSize{ AllocSize - sizeof( BlockMeta ) };

  alignas( long double ) char InitialBuffer[AllocSize];
  BlockMeta* BlockList{ nullptr };

  void grow()
  {
    char* NewMeta = static_cast<char*>( std::malloc( AllocSize ) );
    if( NewMeta == nullptr ) std::terminate();
    BlockList = new( NewMeta ) BlockMeta{ BlockList, 0 };
  }

  void* allocateMassive( std::size_t NBytes )
  {
    NBytes += sizeof( BlockMeta );
    BlockMeta* NewMeta = reinterpret_cast<BlockMeta*>( std::malloc( NBytes ) );
    if( NewMeta == nullptr ) std::terminate();
    BlockList->Next = new( NewMeta ) BlockMeta{ BlockList->Next, 0 };
    return static_cast<void*>( NewMeta + 1 );
  }

public:
  BumpPointerAllocator() : BlockList( new( InitialBuffer ) BlockMeta{ nullptr, 0 } ) {}

  void* allocate( size_t N )
  {
    N = ( N + 15u ) & ~15u;
    if( N + BlockList->Current >= UsableAllocSize )
    {
      if( N > UsableAllocSize ) return allocateMassive( N );
      grow();
    }
    BlockList->Current += N;
    return static_cast<void*>( reinterpret_cast<char*>( BlockList + 1 ) + BlockList->Current - N );
  }

  void reset()
  {
    while( BlockList )
    {
      BlockMeta* Tmp = BlockList;
      BlockList      = BlockList->Next;
      if( reinterpret_cast<char*>( Tmp ) != InitialBuffer ) std::free( Tmp );
    }
    BlockList = new( InitialBuffer ) BlockMeta{ nullptr, 0 };
  }

  ~BumpPointerAllocator() { reset(); }
};

class DefaultAllocator
{
  BumpPointerAllocator Alloc;

public:
  void reset() { Alloc.reset(); }

  template<typename T, typename... Args> T* makeNode( Args&&... args ) { return new( Alloc.allocate( sizeof( T ) ) ) T( std::forward<Args>( args )... ); }

  void* allocateNodeArray( size_t sz ) { return Alloc.allocate( sizeof( llvm::itanium_demangle::Node* ) * sz ); }
};
}  // unnamed namespace

//===----------------------------------------------------------------------===//
// Code beyond this point should not be synchronized with LLVM.
//===----------------------------------------------------------------------===//

using Demangler = llvm::itanium_demangle::ManglingParser<DefaultAllocator>;

namespace
{

enum : int
{
  demangle_invalid_args         = -3,
  demangle_invalid_mangled_name = -2,
  demangle_memory_alloc_failure = -1,
  demangle_success              = 0,
};
}

#if defined( _WIN32 )
  #if defined( _LIBCXXABI_DISABLE_VISIBILITY_ANNOTATIONS ) || ( defined( __MINGW32__ ) && !defined( _LIBCXXABI_BUILDING_LIBRARY ) )
    #define _LIBCXXABI_FUNC_VIS
  #elif defined( _LIBCXXABI_BUILDING_LIBRARY )
    #define _LIBCXXABI_FUNC_VIS __declspec( dllexport )
  #else
    #define _LIBCXXABI_FUNC_VIS __declspec( dllimport )
  #endif
#else
  #if !defined( _LIBCXXABI_DISABLE_VISIBILITY_ANNOTATIONS )
    #define _LIBCXXABI_FUNC_VIS __attribute__( ( __visibility__( "default" ) ) )
    #if __has_attribute( __type_visibility__ )
    #else
    #endif
  #else
    #define _LIBCXXABI_FUNC_VIS
  #endif
#endif

namespace __cxxabiv1
{
extern "C" _LIBCXXABI_FUNC_VIS char* __cxa_demangle( const char* MangledName, char* Buf, size_t* N, int* Status )
{
  if( MangledName == nullptr || ( Buf != nullptr && N == nullptr ) )
  {
    if( Status ) *Status = demangle_invalid_args;
    return nullptr;
  }

  int                           InternalStatus{ demangle_success };
  Demangler                     Parser( MangledName, MangledName + std::strlen( MangledName ) );
  llvm::itanium_demangle::Node* AST = Parser.parse();

  if( AST == nullptr ) InternalStatus = demangle_invalid_mangled_name;
  else
  {
    llvm::itanium_demangle::OutputBuffer O( Buf, N );
    AST->print( O );
    O += '\0';
    if( N != nullptr ) *N = O.getCurrentPosition();
    Buf = O.getBuffer();
  }

  if( Status ) *Status = InternalStatus;
  return InternalStatus == demangle_success ? Buf : nullptr;
}
}  // namespace __cxxabiv1
