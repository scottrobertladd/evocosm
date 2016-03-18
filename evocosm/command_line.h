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

#if !defined(COMMAND_LINE_H)
#define COMMAND_LINE_H

// Standard C++
#include <string>
#include <vector>
#include <set>

using std::string;
using std::vector;
using std::set;

// a very simple command-line parser
class command_line
{
public:
    struct option
    {
        string m_name;
        string m_value;

        option(const string & a_name)
            : m_name(a_name), m_value("")
        {
            // nada
        }
    };

    // create a command line parser
    command_line(int argc, char * argv[], const set<string> & bool_opts);

    // retrieve a list of options
    const vector<option> & get_options() const
    {
        return m_options;
    }

    // retrieve a list of input values
    const vector<string> & get_inputs() const
    {
        return m_inputs;
    }

private:
    // the set of option names, with associated values
    vector<option> m_options;

    // the set of inputs -- command line arguments not associated with an option
    vector<string> m_inputs;

    // a list of boolean options, which have no values
    const std::set<std::string> & m_bool_opts;
};

#endif
