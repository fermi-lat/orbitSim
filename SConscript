#$Id: SConscript,v 1.3 2008/02/22 01:34:34 golpa Exp $
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('orbitSimLib', depsOnly = 1)
orbitSimLib = libEnv.StaticLibrary('orbitSim', listFiles(['src/*.cxx']))

progEnv.Tool('orbitSimLib')
gttakosim = progEnv.Program('gttakosim', listFiles(['src/orbSim/*.cxx']))
test_orbitSim = progEnv.Program('test_orbitSim', listFiles(['src/test/*.cxx']))

progEnv.Tool('registerObjects', package = 'orbitSim', libraries = [orbitSimLib], includes = listFiles(['orbitSim/*.h']), binaries = [gttakosim],
             testApps = [test_orbitSim], pfiles = listFiles(['pfiles/*.par']))
