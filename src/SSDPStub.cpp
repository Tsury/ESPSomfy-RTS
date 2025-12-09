// SSDP Stub - Empty implementation when SSDP is disabled
#include "SSDPStub.h"

#ifndef ENABLE_SSDP

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SSDP)
SSDPClassStub SSDP;
#endif

#endif // ENABLE_SSDP
