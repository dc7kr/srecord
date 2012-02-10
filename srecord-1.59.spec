#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
#    W   W    A    RRRR   N   N   III  N   N  III  N   N   GGG
#    W   W   A A   R   R  NN  N    I   NN  N   I   NN  N  G   G
#    W   W  A   A  RRRR   N N N    I   N N N   I   N N N  G
#    W W W  AAAAA  R R    N  NN    I   N  NN   I   N  NN  G  GG
#    W W W  A   A  R  R   N   N    I   N   N   I   N   N  G   G
#     W W   A   A  R   R  N   N   III  N   N  III  N   N   GGG
#
# Warning: DO NOT send patches which fix this file. IT IS NOT the original
# source file. This file is GENERATED from the Aegis repository file manifest.
# If you find a bug in this file, it could well be an Aegis bug.
#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
Summary: srecord
Name: srecord
Version: 1.59.D001
Release: 1
License: GPL
Group: Development/Tools
Source: http://srecord.sourceforge.net/%{name}-%{version}.tar.gz
URL: http://srecord.sourceforge.net/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires: boost-devel
BuildRequires: diffutils
BuildRequires: groff
BuildRequires: libgcrypt-devel
BuildRequires: libtool
BuildRequires: sharutils

%description
This package contains the shared libraries for programs that manipulate EPROM
load files.

%package bin
Summary: srecord programs

%description bin
The srecord package is a collection of powerful tools for manipulating EPROM
load files. It reads and writes numerous EPROM file formats, and can perform
many different manipulations.

%package libs
Summary: srecord libraries
Group: Development/Tools

%description libs
This package contains the shared libraries for programs that manipulate EPROM
load files.

%package devel
Summary: srecord development files
Group: Development/Tools
Requires: srecord-libs%{?_isa} = %{?epoch:%{epoch}:}%{version}-%{release}

%description devel
This package contains static libraries and header files for compiling programs
that manipulate EPROM load files.

%post
/sbin/ldconfig

%postun
/sbin/ldconfig


%prep
%setup -q


%build
%configure
make


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/usr/lib*/*.la


%check
make sure


%clean
rm -rf $RPM_BUILD_ROOT


%files bin
%defattr (-,root,root,-)
%doc LICENSE BUILDING README
%{_bindir}/srec_cat
%{_bindir}/srec_cmp
%{_bindir}/srec_info
%{_mandir}/man1/srec_cat.1.gz
%{_mandir}/man1/srec_cmp.1.gz
%{_mandir}/man1/srec_examples.1.gz
%{_mandir}/man1/srec_info.1.gz
%{_mandir}/man1/srec_input.1.gz
%{_mandir}/man1/srec_license.1.gz
%{_mandir}/man5/srec_aomf.5.gz
%{_mandir}/man5/srec_ascii_hex.5.gz
%{_mandir}/man5/srec_atmel_generic.5.gz
%{_mandir}/man5/srec_binary.5.gz
%{_mandir}/man5/srec_brecord.5.gz
%{_mandir}/man5/srec_cosmac.5.gz
%{_mandir}/man5/srec_dec_binary.5.gz
%{_mandir}/man5/srec_emon52.5.gz
%{_mandir}/man5/srec_fairchild.5.gz
%{_mandir}/man5/srec_fastload.5.gz
%{_mandir}/man5/srec_formatted_binary.5.gz
%{_mandir}/man5/srec_forth.5.gz
%{_mandir}/man5/srec_fpc.5.gz
%{_mandir}/man5/srec_idt.5.gz
%{_mandir}/man5/srec_intel.5.gz
%{_mandir}/man5/srec_intel16.5.gz
%{_mandir}/man5/srec_mif.5.gz
%{_mandir}/man5/srec_mips_flash.5.gz
%{_mandir}/man5/srec_mos_tech.5.gz
%{_mandir}/man5/srec_motorola.5.gz
%{_mandir}/man5/srec_msbin.5.gz
%{_mandir}/man5/srec_needham.5.gz
%{_mandir}/man5/srec_os65v.5.gz
%{_mandir}/man5/srec_ppb.5.gz
%{_mandir}/man5/srec_ppx.5.gz
%{_mandir}/man5/srec_signetics.5.gz
%{_mandir}/man5/srec_spasm.5.gz
%{_mandir}/man5/srec_spectrum.5.gz
%{_mandir}/man5/srec_stewie.5.gz
%{_mandir}/man5/srec_tektronix.5.gz
%{_mandir}/man5/srec_tektronix_extended.5.gz
%{_mandir}/man5/srec_ti_tagged.5.gz
%{_mandir}/man5/srec_ti_tagged_16.5.gz
%{_mandir}/man5/srec_ti_txt.5.gz
%{_mandir}/man5/srec_vmem.5.gz
%{_mandir}/man5/srec_wilson.5.gz


%files libs
%defattr (-,root,root,-)
%{_libdir}/libsrecord.so*


%files devel
%defattr (-,root,root,-)
%{_includedir}/srecord
%{_libdir}/libsrecord.a
%{_libdir}/pkgconfig/srecord.pc
%{_mandir}/man3/srecord.3.gz
%{_mandir}/man3/srecord_license.3.gz
