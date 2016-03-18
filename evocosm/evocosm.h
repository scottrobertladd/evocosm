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

#if !defined(LIBEVOCOSM_EVOCOSM_H)
#define LIBEVOCOSM_EVOCOSM_H

#if defined(_MSC_VER)
#pragma warning (disable : 4786)
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

// Standard C++ library
#include <vector>
#include <unistd.h>

// libevocosm
#include "validator.h"
#include "listener.h"
#include "organism.h"
#include "landscape.h"
#include "mutator.h"
#include "reproducer.h"
#include "scaler.h"
#include "selector.h"
#include "analyzer.h"

//! A toolkit and framework for implementing evolutionary algorithms.
/*!
    Evocosm classes abstract the fundamental components of an
    evolutionary algorithm. Evolutionary algorithms come in a variety of shapes
    and flavors, but at their core, they all share certain characteristics:
    populations that reproduce and mutate through a series of generations,
    producing future generations based on some measure of fitness. An amazing
    variety of algorithms can be built on that general framework, which lead
    me to construct a set of core classes as the basis for future applications.
*/
namespace libevocosm
{
    using std::vector;

    //! Associates organisms with the components of an evolutionary system.
    /*!
        This is where it all comes together: An evocosm binds a
        evocosm of organisms to a set of objects that define how
        those organisms evolve.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class evocosm : protected globals
    {
    protected:
        //! The populations of organisms
        vector<OrganismType> & m_population;

        //! Fitness landscapes common to all populations
        landscape<OrganismType> & m_landscape;

        //! A mutator to randomly influence genes
        mutator<OrganismType> & m_mutator;

        //! Creates new organisms
        reproducer<OrganismType> & m_reproducer;

        //! Scales the fitness of the evocosm
        scaler<OrganismType> & m_scaler;

        //! Selects organisms that survive from one generation to the next
        selector<OrganismType> & m_selector;

        //! Reports the a evocosm for analysis or display
        analyzer<OrganismType> & m_analyzer;

        //! A listener for evocosm progress
        listener<OrganismType> & m_listener;

        //! Count of iterations made
        size_t m_iteration;

        //! Number microseconds for process to sleep on yield
        unsigned int m_sleep_time;

    public:
        //! Creation constructor
        /*!
            Creates a new evocosm. Think of an evocosm as a director, a tool for
            associating organisms with their landscape.
            Note that these arguments are modifiable references, and that the
            referenced objects must continue to exist during the lifetime of the
            evocosm.
            \param a_population Initial population of organisms
            \param a_landscape Initial set of landscaoes for testing organism fitness
            \param a_mutator - A concrete implementation of mutator
            \param a_reproducer - A concrete implementation of reproducer
            \param a_scaler - A concrete implementation of scaler
            \param a_selector - A concrete implementation of selector
            \param a_analyzer - A concrete implementation of analyzer
            \param a_listener - a listener for events
        */
        evocosm(vector<OrganismType> &     a_population,
                landscape<OrganismType> &  a_landscape,
                mutator<OrganismType> &    a_mutator,
                reproducer<OrganismType> & a_reproducer,
                scaler<OrganismType> &     a_scaler,
                selector<OrganismType> &   a_selector,
                analyzer<OrganismType> &   a_analyzer,
                listener<OrganismType> &   a_listener);

        //! Copy constructor
        /*!
            Creates a new evocosm identical to an existing one.
            \param a_source - The source object
        */
        evocosm(const evocosm<OrganismType> & a_source);

        //! Virtual destructor
        /*!
            A virtual destructor. By default, it does nothing; this is
            a placeholder that identifies this class as a potential base,
            ensuring that objects of a derived class will have their
            destructors called if they are destroyed through a base-class
            pointer.
        */
        virtual ~evocosm();

        //! Assignment operator
        /*!
            Assigns an existing object the state of another.
            \param a_source - The source object
            \return Reference to target object
        */
        evocosm & operator = (const evocosm<OrganismType> & a_source);

        //! Compute next generation
        /*!
            A generation represents a cycle in the life of an evocosm; this
            function performs one sequence of fitness testing & scaling,
            reporting, breeding, and mutation. This method can be
            replaced by in a derived class to define a different processing
            sequence; the default sequence defined here is good for most
            evolutionary algorithms I've created.
            \return Returns <i>true</i> when the generation has reached a specific goal.
        */
        virtual bool run_generation();

        //! Directly view population
        /*! <b>Use with caution!</b> This function provides direct read-write
            access to an evocosm's population. This is necessary when the
            organisms need special manipulation, such as when they can not be
            randomized by a default constructor.
        */
        vector<OrganismType> & get_population()
        {
            return m_population;
        }

