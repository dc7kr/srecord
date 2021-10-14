//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2002, 2005-2008 Peter Miller
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

#ifndef INCLUDE_SREC_OUTPUT_FILE_DEC_BINARY_H
#define INCLUDE_SREC_OUTPUT_FILE_DEC_BINARY_H

#include <lib/srec/output/file.h>

/**
  * The srec_output_file_dec_binary class is used to write a DEC Binary
  * (XXDP) formatted file.
  */
class srec_output_file_dec_binary:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_dec_binary();

private:
    /**
      * A constructor.  The input will be read from the named file (or
      * the standard input if the file name is "-"). It is private on
      * purpose, use the create class method intead.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_dec_binary(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.  The input will be read from
      * the named file (or the standard input if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    const char *mode() const;

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * See base class for documentation.  We are over-riding it because
      * we use raw binary, so we call the put_char() method.  This
      * method also tracks the byte_offset, so that we can align to
      * specific boundaries.  Calls the checksum_add() method.
      */
    void put_byte(unsigned char);

    /**
      * The byte_offset instance variable is used to track the location
      * in the output file.  Maintained by the put_byte() method.
      */
    unsigned long byte_offset;

    /**
      * See the base class for documentation.  We are over-riding the
      * base class implementation, because it is big-endian and this
      * format needs little-endian.
      */
    void put_word(int);

    /**
      * The pref_block_size is used to remember the preferred
      * block size.  Set by the constructor.  Read by the
      * preferred_block_size_get() method.
      */
    int pref_block_size;

    /**
      * The preferred_block_size_calculate method is used to determine
      * the best block size to pack into 512 byte blocks.
      */
    static int preferred_block_size_calculate();

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_dec_binary();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_dec_binary(const srec_output_file_dec_binary &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_dec_binary &operator=(
            const srec_output_file_dec_binary &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_DEC_BINARY_H
