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
cp sample.dat $BUILDDIR/SOURCES
cd ../../../YaizuComLib/src/stkwebapp
make all
cp stkwebappstop $BUILDDIR/SOURCES/samplestop


# Make SPEC file
cd $BUILDDIR
SPEC=sample.spec
cat <<EOF > ./$SPEC
Name:    YaizuSample
Version: 1
Release: 1%{?dist}
Summary: YaizuSample

License: No License No Life
Source1: sample
Source2: sample.conf
Source4: sample.service
Source23: sample.dat
Source24: samplestop

%description
YaizuSample!!

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system

install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE4} %{buildroot}/%{_sysconfdir}/systemd/system
install -p -m 644 %{SOURCE23} %{buildroot}/%{_sysconfdir}
install -p -m 755 %{SOURCE24} %{buildroot}/%{_bindir}


%files
%{_bindir}/sample
%config(noreplace) %{_sysconfdir}/sample.conf
%config(noreplace) %{_sysconfdir}/systemd/system/sample.service
%config(noreplace) %{_sysconfdir}/sample.dat
%{_bindir}/samplestop

%pre
if [ \$1 = 2 ]; then
    echo "Upgrade installation (pre)"
    systemctl daemon-reload
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
    semanage port -a -t http_port_t -p tcp 8081
    firewall-cmd --add-port=8081/tcp --permanent
    firewall-cmd --reload
    systemctl daemon-reload
    systemctl start sample.service
    systemctl enable sample.service
elif [ \$1 = 2 ]; then
    echo "Upgrade installation (post)"
    systemctl daemon-reload
    systemctl start sample.service
fi

%preun
if [ \$1 = 0 ]; then
    echo "Uninstallation (preun)"
    systemctl daemon-reload
    systemctl stop sample.service
    while [ \`ps -ef | grep "/usr/bin/sample" | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
    semanage port -d -t http_port_t -p tcp 8081
    firewall-cmd --remove-port=8081/tcp --permanent
    firewall-cmd --reload
    systemctl disable sample.service
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
