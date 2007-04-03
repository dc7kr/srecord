//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2005-2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for include/srec/input/file/guess.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_GUESS_H
#define INCLUDE_SREC_INPUT_FILE_GUESS_H

#include <lib/srec/input.h>

/**
  * The srec_input_file_guess function is used to open a file of an
  * unknown type.  It attempts all of the know formats one after the
  * other.
  *
  * @param file_name
  *     The name of the file to be opened.
  */
srec_input *srec_input_file_guess(const string &file_name);

#endif // INCLUDE_SREC_INPUT_FILE_GUESS_H
