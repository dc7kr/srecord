//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LIB_SREC_INPUT_CATENATE_H
#define LIB_SREC_INPUT_CATENATE_H

#include <lib/srec/input.h>

/**
  * The srec_input_catenate class is used to represent an input source
  * which presents two inoput sources af if they were a single input
  * source.
  */
class srec_input_catenate:
    public srec_input
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_catenate();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param in1
      *     The first of the inputs to be read.
      * @param in2
      *     The second of the inputs to be read.
      */
    srec_input_catenate(const pointer &in1, const pointer &in2);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param in1
      *     The first of the inputs to be read.
      * @param in2
      *     The second of the inputs to be read.
      */
    static pointer create(const pointer &in1, const pointer &in2);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    std::string filename() const;

    // See base class for documentation.
    std::string filename_and_line() const;

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    void disable_checksum_validation();

private:
    /**
      * The in1 instance variable is used to remember the first of the
      * inputs to be read.
      */
    pointer in1;

    /**
      * The in2 instance variable is used to remember the second of the
      * inputs to be read.
      */
    pointer in2;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_catenate();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_catenate(const srec_input_catenate &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_catenate &operator=(const srec_input_catenate &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_INPUT_CATENATE_H
