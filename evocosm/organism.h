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

#if !defined(LIBEVOCOSM_ORGANISM_H)
#define LIBEVOCOSM_ORGANISM_H

// Standard C++ Library
#include <cstddef>

// libevocosm
#include "evocommon.h"

namespace libevocosm
{
    using std::vector;

    //! An evolving organism
    /*!
        An organism is a solution to a problem posed by a fitness landscape;
        it has "genes" that define its behavior, and a fitness value assigned
        during fitness testing.

        Evocosm provides the freedom to define organisms as anything: bit
        strings, floating-point numbers, finite state machines, LISP programs,
        or external robots controlled via radio waves.
        \param Genotype - The type of genes for this organism class
    */
    template <typename Genotype>
    class organism : protected globals
    {
    public:
        /*!
            Fitness is assigned by the landscape in which an organism is tested.
            A scaler object may alter fitness values after testing; a selector
            uses fitness to determine which organisms survive from one generation
            to the next, and a reproducer creates new organisms from parents
            selected by fitness.
        */
        double fitness;

        /*!
            Genes define an organism's behavior in a fitness landscape. An initial
            population usually (but not always) contains organisms with random genes;
            a landscape tests the behavior defined by genes to calculate the fitness
            of an organism. A reproducer will create children from the genes of their
            parents, and a mutator may make random changes in those genes.
        */
        Genotype genes;

        //! Creation constructor
        /*!
            Creates a new organism, with default genes and zero fitness.
        */
        organism()
            : fitness(0.0),
              genes()
        {
            // nada
        }

        //! Value constructor
        /*!
            Creates a new organism with specific genes.
            \param a_genes - Gene value for the new organism
        */
        organism(const Genotype & a_genes)
            : fitness(0.0),
              genes(a_genes)
        {
            // nada
        }

        //! Copy constructor
        /*!
            Creates a new object identical to an existing one.
            \param a_source - The source object
        */
        organism(const organism & a_source)
            : fitness(a_source.fitness),
              genes(a_source.genes)
        {
            // nada
        }

        //! Virtual destructor
        /*!
            A virtual destructor. By default, it does nothing; this is
            a placeholder that identifies this class as a potential base,
            ensuring that objects of a derived class will have their
            destructors called if they are destroyed through a base-class
            pointer.
        */
        virtual ~organism()
        {
            // nada
        }

        //! assignment operator
        /*!
            Assigns an existing object the state of another.
            \param a_source - The source object
            \return A reference to <i>this</i>
        */
        organism & operator = (const organism & a_source)
        {
            fitness = a_source.fitness;
            genes   = a_source.genes;
            return *this;
        }

        //! assignment operator
        /*!
            Assigns an existing object the state of another.
            \param a_genes - The source object
            \return A reference to <i>this</i>
        */
        organism & operator = (const Genotype & a_genes)
        {
            fitness = 0.0;
            genes   = a_genes;
            return *this;
        }

        //! Comparison operator for algorithms
        /*!
            Many Standard C++ algorithms require a "less than" comparison operator
            for container elements.
            \param a_right - Right hand argument for less than operator
        */
        virtual bool operator < (const organism & a_right) const
        {
            return (fitness > a_right.fitness);
        }

        //! Resets an object to it's initial state
        /*!
            Resets all data in an organisim to the initial (or startup) state, including
            setting the fitness to zero. Derived classes should override this method to
            "clear" any data.
        */
        virtual void reset()
        {
            fitness = 0.0;
        }
    };

};

#endif
