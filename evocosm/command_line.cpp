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

#include <cstring>

#include "command_line.h"

command_line::command_line(int argc, char * argv[], const set<string> & bool_opts)
  : m_options(),
    m_inputs(),
    m_bool_opts(bool_opts)
{
    bool option_active = false;
    vector<option>::iterator active_option;

    for (int i = 1; i < argc; ++i)
    {
        // is it an option?
        if ((strlen(argv[i]) > 1) && (argv[i][0] == '-'))
        {
            // store new key
            string opt_key = string(&argv[i][1]);
            m_options.push_back(option(opt_key));

            // active option is now last in list
            active_option = m_options.end();
            --active_option;

            // check option list to see if we're looking for an argument
            option_active = (m_bool_opts.find(opt_key) == m_bool_opts.end());
        }
        else
        {
            if (option_active)
                active_option->m_value = string(argv[i]);
            else
                m_inputs.push_back(string(argv[i]));

            option_active = false;
        }
    }
}
