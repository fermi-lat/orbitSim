# -*- python -*-
# $Id: SConscript,v 1.13 2009/06/23 18:31:23 glastrm Exp $
# Authors: John Vernaleo <John.C.Vernaleo@nasa.gov>
# Version: orbitSim-02-00-01
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('orbitSimLib', depsOnly = 1)
orbitSimLib = libEnv.StaticLibrary('orbitSim', listFiles(['src/*.cxx']))

progEnv.Tool('orbitSimLib')
gtorbsim = progEnv.Program('gtorbsim', listFiles(['src/orbSim/*.cxx']))

progEnv.Tool('registerTargets', package = 'orbitSim',
             staticLibraryCxts = [[orbitSimLib, libEnv]],
             includes = listFiles(['orbitSim/*.h']),
             binaryCxts = [[gtorbsim, progEnv]],
             pfiles = listFiles(['pfiles/*.par']))
