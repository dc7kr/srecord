#
# This file is generated.  Please do not send the maintainer
# patches to this file.  Please update etc/spec.sh and send a
# patch to that file, instead.
#
Summary: manipulate EPROM load files
Name: srecord
Version: 1.15
Release: 1
Copyright: GPL
Group: Development/Tools
Source: http://srecord.sourceforge.net/srecord-1.15.tar.gz
URL: http://srecord.sourceforge.net/
BuildRoot: /tmp/srecord-build-root

%description

The SRecord package is a collection of powerful tools for manipulating
EPROM load files.

* The SRecord package understands a number of file formats: Motorola
  S-Record, Intel, Tektronix, Binary.  These file formats may be read
  and written.  Also C array definitions, for output only.

* The SRecord package has a number of tools: srec_cat for copying and
  and converting files, srec_cmp for comparing files and srec_info for
  printing summaries.

* The SRecord package has a number for filters: checksum to add checksums
  to the data, crop to keep address ranges, exclude to remove address
  ranges, fill to plug holes in the data, length to insert the data
  length, maximum to insert the data address maximum, minimum to insert
  the data address minimum, offset to adjust addresses, and split for
  wide data buses and memory striping.

More than one filter may be applied to each input file.  Different filters
may be applied to each input file.  All filters may be applied to all
file formats.

%prep
%setup
./configure --prefix=/usr

%build
make

%install
make RPM_BUILD_ROOT=$RPM_BUILD_ROOT install

%files
/usr/bin/srec_cat
/usr/bin/srec_cmp
/usr/bin/srec_info
/usr/man/man1/srec_cat.1*
/usr/man/man1/srec_cmp.1*
/usr/man/man1/srec_info.1*
/usr/man/man1/srec_license.1*
/usr/man/man5/srec_ascii_hex.5*
/usr/man/man5/srec_atmel_generic.5*
/usr/man/man5/srec_cosmac.5*
/usr/man/man5/srec_dec_binary.5*
/usr/man/man5/srec_emon52.5*
/usr/man/man5/srec_fairchild.5*
/usr/man/man5/srec_fastload.5*
/usr/man/man5/srec_formatted_binary.5*
/usr/man/man5/srec_fpc.5*
/usr/man/man5/srec_intel.5*
/usr/man/man5/srec_mos_tech.5*
/usr/man/man5/srec_motorola.5*
/usr/man/man5/srec_needham.5*
/usr/man/man5/srec_os65v.5*
/usr/man/man5/srec_signetics.5*
/usr/man/man5/srec_spasm.5*
/usr/man/man5/srec_spectrum.5*
/usr/man/man5/srec_tektronix.5*
/usr/man/man5/srec_tektronix_extended.5*
/usr/man/man5/srec_ti_tagged.5*
/usr/man/man5/srec_wilson.5*
