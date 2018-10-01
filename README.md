# Standalone gtorbsim

## Install instructions

Installation requires Cmake 3.11 and an active internet connection that can
reach github.com. The approach

 * `git clone -b standalone https://github.com/fermi-lat/orbitSim.git`
 * `mkdir orbitSim/_build && cd orbitSim/_build`
 * `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/.local ..`
 * `make -j 4`
 * `make install`
 * `export PFILES=${HOME}/pfiles:${PFILES}`
 * `export FERMI_INST_DIR=${HOME}/.local`

