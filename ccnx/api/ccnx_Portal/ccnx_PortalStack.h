/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file ccnx_PortalStack.h
 * @brief A polymorphic interface to different Portal Stack implementations.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef CCNx_Portal_API_ccnx_PortalImplementation_h
#define CCNx_Portal_API_ccnx_PortalImplementation_h

struct CCNxPortalStack;
/**
 * A set of functions and state for the Portal instance.
 */
typedef struct CCNxPortalStack CCNxPortalStack;

#include <parc/algol/parc_Properties.h>
#include <ccnx/api/ccnx_Portal/ccnx_PortalFactory.h>

/**
 * Create function for a `CCNxPortalStack`
 *
 * @param [in] factory An instance of a {@link CCNxPortalFactory}.
 * @param [in] attributes A pointer to a valid CCNxPortalAttributes instance.
 * @param [in] start A pointer to a function that takes `*privateData` as a parameter.
 * @param [in] stop A pointer to a function that takes `*privateData` as a parameter.
 * @param [in] receive A pointer to a function that takes `*privateData` as a parameter and returns a pointer to a {@link CCNxMetaMessage}.
 * @param [in] send A pointer to a function that takes `*privateData` and `*portalMessage` as parameters.
 * @param [in] listen A pointer to a function that takes `*privateData` and a {@link CCNxName} as parameters.
 * @param [in] ignore A pointer to a function that takes `*privateData` and a {@link CCNxName} as parameters.
 * @param [in] getFileId A pointer to a function that takes `*privateData` as a parameter and returns an `int`.
 * @param [in] setAttributes A pointer to a function that takes `*privateData` and a {@link CCNxPortalAttributes} as parameters.
 * @param [in] getAttributes A pointer to a function that takes `*privateData` as a parameter and returns an instance of `CCNxPortalAttributes`.
 * @param [in] private A pointer to data containing shared state for all of the given functions.
 * @param [in] releasePrivateData A pointer to a function that takes `**privateData` as a parameter.
 *
 * @return  A pointer to a new `CCNxPortalStack`.
 *
 */
CCNxPortalStack *
ccnxPortalStack_Create(const CCNxPortalFactory *factory,
                       const CCNxPortalAttributes *attributes,
                       void (*start)(void *private),
                       void (*stop)(void *private),
                       CCNxMetaMessage *(*receive)(void *private, const CCNxStackTimeout *timeout),
                       bool (*send)(void *private, const CCNxMetaMessage *message, const CCNxStackTimeout *timeout),
                       bool (*listen)(void *private, const CCNxName *name, const CCNxStackTimeout *timeout),
                       bool (*ignore)(void *private, const CCNxName *name, const CCNxStackTimeout *timeout),
                       int (*getFileId)(void *private),
                       bool (*setAttributes)(void *private, const CCNxPortalAttributes *attributes),
                       CCNxPortalAttributes * (*getAttributes)(void *private),
                       void *private,
                       void (*releasePrivateData)(void **private));

/**
 * Increase the number of references to a `PARCBuffer`.
 *
 * Note that new `PARCBuffer` is not created,
 * only that the given `PARCBuffer` reference count is incremented.
 * Discard the reference by invoking `parcBuffer_Release`.
 *
 * @param [in] portal A pointer to a valid CCNxPortalStack instance.
 *
 * @return The input `CCNxPortalStack` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *x = parcBuffer_Allocate(10);
 *
 *     PARCBuffer *x_2 = parcBuffer_Acquire(x);
 *
 *     parcBuffer_Release(&x);
 *     parcBuffer_Release(&x_2);
 * }
 * @endcode
 */
CCNxPortalStack *ccnxPortalStack_Acquire(const CCNxPortalStack *portal);

/**
 * Release a previously acquired reference to the specified `CCNxPortalStack` instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] portalPtr A pointer to a pointer to a `CCNxPortalStack` instance to be released.
 *
 * Example:
 * @code
 * {
 *     CCNxPortalStack *stack = ccnxPortalStack_Create(...);
 *
 *     ccnxPortalStack_Release(&stack);
 * }
 * @endcode
 *
 * @see {@link ccnxPortalStack_Create}
 * @see {@link ccnxPortalStack_Acquire}
 */
void ccnxPortalStack_Release(CCNxPortalStack **portalPtr);

/**
 * Start a `CCNxPortalStack`
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`
 *
 * @return `true` if stack started successfully, else `false`.
 *
 */
bool ccnxPortalStack_Start(const CCNxPortalStack *implementation);

