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

#if !defined(LIBEVOCOSM_MUTATOR_H)
#define LIBEVOCOSM_MUTATOR_H

// libevocosm
#include "organism.h"

namespace libevocosm
{
    //! Mutates organisms
    /*!
        A mutator applies mutations (random, usually small changes) to a
        set of organisms. Mutation is highly dependent on the type of
        organism. In traditional genetic algorithms, a mutation flips one
        or more bits in an integer (i.e., chromosome). Evolving a path for
        the Traveling Salesman Problem involves complex mutations that
        maintain valid permutations of destination points.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class mutator : protected globals
    {
    public:
        //! Virtual destructor
        /*!
            A virtual destructor. By default, it does nothing; this is
            a placeholder that identifies this class as a potential base,
            ensuring that objects of a derived class will have their
            destructors called if they are destroyed through a base-class
            pointer.
        */
        virtual ~mutator()
        {
            // nada
        }

        //! Mutate a set of organisms
        /*!
            Mutates some (maybe none, maybe all) organisms in a given set.
            \param a_population - Set of organisms to be mutated
        */
        virtual void mutate(vector<OrganismType> & a_population) = 0;
    };
};

#endif
