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

#if !defined(LIBEVOCOSM_ANALYZER_H)
#define LIBEVOCOSM_ANALYZER_H

// libevocosm
#include "organism.h"
#include "listener.h"

namespace libevocosm
{
    //! Reports on a given population
    /*!
        After fitness testing, the Evocosm invokes an analyzer. At a minimum,
        the analyzer determines if the population has reached a "final" or
        "finished" state.
        \param OrganismType - The type of organism
    */
    template <typename OrganismType>
    class analyzer : protected globals
    {
    public:
        //! Constructor
        /*!
            Creates a new reporter object
            \param a_listener - a listener for events
        */
        analyzer(listener<OrganismType> & a_listener, size_t max_iterations = 0)
            : m_listener(a_listener),
              m_max_iterations(max_iterations)
        {
            // nada
        }

        //! Copy constructor
        analyzer(const analyzer & a_source)
            : m_listener(a_source.m_listener),
              m_max_iterations(a_source.m_max_iterations)
        {
            // nada
        }

        //! Assignment operator
        analyzer & operator = (const analyzer & a_source)
        {
            m_listener = a_source.m_listener;
            m_max_iterations = a_source.m_max_iterations;
        }

        //! Virtual destructor
        /*!
            A virtual destructor. By default, it does nothing; this is
            a placeholder that identifies this class as a potential base,
            ensuring that objects of a derived class will have their
            destructors called if they are destroyed through a base-class
            pointer.
        */
        virtual ~analyzer()
        {
            // nada
        }

        //! Reports on a population
        /*!
            The report method can do almost anything. In most case, it will display
            the "best" chromosome, draw a progress graph, or notify the user that
            another generation has passed. The return value tells an evocosm whether
            to continue evolving or not. The default implementation simply watches
            to see if a maximum number of iterations ahve been perfromed.
            \param a_population - A population of organisms
            \param a_iteration - Iteration count for this report
            \return <b>true</b> if evolution should continue; <b>false</b> if not
        */
        virtual bool analyze(const vector<OrganismType> & a_population,  size_t a_iteration)
        {
            if (m_max_iterations > 0)
                return (a_iteration < m_max_iterations);
            else
                return true;
        }

    protected:
        //! The listener for events
        listener<OrganismType> & m_listener;

        //! Maximum iterations; ignored if zero
        size_t m_max_iterations;
    };
};

#endif
