//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2002, 2003, 2006-2008 Peter Miller
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

#ifndef INCLUDE_SREC_MEMORY_WALKER_H
#define INCLUDE_SREC_MEMORY_WALKER_H

#include <boost/shared_ptr.hpp>

class srec_record; // forward

/**
  * The srec_memory_walker class is used to represent an abstract handler
  * for the action to perform when walking a memory image.
  */
class srec_memory_walker
{
public:
    typedef boost::shared_ptr<srec_memory_walker> pointer;

    /**
      * The destructor.
      */
    virtual ~srec_memory_walker();

    /**
      * The observe method is used by the memory walker to provide data.
      * Derived classes are required to impliment this method, and do
      * something with the data.
      */
    virtual void observe(unsigned long, const void *, int) = 0;

    /**
      * The notify_upper_bound method is used to notify the walker of
      * the upper bound (highest address plus one) of the observe calls
      * to come.  Shall be called before the any observe calls are made.
      * By default, nothing happens.
      */
    virtual void notify_upper_bound(unsigned long);

    /**
      * The observe_header method is used to inform the walker of the
      * header record.  The default does nothing.
      */
    virtual void observe_header(const srec_record * = 0);

    /**
      * The observe_start_address method is used to inform the walker
      * of the execution start address record.  The default does nothing.
      */
    virtual void observe_start_address(const srec_record * = 0);

protected:
    /**
      * The default constructor.  May only be called by derived classes.
      */
    srec_memory_walker();

private:
    /**
      * The copy constructor.  Do not use.
      */
    srec_memory_walker(const srec_memory_walker &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_memory_walker &operator=(const srec_memory_walker &);
};

#endif // INCLUDE_SREC_MEMORY_WALKER_H
