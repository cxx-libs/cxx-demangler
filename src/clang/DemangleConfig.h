//===--- DemangleConfig.h ---------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a variety of feature test macros copied from
// include/llvm/Support/Compiler.h so that LLVMDemangle does not need to take
// a dependency on LLVMSupport.
//
//===----------------------------------------------------------------------===//
#pragma once

#ifndef __has_attribute
  #define __has_attribute( x ) 0
#endif

#ifndef __has_builtin
  #define __has_builtin( x ) 0
#endif

#if __has_builtin( __builtin_unreachable )
  #define DEMANGLE_UNREACHABLE __builtin_unreachable()
#elif defined( _MSC_VER )
  #define DEMANGLE_UNREACHABLE __assume( false )
#else
  #define DEMANGLE_UNREACHABLE
#endif

/// DEMANGLE_ABI is the export/visibility macro used to mark symbols declared in
/// llvm/Demangle as exported when built as a shared library.
#if defined( LLVM_BUILD_STATIC ) || !defined( LLVM_ENABLE_LLVM_EXPORT_ANNOTATIONS )
  #define DEMANGLE_ABI
#else
  #if defined( _WIN32 )
    #if defined( LLVM_EXPORTS )
      #define DEMANGLE_ABI __declspec( dllexport )
    #else
      #define DEMANGLE_ABI __declspec( dllimport )
    #endif
  #else
    #if __has_attribute( visibility )
      #define DEMANGLE_ABI __attribute__( ( __visibility__( "default" ) ) )
    #else
      #define DEMANGLE_ABI
    #endif
  #endif
#endif
