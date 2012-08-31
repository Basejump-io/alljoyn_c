/**
 * @file
 * This file defines types for statically describing a message bus interface
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
#ifndef _ALLJOYN_C_INTERFACEDESCRIPTION_H
#define _ALLJOYN_C_INTERFACEDESCRIPTION_H

#include <qcc/platform.h>
#include <alljoyn_c/AjAPI.h>
#include <alljoyn_c/Message.h>
#include <Status.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _alljoyn_interfacedescription_handle*        alljoyn_interfacedescription;

/** @name Access type */
// @{
static const uint8_t ALLJOYN_PROP_ACCESS_READ  = 1; /**< Read Access type */
static const uint8_t ALLJOYN_PROP_ACCESS_WRITE = 2; /**< Write Access type */
static const uint8_t ALLJOYN_PROP_ACCESS_RW    = 3; /**< Read-Write Access type */
// @}
/** @name Anotation flags */
// @{
static const uint8_t ALLJOYN_MEMBER_ANNOTATE_NO_REPLY   = 1; /**< No reply annotate flag */
static const uint8_t ALLJOYN_MEMBER_ANNOTATE_DEPRECATED = 2; /**< Deprecated annotate flag */
// @}

typedef struct {
    alljoyn_interfacedescription iface;         /**< Interface that this member belongs to */
    alljoyn_messagetype memberType;             /**< %Member type */
    const char* name;                           /**< %Member name */
    const char* signature;                      /**< Method call IN arguments (NULL for signals) */
    const char* returnSignature;                /**< Signal or method call OUT arguments */
    const char* argNames;                       /**< Comma separated list of argument names - can be NULL */

    const void* internal_member;                /**< For internal use only */
} alljoyn_interfacedescription_member;

/**
 * find out the amount of annotations the alljoyn_interfacedescription_member has
 *
 * The number of the annotation is only valid as long as no annotations have
 * been removed or added to the interface.  For this reason this function should
 * only be used after calling alljoyn_interfacedescription_activate.
 *
 * @param[in] member The alljoyn interfacedescription member that we want the
 *                   annotations count from
 * @return the number of annotations
 */
extern AJ_API size_t alljoyn_interfacedescription_member_getannotationscount(alljoyn_interfacedescription_member member);

/**
 * Obtain the name and value for the annotation index.
 * The order of the annotation is only valid as long as no new annotations are
 * removed or added to the interface.  For this reason this function should
 * only be used after calling alljoyn_interfacedescription_activate.
 *
 * @param[in] member the alljoyn interfacedescription member that we want to read annotations from
 * @param[in] index  the index of the annotation of interest
 * @param[out] name   the name of the annotation
 * @param[in,out] name_size the size of the name string if name==NULL this will return the size of the name string plus nul character
 * @param[out] value  the value of the annotation
 * @param[in,out] value_size the size of the value string if value == NULL this will return the size of the value string plus nul character
 */
extern AJ_API void alljoyn_interfacedescription_member_getannotationatindex(alljoyn_interfacedescription_member member,
                                                                            size_t index,
                                                                            char* name, size_t* name_size,
                                                                            char* value, size_t* value_size);

/**
 * Get this member's annotation value return the size of the value string if
 * name is NULL.
 *
 * @param[in] member The AllJoyn InterfaceDescription member that we want the annotation from
 * @param[in] name   Name of the annotation to look for
 * @param[out] value  Value to compare with
 * @param[in,out] value_size size of the value string if value == NULL it will return the size of the value string plus nul character
 * @return    true iff annotations[name] == value
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_member_getannotation(alljoyn_interfacedescription_member member, const char* name, char* value, size_t* value_size);

typedef struct {
    const char* name;               /**< %Property name */
    const char* signature;          /**< %Property type */
    uint8_t access;                 /**< Access is #ALLJOYN_PROP_ACCESS_READ, #ALLJOYN_PROP_ACCESS_WRITE, or #ALLJOYN_PROP_ACCESS_RW */

    const void* internal_property;  /**< For internal use only */
} alljoyn_interfacedescription_property;

