# -*- python -*-
# $Id: SConscript,v 1.10 2008/09/25 18:31:00 glastrm Exp $
# Authors: John Vernaleo <John.C.Vernaleo@nasa.gov>
# Version: orbitSim-01-00-01
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('orbitSimLib', depsOnly = 1)
orbitSimLib = libEnv.StaticLibrary('orbitSim', listFiles(['src/*.cxx']))

progEnv.Tool('orbitSimLib')
gtorbsim = progEnv.Program('gtorbsim', listFiles(['src/orbSim/*.cxx']))

progEnv.Tool('registerObjects', package = 'orbitSim', libraries = [orbitSimLib], includes = listFiles(['orbitSim/*.h']), binaries = [gtorbsim],
             pfiles = listFiles(['pfiles/*.par']))
