# -*- python -*-
# $Id: SConscript,v 1.7 2008/08/25 19:05:40 peachey Exp $
# Authors: John Vernaleo <John.C.Vernaleo@nasa.gov>
# Version: orbitSim-00-01-05
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