/**
 * find out the amount of annotations the alljoyn_interfacedescription_member has
 *
 * The number of the annotation is only valid as long as no annotations have
 * been removed or added to the interface.  For this reason this function should
 * only be used after calling alljoyn_interfacedescription_activate.
 *
 * @param[in] property The alljoyn interfacedescription property that we want the
 *                   annotations count from
 * @return the number of annotations
 */
extern AJ_API size_t alljoyn_interfacedescription_property_getannotationscount(alljoyn_interfacedescription_property property);

/**
 * Obtain the name and value for the annotation index.
 * The order of the annotation is only valid as long as no new annotations are
 * removed or added to the interface.  For this reason this function should
 * only be used after calling alljoyn_interfacedescription_activate.
 *
 * @param[in] property the alljoyn interfacedescription property that we want to read annotations from
 * @param[in] index  the index of the annotation of interest
 * @param[out] name   the name of the annotation
 * @param[in,out] name_size the size of the name string if name==NULL this will return the size of the name string plus nul character
 * @param[out] value  the value of the annotation
 * @param[in,out] value_size the size of the value string if value == NULL this will return the size of the value string plus nul character
 */
extern AJ_API void alljoyn_interfacedescription_property_getannotationatindex(alljoyn_interfacedescription_property property,
                                                                              size_t index,
                                                                              char* name, size_t* name_size,
                                                                              char* value, size_t* value_size);

/**
 * Get this member's annotation value return the size of the value string if
 * name is NULL.
 *
 * @param[in] property The AllJoyn InterfaceDescription property that we want the annotation from.
 * @param[in] name   Name of the annotation to look for
 * @param[out] value  Value to compare with
 * @param[in,out] value_size size of the value string if value == NULL it will return the size of the value string plus nul character
 * @return    true iff annotations[name] == value
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_property_getannotation(alljoyn_interfacedescription_property property, const char* name, char* value, size_t* value_size);

/**
 * Activate this interface. An interface must be activated before it can be used. Activating an
 * interface locks the interface so that is can no longer be modified.
 *
 * @param iface InterfaceDescription to activate.
 */
extern AJ_API void alljoyn_interfacedescription_activate(alljoyn_interfacedescription iface);

/**
 * Add an annotation to the interface.
 *
 * @param name       Name of annotation.
 * @param value      Value of the annotation
 * @return
 *      - #ER_OK if successful.
 *      - #ER_BUS_PROPERTY_ALREADY_EXISTS if the property can not be added
 *                                        because it already exists.
 */
extern AJ_API QStatus alljoyn_interfacedescription_addannotation(alljoyn_interfacedescription iface, const char* name, const char* value);

/**
 * Lookup an interface annotation
 *
 * @param       iface   Interface on which to lookup the member
 * @param       name    Name of the annotation to lookup
 * @param[out]  value   The value of the annotation
 * @param[in,out] value_size return the size of the string if value == NULL plus nul character or
 *                           specifies the max size of value if value != NULL.
 *
 * @return QC_FALSE if annotation does not exist, QC_TRUE otherwise.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_getannotation(alljoyn_interfacedescription iface, const char* name, char* value, size_t* value_size);


/**
 * find out the amount of annotations the alljoyn_interfacedescription has
 *
 * The number of the annotation is only valid as long as no annotations have
 * been removed or added to the interface.  For this reason this function should
 * only be used after calling alljoyn_interfacedescription_activate.
 *
 * @param[in] iface The alljoyn_interfacedescription that we want the annotations count from
 *
 * @return the number of annotations
 */
extern AJ_API size_t alljoyn_interfacedescription_getannotationscount(alljoyn_interfacedescription iface);

/**
 * Obtain the name and value for the annotation index.
 * The order of the annotation is only valid as long as no new annotations are
 * removed or added to the interface.  For this reason this function should
 * only be used after calling alljoyn_interfacedescription_activate.
 *
 * @param[in] iface the alljoyn interfacedescription that we want to read annotations from
 * @param[in] index  the index of the annotation of interest
 * @param[out] name   the name of the annotation
 * @param[in,out] name_size the size of the name string if name==NULL this will return the size of the name string plus nul character
 * @param[out] value  the value of the annotation
 * @param[in,out] value_size the size of the value string if value == NULL this will return the size of the value string plus nul character
 */
