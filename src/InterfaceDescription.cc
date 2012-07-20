/**
 * @file
 *
 * This file implements the InterfaceDescription class
 */

/******************************************************************************
 * Copyright 2009-2011, Qualcomm Innovation Center, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 ******************************************************************************/

#include <alljoyn/InterfaceDescription.h>
#include <alljoyn_c/InterfaceDescription.h>
#include <Status.h>

#define QCC_MODULE "ALLJOYN"

struct _alljoyn_interfacedescription_handle {
    /* Empty by design, this is just to allow the type restrictions to save coders from themselves */
};

void alljoyn_interfacedescription_activate(alljoyn_interfacedescription iface)
{
    ((ajn::InterfaceDescription*)iface)->Activate();
}

QC_BOOL alljoyn_interfacedescription_getmember(const alljoyn_interfacedescription iface, const char* name,
                                               alljoyn_interfacedescription_member* member)
{
    const ajn::InterfaceDescription::Member* found_member = ((const ajn::InterfaceDescription*)iface)->GetMember(name);
    if (found_member != NULL) {
        member->iface = (alljoyn_interfacedescription)found_member->iface;
        member->memberType = (alljoyn_messagetype)found_member->memberType;
        member->name = found_member->name.c_str();
        member->signature = found_member->signature.c_str();
        member->returnSignature = found_member->returnSignature.c_str();
        member->argNames = found_member->argNames.c_str();
        member->annotation = found_member->annotation;
        member->internal_member = found_member;
    }
    return (found_member == NULL ? QC_FALSE : QC_TRUE);
}

QStatus alljoyn_interfacedescription_addmember(alljoyn_interfacedescription iface, alljoyn_messagetype type,
                                               const char* name, const char* inputSig, const char* outSig,
                                               const char* argNames, uint8_t annotation)
{
    return ((ajn::InterfaceDescription*)iface)->AddMember((ajn::AllJoynMessageType)type, name, inputSig, outSig,
                                                          argNames, annotation);
}

size_t alljoyn_interfacedescription_getmembers(const alljoyn_interfacedescription iface,
                                               alljoyn_interfacedescription_member* members,
                                               size_t numMembers)
{
    const ajn::InterfaceDescription::Member** tempMembers = NULL;
    if (members != NULL) {
        tempMembers = new const ajn::InterfaceDescription::Member *[numMembers];
    }
    size_t ret = ((const ajn::InterfaceDescription*)iface)->GetMembers(tempMembers, numMembers);
    for (size_t i = 0; i < ret; i++) {
        members[i].iface = (alljoyn_interfacedescription)tempMembers[i]->iface;
        members[i].memberType = (alljoyn_messagetype)tempMembers[i]->memberType;
        members[i].name = tempMembers[i]->name.c_str();
        members[i].signature = tempMembers[i]->signature.c_str();
        members[i].returnSignature = tempMembers[i]->returnSignature.c_str();
        members[i].argNames = tempMembers[i]->argNames.c_str();
        members[i].annotation = tempMembers[i]->annotation;
        members[i].internal_member = tempMembers[i];
    }

    if (tempMembers != NULL) {
        delete [] tempMembers;
    }

    return ret;
}

QC_BOOL alljoyn_interfacedescription_hasmember(alljoyn_interfacedescription iface,
                                               const char* name, const char* inSig, const char* outSig)
{
    return (((ajn::InterfaceDescription*)iface)->HasMember(name, inSig, outSig) == true ? QC_TRUE : QC_FALSE);
}

QStatus alljoyn_interfacedescription_addmethod(alljoyn_interfacedescription iface, const char* name, const char* inputSig, const char* outSig, const char* argNames, uint8_t annotation, const char* accessPerms)
{
    return ((ajn::InterfaceDescription*)iface)->AddMember(ajn::MESSAGE_METHOD_CALL, name, inputSig, outSig, argNames, annotation, accessPerms);
}

QC_BOOL alljoyn_interfacedescription_getmethod(alljoyn_interfacedescription iface, const char* name, alljoyn_interfacedescription_member* member)
{
    const ajn::InterfaceDescription::Member* found_member = ((const ajn::InterfaceDescription*)iface)->GetMember(name);
    /*
     * only return the member if it is a MESSAGE_METHOD_CALL type all others return NULL
     */
    if (found_member && found_member->memberType == ajn::MESSAGE_METHOD_CALL) {
        member->iface = (alljoyn_interfacedescription)found_member->iface;
        member->memberType = (alljoyn_messagetype)found_member->memberType;
        member->name = found_member->name.c_str();
        member->signature = found_member->signature.c_str();
        member->returnSignature = found_member->returnSignature.c_str();
        member->argNames = found_member->argNames.c_str();
        member->annotation = found_member->annotation;
        member->internal_member = found_member;
    } else {
        found_member = NULL;
    }
    return (found_member == NULL ? QC_FALSE : QC_TRUE);
}

QStatus alljoyn_interfacedescription_addsignal(alljoyn_interfacedescription iface, const char* name, const char* sig, const char* argNames, uint8_t annotation, const char* accessPerms)
{
    return ((ajn::InterfaceDescription*)iface)->AddMember(ajn::MESSAGE_SIGNAL, name, sig, NULL, argNames, annotation, accessPerms);
}

