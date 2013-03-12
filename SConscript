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

# Add support for multiple build targets in the same workset
env.VariantDir('$OBJDIR', 'src', duplicate = 0)
env.VariantDir('$OBJDIR/samples', 'samples', duplicate = 0)

# The return value is the collection of files installed in the build destination.
returnValue = []

# Install headers
alljoyn_core_headers = env.Install('inc/alljoyn_c', '$DISTDIR/inc/alljoyn/DBusStdDefines.h')
alljoyn_core_headers += env.Install('inc/alljoyn_c', '$DISTDIR/inc/alljoyn/Status.h')

returnValue += alljoyn_core_headers

c_headers = env.Install('$DISTDIR/inc/alljoyn_c', env.Glob('inc/alljoyn_c/*.h'))
returnValue += c_headers

env.Depends(c_headers, alljoyn_core_headers)

# Header file includes
env.Append(CPPPATH = [env.Dir('$DISTDIR/inc/alljoyn_c')])

# Make private headers available
env.Append(CPPPATH = [env.Dir('src')])

# AllJoyn Libraries
libs = env.SConscript('$OBJDIR/SConscript')
dlibs = env.Install('$DISTDIR/lib', libs)
returnValue += dlibs

env.Append(LIBPATH = [env.Dir('$DISTDIR/lib')])
# the variable dlibs contains the file nodes for the  static library and the 
# shared library however it may contain more files such as .pdb files on windows.  
# Search through the list and assign the static library to the ALLJOYN_C_LIB_STATIC 
# env variable and the shared library to ALLJOYN_C_LIB_SHARED env variable.
for x in dlibs: 
    if env['LIBSUFFIX'] in str(x):
        env['ALLJOYN_C_LIB_STATIC'] = x;
    if env['SHLIBSUFFIX'] in str(x):
        env['ALLJOYN_C_LIB_SHARED'] = x;

# Build docs
env.SConscript('docs/SConscript')

# Sample programs
progs = env.SConscript('$OBJDIR/samples/SConscript')
returnValue += env.Install('$DISTDIR/bin/samples', progs)

# Build unit Tests
env.SConscript('unit_test/SConscript', variant_dir='$OBJDIR/unittest', duplicate=0)

Return('returnValue')
