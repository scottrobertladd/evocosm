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

#if !defined(LIBEVOCOSM_LANDSCAPE_H)
#define LIBEVOCOSM_LANDSCAPE_H

// libevocosm
#include "organism.h"

#ifdef _OPENMP
#include "omp.h"
#endif

namespace libevocosm
{
    //! An abstract interface defining a fitness landscape
    /*!
        A "fitness" landscape defines the environment in which an
        organism competes for survival. A class implementing this
        interface will test each organism in a list against some criteria.
        The landscape is tied to the nature of the organism; think of an
        organism as a potential solution to a problem posed by the landscape.

        A floating-point organism, for example, could be tested by a fitness
        landscape that represents a function to be maximized. Or, an organsism
        describing the shape of wing could be tested by a landscape
        implementation that understands airflow.
        \param OrganismType - A concrete implementation of the organism template (e.g., the type of organism that "lives" in this landscape)
    */
    template <class OrganismType>
    class landscape : protected globals
    {
        public:
            //! Constructor
            /*!
                Creates a new landscape object
                \param a_listener - a listener for events
            */
            landscape(listener<OrganismType> & a_listener)
              : m_listener(a_listener)
            {
                // nada
            }

            //! Copy constructor
            landscape(const landscape & a_source)
              : m_listener(a_source.m_listener)
            {
                // nada
            }

            //! Assignment operator
            landscape & operator = (const landscape & a_source)
            {
                m_listener = a_source.m_listener;
                return *this;
            }

            //! Virtual destructor
            /*!
                A virtual destructor. By default, it does nothing; this is
                a placeholder that identifies this class as a potential base,
                ensuring that objects of a derived class will have their
                destructors called if they are destroyed through a base-class
                pointer.
            */
            virtual ~landscape()
            {
                // nada
            }

            //! Performs fitness testing
            /*!
                Tests a single chromosome for fitness.
                \param a_organism - The organism to be tested by the landscape.
                \param a_verbose - Display verbose information for test
                \return Computed fitness for this organism
            */
            virtual double test(OrganismType & a_organism, bool a_verbose = false) const = 0;

            //! Performs fitness testing
            /*!
                Tests each chromosome in a_population for fitness.
                \param a_population - A vector containing organisms to be tested by the landscape.
                \return A fitness value for the population as a whole; application-defined.
            */
            virtual double test(vector<OrganismType> & a_population) const
            {
                double result = 0.0;

                for (int n = 0; n < (int)a_population.size(); ++n)
                {
                    a_population[n].fitness = test(a_population[n]);
                    result += a_population[n].fitness;
                }

                // return average fitness
                return 0.0; // result / (double)a_population.size();
            }

        protected:
            //! The listener for landscape events
            listener<OrganismType> & m_listener;
    };
};

#endif
