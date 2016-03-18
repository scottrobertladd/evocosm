/*
    Evocosm is a C++ framework for implementing evolutionary algorithms.
    It is part of the Drakontos Library of Interesting and Esoteric Oddities

    Copyright 2016 Scott Robert Ladd. All rights reserved.

    Evocosm is user-supported open source software. It's continued development is dependent on
    financial support from the community. You can provide funding by visiting the Evocosm
    website at:

        http://www.drakontos.com

    You license Evocosm under the Simplified BSD License (FreeBSD License).
*/

#include "evocommon.h"

// a global random number generator for all Evocosm classes
libevocosm::prng libevocosm::globals::g_random;

#if defined(_MSC_VER)
std::string libevocosm::globals::g_version("4.0.0");
#else
std::string libevocosm::globals::g_version(EVOCOSM_VERSION);
#endif
