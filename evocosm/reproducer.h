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

#if !defined(LIBEVOCOSM_REPRODUCER_H)
#define LIBEVOCOSM_REPRODUCER_H

// libevocosm
#include "organism.h"

namespace libevocosm
{
    //! Creates new organisms from an existing population
    /*!
        In most cases, a reproducer generates new organisms using parents
        selected (by fitness) from an existing population. In some singular
        (and probably rare) cases, a reproducer might generate new, random
        organisms in order to keep diversity high.
        Reproduction techniques can include crossover and asexual, sexual
        and (my favorite) try-sexual models.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class reproducer : protected globals
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
        virtual ~reproducer()
        {
            // nada
        }

        //! Creates children
        /*!
            A population provides genetic material for the creation of new children (who,
            will, like real children, forget where they left their shoes).
            \param a_population - A population of organisms
            \param a_limit - The maximum number of children the generate
            \return A vector containing new "child" organisms
        */
        virtual vector<OrganismType> breed(const vector<OrganismType> & a_population, size_t a_limit) = 0;
    };
};

#endif
