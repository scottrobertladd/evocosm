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

#if !defined(EVOCOSM_FUNCTION_OPTIMIZER_H)
#define EVOCOSM_FUNCTION_OPTIMIZER_H

#include <vector>
#include <stdexcept>
#include <limits>

// other elements of Evocosm
#include "evocosm.h"
#include "evoreal.h"

// OpenMP support, if requested
#if defined(_OPENMP)
#include <omp.h>
#endif

namespace libevocosm
{
    //! Global things used by all optimizer classes
    /*!
        I put all cross-class, shared items into a single base class; this
        encapsulates global data, and also show why multiple inheritance
        is a useful tool.
    */
    class fopt_global
    {
    protected:
        //! Definition of a function type
        /*!
            This is the type of function being optimized. <i><b>The inner nature
            of this function is not defined by this optimizer.</b></i> A t_function
            can be anything -- a simple numerical formula or a simulation. For
            example: Consider a set of arguments that describe the shape of a wing,
            and a function that tests shapes in a simulated wind tunnel, the
            fitness representing "lift".
        */
        typedef vector<double> t_function(vector<double> a_args);

        //! Provides mutation and crossover services for doubles
        static evoreal g_evoreal;
    };

    //! A potential solution to the problem at hand
    /*!
        To support a wide variety of applications, I defined a function solution
        as a vector of doubles. This allows functions of any dimension to be
        optimized (within platform limits, of course). Note that the solution
        begins with an empty vector; this is because the number of elements and
        their initialization is application specific. Initialization of the genes
        takes place in the constructor for function_optimizer through a pointer
        to a user-supplied function.
    */
    class function_solution : public organism< vector<double> >, protected fopt_global
    {
    public:
        /*!
            Value is the actual computed value of this solution, which may or may not
            correspond directly to fitness. For example, when minimizing a function,
            the lowest value may correspond to the highest fitness. Generally, this is
            a display property.
        */
        double value;

    protected:
        /*! used to cast base class pointer to concrete class */
        virtual void child_copy(const organism & a_source)
        {
            value = dynamic_cast<const function_solution &>(a_source).value;
        }

    private:
        double m_minarg;
        double m_maxarg;
        double m_extent;

    public:
        //! Default contructor
        /*!
            Creates an empty solution.
        */
        function_solution()
          : organism< vector<double> >(),
            value(0.0),
            m_minarg(-1.0),
            m_maxarg(1.0),
            m_extent(2.0)
        {
            // nada
        }

        //! Creation constructor
        /*!
            Creates a new solution with an empty vector.
        */
        function_solution(int a_nargs, double a_minarg, double a_maxarg)
          : organism< vector<double> >(), value(0.0)
        {
            if (a_maxarg < a_minarg)
            {
                double temp = a_maxarg;
                a_maxarg = a_minarg;
                a_minarg = temp;
            }

            m_minarg = a_minarg;
            m_maxarg = a_maxarg;
            m_extent = a_maxarg - a_minarg;

            // values fall in the range [-1,1]
            for (int n = 0; n < a_nargs; ++n)
                genes.push_back(g_random.get_real() * m_extent + a_minarg);
        }

        //! Construct from raw genes
        /*!
            Constructs a new function solution from a set of genes
        */
        function_solution(const vector<double> & a_source)
          : organism< vector<double> >(a_source), value(0.0)
        {
            // nada
        }

        //! Construct from base class
        /*!
            Constructs a function solution from a base-class object.
            \param a_source - The source object
        */
        function_solution(const organism< vector<double> > & a_source)
          : organism< vector<double> >(a_source), value(0.0)
        {
            // nada
        }

        //! Copy constructor
        /*!
            Duplicates an existing object.
            \param a_source - The source object
        */
        function_solution(const function_solution & a_source)
          : organism< vector<double> >(a_source),
            value(a_source.value),
            m_minarg(a_source.m_minarg),
            m_maxarg(a_source.m_maxarg),
            m_extent(a_source.m_extent)
        {
            // nada
        }

        //! Virtual destructor
        /*!
            Satisfies the requirements of the base class; does nothing
        */
        virtual ~function_solution()
        {
            // nada
        }

        //! Assignment operator
        /*!
            Assigns the state of one solution to another.
            \param a_source - The source object
            \return A reference to <i>this</i>
        */
        function_solution & operator = (const function_solution & a_source)
        {
            organism< vector<double> >::operator = (a_source);
            value = a_source.value;
            m_minarg = a_source.m_minarg;
            m_maxarg = a_source.m_maxarg;
            m_extent = a_source.m_extent;
            return *this;
        }

