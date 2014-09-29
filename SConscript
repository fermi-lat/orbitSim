# -*- python -*-
# $Id: SConscript,v 1.24 2014/09/25 19:20:24 asercion Exp $
# Authors: Eric Winter <Eric.L.Winter@nasa.gov>
# Version: orbitSim-02-00-11
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

orbitSimLib = libEnv.StaticLibrary('orbitSim', listFiles(['src/*.cxx']))

progEnv.Tool('orbitSimLib')
gtorbsim = progEnv.Program('gtorbsim', listFiles(['src/orbSim/*.cxx']))

progEnv.Tool('registerTargets', package = 'orbitSim',
             staticLibraryCxts = [[orbitSimLib, libEnv]],
             includes = listFiles(['orbitSim/*.h']),
             binaryCxts = [[gtorbsim, progEnv]],
             pfiles = listFiles(['pfiles/*.par']))
