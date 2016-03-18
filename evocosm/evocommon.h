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

#if !defined(LIBEVOCOSM_EVOCOMMON_H)
#define LIBEVOCOSM_EVOCOMMON_H

// Standard C++ Library
#include <string>
#include <ctime>

namespace libevocosm
{
    //! The random number generator used by Evocosm
    /*!
        The 64-bit "Keep It Simple Stupid" psuedorandom number generator
        by Marsaglia in the following thread:
            http://www.thecodingforums.com/threads/64-bit-kiss-rngs.673657/
    */
    class prng
    {
    private:
        unsigned long long int x = 1234567890987654321ULL;
        unsigned long long int c =  123456123456123456ULL;
        unsigned long long int y =  362436362436362436ULL;
        unsigned long long int z =    1066149217761810ULL;
        unsigned long long int t =   29979245822353888ULL;
        unsigned long long int s =   t;

    public:
        //! Constructor
        prng(const unsigned long long seed = (unsigned long long int)(time(nullptr)))
        {
            set_seed(seed);
        }
        
        void set_seed(unsigned long long int seed = (unsigned long long int)(time(nullptr)))
        {
            s = seed;
            t = s;
        }
        
        unsigned long long int get_seed()
        {
            return s;
        }
        
        unsigned long long int next()
        {
            t = (t = (x << 58) + c, c = (x >> 6), x += t, c += (x < t), x)
              + (y ^= (y << 13), y ^= (y >> 17), y ^= (y << 43))
              + (z = 6906969069LL * z + 1234567);
            
            return t;
        }

        //! get a random index value
        size_t get_index(size_t n)
        {
            return next() % n;
        }

        //! get the next value in the range [0,1)
        double get_real()
        {
            return (double)((double)next() / 18446744073709551615.0);
        }
    };

    //! Elements shared by all classes in Evocosm
    /*!
        All Evocosm classes are derived from this class, a singleton for shared
        attributes.
    */
    class globals
    {
    protected:
        //! Static function to allow use of g_random function pointer in random_shuffle
        static size_t rand_index(size_t n)
        {
            return g_random.get_index(n);
        }

        //! A shared random number generator
        static prng g_random;

        //! Version number
        static std::string g_version;

    public:
        //! Set the seed for the random number generator
        static void set_seed(const unsigned long long int a_seed)
        {
            g_random.set_seed(a_seed);
        }

        //! Set the seed for the random number generator
        static uint32_t get_seed()
        {
            return g_random.get_seed();
        }

        //! Get version number
        static std::string version()
        {
            return g_version;
        }
    };

}

#endif
