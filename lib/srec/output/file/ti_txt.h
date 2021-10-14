//
//      srecord - manipulate eprom load files
//      Copyright (C) 2007 Peter Miller
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

#ifndef INCLUDE_SREC_OUTPUT_FILE_ti_txt_H
#define INCLUDE_SREC_OUTPUT_FILE_ti_txt_H

#include <lib/srec/output/file.h>

/**
  * The srec_output_file_ti_txt class is used to write a file in
  * ti_txt (MSP430) format.
  */
class srec_output_file_ti_txt:
    public srec_output_file
{
public:
    /**
      * The destrutcor.
      */
    virtual ~srec_output_file_ti_txt();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_ti_txt(const std::string &file_name);

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
    const char *format_name() const;

private:
    /**
      * The address instance variable is used to remember where we
      * are up to in the output.  Used to limit the number of @ lines
      * emitted.
      */
    unsigned long address;

    /**
      * The address_set instance variable is used to remember whether or
      * not we have emitted the first address line.  The first address,
      * even if it is zero, is not optional.
      */
    bool address_set;

    /**
      * The address_length instance variable is used to remember how
      * many bytes of address to emit.  Range: 2 to 4.  Default: 2.
      */
    int address_length;

    /**
      * The pref_block_size instance variable is used to remember the
      * number of bytes in the preferred block size.
      *
      * The format definition says it must be exactly 16.  We will allow
      * some leeway.
      */
    int pref_block_size;

    /**
      * The column instance variable is used to remember which column
      * we are up to in the output.  Used to limit the length of lines
      * in the output.
      */
    int column;

    /**
      * The line_length instance variable is used to remember how many
      * columns wide the line is allowed to be.
      *
      * The format defintion says exactly 16 bytes per line (implying a
      * line length 47 characters).  We will allow some leeway.
      */
    int line_length;

    /**
      * The put_byte_wrap method is used to write all data bytes to
      * the file.  It calls the put_byte method to do all the work,
      * but first it checks to see if this byte will fit on the line,
      * considering the line_length and throwing a newline if necessary.
      * The address is advanced by one.  The column is adjusted, too.
      */
    void put_byte_wrap(unsigned char c);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_ti_txt();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_ti_txt(const srec_output_file_ti_txt &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_ti_txt &operator=(const srec_output_file_ti_txt &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_ti_txt_H