extern AJ_API void alljoyn_interfacedescription_getannotationatindex(alljoyn_interfacedescription iface,
                                                                     size_t index,
                                                                     char* name, size_t* name_size,
                                                                     char* value, size_t* value_size);

/**
 * Lookup a member description by name
 *
 * @param       iface   Interface on which to lookup the member
 * @param       name    Name of the member to lookup
 * @param[out]  member  The description of the member
 *
 * @return QCC_FALSE if member does not exist, QCC_TRUE otherwise.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_getmember(const alljoyn_interfacedescription iface, const char* name,
                                                              alljoyn_interfacedescription_member* member);

/**
 * Add a member to the interface.
 *
 * @param iface       Interface on which to add the member.
 * @param type        Message type.
 * @param name        Name of member.
 * @param inputSig    Signature of input parameters or NULL for none.
 * @param outSig      Signature of output parameters or NULL for none.
 * @param argNames    Comma separated list of input and then output arg names used in annotation XML.
 * @param annotation  Annotation flags.
 *
 * @return
 *      - #ER_OK if successful
 *      - #ER_BUS_MEMBER_ALREADY_EXISTS if member already exists
 */
extern AJ_API QStatus alljoyn_interfacedescription_addmember(alljoyn_interfacedescription iface, alljoyn_messagetype type,
                                                             const char* name, const char* inputSig, const char* outSig,
                                                             const char* argNames, uint8_t annotation);

/**
 * Add an annotation to a member
 *
 * @param iface       Interface on which to add the member.
 * @param member      Name of member.
 * @param name        Name of annotation
 * @param value       Value for the annotation
 *
 * @return
 *      - #ER_OK if successful
 *      - #ER_BUS_MEMBER_ALREADY_EXISTS if member already exists
 */
extern AJ_API QStatus alljoyn_interfacedescription_addmemberannotation(alljoyn_interfacedescription iface,
                                                                       const char* member,
                                                                       const char* name,
                                                                       const char* value);

/**
 * Add an annotation to a member
 *
 * @param iface       Interface on which to add the member.
 * @param member      Name of member.
 * @param name        Name of annotation
 * @param[out] value  Value for the annotation
 * @param[in,out] value_size The size value.  If value == NULL returns the size
 *                           of the string plus nul character  so a new char* can be allocated.
 *                           If value != NULL specifies the size of value.
 *
 * @return QC_FALSE if annotation does not exist, QC_TRUE otherwise.
 */
QCC_BOOL AJ_API alljoyn_interfacedescription_getmemberannotation(alljoyn_interfacedescription iface,
                                                                 const char* member,
                                                                 const char* name,
                                                                 char* value,
                                                                 size_t* value_size);

/**
 * Get all the members.
 *
 * @param iface       The interface from which to get all members.
 * @param members     A pointer to a Member array to receive the members. Can be NULL in
 *                    which case no members are returned and the return value gives the number
 *                    of members available.
 * @param numMembers  The size of the Member array. If this value is smaller than the total
 *                    number of members only numMembers will be returned.
 *
 * @return  The number of members returned or the total number of members if members is NULL.
 */
extern AJ_API size_t alljoyn_interfacedescription_getmembers(const alljoyn_interfacedescription iface,
                                                             alljoyn_interfacedescription_member* members,
                                                             size_t numMembers);

/**
 * Check for existence of a member. Optionally check the signature also.
 * @remark
 * if the a signature is not provided this method will only check to see if
 * a member with the given @c name exists.  If a signature is provided a
 * member with the given @c name and @c signature must exist for this to return true.
 *
 * @param iface      Interface to query for a member.
 * @param name       Name of the member to lookup
 * @param inSig      Input parameter signature of the member to lookup
 * @param outSig     Output parameter signature of the member to lookup (leave NULL for signals)
 * @return QCC_TRUE if the member name exists.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_hasmember(alljoyn_interfacedescription iface,
                                                              const char* name, const char* inSig,
                                                              const char* outSig);

/**
 * Add a method call member to the interface.
 *
 * @param iface       Interface on which to add the method member.
 * @param name        Name of method call member.
 * @param inputSig    Signature of input parameters or NULL for none.
 * @param outSig      Signature of output parameters or NULL for none.
 * @param argNames    Comma separated list of input and then output arg names used in annotation XML.
 * @param annotation  Annotation flags. Default value 0.
 * @param accessPerms Access permission requirements on this call. Default value 0.
 *
 * @return
 *      - #ER_OK if successful
 *      - #ER_BUS_MEMBER_ALREADY_EXISTS if member already exists
 */
