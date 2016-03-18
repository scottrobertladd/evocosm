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

#if !defined(LIBEVOCOSM_EVOREAL_H)
#define LIBEVOCOSM_EVOREAL_H

// libevocosm
#include "evocommon.h"

namespace libevocosm
{
    //! Tools for evolving real numbers
    /*!
        The majority of genetic algorithms work on pure bit strings, converting
        those strings to the desired types for fitness testing. In Lawrence
        Davis' book "Handbook of Genetic Algorithms," he transforms a 44-bit
        string into two floating point values via a series of operations. I've
        seen similar techniques elsewhere, and I find them a bit cumbersome.
        <p>
        In the purest sense, a GA should have no knowledge of the format of the
        data it is modifying; however, natural chromosomes do encode some
        structure in their sequence; for example, crossover appears to take place
        in specific positions along the chromosome. And while mutation doesn't
        care about a chromosome's structure, it does affect that structure.
        In context of a computer program, the structure of a chromosome isn't
        so important as the ability to logically modify its bits through
        crossover and mutation.
        <p>
        I built tools for the mutation and crossover of encoded floating-point
        values of types <b>float</b> and <b>double</b>. The code that follows
        assumes we are working with 32-bit floats and 64-bit IEEE-754 doubles,
        which, in my experience, the norm for many C and C++ compilers. Yes,
        I'm aware of the VAX and other systems; this code is explicitly
        non-portable outside implementations of IEC 60559/IEEE-754.
    */
    class evoreal : protected globals
    {
    public:
        //! Creation constructor
        /*!
            Creates a new evoreal object based on a set of  weights that define
            the chance of mutation in various components of a floating-point value.
            The default weights have worked well in a variety of applications, but
            are (of course) settable for specific application and experimentation.
            <p>
            Each weight is a percentage of the total of all three weights; for
            example, if the three weights add to 100 (as they do by efault), and
            a_sign_weight is 12, the chance of a mutation in the sign bit is 12%.
            The default weights were chosen based on experience in using these
            tools in a variety of applications.
            \param a_sign_weight - Weight assigned to changes in sign
            \param a_exponent_weight - Weight assigned to changes in the exponent
            \param a_mantissa_weight - Weight assigned to changes in the mantissa
        */
        evoreal(float a_sign_weight = 5.0F, float a_exponent_weight =  5.0F, float a_mantissa_weight = 90.0F);

        //! Copy constructor
        /*!
            Creates a new evoreal with the same states as an existing one.
            \param a_source - The source object
        */
        evoreal(evoreal & a_source);

        //! Assignment
        /*!
            Assigns the state of one evoreal to another.
            \param a_source - The source object
        */
        evoreal & operator = (evoreal & a_source);

        //! Mutation for <b>float</b> values
        /*!
            Returns a new float that is a mutated version of the argument.
            \param a_f - Number to be cloned; the result is then mutated
            \return A clone of a_f that has bene mutated
        */
        float  mutate(float  a_f);

        //! Mutation for <b>double</b> values
        /*!
            Returns a new float that is a mutated version of the argument.
            \param a_d - Number to be cloned; the result is then mutated
            \return A clone of a_d that has bene mutated
        */
        double mutate(double a_d);

        //! Crossover for <b>float</b> values
        /*!
            Creates a new float by combining two values through a
            real-specialized form of crossover.
            \param a_f1 - First parent number
            \param a_f2 - Second parent number
            \return A combination of the two arguments
        */
        float crossover(float  a_f1, float  a_f2);

        //! Crossover for <b>double</b> values
        /*!
            Creates a new double by combining two values through a
            real-specialized form of crossover.
            \param a_d1 - First parent number
            \param a_d2 - Second parent number
            \return A combination of the two arguments
        */
        double crossover(double a_d1, double a_d2);

    private:
        // weights used to select parts of a number for manipulation
        const float m_total_weight;
        const float m_sign_weight;
        const float m_exp_weight;

#if defined(_MSC_VER) && (_MSC_VER < 1300)
        static const long FLT_EXP_BITS;
        static const long DBL_EXP_BITS;
#else
        static const long FLT_EXP_BITS = 0x7F800000L;
        static const long DBL_EXP_BITS = 0x7FF00000UL;
#endif
    };
}

#endif

