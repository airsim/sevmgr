C++ Simulation-Oriented Discrete Event Management Library
=========================================================

# Summary
SEvMgr is a C++ library of discrete event queue management classes and
functions, exclusively targeting simulation purposes.

SEvMgr makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the 
Boost (C++ Standard Extensions: https://www.boost.org) library is used.

SEvMgr is the one of the components of the Travel Market Simulator
(https://travel-sim.org). However, it may be used in a
stand-alone mode.

# Installation

## On Fedora/CentOS/RedHat distribution:
Just use DNF (or Yum on older distributions):
```bash
$ dnf -y install sevmgr-devel sevmgr-doc
```

You can also get the RPM packages (which may work on Linux
distributions like Novel Suse and Mandriva) from the Fedora repository
(_e.g._, for Fedora Rawhide, 
https://fr2.rpmfind.net/linux/RPM/fedora/devel/rawhide/x86_64/)


## Building the library and test binary from Git repository
The Git repository may be cloned as following:
```bash
$ git clone git@github.com:airsim/sevmgr.git sevmgrgit # through SSH
$ git clone https://github.com/airsim/sevmgr.git # if the firewall filters SSH
$ cd sevmgrgit
```

Then, you need the following packages (Fedora/RedHat/CentOS names here, 
but names may vary according to distributions):
* cmake
* gcc-c++
* boost-devel / libboost-dev
* python-devel / python-dev
* gettext-devel / gettext-dev
* sqlite3-devel / libsqlite3-dev
* readline-devel / readline-dev
* ncurses-devel
* soci-mysql-devel, soci-sqlite3-devel
* stdair-devel / libstdair-dev
* doxygen, ghostscript, graphviz
* tetex-latex (optional)
* rpm-build (optional)


## Building the library and test binary from the tarball
The latest stable source tarball (`sevmgr*.tar.gz` or `.bz2`) can be
found on GitHub: http://github.com/airsim/sevmgr/releases

To customise the following to your environment, you can alter the path
to the installation directory:
```bash
export INSTALL_BASEDIR="${HOME}/dev/deliveries"
export SEVMGR_VER="1.00.12"
```

Then, as usual:
* To configure the project
  * When dependencies are installed in standard directories (_e.g._, `/usr`):
```bash
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/sevmgr-${SEVMGR_VER} \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON \
   -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF ..
```
  * When dependencies are installed in non-standard directories (_e.g._,
    in the same base directory as of RMOL, that is, `${INSTALL_BASEDIR}`):
```bash
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/sevmgr-${SEVMGR_VER} \
   -DWITH_STDAIR_PREFIX=${INSTALL_BASEDIR}/stdair-stable \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON \
   -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF ..
```
* To build the project, type:
```bash
  make
```
* To test the project, type:
```bash
  make check
```
* To install the library (`libsevmgr*.so*`) and the binary (`sevmgr`),
  just type:
```bash
  make install
  cd ${INSTALL_BASEDIR}
  rm -f sevmgr-stable && ln -s sevmgr-${SEVMGR_VER} sevmgr-stable
  cd -
```
* To package the source files, type:
```bash
  make dist
```
* To package the binary and the (HTML and PDF) documentation:
```bash
  make package
```
* To browse the (just installed, if enabled) HTML documentation:
```bash
  midori file://${INSTALL_BASEDIR}/sevmgr-$SEVMGR_VER/share/doc/sevmgr/html/index.html
```
* To browse the (just installed, if enabled) PDF documentation:
```bash
  evince ${INSTALL_BASEDIR}/sevmgr-$SEVMGR_VER/share/doc/sevmgr/html/refman.pdf
```
* To run the local binary version:
```bash
  ./sevmgr/sevmgr -b
```
* To run the installed version:
```bash
  ${INSTALL_BASEDIR}/sevmgr-$SEVMGR_VER/bin/sevmgr -b
```

Denis Arnaud

