#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/sample
mkdir -p $BUILDDIR/SOURCES


# Build
cd ../src/sample
make all
cp sample $BUILDDIR/SOURCES
cp sample.conf $BUILDDIR/SOURCES
echo servicehost=127.0.0.1 >> $BUILDDIR/SOURCES/sample.conf
echo serviceport=8081 >> $BUILDDIR/SOURCES/sample.conf
cp sample.service $BUILDDIR/SOURCES
cp sample_nginx.conf $BUILDDIR/SOURCES
cp sample.html $BUILDDIR/SOURCES
cp jquery-3.2.0.min.js $BUILDDIR/SOURCES
cp style.css $BUILDDIR/SOURCES


# Make SPEC file
cd $BUILDDIR
SPEC=sample.spec
cat <<EOF > ./$SPEC
Name:    YaizuSample
Version: 1
Release: 1%{?dist}
Summary: YaizuSample
Requires: nginx = 1:1.12.2-1.el7_4.ngx

License: No License No Life
Source1: sample
Source2: sample.conf
Source3: sample_nginx.conf
SOurce4: sample.service
Source5: sample.html
Source6: jquery-3.2.0.min.js
Source7: style.css

%description
YaizuSample!!

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/nginx/conf.d
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/nginx/conf.d
install -p -m 644 %{SOURCE4} %{buildroot}/%{_sysconfdir}/systemd/system
install -p -m 644 %{SOURCE5} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE6} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE7} %{buildroot}/%{_datarootdir}/nginx/html

%files
%{_bindir}/sample
%{_sysconfdir}/sample.conf
%{_sysconfdir}/nginx/conf.d/sample_nginx.conf
%{_sysconfdir}/systemd/system/sample.service
%{_datarootdir}/nginx/html/sample.html
%{_datarootdir}/nginx/html/jquery-3.2.0.min.js
%{_datarootdir}/nginx/html/style.css

%post
setsebool httpd_can_network_connect on -P
firewall-cmd --add-port=8080/tcp --permanent
firewall-cmd --reload
systemctl stop nginx.service
systemctl start nginx.service
systemctl enable nginx.service
systemctl start sample.service
systemctl enable sample.service

%preun
systemctl stop nginx.service
systemctl stop sample.service

%postun
firewall-cmd --remove-port=8080/tcp --permanent
firewall-cmd --reload
systemctl disable sample.service
systemctl start nginx.service

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC
if [ -e ../deployment ]; then
    yes|rm -r ../deployment
fi
mkdir -p ../deployment
cp RPMS/x86_64/YaizuSample-1-1.el7.centos.x86_64.rpm ../deployment