extern AJ_API QStatus alljoyn_interfacedescription_addmethod(alljoyn_interfacedescription iface, const char* name, const char* inputSig, const char* outSig, const char* argNames, uint8_t annotation, const char* accessPerms);

/**
 * Lookup a member method description by name
 *
 * @param       iface Interface on which to lookup the method.
 * @param       name  Name of the method to lookup
 * @param[out]  member  The description of the member
 * @return
 *      - Pointer to member.
 *      - NULL if does not exist.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_getmethod(alljoyn_interfacedescription iface, const char* name, alljoyn_interfacedescription_member* member);

/**
 * Add a signal member to the interface.
 *
 * @param iface       Interface on which to add the signal member.
 * @param name        Name of method call member.
 * @param sig         Signature of parameters or NULL for none.
 * @param argNames    Comma separated list of arg names used in annotation XML.
 * @param annotation  Annotation flags. Default value 0.
 * @param accessPerms Access permission requirements on this call. Default value 0.
 *
 * @return
 *      - #ER_OK if successful
 *      - #ER_BUS_MEMBER_ALREADY_EXISTS if member already exists
 */
extern AJ_API QStatus alljoyn_interfacedescription_addsignal(alljoyn_interfacedescription iface, const char* name, const char* sig, const char* argNames, uint8_t annotation, const char* accessPerms);

/**
 * Lookup a member signal description by name
 *
 * @param name  Name of the signal to lookup
 * @return
 *      - Pointer to member.
 *      - NULL if does not exist.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_getsignal(alljoyn_interfacedescription iface, const char* name, alljoyn_interfacedescription_member* member);

/**
 * Lookup a property description by name
 *
 * @param iface     Interface to query for a property.
 * @param name      Name of the property to lookup
 * @param property  The description of the property
 * @return QCC_TRUE if the property was found, QCC_FALSE otherwise
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_getproperty(const alljoyn_interfacedescription iface, const char* name,
                                                                alljoyn_interfacedescription_property* property);

/**
 * Get all the properties.
 *
 * @param iface     Interface to query for properties.
 * @param props     A pointer to a Property array to receive the properties. Can be NULL in
 *                  which case no properties are returned and the return value gives the number
 *                  of properties available.
 * @param numProps  The size of the Property array. If this value is smaller than the total
 *                  number of properties only numProperties will be returned.
 *
 *
 * @return  The number of properties returned or the total number of properties if props is NULL.
 */
extern AJ_API size_t alljoyn_interfacedescription_getproperties(const alljoyn_interfacedescription iface,
                                                                alljoyn_interfacedescription_property* props,
                                                                size_t numProps);

/**
 * Add a property to the interface.
 *
 * @param iface      Interface on which to add the property.
 * @param name       Name of property.
 * @param signature  Property type.
 * @param access     #PROP_ACCESS_READ, #PROP_ACCESS_WRITE or #PROP_ACCESS_RW
 * @return
 *      - #ER_OK if successful.
 *      - #ER_BUS_PROPERTY_ALREADY_EXISTS if the property can not be added
 *                                        because it already exists.
 */
extern AJ_API QStatus alljoyn_interfacedescription_addproperty(alljoyn_interfacedescription iface, const char* name,
                                                               const char* signature, uint8_t access);

/**
 * Add an annotation to an existing property
 * @param iface      Interface on which to add the property.
 * @param property   Name of property.
 * @param name       Name of annotation
 * @param value      value of annotation
 *
 * @return
 *      - #ER_OK if successful.
 *      - #ER_BUS_PROPERTY_ALREADY_EXISTS if the annotation can not be added to the property because it already exists.
 */
