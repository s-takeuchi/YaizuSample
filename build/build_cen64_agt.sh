#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/agent
if [ -e $BUILDDIR ]; then
    yes|rm -r $BUILDDIR
fi
mkdir -p $BUILDDIR/SOURCES


# Build
cd ../src/agent
make all
cp servalagt $BUILDDIR/SOURCES
cp servalagt.conf $BUILDDIR/SOURCES
cp servalagt.service $BUILDDIR/SOURCES


# Make SPEC file
cd $BUILDDIR
SPEC=agent.spec
cat <<EOF > ./$SPEC
Name:    servalagt
Version: 1.0
Release: 0.el8
Summary: SERVAL agent

License: No License No Life
Source1: servalagt
Source2: servalagt.conf
Source3: servalagt.service

%description
SERVAL agent

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system

install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/systemd/system

%files
%{_bindir}/servalagt
%config(noreplace) %{_sysconfdir}/servalagt.conf
%config(noreplace) %{_sysconfdir}/systemd/system/servalagt.service

%pre
if [ \$1 = 2 ]; then
    echo "Upgrade installation (pre)"
    systemctl daemon-reload
    systemctl stop servalagt.service
    while [ \`ps -ef | grep "/usr/bin/servalagt " | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
fi

%post
if [ \$1 = 1 ]; then
    echo "New installation (post)"
    mkdir -p %{_datadir}/servalagt
    echo workdir=%{_datadir}/servalagt >> %{_sysconfdir}/servalagt.conf
    echo scriptencode=UTF8 >> %{_sysconfdir}/servalagt.conf
    setsebool httpd_can_network_connect on -P
    systemctl daemon-reload
    systemctl start servalagt.service
    systemctl enable servalagt.service
elif [ \$1 = 2 ]; then
    echo "Upgrade installation (post)"
    systemctl daemon-reload
    systemctl start servalagt.service
fi

%preun
if [ \$1 = 0 ]; then
    echo "Uninstallation (preun)"
    systemctl stop servalagt.service
    while [ \`ps -ef | grep "/usr/bin/servalagt " | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
    systemctl disable servalagt.service
fi

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

if [ -e ../deployment/servalagt-1.*.rpm ]; then
    yes|rm ../deployment/servalagt-1.*.rpm
fi
if [ ! -e ../deployment ]; then
    mkdir -p ../deployment
fi
cp RPMS/x86_64/servalagt-1.*.rpm ../deployment
