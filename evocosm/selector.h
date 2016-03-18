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

#if !defined(EVOCOSM_SELECTOR_H)
#define EVOCOSM_SELECTOR_H

// Standard C++ Library
#include <algorithm>

// libevocosm
#include "organism.h"

namespace libevocosm
{
    //! Selects organisms that survive
    /*!
        A selector decides which organisms survive from one generation to the
        next. Some evolutionary algorithms will not use a selector; others will.
        In general, it is effective to keep the "best" organisms from one
        generation ot the next, so that good genes do not become lost at
        random.
        <p>
        Of course, "real" nature isn't so kind; just because you're the
        biggest stag in the forest doesn't mean you won't get shot! In fact,
        that may say something about the "best" -- maybe the "best" stag is
        the one that is too ugly for anyone to hunt?
        \param OrganismType - The type of organism being selected
    */
    template <class OrganismType>
    class selector : protected globals
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
        virtual ~selector()
        {
            // nada
        }

        //! Select individuals that survive
        /*!
            Produces a vector containing copies of the organisms selected for
            survival.
            \param a_population - A population of organisms
            \return A population of copied survivors
        */
        virtual vector<OrganismType> select_survivors(vector<OrganismType> & a_population) = 0;
    };

    //! A do-nothing selector
    /*!
        The null_selector doesn't select anything; it's just a placeholder used
        in evolutionary algorithms that do not use survival.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class null_selector : public selector<OrganismType>
    {
    public:
        // Do-nothing selection function
        /*!
            Has no effect on the target population.
            \param a_population - A population of organisms
        */
        virtual vector<OrganismType> select_survivors(vector<OrganismType> & a_population)
        {
            return vector<OrganismType>(); // an empty vector
        }
    };

    //! A do-nothing selector
    /*!
        The all_selector returns a copy of the original population, all organisms survive.
        This is primarily used for development and testing, and is of little practical use.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class all_selector : public selector<OrganismType>
    {
    public:
        // Do-nothing selection function
        /*!
            Has no effect on the target population.
            \param a_population - A population of organisms
        */
        virtual vector<OrganismType> select_survivors(vector<OrganismType> & a_population)
        {
            vector<OrganismType> result;

            for (int n = 0; n < a_population.size(); ++n)
                result.push_back(a_population[n]);

            return result;
        }
    };

    //! Implements a elitism selector
    /*!
        Elitism is one of the simplest type of selection -- it simply says that
        the <i>n</i> best organisms will survive into the next generation.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class elitism_selector : public selector<OrganismType>
    {
    public:
        //! Constructor
        /*!
            Constructs a new elistism_selector with a value that defines the number of
            organisms that should survive into the next generation.
            \param a_factor - Percentage of bets fitness required to surivive
        */
        elitism_selector(double a_factor = 0.9)
            : m_factor(a_factor)
        {
            // nada
        }

        //! Copy constructor
        /*!
            Creates a new object identical to an existing one.
            \param a_source - The source object
        */
        elitism_selector(const elitism_selector<OrganismType> & a_source)
            : m_factor(a_source.m_factor)
        {
            // nada
        }

        //! Assignment operator
        /*!
            Assigns an existing object the state of another.
            \param a_source - The source object
        */
        elitism_selector & operator = (const elitism_selector<OrganismType> & a_source)
        {
            m_factor = a_source.m_factor;
        }

        //! Select individuals that survive
        /*!
            Produces a vector containing copies of the organisms selected for
            survival.
            \param a_population - A population of organisms
            \return A population of copied survivors
        */
        virtual vector<OrganismType> select_survivors(vector<OrganismType> & a_population);

    private:
        // number of organisms to keep
        double m_factor;
    };

    template <class OrganismType>
    vector<OrganismType> elitism_selector<OrganismType>::select_survivors(vector<OrganismType> & a_population)
    {
        // create a new vector
        vector<OrganismType> chosen_ones;

        // get population stats
        fitness_stats<OrganismType> stats(a_population);

        // calculate survival based on percentage of best fitness
        double threshold = m_factor * stats.getBest().fitness;

        // pick survivors
        for (size_t n = 0; n < a_population.size(); ++n)
        {
            if (a_population[n].fitness > threshold)
                chosen_ones.push_back(a_population[n]);
        }

        // return result
        return chosen_ones;
    }

};

#endif
