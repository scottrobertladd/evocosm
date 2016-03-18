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

#if !defined(LIBEVOCOSM_SCALER_H)
#define LIBEVOCOSM_SCALER_H

// libevocosm
#include "organism.h"
#include "stats.h"

namespace libevocosm
{
    //! Fitness scaling for a population
    /*!
        As a population converges on a definitive solution, the difference
        between fitness values may become very small. That prevents the
        best solutions from having a significant advantage in reproduction.
        Fitness scaling solves this problem by adjusting the fitness values
        to the advantage of the most-fit chromosomes.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class scaler : protected globals
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
        virtual ~scaler()
        {
            // nada
        }

        //! Scale a population's fitness values
        /*!
            The scale_fitness method can adjust the fitness of a population
            to make it more likely that the "best" (whatever that menas)
            organisms have the best chance of reproduction.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population) = 0;
    };

    //! A do-nothing scaler
    /*!
        The null_scaler doesn't scale anything; it's just a placeholder used
        in evolutionary algorithms that do not use fitness scaling.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class null_scaler : public scaler<OrganismType>
    {
    public:
        //! Do-nothing scaling function
        /*!
            Has no effect on the target population.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population)
        {
            // nada
        }
    };

    //! A linear normalization scaler
    /*!
        A simple scaler implementing a configurable linear normalization scaler, as
        per Goldberg 1979.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class linear_norm_scaler : public scaler<OrganismType>
    {
    public:
        //! Constructor
        /*!
            Creates a new scaler for linear normalization.
        */
        linear_norm_scaler(double a_fitness_multiple = 2.0)
            : m_fitness_multiple(a_fitness_multiple)
        {
            // nada
        }

        //! Scaling function
        /*!
            Performs linear normalization on the fitness of the target population.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population)
        {
            // calculate max, average, and minimum fitness for the population
            fitness_stats<OrganismType> stats(a_population);

            // calculate coefficients for fitness scaling
            double slope;
            double intercept;
            double delta;

            if (stats.getMin() > ((m_fitness_multiple * stats.getMean() - stats.getMax()) / (m_fitness_multiple - 1.0)))
            {
                // normal scaling
                delta = stats.getMax() - stats.getMean();
                slope = (m_fitness_multiple - 1.0) * stats.getMean() / delta;
                intercept = stats.getMean() * (stats.getMax() - m_fitness_multiple * stats.getMean()) / delta;
            }
            else
            {
                // extreme scaling
                delta = stats.getMean() - stats.getMin();
                slope = stats.getMean() / delta;
                intercept = -stats.getMin() * stats.getMean() / delta;
            }

            // adjust fitness values
            for (size_t n = 0; n < a_population.size(); ++n)
                a_population[n].fitness = slope * a_population[n].fitness + intercept;
        }

    private:
        double m_fitness_multiple;
    };

    //! A windowed fitness scaler
    /*!
        Implements windowed fitness scaling, whereby all fitness values are modified
        by subtracting the minimum fitness in the population.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class windowed_scaler : public scaler<OrganismType>
    {
    public:
        //! Constructor
        /*!
            Creates a new windowed scaler with a given set of parameters.
        */
        windowed_scaler()
        {
            // nada
        }

        //! Scaling function
        /*!
            Performs windowed scaling on the fitness of the target population.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population)
        {
            fitness_stats<OrganismType> stats(a_population);

            // assign new fitness values
            for (int n = 0; n < a_population.size(); ++n)
                a_population[n].fitness = stats.getMin();
        }
    };

    //! An exponential fitness scaler
    /*!
        Implements an exponential fitness scaling, whereby all fitness values are modified
        such that new fitness = (a * fitness + b) ^ n.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class exponential_scaler : public scaler<OrganismType>
    {
    public:
        //! Constructor
        /*!
            Creates a new exponential scaler with a given set of parameters. The
            formula used is new_fitness = (a * fitness + b) ^ power.
            \param a_a - A multplier against the fitness
            \param a_b - Added to fitness before exponentiation
            \param a_power - Power applied to the value
        */
        exponential_scaler(double a_a = 1.0, double a_b = 1.0, double a_power = 2.0)
          : m_a(a_a),
            m_b(a_b),
            m_power(a_power)
        {
            // nada
        }

        //! Scaling function
        /*!
            Performs exponential scaling on the fitness of the target population.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population)
        {
            // assign new fitness values
            for (int n = 0; n < a_population.size(); ++n)
                a_population[n].fitness = pow((m_a * a_population[n].fitness + m_b),m_power);
        }

    private:
        double m_a;
        double m_b;
        double m_power;
    };

    //! A quadratic scaler
    /*!
        Uses a quadratic equation to scale the fitness of organisms.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class quadratic_scaler : public scaler<OrganismType>
    {
    public:
        //! Constructor
        /*!
            Creates a new scaler for quadratic scaling.
        */
        quadratic_scaler(double a_a, double a_b, double a_c)
            : m_a(a_a), m_b(a_b), m_c(a_c)
        {
            // nada
        }

        //! Scaling function
        /*!
            Performs quadratic scling on the fitness of the target population.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population)
        {
            // adjust fitness values
            for (int n = 0; n < a_population.size(); ++n)
            {
                double f = a_population[n].fitness;
                a_population[n].fitness = m_a * pow(f,2.0) + m_b * f + m_c;
            }
        }

    private:
        double m_a;
        double m_b;
        double m_c;
    };

    //! A sigma scaler
    /*!
        A sigma scaler, as per Forrest and Tanese.
        \param OrganismType - The type of organism
    */
    template <class OrganismType>
    class sigma_scaler : public scaler<OrganismType>
    {
    public:
        //! Constructor
        /*!
            Creates a new sigma scaler
        */
        sigma_scaler()
        {
        }

        //! Scaling function
        /*!
            Performs sigma scaling, which maintains selection pressure over the
            length of a run, thus minimizing the affects of convergence on
            reproductive selection. The function adjusts an organism's fitness
            in relation to the standard deviation of the population's fitness.
            \param a_population - A population of organisms
        */
        virtual void scale_fitness(vector<OrganismType> & a_population)
        {
            fitness_stats<OrganismType> stats(a_population);

            // calculate 2 times the std. deviation (sigma)
            double sigma2 = 2.0 * stats.getSigma();

            // now assign new fitness values
            if (sigma2 == 0.0)
            {
                for (int n = 0; n < a_population.size(); ++n)
                    a_population[n].fitness = 1.0;
            }
            else
            {
                for (int n = 0; n < a_population.size(); ++n)
                {
                    // change fitness
                    a_population[n].fitness = (1.0 + a_population[n].fitness / stats.mean) / sigma2;

                    // avoid tiny or zero fitness value; everyone gets to reproduce
                    if (a_population[n].fitness < 0.1)
                        a_population[n].fitness = 0.1;
                }
            }
        }
    };

};

#endif