        //! Comparison operator for algorithms
        /*!
            Returns true if the target object is greater than a_source. While the
            operator may be "less than", what it really meands for algorithms is
            "comes before" -- and I want the highest fitness to come first.
            \param a_right - Right hand argument for less than operator
            \return <b>true</b> if this organismsfitness if greater than <code>a_right.fitness</code>; <b>false</b> otherwise
        */
        virtual bool operator < (const organism< vector<double> > & a_right) const
        {
            return (fitness > a_right.fitness);
        }
    };

    //! Mutates solutions
    /*!
        Implements chance-based mutation of solutions.
    */
    class function_mutator : public mutator<function_solution>, protected fopt_global
    {
    public:
        //! Creation constructor
        /*!
            Creates a new mutator with a given mutation rate.
        */
        function_mutator(double a_mutation_rate)
          : m_mutation_rate(a_mutation_rate)
        {
            // adjust mutation rate if necessary
            if (m_mutation_rate > 1.0)
                m_mutation_rate = 1.0;
            else if (m_mutation_rate < 0.0)
                m_mutation_rate = 0.0;
        }

        //! Copy constructor
        /*!
            Duplicates an existing object.
            \param a_source - The source object
        */
        function_mutator(const function_mutator & a_source)
            : m_mutation_rate(a_source.m_mutation_rate)
        {
            // nada
        }

        //! Virtual destructor
        /*!
            Satisfies the requirements of the base class; does nothing at this time.
        */
        virtual ~function_mutator()
        {
            // nada
        }

        //! Assignment operator
        /*!
            Assigns the state of one object to another.
            \param a_source - The source object
            \return A reference to <i>this</i>
        */
        function_mutator & operator = (const function_mutator & a_source)
        {
            m_mutation_rate = a_source.m_mutation_rate;
            return *this;
        }

        //! Gets the mutation rate
        /*!
            Returns the mutation rate set for this mutator.
            \return Mutation rate
        */
        double mutation_rate() const
        {
            return m_mutation_rate;
        }

        //! Performs mutations
        /*!
            Mutates a solution using the facilities provided by g_evoreal.
            \param a_population - Solutions to be mutated
        */
        void mutate(vector<function_solution> & a_population);

    private:
        // rate of mutation
        double m_mutation_rate;
    };

    //! Implements reproduction
    /*!
        Handles reproduction for solutions.
    */
    class function_reproducer : public reproducer<function_solution>, protected fopt_global
    {
    public:
        //! Creation constructor
        /*!
            Creates a new reproducer with a given crossover rate.
        */
        function_reproducer(double p_crossover_rate = 1.0)
            : m_crossover_rate(p_crossover_rate)
        {
            // adjust crossover rate if necessary
            if (m_crossover_rate > 1.0)
                m_crossover_rate = 1.0;
            else if (m_crossover_rate < 0.0)
                m_crossover_rate = 0.0;
        }

        //! Copy constructor
        /*!
            Duplicates an existing object.
            \param a_source - The source object
        */
        function_reproducer(const function_reproducer & a_source)
            : m_crossover_rate(a_source.m_crossover_rate)
        {
            // nada
        }

        //! Virtual destructor
        /*!
            Satisfies the requirements of the base class; does nothing at this time.
        */
        virtual ~function_reproducer()
        {
            // nada
        }

        //! Assignment operator
        /*!
            Assigns the state of one object to another.
            \param a_source - The source object
            \return A reference to <i>this</i>
        */
        function_reproducer & operator = (const function_reproducer & a_source)
        {
            m_crossover_rate = a_source.m_crossover_rate;
            return *this;
        }

        //! Gets the crossover rate
        /*!
            Returns the crossover rate set for this reproducer.
            \return Crossover rate
        */
        double crossover_rate() const
        {
            return m_crossover_rate;
        }

        //! Reproduction for solutions
        /*!
            Breeds new solutions, by cloning or the combination of elements from parent organisms. By
            default, the crossover rate is 1.0 (100%), meaning that all children stem from crossover
            between two parents. The crossover is implemented on a argument-by-argument basis; in other
            words, crossover occurs between corresponding elements of the parent solutions.
            \param a_population - A population of solutions
            \param p_limit - Maximum number of children
            \return A vector containing new "child" chromosomes
        */
        virtual vector<function_solution> breed(const vector<function_solution> & a_population, size_t p_limit);

    private:
        // crossover chance
        double m_crossover_rate;
    };

    //! Defines the test for a population of solutions
    /*!
        A "fitness" landscape defines the environment in which an organism
        competes for survival. In the case of function optimization, the
        "test" is (obviously!) a function.
    */
    class function_landscape : public landscape<function_solution>, protected fopt_global
    {
    public:
        //! Creation constructor
        /*!
            Creates a new landscape with a given fitness function.
            \param a_function function to be tested
            \param a_listener a listener for events during testing
        */
        function_landscape(t_function * a_function, listener<function_solution> & a_listener)
          : landscape<function_solution>(a_listener),
            m_function(a_function)
        {
            // nada
        }

