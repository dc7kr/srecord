//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2005-2007 Peter Miller
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

#ifndef INCLUDE_SREC_OUTPUT_FILE_BINARY_H
#define INCLUDE_SREC_OUTPUT_FILE_BINARY_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_binary class is used to represetn an output
  * file in raw binary format.
  */
class srec_output_file_binary:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_binary();

    /**
      * The defualrt constructor.
      * Ouput will be written to the standard output.
      */
    srec_output_file_binary();

    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open and write output to.
      */
    srec_output_file_binary(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    const char *mode() const;

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_binary(const srec_output_file_binary &);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_binary &operator=(const srec_output_file_binary &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_BINARY_H
