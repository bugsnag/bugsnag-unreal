//
//  BSG_KSCrashContext.h
//
//  Created by Karl Stenerud on 2012-01-28.
//
//  Copyright (c) 2012 Karl Stenerud. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall remain in place
// in this source code.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/* Contextual information about a crash.
 */

#ifndef HDR_BSG_KSCrashContext_h
#define HDR_BSG_KSCrashContext_h

#include "BSG_KSCrashReportWriter.h"
#include "BSG_KSCrashSentry.h"
#include "BSG_KSCrashState.h"

#include <signal.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /** A unique identifier (UUID). */
    char *crashID;

    /** Name of this process. */
    char *processName;

    /** System information in JSON format (to be written to the report). */
    char *systemInfoJSON;

    /** The types of crashes that will be handled. */
    BSG_KSCrashType handlingCrashTypes;

    /** Callback allowing the application the opportunity to add extra data to
     * the report file. Application MUST NOT call async-unsafe methods!
     */
    BSG_KSReportWriteCallback onCrashNotify;

    /**
     * File path to write the crash report
     */
    char *crashReportFilePath;

    /**
     * File path to write the recrash report, if the crash reporter crashes
     */
    char *recrashReportFilePath;
} BSG_KSCrash_Configuration;

/** Contextual data used by the crash report writer.
 */
typedef struct {
    BSG_KSCrash_Configuration config;
    BSG_KSCrash_State state;
    BSG_KSCrash_SentryContext crash;
} BSG_KSCrash_Context;

#ifdef __cplusplus
}
#endif

#endif // HDR_KSCrashContext_h
