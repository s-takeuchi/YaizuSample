#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/agent
mkdir -p $BUILDDIR/SOURCES


# Build
cd ../src/agent
make all
cp agent $BUILDDIR/SOURCES
cp agent.conf $BUILDDIR/SOURCES
echo targethost=127.0.0.1 >> $BUILDDIR/SOURCES/agent.conf
echo targetport=8080 >> $BUILDDIR/SOURCES/agent.conf
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
%{_sysconfdir}/agent.conf
%{_sysconfdir}/systemd/system/agent.service


%post
setsebool httpd_can_network_connect on -P
systemctl start agent.service
systemctl enable agent.service

%preun
systemctl stop agent.service

%postun
systemctl disable agent.service

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC
if [ -e ../deployment ]; then
    yes|rm -r ../deployment
fi
mkdir -p ../deployment
cp RPMS/x86_64/YaizuSampleAgent-1-1.*.rpm ../deployment
