//
//	srecord - manipulate eprom load files
//	Copyright (C) 2004 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for include/r250.cc
//

#ifndef INCLUDE_R250_H
#define INCLUDE_R250_H

/**
  * The r250 function is used to produce a 32-bit random number.
  * It repeats every 2**256 samples, long enough for most purposes.
  */
unsigned long r250(void);

#endif // INCLUDE_R250_H