extern AJ_API QStatus alljoyn_interfacedescription_addpropertyannotation(alljoyn_interfacedescription iface,
                                                                         const char* property,
                                                                         const char* name,
                                                                         const char* value);

/**
 * Get the annotation value for a property
 * @param iface      Interface on which to add the property.
 * @param property   Name of the property
 * @param name       Name of annotation
 * @param[out] value      Value for the annotation use NULL to obtain the size of the string in value
 * @param str_size[in,out] the size of the char* value if value == NULL return the size of the value string plus nul character
 *
 * @code
 * size_t str_size;
 * alljoyn_interfacedescription_getpropertyannotation(iface, "myProperty", "org.freedesktop.DBus.Property.EmitsChangedSignal", NULL, &str_size);
 * char* value = (char*)malloc(sizeof(char) * str_size);
 * QCC_BOOL success = alljoyn_interfacedescription_getpropertyannotation(iface, "myProperty", "org.freedesktop.DBus.Property.EmitsChangedSignal", value, &str_size);
 * if (success == QCC_TRUE) {
 *    \/* code that does something with the value *\/
 * }
 * free(value);
 * @endcode
 *
 * @return           QCC_TRUE if found, QCC_FALSE if not found
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_getpropertyannotation(alljoyn_interfacedescription iface,
                                                                          const char* property,
                                                                          const char* name,
                                                                          char* value,
                                                                          size_t* str_size);

/**
 * Check for existence of a property.
 *
 * @param iface      Interface to query.
 * @param name       Name of the property to lookup
 * @return true if the property exists.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_hasproperty(const alljoyn_interfacedescription iface, const char* name);

/**
 * Check for existence of any properties
 *
 * @param iface      Interface to query.
 * @return  true if interface has any properties.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_hasproperties(const alljoyn_interfacedescription iface);

/**
 * Returns the name of the interface
 *
 * @param iface      Interface to query.
 * @return the interface name.
 */
extern AJ_API const char* alljoyn_interfacedescription_getname(const alljoyn_interfacedescription iface);

/**
 * Returns a description of the interface in introspection XML format
 * @return The interface description in introspection XML format.
 *
 * @param iface      Interface to query
 * @param[out] str   The character string that will hold the XML string
 *                   representation of the interface
 * @param[in]  buf   The size of the char* array that will hold the string
 * @param indent     Number of space chars to use in XML indentation.
 * @return  The number of characters (excluding the terminating null byte) which
 *          would have been written to the final string if enough space is
 *          available.  Thus returning a value of buf or larger means the output
 *          was truncated.
 */
extern AJ_API size_t alljoyn_interfacedescription_introspect(const alljoyn_interfacedescription iface, char* str, size_t buf, size_t indent);

/**
 * Indicates if this interface is secure. Secure interfaces require end-to-end authentication.
 * The arguments for methods calls made to secure interfaces and signals emitted by secure
 * interfaces are encrypted.
 * @param iface      Interface to query.
 * @return true if the interface is secure.
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_issecure(const alljoyn_interfacedescription iface);

/**
 * Equality operation.
 *
 * @param one   Interface to compare to other
 * @param other Interface to compare to one
 *
 * @return QCC_TRUE if one == other
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_eql(const alljoyn_interfacedescription one,
                                                        const alljoyn_interfacedescription other);

/**
 * Equality operation.
 *
 * @param one   alljoyn_interfacedescription_member to compare to other
 * @param other alljoyn_interfacedescription_member to compare to one
 *
 * @return QCC_TRUE if one == other
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_member_eql(const alljoyn_interfacedescription_member one,
                                                               const alljoyn_interfacedescription_member other);

/**
 * Equality operation.
 *
 * @param one   alljoyn_interfacedescription_property to compare to other
 * @param other alljoyn_interfacedescription_property to compare to one
 *
 * @return QCC_TRUE if one == other
 */
extern AJ_API QCC_BOOL alljoyn_interfacedescription_property_eql(const alljoyn_interfacedescription_property one,
                                                                 const alljoyn_interfacedescription_property other);

#ifdef __cplusplus
} /* extern "C" */
#endif

#undef QCC_MODULE
#endif
