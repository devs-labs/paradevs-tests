/**
 * @file tests/pdevs/models.hpp
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

#ifndef TESTS_PDEVS_MODELS_HPP
#define TESTS_PDEVS_MODELS_HPP 1

#include <paradevs/common/time/DoubleTime.hpp>
#include <paradevs/common/utils/Trace.hpp>

#include <paradevs/kernel/pdevs/Dynamics.hpp>

#include <chrono>

#define DELAY 100

namespace paradevs { namespace tests { namespace pdevs {

void delay()
{
    for (unsigned int i = 0; i < DELAY; ++i) {
        std::vector < int > v;

        for (unsigned int j = 1000; j > 0; --j) {
            v.push_back(j);
        }
        std::sort(v.begin(), v.end());
    }
}

struct data
{
    double x;
    double y;

    data() : x(0), y(0)
    { }

    data(double _x, double _y) : x(_x), y(_y)
    { }
};

class A :
        public paradevs::pdevs::Dynamics < common::DoubleTime >
{
public:
    A(const std::string& name, const common::NoParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime >(name, parameters)
    { }
    virtual ~A()
    { }

    void dint(typename common::DoubleTime::type t)
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

        // delay();
        ++_value.x;
        --_value.y;

        // std::cout << t << ": " << get_name() << " => dint -> "
        //           << _value.x << " " << _value.y << std::endl;

        if (_phase == SEND) {
            _phase = WAIT;
        }
    }

    void dext(typename common::DoubleTime::type t,
              typename common::DoubleTime::type /* e */,
              const common::Bag < common::DoubleTime >& msgs)
    {

#ifndef WITH_TRACE
        (void)t;
        (void)msgs;
#endif

        // for (common::Bag < common::DoubleTime >::const_iterator it =
        //          msgs.begin(); it != msgs.end(); ++it) {
        //     std::cout << t << ": " << get_name()
        //               << " => " << it->get_content().get_content < double >()
        //               << std::endl;
        // }

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::DELTA_EXT)
            << "messages = " << msgs.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        _phase = SEND;
    }

    void dconf(typename common::DoubleTime::type t,
               typename common::DoubleTime::type /* e */,
               const common::Bag < common::DoubleTime >& msgs)
    {

#ifndef WITH_TRACE
        (void)t;
        (void)msgs;
#endif

        // std::cout << t << ": " << get_name() << " => " << msgs.to_string()
        //           << std::endl;

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::DELTA_CONF)
            << "messages = " << msgs.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif

    }

    typename common::DoubleTime::type start(typename common::DoubleTime::type t)
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

        _phase = WAIT;
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type t) const
    {

#ifndef WITH_TRACE
        (void)t;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::TA);
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        if (_phase == WAIT) {
            return 1;
        } else {
            return 0;
        }
    }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {
#ifndef WITH_TRACE
        (void)t;
#endif
        common::Bag < common::DoubleTime > msgs;

        msgs.push_back(
            common::ExternalEvent < common::DoubleTime >(
                "out", common::Value(_value)));

        // std::cout << t << ": " << get_name() << " => lambda" << std::endl;

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

    void observation(std::ostream& /* file */) const
    { }

private:
    enum Phase { WAIT, SEND };

    Phase _phase;
    data _value;
};

class B :
        public paradevs::pdevs::Dynamics < common::DoubleTime >
{
public:
    B(const std::string& name, const common::NoParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime >(name, parameters),
        _value(0)
    { }
    virtual ~B()
    { }

    void dint(typename common::DoubleTime::type t)
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

        // delay();

        // std::cout << t << ": " << get_name() << " => dint -> "
        //           << _value << std::endl;

        if (_phase == SEND) {
            _phase = WAIT;
        }
    }

    void dext(typename common::DoubleTime::type t,
              typename common::DoubleTime::type /* e */,
              const common::Bag < common::DoubleTime >& msgs)
    {

#ifndef WITH_TRACE
        (void)t;
        (void)msgs;
#endif

        // for (common::Bag < common::DoubleTime >::const_iterator it =
        //          msgs.begin(); it != msgs.end(); ++it) {
        //     std::cout << t << ": " << get_name()
        //               << " => " << it->get_content().get_content < data >().x
        //               << " " << it->get_content().get_content < data >().y
        //               << std::endl;
        // }

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::DELTA_EXT)
            << "messages = " << msgs.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        _phase = SEND;
    }

    void dconf(typename common::DoubleTime::type t,
               typename common::DoubleTime::type /* e */,
               const common::Bag < common::DoubleTime >& msgs)
    {

#ifndef WITH_TRACE
        (void)t;
        (void)msgs;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::DELTA_CONF)
            << "messages = " << msgs.to_string();
        common::Trace < common::DoubleTime >::trace().flush();
