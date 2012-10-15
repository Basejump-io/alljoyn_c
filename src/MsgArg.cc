/**
 * @file
 *
 * This file implements the MsgArg class
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

#include <qcc/platform.h>

#include <cstdarg>
#include <assert.h>

#include <stdio.h>

#include <alljoyn/Message.h>
#include <alljoyn/MsgArg.h>
#include <alljoyn_c/Message.h>
#include <alljoyn_c/MsgArg.h>
#include "MsgArgC.h"

#define QCC_MODULE "ALLJOYN"

struct _alljoyn_msgarg_handle {
    /* Empty by design, this is just to allow the type restrictions to save coders from themselves */
};

//struct _alljoyn_msgarg_array_handle {
//    /* Empty by design, this is just to allow the type restrictions to save coders from themselves */
//};

alljoyn_msgarg alljoyn_msgarg_create() {
    ajn::MsgArgC* arg = new ajn::MsgArgC[1];
    return (alljoyn_msgarg)arg;
}

alljoyn_msgarg alljoyn_msgarg_create_and_set(const char* signature, ...)
{
    ajn::MsgArgC* arg = new ajn::MsgArgC[1];
    arg->typeId = ajn::ALLJOYN_INVALID;
    va_list argp;
    va_start(argp, signature);
    QStatus status = ER_OK;

    ((ajn::MsgArgC*)arg)->Clear();
    size_t sigLen = (signature ? strlen(signature) : 0);
    if ((sigLen < 1) || (sigLen > 255)) {
        status = ER_BUS_BAD_SIGNATURE;
    } else {
        status = ((ajn::MsgArgC*)arg)->VBuildArgsC(signature, sigLen, ((ajn::MsgArgC*)arg), 1, &argp);
        if ((status == ER_OK) && (*signature != 0)) {
            status = ER_BUS_NOT_A_COMPLETE_TYPE;
        }
    }
    va_end(argp);
    return (alljoyn_msgarg)arg;
}

void alljoyn_msgarg_destroy(alljoyn_msgarg arg)
{
    if (arg != NULL) {
        delete [] (ajn::MsgArgC*)arg;
    }
}

alljoyn_msgarg alljoyn_msgarg_array_create(size_t size)
{
    ajn::MsgArgC* args = new ajn::MsgArgC[size];
    for (size_t i = 0; i < size; i++) {
        args[i].Clear();
    }
    return (alljoyn_msgarg)args;
}

//void  alljoyn_msgarg_array_destroy(alljoyn_msgarg_array arg)
//{
//    if (arg != NULL) {
//        delete [] (ajn::MsgArgC*)arg;
//    }
//}

alljoyn_msgarg alljoyn_msgarg_array_element(alljoyn_msgarg arg, size_t index)
{
    if (!arg) {
        return NULL;
    }
    ajn::MsgArgC* array_arg = (ajn::MsgArgC*)arg;
    return (alljoyn_msgarg)(&array_arg[index]);
}

QStatus alljoyn_msgarg_set(alljoyn_msgarg arg, const char* signature, ...)
{
    if (!arg) {
        return ER_BAD_ARG_1;
    }
    va_list argp;
    va_start(argp, signature);
    QStatus status = ER_OK;

    ((ajn::MsgArgC*)arg)->Clear();
    size_t sigLen = (signature ? strlen(signature) : 0);
    if ((sigLen < 1) || (sigLen > 255)) {
        status = ER_BUS_BAD_SIGNATURE;
    } else {
        status = ((ajn::MsgArgC*)arg)->VBuildArgsC(signature, sigLen, ((ajn::MsgArgC*)arg), 1, &argp);
        if ((status == ER_OK) && (*signature != 0)) {
            status = ER_BUS_NOT_A_COMPLETE_TYPE;
        }
    }
    va_end(argp);
    return status;
}

