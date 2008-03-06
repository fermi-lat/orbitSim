#$Id: SConscript,v 1.4 2008/02/26 05:19:02 glastrm Exp $
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
