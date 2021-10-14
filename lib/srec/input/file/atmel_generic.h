//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001-2003, 2006-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#ifndef LIB_SREC_INPUT_FILE_ATMEL_GENERIC_H
#define LIB_SREC_INPUT_FILE_ATMEL_GENERIC_H

#include <lib/endian.h>
#include <lib/srec/input/file.h>

/**
  * The srec_input_file_atmel_generic class is used to represent the
  * parse stat of an Atmel Genric formatted file.
  */
class srec_input_file_atmel_generic:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_atmel_generic();

private:
    /**
      * the constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      * @param end
      *     The byte order.
      */
    srec_input_file_atmel_generic(const std::string &file_name, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @param end
      *     The byte order.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create_be(const std::string &file_name);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @param end
      *     The endian-ness of the file
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name,
        endian_t end = endian_big);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The read_inner method is used to read a single record form
      * the file.  The read method is a wrapper around it.
      */
    int read_inner(srec_record &);

    /**
      * The seen_some_input instance variable is used to remember whther
      * any input data has been seen to date.
      */
    bool seen_some_input;

    /**
      * The end instance variable is used to remember whether the
      * input is big endian or little endian.
      */
    endian_t end;

    /**
      * The deafult constructor.  Do not use.
      */
    srec_input_file_atmel_generic();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_atmel_generic(const srec_input_file_atmel_generic &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_atmel_generic &operator=(
        const srec_input_file_atmel_generic &);
};

#endif // LIB_SREC_INPUT_FILE_ATMEL_GENERIC_H
