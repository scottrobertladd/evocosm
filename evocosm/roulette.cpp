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

#include <stdexcept>
#include <cstring>

// libevocosm
#include "validator.h"
#include "roulette.h"
using namespace libevocosm;

// creation constructor
roulette_wheel::roulette_wheel(const vector<double> & a_weights, double a_min_weight, double a_max_weight)
  : m_size(a_weights.size()),
    m_weights(NULL),
    m_total_weight(0.0),
    m_min_weight(fabs(a_min_weight)),
    m_max_weight(fabs(a_max_weight))
{
    validate_less(a_min_weight,a_max_weight,"Minimum weight must be less than maximum");
    validate_not(m_size,size_t(0),"Roulette wheel can not have zero size");

    m_weights = new double[m_size];

    for (size_t i = 0; i < m_size; ++i)
    {
        m_weights[i]   = fabs(a_weights[i]);

        if (m_weights[i] < m_min_weight)
            m_weights[i] = m_min_weight;
        else
            if (m_weights[i] > m_max_weight)
                m_weights[i] = m_max_weight;

        m_total_weight += m_weights[i];
    }

    validate_greater(m_total_weight,0.0,"Roulette wheel must have a total weight > zero");
}

// create from C array
roulette_wheel::roulette_wheel(const double * a_weights, size_t a_size, double a_min_weight, double a_max_weight)
  : m_size(a_size),
    m_weights(NULL),
    m_total_weight(0.0),
    m_min_weight(fabs(a_min_weight)),
    m_max_weight(fabs(a_max_weight))
{
    validate_not(m_size,size_t(0),"Roulette wheel can not have zero size");
    validate_less(m_min_weight,m_max_weight,"Minimum weight must be less than maximum");
    validate_greater(m_min_weight,0.0,"Minimum weight must be > 0");

    m_weights = new double[m_size];

    for (size_t i = 0; i < m_size; ++i)
    {
        m_weights[i]   = fabs(a_weights[i]);

        if (m_weights[i] < m_min_weight)
            m_weights[i] = m_min_weight;
        else
            if (m_weights[i] > m_max_weight)
                m_weights[i] = m_max_weight;

        m_total_weight += m_weights[i];
    }

    validate_greater(m_total_weight,0.0,"Roulette wheel must have a total weight > zero");
}

// copy constructor
roulette_wheel::roulette_wheel(const roulette_wheel & a_source)
  : m_size(a_source.m_size),
    m_weights(NULL),
    m_total_weight(a_source.m_total_weight),
    m_min_weight(a_source.m_min_weight),
    m_max_weight(a_source.m_max_weight)
{
    m_weights = new double[m_size];
    memcpy(m_weights,a_source.m_weights,sizeof(double) * m_size);
}

// assignment operator
roulette_wheel & roulette_wheel::operator = (const roulette_wheel & a_source)
{
    if (this != &a_source)
    {
        m_size         = a_source.m_size;
        m_weights      = new double[m_size];
        memcpy(m_weights,a_source.m_weights,sizeof(double) * m_size);
        m_total_weight = a_source.m_total_weight;
        m_min_weight   = a_source.m_min_weight;
        m_max_weight   = a_source.m_max_weight;
    }

    return *this;
}

// destructor
roulette_wheel::~roulette_wheel()
{
    delete [] m_weights;
}

// change the a_weight of an entry
double roulette_wheel::set_weight(size_t a_index, double a_weight)
{
    validate_less(a_index,m_size,"invalid roulette wheel index");

    a_weight = fabs(a_weight);

    if (a_weight < m_min_weight)
        a_weight = m_min_weight;
    else
        if (a_weight > m_max_weight)
            a_weight = m_max_weight;

    m_total_weight -= m_weights[a_index];
    m_total_weight += a_weight;

    double res = m_weights[a_index];
    m_weights[a_index] = a_weight;

    return res;
}

// interrogator
double roulette_wheel::get_weight(size_t a_index) const
{
    validate_less(a_index,m_size,"invalid roulette wheel index");
    return m_weights[a_index];
}

// retrieve a random index
size_t roulette_wheel::get_index() const
{
    double choice = g_random.get_real() * m_total_weight;

    size_t i = 0;

    while ((i < m_size) && (choice > m_weights[i]))
    {
        choice -= m_weights[i];
        ++i;
    }

    return i;
}