QStatus alljoyn_msgarg_get(alljoyn_msgarg arg, const char* signature, ...)
{
    if (!arg) {
        return ER_BAD_ARG_1;
    }
    size_t sigLen = (signature ? strlen(signature) : 0);
    if (sigLen == 0) {
        return ER_BAD_ARG_2;
    }
    va_list argp;
    va_start(argp, signature);
    QStatus status = ((ajn::MsgArgC*)arg)->VParseArgsC(signature, sigLen, ((ajn::MsgArgC*)arg), 1, &argp);
    va_end(argp);
    return status;
}

alljoyn_msgarg alljoyn_msgarg_copy(const alljoyn_msgarg source)
{
    if (!source) {
        return NULL;
    }
    ajn::MsgArgC* ret = new ajn::MsgArgC[1];
    *ret = *(ajn::MsgArgC*)source;
    return (alljoyn_msgarg) ret;
}

QCC_BOOL alljoyn_msgarg_equal(alljoyn_msgarg lhv, alljoyn_msgarg rhv)
{
    if (!lhv || !rhv) {
        return QCC_FALSE;
    }
    return (*(ajn::MsgArgC*)lhv) == (*(ajn::MsgArgC*)rhv);
}

QStatus alljoyn_msgarg_array_set(alljoyn_msgarg args, size_t* numArgs, const char* signature, ...)
{
    if (!args) {
        return ER_BAD_ARG_1;
    }
    va_list argp;
    va_start(argp, signature);
    QStatus status = ((ajn::MsgArgC*)args)->MsgArgUtilsSetVC(((ajn::MsgArgC*)args), *numArgs, signature, &argp);
    va_end(argp);
    return status;
}

QStatus alljoyn_msgarg_array_get(const alljoyn_msgarg args, size_t numArgs, const char* signature, ...)
{
    if (!args) {
        return ER_BAD_ARG_1;
    }
    if (!numArgs) {
        return ER_BAD_ARG_2;
    }
    size_t sigLen = (signature ? strlen(signature) : 0);
    if (sigLen == 0) {
        return ER_BAD_ARG_3;
    }
    va_list argp;
    va_start(argp, signature);
    QStatus status = ((ajn::MsgArgC*)args)->VParseArgsC(signature, sigLen, ((ajn::MsgArgC*)args), numArgs, &argp);
    va_end(argp);
    return status;
}

size_t alljoyn_msgarg_tostring(alljoyn_msgarg arg, char* str, size_t buf, size_t indent)
{
    if (!arg) {
        return (size_t)0;
    }
    qcc::String s = ((ajn::MsgArgC*)arg)->ToString(indent);
    /*
     * it is ok to send in NULL for str when the user is only interested in the
     * size of the resulting string.
     */
    if (str) {
        strncpy(str, s.c_str(), buf);
        str[buf - 1] = '\0'; //prevent sting not being null terminated.
    }
    return s.size() + 1;
}

size_t alljoyn_msgarg_array_tostring(const alljoyn_msgarg args, size_t numArgs, char* str, size_t buf, size_t indent)
{
    if (!args) {
        return (size_t)0;
    }
    qcc::String s = ((ajn::MsgArgC*)args)->ToString((ajn::MsgArgC*)args, numArgs, indent);
    /*
     * it is ok to send in NULL for str when the user is only interested in the
     * size of the resulting string.
     */
    if (str) {
        strncpy(str, s.c_str(), buf);
        str[buf - 1] = '\0'; //prevent sting not being null terminated.
    }
    return s.size() + 1;
}

size_t alljoyn_msgarg_signature(alljoyn_msgarg arg, char* str, size_t buf)
{
    if (!arg) {
        return (size_t)0;
    }
    qcc::String s = ((ajn::MsgArgC*)arg)->Signature();
    /*
     * it is ok to send in NULL for str when the user is only interested in the
     * size of the resulting string.
     */
    if (str) {
        strncpy(str, s.c_str(), buf);
        str[buf - 1] = '\0'; //prevent sting not being null terminated.
    }
    return s.size() + 1;
}

