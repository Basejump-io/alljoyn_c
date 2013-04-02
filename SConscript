# Copyright 2010 - 2013, Qualcomm Innovation Center, Inc.
# 
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
# 
#        http://www.apache.org/licenses/LICENSE-2.0
# 
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
# 

import os
import sys

Import('env')

vars = Variables();

vars.Update(env)

Help(vars.GenerateHelpText(env))

sys.path.append('../build_core/tools/scons')

# Dependent Projects
if not env.has_key('_ALLJOYNCORE_'):
    env.SConscript('../alljoyn_core/SConscript')

# Make alljoyn C dist a sub-directory of the alljoyn dist.
env['C_DISTDIR'] = env['DISTDIR'] + '/c'
env['C_TESTDIR'] = env['TESTDIR'] + '/c'

# Add support for multiple build targets in the same workset
env.VariantDir('$OBJDIR', 'src', duplicate = 0)
env.VariantDir('$OBJDIR/samples', 'samples', duplicate = 0)

# The return value is the collection of files installed in the build destination.
returnValue = []

# Install headers
# C bindings use the following headers from alljoyn_core
alljoyn_core_headers = env.Install('inc/alljoyn_c', '$CPP_DISTDIR/inc/alljoyn/DBusStdDefines.h')
alljoyn_core_headers += env.Install('inc/alljoyn_c', '$CPP_DISTDIR/inc/alljoyn/Status.h')
alljoyn_core_headers += env.Install('inc/qcc', '$CPP_DISTDIR/inc/qcc/platform.h')
alljoyn_core_headers += env.Install('inc/qcc/${OS_GROUP}', '$CPP_DISTDIR/inc/qcc/${OS_GROUP}/platform_types.h')

if env['OS_GROUP'] == 'windows' or env['OS_GROUP'] == 'win8':
    alljoyn_core_headers += env.Install('inc/qcc/${OS_GROUP}', '$CPP_DISTDIR/inc/qcc/${OS_GROUP}/mapping.h')

returnValue += alljoyn_core_headers

c_headers = env.Install('$C_DISTDIR/inc/alljoyn_c', env.Glob('inc/alljoyn_c/*.h'))
c_headers += env.Install('$C_DISTDIR/inc/qcc', env.Glob('inc/qcc/*.h'))
c_headers += env.Install('$C_DISTDIR/inc/qcc/${OS_GROUP}', env.Glob('inc/qcc/${OS_GROUP}/*.h'))
returnValue += c_headers

# make sure the headers from alljoyn_core have been copied into the alljoyn_c headers 
env.Depends(c_headers, alljoyn_core_headers)

# Header file includes
env.Append(CPPPATH = [env.Dir('inc')])

# Make private headers available
env.Append(CPPPATH = [env.Dir('src')])

# AllJoyn Libraries
libs = env.SConscript('$OBJDIR/SConscript')
dlibs = env.Install('$C_DISTDIR/lib', libs)
returnValue += dlibs

env.Append(LIBPATH = [env.Dir('$C_DISTDIR/lib')])
# the variable dlibs contains the file nodes for the  static library and the 
# shared library however it may contain more files such as .pdb files on windows.  
# Search through the list and assign the static library to the ALLJOYN_C_LIB_STATIC 
# env variable and the shared library to ALLJOYN_C_LIB_SHARED env variable.
for x in dlibs: 
    if env['OS_GROUP'] == 'windows' or env['OS_GROUP'] == 'winrt':
        if 'alljoyn_c_static' + env['LIBSUFFIX'] in str(x):
            env['ALLJOYN_C_LIB_STATIC'] = x;
    else:
        if 'alljoyn_c' + env['LIBSUFFIX'] in str(x):
            env['ALLJOYN_C_LIB_STATIC'] = x;
    if 'alljoyn_c' + env['SHLIBSUFFIX'] in str(x):
        env['ALLJOYN_C_LIB_SHARED'] = x;

# Build docs
env.SConscript('docs/SConscript')

# Sample programs
progs = env.SConscript('$OBJDIR/samples/SConscript')
returnValue += progs

# Build unit Tests
env.SConscript('unit_test/SConscript', variant_dir='$OBJDIR/unittest', duplicate=0)

# don't try and build the samples till the header files are copied into place
env.Depends(dlibs, c_headers)

Return('returnValue')
