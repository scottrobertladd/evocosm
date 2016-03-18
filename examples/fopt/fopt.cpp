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

// Standard C++
#include <cerrno>
#include <cmath>
#include <limits>
#include <iostream>
#include <iomanip>
using namespace std;

// Function optimizer
#include "../../evocosm/function_optimizer.h"
using namespace libevocosm;

double const PI = 3.1415926535897932384626433832795028841971694;

// we're looking for a peak of ~7.9468 at ~(-0.6550, 0.5)
vector<double> sample_test(vector<double> p_args)
{
    double z = 0.0;

    // Make certain we have two, and only two values
    if (p_args.size() == 2)
    {
        // run it through the formula
        double x = p_args[0];
        double y = p_args[1];

        // automatically reject out-of-range values
        if ((x <= 1.0) && (x >= -1.0) && (y <= 1.0) && (y >= -1.0))
        {
            // the actual formula
            z = 0.8 + pow(x + 0.5, 2.0) + 2.0 * pow(y - 0.5, 2.0) - 0.3 * cos(3.0 * PI * x) - 0.4 * cos(4.0 * PI * y);

            // check for cosine errors (MS uses errno for cos errors)
            if (errno != 0)
            {
                // bad cosine equals lousy fitness
                z = 0.0;
                errno = 0;
            }
        }
    }

    vector<double> result;

    result.push_back(z);
    result.push_back((z != 0) ? (1.0 / z) : 0.0); // fitness is recip of value

    return result;
}

int main()
{
    // create the optimizer
    cout << "Creating optimizer... " << flush;
    function_optimizer optimizer(&sample_test, 2, -1.0, +1.0, (size_t)10000, 0.25, (size_t)1000);
    cout << "done" << endl;

    // run the optimizer
    cout << "Beginning optimizer run" << endl;
    optimizer.run();

    // done -- wasn't that easy? ;}
    cout << "Done" << endl;
    return 0;
}