size_t alljoyn_msgarg_array_signature(alljoyn_msgarg values, size_t numValues, char* str, size_t buf)
{
    if (!values) {
        return (size_t)0;
    }
    qcc::String s = ((ajn::MsgArgC*)values)->Signature((ajn::MsgArgC*)values, numValues).c_str();
    /*
     * it is ok to send in NULL for str when the user is only interested in the
     * size of the resulting string.
     */
    if (str) {
        strncpy(str, s.c_str(), buf);
        str[buf - 1] = '\0'; //prevent sting not being null terminated.
    }
    return s.size() + 1;

}

QCC_BOOL alljoyn_msgarg_hassignature(alljoyn_msgarg arg, const char* signature)
{
    if (!arg) {
        return QCC_FALSE;
    }
    return ((ajn::MsgArgC*)arg)->HasSignature(signature);
}

QStatus alljoyn_msgarg_getdictelement(alljoyn_msgarg arg, const char* elemSig, ...)
{
    if (!arg) {
        return ER_BAD_ARG_1;
    }
    size_t sigLen = (elemSig ? strlen(elemSig) : 0);
    if (sigLen < 4) {
        return ER_BAD_ARG_2;
    }

    /* Check this MsgArg is an array of dictionary entries */
    if ((((ajn::MsgArgC*)arg)->typeId != ajn::ALLJOYN_ARRAY || ((ajn::MsgArgC*)arg)->v_array.GetElemSig()[0] != '{')) {
        return ER_BUS_NOT_A_DICTIONARY;
    }
    /* Check key has right type */
    if (((ajn::MsgArgC*)arg)->v_array.GetElemSig()[1] != elemSig[1]) {
        return ER_BUS_SIGNATURE_MISMATCH;
    }
    va_list argp;
    va_start(argp, elemSig);
    /* Get the key as a MsgArg */
    ajn::MsgArgC key;
    size_t numArgs;
    ++elemSig;
    QStatus status = ((ajn::MsgArgC*)arg)->VBuildArgsC(elemSig, 1, &key, 1, &argp, &numArgs);
    if (status == ER_OK) {
        status = ER_BUS_ELEMENT_NOT_FOUND;
        /* Linear search to match the key */
        const ajn::MsgArg* entry = ((ajn::MsgArgC*)arg)->v_array.GetElements();
        for (size_t i = 0; i < ((ajn::MsgArgC*)arg)->v_array.GetNumElements(); ++i, ++entry) {
            if (*entry->v_dictEntry.key == key) {
                status = ER_OK;
                break;
            }
        }
        if (status == ER_OK) {
            status = ((ajn::MsgArgC*)arg)->VParseArgsC(elemSig, sigLen - 3, entry->v_dictEntry.val, 1, &argp);
        }
    }
    va_end(argp);
    return status;
}

void alljoyn_msgarg_clear(alljoyn_msgarg arg)
{
    if (!arg) {
        return;
    }
    ((ajn::MsgArgC*)arg)->Clear();
}

AllJoynTypeId alljoyn_msgarg_gettype(alljoyn_msgarg arg)
{
    if (!arg) {
        return ALLJOYN_INVALID;
    }
    return (AllJoynTypeId)((ajn::MsgArgC*)arg)->typeId;
}

void alljoyn_msgarg_stabilize(alljoyn_msgarg arg)
{
    if (!arg) {
        return;
    }
    ((ajn::MsgArgC*)arg)->Stabilize();
}

/*******************************************************************************
 * This set of functions were originally designed for the alljoyn_unity bindings
 * however they did not not properly map with the C++ MsgArg Class.  These calls
 * are being left in here till proper mapping between 'C' and 'unity' can be
 * completed. And can be verified that the code continues to work.
 ******************************************************************************/

QStatus alljoyn_msgarg_array_set_offset(alljoyn_msgarg args, size_t argOffset, size_t* numArgs, const char* signature, ...)
{
    va_list argp;
    va_start(argp, signature);
    QStatus status = ((ajn::MsgArgC*)args)->MsgArgUtilsSetVC(((ajn::MsgArgC*)args) + argOffset, *numArgs, signature, &argp);
    va_end(argp);
    return status;
}


