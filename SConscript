# -*- python -*-
# $Id: SConscript,v 1.29 2016/02/23 16:38:03 jchiang Exp $
# Authors: Eric Winter <Eric.L.Winter@nasa.gov>, Alex Reustle <alexander.reustle@nasa.gov>
# Version: orbitSim-03-00-01
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
