# -*- python -*-
# $Id: SConscript,v 1.18 2010/02/22 20:51:35 jrb Exp $
# Authors: John Vernaleo <John.C.Vernaleo@nasa.gov>
# Version: orbitSim-02-00-05
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