        //! Get the sleep time property value
        /*!
            Get the sleep time setting for this listerner.
            /return current value of sleep time (microseconds)
        */
        unsigned int get_sleep_time()
        {
            return m_sleep_time;
        }

        //! Set the sleep time property value
        /*!
            Set the sleep time property value.
            /param a_sleep_time new value of sleep time (microseconds)
        */
        void set_sleep_time(unsigned int a_sleep_time)
        {
            m_sleep_time = a_sleep_time;
        }

    protected:
        //! Yield
        /*!
            Evocosm periodically invokes this function to allow other processes
            to run. In most cases, this will be some sort of platform-specific
            sleep function, such as usleep.
        */
        void yield()
        {
            if (m_sleep_time > 0)
            {
              #if defined(_MSC_VER)
                Sleep(m_sleep_time);
              #else
                usleep((useconds_t)m_sleep_time);
              #endif
            }
        }

    };

    // constructors
    template <class OrganismType>
    evocosm<OrganismType>::evocosm(vector<OrganismType> &     a_population,
                                   landscape<OrganismType> &  a_landscape,
                                   mutator<OrganismType> &    a_mutator,
                                   reproducer<OrganismType> & a_reproducer,
                                   scaler<OrganismType> &     a_scaler,
                                   selector<OrganismType> &   a_selector,
                                   analyzer<OrganismType> &   a_analyzer,
                                   listener<OrganismType> &   a_listener)
      : m_population(a_population),
        m_landscape(a_landscape),
        m_mutator(a_mutator),
        m_reproducer(a_reproducer),
        m_scaler(a_scaler),
        m_selector(a_selector),
        m_analyzer(a_analyzer),
        m_listener(a_listener),
        m_iteration(0),
        m_sleep_time(10000) // default to 10ms sleep time
    {
        // nada
    }

    // copy constructor
    template <class OrganismType>
    evocosm<OrganismType>::evocosm(const evocosm<OrganismType> & a_source)
      : m_population(a_source.a_population),
        m_landscape(a_source.m_landscape),
        m_mutator(a_source.m_mutator),
        m_reproducer(a_source.m_reproducer),
        m_scaler(a_source.m_scaler),
        m_selector(a_source.m_selector),
        m_analyzer(a_source.m_analyzer),
        m_listener(a_source.m_listener),
        m_iteration(a_source.m_iteration),
        m_sleep_time(a_source.m_sleep_time)
    {
        // nada
    }

    // destructor
    template <class OrganismType>
    evocosm<OrganismType>::~evocosm()
    {
        // nada
    }

    // assignment operator
    template <class OrganismType>
    evocosm<OrganismType> & evocosm<OrganismType>::operator = (const evocosm<OrganismType> & a_source)
    {
        m_population  = a_source.m_population;
        m_landscape   = a_source.m_landscape;
        m_scaler      = a_source.m_scaler;
        m_analyzer    = a_source.m_analyzer;
        m_listener    = a_source.m_analyzer;
        m_iteration   = a_source.m_iteration;
        m_sleep_time  = a_source.m_sleep_time;

        return *this;
    }

    // compute next generation
    template <class OrganismType>
    bool evocosm<OrganismType>::run_generation()
    {
        bool keep_going = true;

        ++m_iteration;

        // announce beginning of new generation
        m_listener.ping_generation_begin(m_population, m_iteration);

        // check population fitness
        m_landscape.test(m_population);
        yield();

        // we're done testing this generation
        m_listener.ping_generation_end(m_population, m_iteration);
        yield();

        // analyze the results of testing, and decide if we're going to stop or not
        keep_going = m_analyzer.analyze(m_population, m_iteration);

        if (keep_going)
        {
            // fitness scaling
            m_scaler.scale_fitness(m_population);
            yield();

            // get survivors and number of chromosomes to add
            vector<OrganismType> survivors = m_selector.select_survivors(m_population);
            yield();

            // give birth to new chromosomes
            vector<OrganismType> children = m_reproducer.breed(m_population, m_population.size() - survivors.size());
            yield();

            // debugging only
            //fitness_stats<OrganismType> s(survivors);
            //fitness_stats<OrganismType> c(children);

            // mutate the child chromosomes
            m_mutator.mutate(children);
            yield();

            // append children to survivors and replace existing population form combined vector
            survivors.insert(survivors.end(),children.begin(),children.end());
            m_population = survivors;
            yield();
        }
        else
        {
            m_listener.run_complete(m_population);
        }

        return keep_going;
    }
};

#endif
