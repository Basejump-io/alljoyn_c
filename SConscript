# Copyright 2010 - 2011, Qualcomm Innovation Center, Inc.
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

# Install headers
# Install DBusStd.h in alljoyn_c inc folder and dist folder
dbus_header = env.Install('inc/alljoyn_c', '$DISTDIR/inc/alljoyn/DBusStd.h')
headers = env.Install('$DISTDIR/inc/alljoyn_c', env.Glob('inc/alljoyn_c/*.h'))
env.Depends(headers, dbus_header)

#Install Status.h in alljoyn_c inc folder
headers += env.Install('inc', '$DISTDIR/inc/Status.h')

# Header file includes
env.Append(CPPPATH = [env.Dir('inc')])

# Make private headers available
env.Append(CPPPATH = [env.Dir('src')])

# AllJoyn Libraries
libs = env.SConscript('$OBJDIR/SConscript')
dlibs = env.Install('$DISTDIR/lib', libs)
env.Append(LIBPATH = [env.Dir('$DISTDIR/lib')])
env['ALLJOYN_C_LIB_SHARED'] = dlibs[0]
env['ALLJOYN_C_LIB_STATIC'] = dlibs[1]

env.Depends(libs, headers)

# Build unit Tests
env.SConscript('unit_test/SConscript', variant_dir='$OBJDIR/unittest', duplicate=0)

# Sample programs
progs = env.SConscript('$OBJDIR/samples/SConscript')
env.Install('$DISTDIR/bin/samples', progs)

