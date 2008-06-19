# -*- python -*-
# $Id: SConscript,v 1.5 2008/03/06 20:37:05 glastrm Exp $
# Authors: Giuseppe Romeo <Giuseppe.Romeo@gsfc.nasa.gov>
# Version: orbitSim-00-01-04
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('orbitSimLib', depsOnly = 1)
orbitSimLib = libEnv.StaticLibrary('orbitSim', listFiles(['src/*.cxx']))

progEnv.Tool('orbitSimLib')
gttakosim = progEnv.Program('gttakosim', listFiles(['src/orbSim/*.cxx']))

progEnv.Tool('registerObjects', package = 'orbitSim', libraries = [orbitSimLib], includes = listFiles(['orbitSim/*.h']), binaries = [gttakosim],
             pfiles = listFiles(['pfiles/*.par']))