QC_BOOL alljoyn_interfacedescription_getsignal(alljoyn_interfacedescription iface, const char* name, alljoyn_interfacedescription_member* member)
{
    const ajn::InterfaceDescription::Member* found_member = ((const ajn::InterfaceDescription*)iface)->GetMember(name);
    /*
     * only return the member if it is a MESSAGE_SIGNAL type all others return NULL
     */
    if (found_member && found_member->memberType == ajn::MESSAGE_SIGNAL) {
        member->iface = (alljoyn_interfacedescription)found_member->iface;
        member->memberType = (alljoyn_messagetype)found_member->memberType;
        member->name = found_member->name.c_str();
        member->signature = found_member->signature.c_str();
        member->returnSignature = found_member->returnSignature.c_str();
        member->argNames = found_member->argNames.c_str();
        member->annotation = found_member->annotation;
        member->internal_member = found_member;
    } else {
        found_member = NULL;
    }
    return (found_member == NULL ? QC_FALSE : QC_TRUE);
}

QC_BOOL alljoyn_interfacedescription_getproperty(const alljoyn_interfacedescription iface, const char* name,
                                                 alljoyn_interfacedescription_property* property)
{
    const ajn::InterfaceDescription::Property* found_prop = ((const ajn::InterfaceDescription*)iface)->GetProperty(name);
    if (found_prop != NULL) {
        property->name = found_prop->name.c_str();
        property->signature = found_prop->signature.c_str();
        property->access = found_prop->access;
        property->internal_property = found_prop;
    }
    return (found_prop == NULL ? QC_FALSE : QC_TRUE);
}

size_t alljoyn_interfacedescription_getproperties(const alljoyn_interfacedescription iface,
                                                  alljoyn_interfacedescription_property* props,
                                                  size_t numProps)
{
    const ajn::InterfaceDescription::Property** tempProps = NULL;
    if (props != NULL) {
        tempProps = new const ajn::InterfaceDescription::Property *[numProps];
    }
    size_t ret = ((const ajn::InterfaceDescription*)iface)->GetProperties(tempProps, numProps);
    for (size_t i = 0; i < ret; i++) {
        props[i].name = tempProps[i]->name.c_str();
        props[i].signature = tempProps[i]->signature.c_str();
        props[i].access = tempProps[i]->access;

        props[i].internal_property = tempProps[i];
    }

    if (tempProps != NULL) {
        delete [] tempProps;
    }

    return ret;
}

QStatus alljoyn_interfacedescription_addproperty(alljoyn_interfacedescription iface, const char* name,
                                                 const char* signature, uint8_t access)
{
    return ((ajn::InterfaceDescription*)iface)->AddProperty(name, signature, access);
}

QC_BOOL alljoyn_interfacedescription_hasproperty(const alljoyn_interfacedescription iface, const char* name)
{
    return (((const ajn::InterfaceDescription*)iface)->HasProperty(name) == true ? QC_TRUE : QC_FALSE);
}

QC_BOOL alljoyn_interfacedescription_hasproperties(const alljoyn_interfacedescription iface)
{
    return (((const ajn::InterfaceDescription*)iface)->HasProperties() == true ? QC_TRUE : QC_FALSE);
}

const char* alljoyn_interfacedescription_getname(const alljoyn_interfacedescription iface)
{
    return ((const ajn::InterfaceDescription*)iface)->GetName();
}

size_t alljoyn_interfacedescription_introspect(const alljoyn_interfacedescription iface, char* str, size_t buf, size_t indent)
{
    if (!iface) {
        return (size_t)0;
    }
    qcc::String s = ((const ajn::InterfaceDescription*)iface)->Introspect(indent);
    /*
     * it is ok to send in NULL for str when the user is only interested in the
     * size of the resulting string.
     */
    if (str) {
        strncpy(str, s.c_str(), buf);
        str[buf] = '\0'; //prevent sting not being null terminated.
    }
    return s.size();
}

QC_BOOL alljoyn_interfacedescription_issecure(const alljoyn_interfacedescription iface)
{
    return ((const ajn::InterfaceDescription*)iface)->IsSecure();
}

QC_BOOL alljoyn_interfacedescription_eql(const alljoyn_interfacedescription one,
                                         const alljoyn_interfacedescription other)
{
    const ajn::InterfaceDescription& _one = *((const ajn::InterfaceDescription*)one);
    const ajn::InterfaceDescription& _other = *((const ajn::InterfaceDescription*)other);

    return (_one == _other ? QC_TRUE : QC_FALSE);
}

QC_BOOL alljoyn_interfacedescription_member_eql(const alljoyn_interfacedescription_member one,
                                                const alljoyn_interfacedescription_member other)
{
    const ajn::InterfaceDescription::Member _one = *((ajn::InterfaceDescription::Member*)one.internal_member);
    const ajn::InterfaceDescription::Member _other = *((ajn::InterfaceDescription::Member*)other.internal_member);

    return (_one == _other ? QC_TRUE : QC_FALSE);
}

QC_BOOL alljoyn_interfacedescription_property_eql(const alljoyn_interfacedescription_property one,
                                                  const alljoyn_interfacedescription_property other)
{
    const ajn::InterfaceDescription::Property _one = *((ajn::InterfaceDescription::Property*)one.internal_property);
    const ajn::InterfaceDescription::Property _other = *((ajn::InterfaceDescription::Property*)other.internal_property);

    return (_one == _other ? QC_TRUE : QC_FALSE);
}