        //! Copy constructor
        function_landscape(const function_landscape & a_source)
          : landscape<function_solution>(a_source),
            m_function(a_source.m_function)
        {
            // nada
        }

        //! Assignment
        function_landscape & operator = (const function_landscape & a_source)
        {
            landscape<function_solution>::operator = (a_source);
            m_function = a_source.m_function;
            return *this;
        }

        //! Virtual destructor
        /*!
            Satisfies the requirements of the base class; does nothing at this time.
        */
        ~function_landscape()
        {
            // nada
        }

        //! Performs fitness testing
        /*!
            Tests a single chromosome for fitness by calling the fitness function provided
            in the constructor.
            \param a_organism the organism to be tested by the landscape.
            \param a_verbose should this test produce lots of output?
            \return Computed fitness for this organism
        */
        virtual double test(function_solution & a_organism, bool a_verbose = false) const
        {
            vector<double> z = m_function(a_organism.genes);
            a_organism.value   = z[0];
            a_organism.fitness = z[1];
            return a_organism.fitness;
        }

    private:
        // fitness function pointer
        t_function * m_function;
    };

    //! Reports the state of a population of solutions
    /*!
        A simple analyzer for diaplying information about the populations
        as it evolves.
    */
    class function_analyzer : public analyzer<function_solution>
    {
    private:
        function_solution m_prev_best;
        size_t m_count;

    public:
        //! Constructor
        /*!
            Creates a new analyzer object
            \param a_listener - a listener for events
        */
        function_analyzer(listener<function_solution> & a_listener, size_t max_iterations)
            : analyzer<function_solution>(a_listener, max_iterations),
              m_prev_best(function_solution()),
              m_count(0)
        {
            // nada
        }

        //! Reports on a population
        /*!
            The report method can do almost anything. In most case, it will display
            the "best" chromosome, draw a progress graph, or notify the user that
            another generation has passed. The return value tells an evocosm whether
            to continue evolution (changes in the population) or not.
            \param a_population - A population of organisms
            \param a_iteration - Iteration count for this report
            \param a_fitness - Assigned the fitness value; implementation-defined
            \return <b>true</b> if the evocosm should evolve the population more; <b>false</b> if no evolution is required.
        */
        virtual bool analyze(const vector<function_solution> & a_population,
                             size_t a_iteration,
                             double & a_fitness);
    };

    //! An listener implementation that ignores all events
    /*!
        This listener displays information about events to stdout
        (standard ouput).
    */
    class function_listener : public null_listener<function_solution>
    {
    public:
        //! Ping that a generation begins
        /*!
            Ping that processing a generation begins.
            \param a_iteration One-based number of the generation begun
        */
        virtual void ping_generation_begin(size_t a_iteration);

        //! Ping that a generation ends
        /*!
            Ping that processing a generation has ended.
            \param a_population population for which processing has ended
            \param a_iteration One-based number of the generation ended
        */
        virtual void ping_generation_end(const vector<function_solution> & a_population, size_t a_iteration);
    };

    //! A generic function optimizer
    /*!
        Using instances of the other classes, this class binds together the pieces to
        create a complete function optimizer. A user of this class defines two functions
        -- a solution initializer and a fitness test -- that define the target problem.
    */
    class function_optimizer : protected fopt_global, protected function_listener
    {
    private:
        // objects that define the characteristics of the genetic algorithm
        vector<function_solution>             m_population;
        function_landscape                    m_landscape;
        function_mutator                      m_mutator;
        function_reproducer                   m_reproducer;
        linear_norm_scaler<function_solution> m_scaler;
        elitism_selector<function_solution>   m_selector;
        evocosm<function_solution> *          m_evocosm;
        const size_t                          m_iterations;
        function_analyzer                     m_analyzer;

    public:
        //! Constructor
        /*!
            Creates a new function_optimizer with the given set of parameters.
            \param a_function - Address of the function to be optimized.
            \param a_nargs - number of arguments per solution
            \param a_minarg - minimum argument value
            \param a_maxarg - maximum argument value
            \param a_norgs - The size of the solution population.
            \param a_mutation_rate - Mutation rate in the range [0,1].
            \param a_iterations - Number of iterations to perform when doing a run.
        */
        function_optimizer(t_function * a_function,
                           size_t       a_nargs,
                           double       a_minarg,
                           double       a_maxarg,
                           size_t       a_norgs,
                           double       a_mutation_rate,
                           size_t       a_iterations);

        //! Destructor
        /*!
            Cleans up resources by removing allocated objects.
        */
        virtual ~function_optimizer();

        //! Performs optimization
        /*!
            This is where the work gets done; run iterates the number of times specificed when creating
            this function_optimizer, reporting via a function_analyzer object on the progress of
            optimization.
        */
        void run();
    };

};

#endif
