#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/server
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
cp sample.js $BUILDDIR/SOURCES
cp sample.dat $BUILDDIR/SOURCES
cp squirrel.svg $BUILDDIR/SOURCES
cp favicon-16x16.png $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/stkcommon.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/stkcommon.css $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp_um/stkcommon_um.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/jquery-3.2.0.min.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/bootstrap-4.4.1-dist.zip $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/IcoMoon-Free.css $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/IcoMoon-Free.ttf $BUILDDIR/SOURCES
cd ../../../YaizuComLib/src/stkwebapp
make all
cp stkwebappstop $BUILDDIR/SOURCES/stopsample
cd $BUILDDIR/SOURCES
unzip $BUILDDIR/SOURCES/bootstrap-4.4.1-dist.zip
cp $BUILDDIR/SOURCES/bootstrap-4.4.1-dist/css/* $BUILDDIR/SOURCES
cp $BUILDDIR/SOURCES/bootstrap-4.4.1-dist/js/* $BUILDDIR/SOURCES


# Make SPEC file
cd $BUILDDIR
SPEC=sample.spec
cat <<EOF > ./$SPEC
Name:    serval
Version: 1.0
Release: 0.el8
Summary: SERVAL server
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
Source11: bootstrap-grid.css
Source12: bootstrap-grid.css.map
Source13: bootstrap-grid.min.css
Source14: bootstrap-grid.min.css.map
Source15: bootstrap-reboot.css
Source16: bootstrap-reboot.css.map
Source17: bootstrap-reboot.min.css
Source18: bootstrap-reboot.min.css.map
Source23: sample.dat
Source24: stopsample
Source25: stkcommon.js
Source26: stkcommon.css
Source27: stkcommon_um.js
Source28: sample.js
Source30: bootstrap.bundle.js
Source31: bootstrap.bundle.js.map
Source32: bootstrap.bundle.min.js
Source33: bootstrap.bundle.min.js.map
Source34: bootstrap.js
Source35: bootstrap.js.map
Source36: bootstrap.min.js
Source37: bootstrap.min.js.map
Source40: IcoMoon-Free.css
Source41: IcoMoon-Free.ttf
Source42: squirrel.svg
Source43: favicon-16x16.png

%description
SERVAL server

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/nginx/conf.d
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/img
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js

install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/nginx/conf.d
install -p -m 644 %{SOURCE4} %{buildroot}/%{_sysconfdir}/systemd/system
install -p -m 644 %{SOURCE5} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE6} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE7} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE8} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE9} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE10} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE11} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE12} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE13} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE14} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE15} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE16} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE17} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE18} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE23} %{buildroot}/%{_sysconfdir}
install -p -m 755 %{SOURCE24} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE25} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE26} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE27} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE28} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE30} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE31} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE32} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE33} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE34} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE35} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE36} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE37} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE40} %{buildroot}/%{_datarootdir}/nginx/html/IcoMoon-Free.css
install -p -m 644 %{SOURCE41} %{buildroot}/%{_datarootdir}/nginx/html/IcoMoon-Free.ttf
install -p -m 644 %{SOURCE42} %{buildroot}/%{_datarootdir}/nginx/html/squirrel.svg
install -p -m 644 %{SOURCE43} %{buildroot}/%{_datarootdir}/nginx/html/img/favicon-16x16.png


%files
%{_bindir}/sample
%config(noreplace) %{_sysconfdir}/sample.conf
%{_sysconfdir}/nginx/conf.d/sample_nginx.conf
%config(noreplace) %{_sysconfdir}/systemd/system/sample.service
%{_datarootdir}/nginx/html/sample.html
%{_datarootdir}/nginx/html/sample.js
%{_datarootdir}/nginx/html/stkcommon.js
%{_datarootdir}/nginx/html/stkcommon.css
%{_datarootdir}/nginx/html/stkcommon_um.js
%{_datarootdir}/nginx/html/jquery-3.2.0.min.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.min.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.min.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.min.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.min.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.min.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.min.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.js.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.min.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.min.js.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.js.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.min.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.min.js.map
%config(noreplace) %{_sysconfdir}/sample.dat
%{_bindir}/stopsample
%{_datarootdir}/nginx/html/IcoMoon-Free.css
%{_datarootdir}/nginx/html/IcoMoon-Free.ttf
%{_datarootdir}/nginx/html/squirrel.svg
%{_datarootdir}/nginx/html/img/favicon-16x16.png


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
    mkdir -p %{_datadir}/serval
    echo workdir=%{_datadir}/serval >> %{_sysconfdir}/sample.conf
    setsebool httpd_can_network_connect on -P
    semanage port -a -t http_port_t -p tcp 8080
    semanage port -a -t http_port_t -p tcp 8081
    if [ -e /usr/bin/firewall-cmd ]; then
        firewall-cmd --add-port=8080/tcp --permanent
        firewall-cmd --reload
    fi
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
    semanage port -d -t http_port_t -p tcp 8080
    semanage port -d -t http_port_t -p tcp 8081
    if [ -e /usr/bin/firewall-cmd ]; then
        firewall-cmd --remove-port=8080/tcp --permanent
        firewall-cmd --reload
    fi
    systemctl disable sample.service
    systemctl start nginx.service
fi

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

if [ -e ../deployment/serval-1.*.rpm ]; then
    yes|rm ../deployment/serval-1.*.rpm
fi
if [ ! -e ../deployment ]; then
    mkdir -p ../deployment
fi
cp RPMS/x86_64/serval-1.*.rpm ../deployment
