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

#if !defined(LIBEVOCOSM_VALIDATOR_H)
#define LIBEVOCOSM_VALIDATOR_H

#include <sstream>
#include <stdexcept>
#include <typeinfo>

namespace libevocosm
{
    using std::stringstream;
    using std::string;
    using std::runtime_error;

    //! Standard validation exception
    /*!
        This is the standard exception thrown for validation failures.
        By deriving from the Standard C++ exception <i>runtime_error</i>,
        validation error is caught by generic <i>catch (std::exception)
        </i> blocks or more specific exception handlers.
        \version 1.0.0
        \date    1 May 2002
    */
    template <typename Type>
    class validation_error : public runtime_error
    {
    private:
        static string build_error_string(const Type & object,
                                         const string & details)
        {
            stringstream message;

            message << "validation error: "
                    << typeid(object).name() << " " << object
                    << details;

            return message.str();
        }

    public:
        //! Constructor
        /*!
            Constructs a <i>validation_error</i> from an object and an optional
            detail string. The resulting error message -- retrieved
            via the <i>what()</i> method inherited from <i>
            runtime_error</i> -- contains the type of object, it's
            content, and the test of the detail string (if one is
            provided).
            \param object  The erroneous object that failed validation
            \param details A string providing detailed information
                           about the validation failure
        */
        validation_error(const Type & object,
                         const string & details = string())
            : runtime_error(build_error_string(object,details))
        {
            // nada
        }
    };

