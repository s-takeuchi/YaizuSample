#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/sample
if [ -e $BUILDDIR ]; then
    yes|rm -r $BUILDDIR
fi
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
cp sample.dat $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/stkcommon.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/jquery-3.2.0.min.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/bootstrap-3.3.7-dist.zip $BUILDDIR/SOURCES
cd ../../../YaizuComLib/src/stkwebapp
make all
cp stkwebappstop $BUILDDIR/SOURCES/samplestop
cd $BUILDDIR/SOURCES
unzip $BUILDDIR/SOURCES/bootstrap-3.3.7-dist.zip
cp $BUILDDIR/SOURCES/bootstrap-3.3.7-dist/css/* $BUILDDIR/SOURCES
cp $BUILDDIR/SOURCES/bootstrap-3.3.7-dist/fonts/* $BUILDDIR/SOURCES
cp $BUILDDIR/SOURCES/bootstrap-3.3.7-dist/js/* $BUILDDIR/SOURCES


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
Source4: sample.service
Source5: sample.html
Source6: jquery-3.2.0.min.js
Source7: bootstrap.css
Source8: bootstrap.css.map
Source9: bootstrap.min.css
Source10: bootstrap.min.css.map
Source11: bootstrap-theme.css
Source12: bootstrap-theme.css.map
Source13: bootstrap-theme.min.css
Source14: bootstrap-theme.min.css.map
Source15: glyphicons-halflings-regular.eot
Source16: glyphicons-halflings-regular.svg
Source17: glyphicons-halflings-regular.ttf
Source18: glyphicons-halflings-regular.woff
Source19: glyphicons-halflings-regular.woff2
Source20: bootstrap.js
Source21: bootstrap.min.js
Source22: npm.js
Source23: sample.dat
Source24: samplestop
Source25: stkcommon.js

%description
YaizuSample!!

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/nginx/conf.d
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js

install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/nginx/conf.d
install -p -m 644 %{SOURCE4} %{buildroot}/%{_sysconfdir}/systemd/system
install -p -m 644 %{SOURCE5} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE6} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE7} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE8} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE9} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE10} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE11} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE12} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE13} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE14} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css
install -p -m 644 %{SOURCE15} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts
install -p -m 644 %{SOURCE16} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts
install -p -m 644 %{SOURCE17} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts
install -p -m 644 %{SOURCE18} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts
install -p -m 644 %{SOURCE19} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts
install -p -m 644 %{SOURCE20} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js
install -p -m 644 %{SOURCE21} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js
install -p -m 644 %{SOURCE22} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js
install -p -m 644 %{SOURCE23} %{buildroot}/%{_sysconfdir}
install -p -m 755 %{SOURCE24} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE25} %{buildroot}/%{_datarootdir}/nginx/html


%files
%{_bindir}/sample
%config(noreplace) %{_sysconfdir}/sample.conf
%{_sysconfdir}/nginx/conf.d/sample_nginx.conf
%config(noreplace) %{_sysconfdir}/systemd/system/sample.service
%{_datarootdir}/nginx/html/sample.html
%{_datarootdir}/nginx/html/stkcommon.js
%{_datarootdir}/nginx/html/jquery-3.2.0.min.js
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap.css
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap.css.map
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap.min.css
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap.min.css.map
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap-theme.css
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap-theme.css.map
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap-theme.min.css
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/css/bootstrap-theme.min.css.map
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts/glyphicons-halflings-regular.eot
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts/glyphicons-halflings-regular.svg
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts/glyphicons-halflings-regular.ttf
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts/glyphicons-halflings-regular.woff
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/fonts/glyphicons-halflings-regular.woff2
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js/bootstrap.js
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js/bootstrap.min.js
%{_datarootdir}/nginx/html/bootstrap-3.3.7-dist/js/npm.js
%config(noreplace) %{_sysconfdir}/sample.dat
%{_bindir}/samplestop

%pre
if [ \$1 = 2 ]; then
    echo "Upgrade installation (pre)"
    systemctl daemon-reload
    systemctl stop nginx.service
    systemctl stop sample.service
    while [ \`ps -ef | grep "/usr/bin/sample" | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
fi

%post
if [ \$1 = 1 ]; then
    echo "New installation (post)"
    setsebool httpd_can_network_connect on -P
    semanage port -a -t http_port_t -p tcp 80
    semanage port -a -t http_port_t -p tcp 8081
    firewall-cmd --add-port=80/tcp --permanent
    firewall-cmd --reload
    systemctl daemon-reload
    systemctl stop nginx.service
    systemctl start nginx.service
    systemctl enable nginx.service
    systemctl start sample.service
    systemctl enable sample.service
elif [ \$1 = 2 ]; then
    echo "Upgrade installation (post)"
    systemctl daemon-reload
    systemctl start nginx.service
    systemctl start sample.service
fi

%preun
if [ \$1 = 0 ]; then
    echo "Uninstallation (preun)"
    systemctl daemon-reload
    systemctl stop nginx.service
    systemctl stop sample.service
    while [ \`ps -ef | grep "/usr/bin/sample" | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
    semanage port -d -t http_port_t -p tcp 80
    semanage port -d -t http_port_t -p tcp 8081
    firewall-cmd --remove-port=80/tcp --permanent
    firewall-cmd --reload
    systemctl disable sample.service
    systemctl start nginx.service
fi

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

if [ -e ../deployment/YaizuSample-1-1.*.rpm ]; then
    yes|rm ../deployment/YaizuSample-1-1.*.rpm
fi
if [ ! -e ../deployment ]; then
    mkdir -p ../deployment
fi
cp RPMS/x86_64/YaizuSample-1-1.*.rpm ../deployment
