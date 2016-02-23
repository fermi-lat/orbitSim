# -*- python -*-
# $Id: SConscript,v 1.27 2014/10/09 17:58:41 asercion Exp $
# Authors: Eric Winter <Eric.L.Winter@nasa.gov>
# Version: orbitSim-03-00-00
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
             data = listFiles(['data/*'], recursive=True),
             pfiles = listFiles(['pfiles/*.par']))