    //! Validates that an object has a specific value.
    /*!
        If value is not equal to <i>constraint</i>, as defined by the appropriate
        == operator, this function throws a <i>validation_error</i>.
        \param object     Object to be tested against <i>constraint</i>
        \param constraint Expected value of object
        \param message    Additional text to be included in a
                          <i>validation_error</i>
    */
    template <typename Type>
    void validate_equals(const Type & object,
                         const Type & constraint,
                         const string & message = string())
    {
        if (object != constraint)
        {
            stringstream details;
            details << " must equal " << constraint << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates that an object does not have a specific value.
    /*!
        If value equals the <i>constraint</i>, as defined by the appropriate
        == operator, this function throws a <i>validation_error</i>.
        \param object     Object to be tested against <i>constraint</i>
        \param constraint Value that object should not equal
        \param message    Additional text to be included in a
                          <i>validation_error</i>
    */
    template <typename Type>
    void validate_not(const Type & object,
                      const Type & constraint,
                      const string & message = string())
    {
        if (object == constraint)
        {
            stringstream details;
            details << " must not equal " << constraint << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates that an object is less than <i>constraint</i>.
    /*!
        If value is greater than or equal to <i>constraint</i> (using the >=
        operator), this function throws a <i>validation_error</i>.
        \param object       Object to be tested
        \param constraint   Object must be < than this value
        \param message      Additional text to be included in a
                            <i>validation_error</i>
    */
    template <typename Type>
    void validate_less(const Type & object,
                      const Type & constraint,
                      const string & message = string())
    {
        if (object >= constraint)
        {
            stringstream details;
            details << " must be less than " << constraint << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates that an object is less than or equal to <i>constraint</i>.
    /*!
        If value is greater than or equal to <i>constraint</i> (using the >=
        operator), this function throws a <i>validation_error</i>.
        \param object       Object to be tested
        \param constraint   Object must be < than this value
        \param message      Additional text to be included in a
                            <i>validation_error</i>
    */
    template <typename Type>
    void validate_less_eq(const Type & object,
                          const Type & constraint,
                          const string & message = string())
    {
        if (object > constraint)
        {
            stringstream details;
            details << " must be less than " << constraint << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates that an object is greater than <i>constraint</i>.
    /*!
        If value is less than or equal to <i>constraint</i> (using the <=
        operator), this function throws a <i>validation_error</i>.
        \param object       Object to be tested
        \param constraint   Object must be > than this value
        \param message      Additional text to be included in a
                            validation_error
    */
    template <typename Type>
    void validate_greater(const Type & object,
                      const Type & constraint,
                      const string & message = string())
    {
        if (object <= constraint)
        {
            stringstream details;
            details << " must be greater than " << constraint << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates that an object is greater than or equal to <i>constraint</i>.
    /*!
        If value is less than or equal to <i>constraint</i> (using the <=
        operator), this function throws a <i>validation_error</i>.
        \param object       Object to be tested
        \param constraint   Object must be > than this value
        \param message      Additional text to be included in a
                            validation_error
    */
    template <typename Type>
    void validate_greater_eq(const Type & object,
                      const Type & constraint,
                      const string & message = string())
    {
        if (object < constraint)
        {
            stringstream details;
            details << " must be greater than " << constraint << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates that an object has a value in a specified range.
    /*!
        If value is less than <i>low_bound</i> (using the < operator) or greater
        than <i>high_bound</i> (using the > operator), this function throws a
        <i>validation_error</i>. The function does not verify that <i>low_bound</i> is
        less than <i>high_bound</i>.
        \param object       Object to be tested
        \param low_bound    Low boundary (inclusive) on value of object
        \param high_bound   High boundary (inclusive) on value of object
        \param message      Additional text to be included in a
                            <i>validation_error</i>
    */
    template <typename Type>
    void validate_range(const Type & object,
                        const Type & low_bound,
                        const Type & high_bound,
                        const string & message = string())
    {
        if ((object < low_bound) || (object > high_bound))
        {
            stringstream details;
            details << " must be between " << low_bound << " and "
                    << high_bound << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Validates an object with a given predicate.
    /*!
        If <i>predicate(object)</i> is <i>false</i> -- indicating an invalid object in
        the context defined by <i>predicate</i> -- this function throws a
        <i>validation_error</i>.
        \param object    Object to be tested against the <i>predicate</i>
        \param constraint A function or functor returning <i>true</i>
                         for valid objects and <i>false</i> for an
                         invalid object
        \param message   Additional text to be included in a
                         <i>validation_error</i>
    */
    template <typename Type, typename Predicate>
    void validate_with(const Type & object,
                       const Predicate & constraint,
                       const string & message = string())
    {
        if (!constraint(object))
        {
            stringstream details;
            details << " failed test " << typeid(constraint).name() << " " << message;
            throw validation_error<Type>(object,details.str());
        }
    }

    //! Enforce a lower limit on the value of an object.
    /*!
        If object's value is less than <i>low_value</i>, as per the < operator,
        object will be set equal to <i>low_value</i>.
        \param object    Object to undergo enforcement
        \param low_value Lower limit on the value of <i>object</i>
    */
    template <typename Type>
    void enforce_lower_limit(Type & object,
                             const Type & low_value)
    {
        if (object < low_value)
            object = low_value;
    }

    //! Enforce an upper limit on the value of an object.
    /*!
        If object's value is greater than <i>high_value</i>, as per the >
        operator, object will be set equal <i>to high_value</i>.
        \param object     Object to undergo enforcement
        \param high_value Upper limit on the value of <i>object</i>
    */
    template <typename Type>
    void enforce_upper_limit(Type & object,
                            const Type & high_value)
    {
        if (object > high_value)
            object = high_value;
    }

    //! Enforce an maximum index on the value of an object.
    /*!
        If object's value is greater than or equal to <i>array_length</i>, as per the >=
        operator, object will be set equal to <i>array_length</i>.
        \param object Object to undergo enforcement
        \param array_length Length of array that <i>object</i> must address
    */
    template <typename Type>
    void enforce_index(Type & object,
                        const Type & array_length)
    {
        if (object >= array_length)
            object = array_length - 1;

        if (object < 0)
            object = 0;
    }

    //! Enforce an range limit on the value of an object.
    /*!
        If object's value is less than <i>low_value</i>, as per the < operator,
        object will be set equal to <i>low_value</i>. If object's value is
        greater than <i>high_value</i>, as per the > operator, object will
        be set equal to <i>high_value</i>.
        \param object     Object to undergo enforcement
        \param low_value  Lower limit on the value of <i>object</i>
        \param high_value Upper limit on the value of <i>object</i>
    */
    template <typename Type>
    void enforce_range(Type & object,
                       const Type & low_value,
                       const Type & high_value)
    {
        if (object < low_value)
            object = low_value;
        else if (object > high_value)
            object = high_value;
    }

    //! Utility function to create a location string.
    /*!
        This function formats a string from a given file name and
        line number. If C++ incorporates parts of C99, this function
        could be extended to support the <i>__func__</i> macro that names
        the current function.
        \param filename The name of a file, usually the Standard C <i>__FILE__</i> macro
        \param line_no  A line number in the file, usually the Standard C <i>__LINE__</i> macro
        \sa LIBEVOCOSM_LOCATION
    */
    inline string build_location_string(const char * filename, long line_no)
    {
        stringstream text;
        text << "in " << filename << ", line " << line_no;
        return text.str();
    }
}

// These macros allow validation to be included on a per-compile basis, based on the settings
// of the DEBUG and NDEBUG preprocessor macros.
#if defined(_DEBUG) && !defined(NDEBUG)
#define LIBEVOCOSM_VALIDATE_EQUALS(object,constraint,details) libevocosm::validate_equals(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_NOT(object,constraint,details) libevocosm::validate_not(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_LESS(object,constraint,details) libevocosm::validate_less(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_LESS_EQ(object,constraint,details) libevocosm::validate_less_eq(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_GREATER(object,constraint,details) libevocosm::validate_greater(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_GREATER_EQ(object,constraint,details) libevocosm::validate_greater_eq(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_RANGE(object,low_bound,high_bound,details) libevocosm::validate_range(object,low_bound,high_bound,details)
#define LIBEVOCOSM_VALIDATE_WITH(object,constraint,details) libevocosm::validate_with(object,constraint,details)
#define LIBEVOCOSM_LOCATION libevocosm::build_location_string(__FILE__,__LINE__)
#else
#define LIBEVOCOSM_VALIDATE_EQUALS(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_NOT(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_LESS(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_LESS_EQ(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_GREATER(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_GREATER_EQ(object,constraint,details)
#define LIBEVOCOSM_VALIDATE_RANGE(object,low_bound,high_bound,details)
#define LIBEVOCOSM_VALIDATE_WITH(object,constraint,details)
#define LIBEVOCOSM_LOCATION std::string()
#endif

#endif