/**
 * Stop a `CCNxPortalStack`
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`
 *
 * @return `true` if stack started successfully, else `false`.
 *
 */
bool ccnxPortalStack_Stop(const CCNxPortalStack *implementation);

/**
 * Receive a message from a `CCNxPortalStack`
 *
 * @param [in] portalStack A pointer to an instance of `CCNxPortalStack`
 * @param [in] timeout A pointer to a CCNxStackTimeout value, or CCNxStackTimeout_Never.
 *
 * @return An instance of {@link CCNxMetaMessage}.
 *
 */
CCNxMetaMessage *ccnxPortalStack_Receive(const CCNxPortalStack *portalStack, const CCNxStackTimeout *timeout);

/**
 * Send a message through a `CCNxPortalStack`
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 * @param [in] portalMessage A pointer to an instance of `CCNxMetaMessage` to send.
 * @param [in] timeout A pointer to a CCNxStackTimeout value, or CCNxStackTimeout_Never.
 *
 * @return `true` if message sent successfully, else `false`.
 *
 */
bool ccnxPortalStack_Send(const CCNxPortalStack *implementation, const CCNxMetaMessage *portalMessage, const CCNxStackTimeout *timeout);

/**
 * Set the attributes on a `CCNxPortalStack`.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 * @param [in] attributes A pointer to an instance of `CCNxPortalAttributes`.
 * @return `true` if attributes set successfully, else `false`.
 *
 */
bool ccnxPortalStack_SetAttributes(const CCNxPortalStack *implementation, const CCNxPortalAttributes *attributes);

/**
 * Get the attributes from a `CCNxPortalStack`.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 * @return A pointer to an instance of `CCNxPortalAttributes` associated with the @p implementation.
 *
 */
const CCNxPortalAttributes *ccnxPortalStack_GetAttributes(const CCNxPortalStack *implementation);

/**
 * Listen for @p name on the @p implementation.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 * @param [in] name A pointer to an instance of {@link CCNxName} to listen for.
 * @param [in] timeout Not used.
 *
 * @return `true` if listen started successfully, else `false`.
 *
 */
bool ccnxPortalStack_Listen(const CCNxPortalStack *implementation, const CCNxName *name, const CCNxStackTimeout *timeout);
/**
 * Ignore (stop listening for) @p name on the @p implementation.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 * @param [in] name A pointer to an instance of {@link CCNxName} to listen for.
 * @param [in] timeout Not used.
 *
 * @return `true` if ignore was successful, else `false`.
 *
 */

bool ccnxPortalStack_Ignore(const CCNxPortalStack *implementation, const CCNxName *name, const CCNxStackTimeout *timeout);
/**
 * Get the file ID for @p implementation.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 *
 * @return `int` The file ID.
 *
 */
int ccnxPortalStack_GetFileId(const CCNxPortalStack *implementation);

/**
 * Get the {@link PARCKeyId} associated with the @p implementation.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 *
 * @return A pointer to the `PARCKeyId`.
 *
 */
const PARCKeyId *ccnxPortalStack_GetKeyId(const CCNxPortalStack *implementation);

/**
 * Get the error code for the most recent operation.
 *
 * @param [in] implementation A pointer to an instance of `CCNxPortalStack`.
 *
 * @return A value corresponding to the UNIX `errno`  see `sys/errno.h`
 *
 */
int ccnxPortalStack_GetErrorCode(const CCNxPortalStack *implementation);

/**
 * Convenience function to get the PARCProperties instance from the CCNxPortalFactory associated with
 * the given CCNxPortalStack instance.
 *
 * @param [in] portalStack A pointer to a valid CCNxPortalStack instance.
 *
 * @return A pointer to a valid `PARCProperties` instance.
 */
PARCProperties *ccnxPortalStack_GetProperties(const CCNxPortalStack *portalStack);

/**
 * Convenience function to get the value of the named property.
 *
 * If the named property is not in the `PARCProperties` associated with the given `CCNxPortalStack` instance,
 * the supplied default value is returned.
 *
 * @param [in] portalStack A pointer to a valid CCNxPortalStack instance.
 * @param [in] name A pointer to a nul-terminated C string representing the name of the property to get.
 * @param [in] defaultValue A pointer to a nul-terminated C string or NULL.
 *
 * @return A pointer to a a nul-terminated C string or NULL
 */
const char *ccnxPortalStack_GetProperty(const CCNxPortalStack *portalStack, const char *restrict name, const char *restrict defaultValue);
#endif
