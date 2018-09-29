# Standalone GTORBSIM


Requires cmake 3.11 and an active internet connection that can reach github.com

# Install instructions

`mkdir _build && cd _build`
`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/.local ..`
`make -j 4`
`make install`
`export PFILES=${HOME}/pfiles`

