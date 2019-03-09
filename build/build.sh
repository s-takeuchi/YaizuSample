#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/sample
mkdir -p $BUILDDIR/SOURCES


# Build
cd ../src/sample
make all
cp ../../src/sample/stkwebapp $BUILDDIR/SOURCES
cp ../../src/sample/sample.conf $BUILDDIR/SOURCES
echo servicehost=127.0.0.1 >> $BUILDDIR/SOURCES/sample.conf
echo serviceport=8081 >> $BUILDDIR/SOURCES/sample.conf
cp ../../src/sample/sample_nginx.conf $BUILDDIR/SOURCES
cp ../../src/sample/index.html $BUILDDIR/SOURCES/sample.html
cp ../../src/sample/jquery-3.2.0.min.js $BUILDDIR/SOURCES
cp ../../src/sample/style.css $BUILDDIR/SOURCES


# Make SPEC file
SPEC=sample.spec
cat <<EOF > ./$SPEC
Name:    YaizuSample
Version: 1
Release: 1%{?dist}
Summary: YaizuSample

License: No License No Life
Source1: stkwebapp
Source2: sample.conf
Source3: sample_nginx.conf
Source4: sample.html
Source5: jquery-3.2.0.min.js
Source6: style.css

%description
YaizuSample!!

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/nginx/conf.d
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/nginx/conf.d
install -p -m 644 %{SOURCE4} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE5} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE6} %{buildroot}/%{_datarootdir}/nginx/html
setsebool httpd_can_network_connect on -P

%files
%{_bindir}/stkwebapp
%{_sysconfdir}/sample.conf
%{_sysconfdir}/nginx/conf.d/sample_nginx.conf
%{_datarootdir}/nginx/html/sample.html
%{_datarootdir}/nginx/html/jquery-3.2.0.min.js
%{_datarootdir}/nginx/html/style.css

EOF

# RpmBuild
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