QStatus alljoyn_msgarg_set_uint8(alljoyn_msgarg arg, uint8_t y) {
    return alljoyn_msgarg_set(arg, "y", y);
}
QStatus alljoyn_msgarg_set_bool(alljoyn_msgarg arg, QCC_BOOL b) {
    return alljoyn_msgarg_set(arg, "b", b);
}
QStatus alljoyn_msgarg_set_int16(alljoyn_msgarg arg, int16_t n) {
    return alljoyn_msgarg_set(arg, "n", n);
}
QStatus alljoyn_msgarg_set_uint16(alljoyn_msgarg arg, uint16_t q) {
    return alljoyn_msgarg_set(arg, "q", q);
}
QStatus alljoyn_msgarg_set_int32(alljoyn_msgarg arg, int32_t i) {
    return alljoyn_msgarg_set(arg, "i", i);
}
QStatus alljoyn_msgarg_set_uint32(alljoyn_msgarg arg, uint32_t u) {
    return alljoyn_msgarg_set(arg, "u", u);
}
QStatus alljoyn_msgarg_set_int64(alljoyn_msgarg arg, int64_t x) {
    return alljoyn_msgarg_set(arg, "x", x);
}
QStatus alljoyn_msgarg_set_uint64(alljoyn_msgarg arg, uint64_t t) {
    return alljoyn_msgarg_set(arg, "t", t);
}
QStatus alljoyn_msgarg_set_double(alljoyn_msgarg arg, double d) {
    return alljoyn_msgarg_set(arg, "d", d);
}
QStatus alljoyn_msgarg_set_string(alljoyn_msgarg arg, const char* s) {
    return alljoyn_msgarg_set(arg, "s", s);
}
QStatus alljoyn_msgarg_set_objectpath(alljoyn_msgarg arg, const char* o) {
    return alljoyn_msgarg_set(arg, "o", o);
}
QStatus alljoyn_msgarg_set_signature(alljoyn_msgarg arg, const char* g) {
    return alljoyn_msgarg_set(arg, "g", g);
}


extern AJ_API QStatus alljoyn_msgarg_get_uint8(const alljoyn_msgarg arg, uint8_t* y){
    return alljoyn_msgarg_get(arg, "y", y);
}
extern AJ_API QStatus alljoyn_msgarg_get_bool(const alljoyn_msgarg arg, uint8_t* b){
    return alljoyn_msgarg_get(arg, "b", b);
}
extern AJ_API QStatus alljoyn_msgarg_get_int16(const alljoyn_msgarg arg, int16_t* n){
    return alljoyn_msgarg_get(arg, "n", n);
}
extern AJ_API QStatus alljoyn_msgarg_get_uint16(const alljoyn_msgarg arg, uint16_t* q){
    return alljoyn_msgarg_get(arg, "q", q);
}
extern AJ_API QStatus alljoyn_msgarg_get_int32(const alljoyn_msgarg arg, int32_t* i){
    return alljoyn_msgarg_get(arg, "i", i);
}
extern AJ_API QStatus alljoyn_msgarg_get_uint32(const alljoyn_msgarg arg, uint32_t* u){
    return alljoyn_msgarg_get(arg, "u", u);
}
extern AJ_API QStatus alljoyn_msgarg_get_int64(const alljoyn_msgarg arg, int64_t* x){
    return alljoyn_msgarg_get(arg, "x", x);
}
extern AJ_API QStatus alljoyn_msgarg_get_uint64(const alljoyn_msgarg arg, uint64_t* t){
    return alljoyn_msgarg_get(arg, "t", t);
}
extern AJ_API QStatus alljoyn_msgarg_get_double(const alljoyn_msgarg arg, double* d){
    return alljoyn_msgarg_get(arg, "d", d);
}
extern AJ_API QStatus alljoyn_msgarg_get_string(const alljoyn_msgarg arg, char* s){
    return alljoyn_msgarg_get(arg, "s", s);
}
extern AJ_API QStatus alljoyn_msgarg_get_objectpath(const alljoyn_msgarg arg, char* o){
    return alljoyn_msgarg_get(arg, "o", o);
}
extern AJ_API QStatus alljoyn_msgarg_get_signature(const alljoyn_msgarg arg, char* g){
    return alljoyn_msgarg_get(arg, "g", g);
}

