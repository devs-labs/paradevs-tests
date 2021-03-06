/**
 * @file tests/dtss/models.hpp
 * @author The PARADEVS Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * PARADEVS - the multimodeling and simulation environment
 * This file is a part of the PARADEVS environment
 *
 * Copyright (C) 2013-2015 ULCO http://www.univ-litoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TESTS_DTSS_MODELS_HPP
#define TESTS_DTSS_MODELS_HPP 1

#include <paradevs/common/time/DoubleTime.hpp>
#include <paradevs/common/utils/Trace.hpp>

#include <paradevs/kernel/dtss/Dynamics.hpp>

namespace paradevs { namespace tests { namespace dtss {

class A :
        public paradevs::dtss::Dynamics < common::DoubleTime,
                                          paradevs::common::NoParameters >
{
public:
    A(const std::string& name,
      const paradevs::common::NoParameters& parameters) :
        paradevs::dtss::Dynamics < common::DoubleTime,
                                   paradevs::common::NoParameters >(name,
                                                                    parameters),
        _value(0)
    { }
    virtual ~A()
    { }

    void transition(const common::Bag < common::DoubleTime >& /* x */,
                    common::DoubleTime::type t)
    {

#ifndef WITH_TRACE
        (void)t;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::DELTA_INT);
        common::Trace < common::DoubleTime >::trace().flush();
#endif

    }

    common::DoubleTime::type start(common::DoubleTime::type t)
    {

#ifndef WITH_TRACE
        (void)t;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::START);
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        return 0;
    }

    common::Bag < common::DoubleTime > lambda(
        common::DoubleTime::type t) const
    {

#ifndef WITH_TRACE
        (void)t;
#endif

        common::Bag < common::DoubleTime > msgs;

        msgs.push_back(common::ExternalEvent <
                           common::DoubleTime >(
                               "out", common::Value(_value)));

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::LAMBDA)
            << "messages = " << msgs.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        return msgs;
    }

private:
    double _value;
};

class B :
        public paradevs::dtss::Dynamics < common::DoubleTime,
                                          paradevs::common::NoParameters >
{
public:
    B(const std::string& name,
      const paradevs::common::NoParameters& parameters) :
        paradevs::dtss::Dynamics < common::DoubleTime,
                                   paradevs::common::NoParameters >(name,
                                                                    parameters),
        _value(0)
    { }
    virtual ~B()
    { }

    void transition(const common::Bag < common::DoubleTime >& x,
                    common::DoubleTime::type t)
    {

#ifndef WITH_TRACE
        (void)x;
        (void)t;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::DELTA_INT)
            << "x = " << x.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif
    }

    common::DoubleTime::type start(common::DoubleTime::type t)
    {

#ifndef WITH_TRACE
        (void)t;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::START);
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        return 0;
    }

    common::Bag < common::DoubleTime > lambda(
        common::DoubleTime::type t) const
    {

#ifndef WITH_TRACE
        (void)t;
#endif

        common::Bag < common::DoubleTime > msgs;

        msgs.push_back(common::ExternalEvent <
                           common::DoubleTime >(
                               "out", common::Value(_value)));

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t, common::LAMBDA)
            << "messages = " << msgs.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        return msgs;
    }

private:
    double _value;
};

} } } // namespace paradevs tests dtss

#endif