#endif

    }

    typename common::DoubleTime::type start(typename common::DoubleTime::type t)
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

        _phase = WAIT;
        return std::numeric_limits < double >::max();
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type t) const
    {

#ifndef WITH_TRACE
        (void)t;
#endif

#ifdef WITH_TRACE
        common::Trace < common::DoubleTime >::trace()
            << common::TraceElement < common::DoubleTime >(
                get_name(), t,
                common::TA);
        common::Trace < common::DoubleTime >::trace().flush();
#endif

        if (_phase == WAIT) {
            return std::numeric_limits < double >::max();
        } else {
            return 0;
        }
    }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {
#ifndef WITH_TRACE
        (void)t;
#endif
        common::Bag < common::DoubleTime > msgs;

        // std::cout << t << ": " << get_name() << " => lambda" << std::endl;

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

    void observation(std::ostream& /* file */) const
    { }

private:
    enum Phase { WAIT, SEND };

    Phase _phase;
    double _value;
};

class TwoStateModel :
        public paradevs::pdevs::Dynamics < common::DoubleTime >
{
public:
    TwoStateModel(const std::string& name,
                  const common::NoParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime >(name, parameters)
    { }
    virtual ~TwoStateModel()
    { }

    void dint(typename common::DoubleTime::type t)
    {
        if (_phase == S1) {
            _phase = S2;
        } else if (_phase == S2) {
            _phase = S1;
        }
        _last_time = t;
    }

    typename common::DoubleTime::type start(
        typename common::DoubleTime::type t)
    {
        _phase = S1;
        _last_time = t;
        return ta(t);
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type /* t */) const
    {
        if (_phase == S1) {
            return 5;
        } else {
            return 6;
        }
    }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {

        std::cout << (t - _last_time) << std::endl;

        return common::Bag < common::DoubleTime >();
    }

private:
    enum Phase { S1, S2 };

    Phase _phase;
    typename common::DoubleTime::type _last_time;
};

class ThreeStateModel :
        public paradevs::pdevs::Dynamics < common::DoubleTime >
{
public:
    ThreeStateModel(const std::string& name,
                    const common::NoParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime >(name, parameters)
    { }
    virtual ~ThreeStateModel()
    { }

    void compute()
    {
        for (unsigned int i = 0; i < heights.size(); ++i) {
            if (heights[i] != -1 and heights[i] < 10) {
                heights[i] += speeds[i] * scales[i];
            }
        }
    }

    void display() const
    {
        for (std::vector < double >::const_iterator it = heights.begin();
             it != heights.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    void display_full() const
    {
        unsigned int i = 1;

        for (std::vector < double >::const_iterator it = heights.begin();
             it != heights.end(); ++it, ++i) {
            if (*it > 10) {
                std::cout << "S" << i;
            }
        }
        std::cout << std::endl;
    }

    bool full() const
    {
        unsigned int n = 0;

        for (std::vector < double >::const_iterator it = heights.begin();
             it != heights.end(); ++it) {
            if (*it > 10) {
                ++n;
            }
        }
        return n > 0;
    }

    bool full_N() const
    {
        unsigned int n = 0;

        for (std::vector < double >::const_iterator it = heights.begin();
             it != heights.end(); ++it) {
            if (*it == -1) {
                ++n;
            }
        }
        return n >= 2;
    }

    void mark_full(typename common::DoubleTime::type t)
    {
        for (std::vector < double >::iterator it = heights.begin();
             it != heights.end(); ++it) {
            if (*it > 10) {
                *it = -1;
                _last_time = t;
            }
        }
    }

    void raz()
    {
        for (std::vector < double >::iterator it = heights.begin();
             it != heights.end(); ++it) {
            if (*it == -1) {
                *it = 0;
            }
        }
    }

    void dint(typename common::DoubleTime::type t)
    {
        mark_full(t);
        if (full_N()) {
            raz();
        }
        compute();
    }

    typename common::DoubleTime::type start(
        typename common::DoubleTime::type t)
    {
        heights = { 0, 0, 0, 0, 0 };
        speeds = { 0.21, 0.3, 0.7, 0.56, 0.14 };
        scales = { 1, 1, 1, 1, 1 };
        _last_time = t;
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type /* t */) const
    { return 1; }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {
        if (full()) {
            std::cout << (t - _last_time) << " ";
            // display_full();
        }
        return common::Bag < common::DoubleTime >();
    }

private:
    std::vector < double > heights;
    std::vector < double > speeds;
    std::vector < double > scales;

    typename common::DoubleTime::type _last_time;
};

} } } // namespace paradevs tests pdevs

#endif
