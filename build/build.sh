#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/sample
mkdir -p $BUILDDIR/SOURCES


# Build
cd ../../src/sample
make all
cd $BUILDDIR
cp ../../src/sample/stkwebapp $BUILDDIR/SOURCES
cp ../../src/sample/stkwebapp.conf $BUILDDIR/SOURCES


# Make SPEC file
SPEC=minimum.spec
cat <<EOF > ./$SPEC
Name:    YaizuSample
Version: 1
Release: 1%{?dist}
Summary: YaizuSample

License: No License No Life
Source1: stkwebapp
Source2: stkwebapp.conf

%description
minimum test command!!

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}

%files
%{_bindir}/stkwebapp
%{_sysconfdir}/stkwebapp.conf
EOF

# RpmBuild
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

