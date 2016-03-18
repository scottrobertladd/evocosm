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

#if !defined(LIBEVOCOSM_ROULETTE_H)
#define LIBEVOCOSM_ROULETTE_H

// standard C++ Library
#include <cmath>
#include <vector>
#include <limits>

// libevocosm
#include "evocommon.h"

namespace libevocosm
{
    using std::vector;

    //! A simulated roulette wheel for weighted selection.
    /*!
        You've probably seen a standard gambler's roulette wheel, a spinning
        circle divided into thirty-seven or thirty-eight equal-sized, pie-shaped
        sections. The croupier sets the wheel spinning and at the same time
        tosses an marble into the bowl in the direction opposite to that in
        which the wheel is moving; when the motion of the wheel ceases, the ball
        comes to rest in one of the numbered sections.

        As an algorithm, a roulette wheel is an array of weights, and the marble
        is a random integer less than the sum of all weights in the population.
        To pick on of the "section" (in this case, an index), the algorithm
        simulates the bouncing metal ball by iterating through the array of
        weights; if the marble value is less than the current weight (i.e., the
        ball has stopped), the algorithm returns the index of that weight.
        Otherwise, the algorithm subtracts the current weight from the
        marble value, and repeats the process with the next element in the
        weight array. Thus the largest weights values tend to be the most likely
        resting places for the marble, since they use a larger area of the
        abstract wheel.
    */
    class roulette_wheel : protected globals
    {
    public:
        //! Creation constructor (from vector)
        /*!
            Creates a new roulette_wheel based on a set of weights.
            \param a_weights - A vector of weights defining the wheel
            \param a_min_weight - Minimum possible weight value (defaults to epsilon for type)
            \param a_max_weight - Maximum possible weight value (defaults to max for type)
        */
        roulette_wheel(const vector<double> & a_weights,
                       double a_min_weight = std::numeric_limits<double>::epsilon(),
                       double a_max_weight = std::numeric_limits<double>::max());

        //! Creation constructor (from c-type array)
        /*!
            Creates a new roulette_wheel based on a set of weights.
            \param a_weights - An array of weights defining the wheel
            \param a_length - Number of elements in <i>a_weights</i>
            \param a_min_weight - Minimum possible weight value (defaults to 1.0)
            \param a_max_weight - Maximum possible weight value (defaults to 100.0)
        */
        roulette_wheel(const double * a_weights,
                       size_t a_length,
                       double a_min_weight = std::numeric_limits<double>::epsilon(),
                       double a_max_weight = std::numeric_limits<double>::max());

        //! Copy constructor
        /*!
            Creates a new roulette_wheel from an existing one.
            \param a_source - The source object
        */
        roulette_wheel(const roulette_wheel & a_source);

        //! Assignment operator
        /*!
            Assigns a roulette_wheel the state of another.
            \param a_source - The source object
        */
        roulette_wheel & operator = (const roulette_wheel & a_source);

        //! Destructor
        /*!
            Destroys a roulette_wheel, freeing resources.
        */
        ~roulette_wheel();

        //! Get size (number of weights)
        /*!
            Gets the number of weights indexed by the roulette wheel.
            \return The number of weights
        */
        size_t get_size() const
        {
            return m_size;
        }

        //! Change the weight assigned to an entry
        /*!
            Changes the weight assigned to a specific wheel index.
            \param a_index - Index to change
            \param a_weight - New weight Value
            \return Previous weight for this index, or -1 if a_index is invalid
        */
        double set_weight(size_t a_index, double a_weight);

        //! Get the weight for an index
        /*!
            Gets the weight assigned to a specific index.
            \param a_index Index for which weight should be returned
            \return Weight for this index, or -1 if a_index is invalid
        */
        double get_weight(size_t a_index) const;

        //! Retrieve a random index
        /*!
            Returns a randomly-selected index value by simulating the
            bouncing of a ball on a roulette wheel, where each index is
            a "slot" with a width determined by its assigned weight.
            \return A random index value
        */
        size_t get_index() const;

    protected:
        //! Number of weights in this wheel
        size_t m_size;

        //! Array of m_weights
        double * m_weights;

        //! Total weight of all indexes
        double m_total_weight;

        //! Minimum possible weight value
        double m_min_weight;

        //! Maximum possible weight value
        double m_max_weight;

    private:
        // internal copy function
        void copy(const roulette_wheel & a_source);
    };
};

#endif
