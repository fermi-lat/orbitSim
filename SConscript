# -*- python -*-
# $Id: SConscript,v 1.25 2014/09/29 17:58:28 asercion Exp $
# Authors: Eric Winter <Eric.L.Winter@nasa.gov>
# Version: orbitSim-02-00-12
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
