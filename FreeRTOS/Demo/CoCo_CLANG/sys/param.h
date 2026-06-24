/* param.h - Board specific parameters
   Copyright (C) 2000, 2002, 2003 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@nerim.fr)	

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

/*! Attribute unused.
    Use this attribute to indicate that a parameter, a variable or a
    static function is not used.  The compiler will not warn about the
    unused variable.  */
#define ATTRIBUTE_UNUSED __attribute__((unused))

/*! Attribute page0.
    Use this attribute to put a global or static variable in page0. */
#define PAGE0_ATTRIBUTE __attribute__((section(".page0")))

#include <asm-turbo9/param.h>

#endif