#define _IMPLEMENT_MSGARG_TYPE_ACCESSOR(rt, nt, mt) \
    rt alljoyn_msgarg_as_ ## nt(const alljoyn_msgarg args, size_t idx) \
    { \
        return ((ajn::MsgArgC*)args)[idx].mt; \
    }
#define _IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(t) _IMPLEMENT_MSGARG_TYPE_ACCESSOR(t ## _t, t, v_ ## t)

_IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(int16);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(uint16);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(int32);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(uint32);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(int64);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR_S(uint64);

_IMPLEMENT_MSGARG_TYPE_ACCESSOR(uint8_t, uint8, v_byte);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR(QCC_BOOL, bool, v_bool);
_IMPLEMENT_MSGARG_TYPE_ACCESSOR(double, double, v_double);

#undef _IMPLEMENT_MSGARG_TYPE_ACCESSOR
#undef _IMPLEMENT_MSGARG_TYPE_ACCESSOR_S

const char* alljoyn_msgarg_as_string(const alljoyn_msgarg args, size_t idx)
{
    return ((ajn::MsgArgC*)args)[idx].v_string.str;
}

const char* alljoyn_msgarg_as_objpath(const alljoyn_msgarg args, size_t idx)
{
    return ((ajn::MsgArgC*)args)[idx].v_objPath.str;
}

void alljoyn_msgarg_as_signature(const alljoyn_msgarg args, size_t idx,
                                 uint8_t* out_len, const char** out_sig)
{
    *out_len = ((ajn::MsgArgC*)args)[idx].v_signature.len;
    *out_sig = ((ajn::MsgArgC*)args)[idx].v_signature.sig;
}

void alljoyn_msgarg_as_handle(const alljoyn_msgarg args, size_t idx, void** out_socketFd)
{
    *out_socketFd = &((ajn::MsgArgC*)args)[idx].v_handle.fd;
}

const alljoyn_msgarg alljoyn_msgarg_as_array(const alljoyn_msgarg args, size_t idx,
                                             size_t* out_len, const char** out_sig)
{
    *out_len = ((ajn::MsgArgC*)args)[idx].v_array.GetNumElements();
    *out_sig = ((ajn::MsgArgC*)args)[idx].v_array.GetElemSig();
    return (const alljoyn_msgarg)(((ajn::MsgArgC*)args)[idx].v_array.GetElements());
}

alljoyn_msgarg alljoyn_msgarg_as_struct(const alljoyn_msgarg args, size_t idx,
                                        size_t* out_numMembers)
{
    *out_numMembers = ((ajn::MsgArgC*)args)[idx].v_struct.numMembers;
    return (alljoyn_msgarg)(((ajn::MsgArgC*)args)[idx].v_struct.members);
}

void alljoyn_msgarg_as_dictentry(const alljoyn_msgarg args, size_t idx,
                                 alljoyn_msgarg* out_key, alljoyn_msgarg* out_val)
{
    *out_key = (alljoyn_msgarg)((ajn::MsgArgC*)args)[idx].v_dictEntry.key;
    *out_val = (alljoyn_msgarg)((ajn::MsgArgC*)args)[idx].v_dictEntry.val;
}

alljoyn_msgarg alljoyn_msgarg_as_variant(const alljoyn_msgarg args, size_t idx)
{
    return (alljoyn_msgarg)((ajn::MsgArgC*)args)[idx].v_variant.val;
}

void alljoyn_msgarg_as_scalararray(const alljoyn_msgarg args, size_t idx,
                                   size_t* out_numElements, const void** out_elements)
{
    *out_numElements = ((ajn::MsgArgC*)args)[idx].v_scalarArray.numElements;
    *out_elements = ((ajn::MsgArgC*)args)[idx].v_scalarArray.v_byte;
}
