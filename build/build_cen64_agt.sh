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
cp agent $BUILDDIR/SOURCES
cp agent.conf $BUILDDIR/SOURCES
cp agent.service $BUILDDIR/SOURCES


# Make SPEC file
cd $BUILDDIR
SPEC=agent.spec
cat <<EOF > ./$SPEC
Name:    YaizuSampleAgent
Version: 1
Release: 1%{?dist}
Summary: YaizuSampleAgent

License: No License No Life
Source1: agent
Source2: agent.conf
Source3: agent.service

%description
YaizuSampleAgent!!

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system

install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/systemd/system

%files
%{_bindir}/agent
%config(noreplace) %{_sysconfdir}/agent.conf
%config(noreplace) %{_sysconfdir}/systemd/system/agent.service

%pre
if [ \$1 = 2 ]; then
    echo "Upgrade installation (pre)"
    systemctl daemon-reload
    systemctl stop agent.service
    while [ \`ps -ef | grep "/usr/bin/agent" | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
fi

%post
if [ \$1 = 1 ]; then
    echo "New installation (post)"
    mkdir -p %{_datadir}/serval
    echo pathtobucket=%{_datadir}/serval >> %{_sysconfdir}/agent.conf
    setsebool httpd_can_network_connect on -P
    systemctl daemon-reload
    systemctl start agent.service
    systemctl enable agent.service
elif [ \$1 = 2 ]; then
    echo "Upgrade installation (post)"
    systemctl daemon-reload
    systemctl start agent.service
fi

%preun
if [ \$1 = 0 ]; then
    echo "Uninstallation (preun)"
    systemctl stop agent.service
    while [ \`ps -ef | grep "/usr/bin/agent" | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
    systemctl disable agent.service
fi

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

if [ -e ../deployment/YaizuSampleAgent-1-1.*.rpm ]; then
    yes|rm ../deployment/YaizuSampleAgent-1-1.*.rpm
fi
if [ ! -e ../deployment ]; then
    mkdir -p ../deployment
fi
cp RPMS/x86_64/YaizuSampleAgent-1-1.*.rpm ../deployment
