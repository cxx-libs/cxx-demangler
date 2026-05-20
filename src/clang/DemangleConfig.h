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
