# Standalone gtorbsim

## Install instructions

Installation requires Cmake 3.11 and an active internet connection that can
reach github.com. The approach

 * `git clone -b standalone https://github.com/fermi-lat/orbitSim.git`
 * `mkdir orbitSim/_build && cd orbitSim/_build`
 * `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/.local ..`
 * `make -j 4`
 * `make install`

The binaries are now installed to `~/.local/bin`. You can access them directly
or add to your path `export PATH=${PATH}:~/.local/bin`.

If you do not have a past gtorbsim par file you can copy the once provided in
`~/.local/share/pfiles` to your `~/pfiles` directory.


 * `export PFILES=${HOME}/pfiles`



 * `export FERMI_INST_DIR=${HOME}/.local`

