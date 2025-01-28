#! /usr/bin/bash

# source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8/x86_64-centos7/setup.sh
source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_106 x86_64-el9-gcc13-opt
# source /cvmfs/geant4.cern.ch/geant4/11.0_cand01/x86_64-centos7-gcc8-optdeb-MT/CMake-setup.sh
export PATH=/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.17.1/Linux-x86_64/bin/:${PATH}
export LD_LIBRARY_PATH=$HOME/local/lib:$LD_LIBRARY_PATH
export QT_QPA_PLATFORM='minimal'