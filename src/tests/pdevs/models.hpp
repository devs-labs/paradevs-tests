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

class A :
        public paradevs::pdevs::Dynamics < common::DoubleTime >
{
public:
    A(const std::string& name, const common::NoParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime >(name, parameters),
        _value(0)
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

        delay();

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
                "out", (void*)&_value));

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

        delay();

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

        msgs.push_back(common::ExternalEvent <
                           common::DoubleTime >(
                               "out", (void*)&_value));

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

    bool full()
    {
        unsigned int n = 0;

        if (S1 == -1) ++n;
        if (S2 == -1) ++n;
        if (S3 == -1) ++n;
        if (S4 == -1) ++n;
        if (S5 == -1) ++n;
        return n >= 4;
    }

    void dint(typename common::DoubleTime::type t)
    {
        if (S1 > 10 or S2 > 10 or S3 > 10 or S4 > 10 or S5 > 10) {
            if (S1 > 10) {
                S1 = -1;
            }
            if (S2 > 10) {
                S2 = -1;
            }
            if (S3 > 10) {
                S3 = -1;
            }
            if (S4 > 10) {
                S4 = -1;
            }
            if (S5 > 10) {
                S5 = -1;
            }
            _last_time = t;
        }
        if (full()) {
            if (S1 == -1) {
                S1 = 0;
                // speed1 = speed1 == 1 ? 5 : 1;
            }
            if (S2 == -1) {
                S2 = 0;
                // speed2 = speed2 == 3 ? 1 : 3;
            }
            if (S3 == -1) {
                S3 = 0;
                // speed3 = speed3 == 2 ? 1 : 2;
            }
            if (S4 == -1) {
                S4 = 0;
                // speed4 = speed4 == 1 ? 3 : 1;
            }
            if (S5 == -1) {
                S5 = 0;
                // speed5 = speed5 == 2 ? 1 : 2;
            }
        }
        if (S1 != -1 and S1 <= 10) {
            S1 += 0.21 * speed1;
        }
        if (S2 != -1 and S2 <= 10) {
            S2 += 0.3 * speed2;
        }
        if (S3 != -1 and S3 <= 10) {
            S3 += 0.7 * speed3;
        }
        if (S4 != -1 and S4 <= 10) {
            S4 += 0.56 * speed4;
        }
        if (S5 != -1 and S5 <= 10) {
            S5 += 0.14 * speed5;
        }

        // std::cout << S1 << " " << S2 << " "
        //           << S3 << " " << S4 << " "
        //           << S5 << " " << std::endl;

    }

    typename common::DoubleTime::type start(
        typename common::DoubleTime::type t)
    {
        S1 = S2 = S3 = S4 = S5 = 0;
        speed1 = speed2 = speed3 = speed4 = speed5 = 1;

        // speed1 = 1;
        // speed2 = 3;
        // speed3 = 2;
        // speed4 = 1;
        // speed5 = 2;
        _last_time = t;
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type /* t */) const
    { return 1; }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {

        if (S1 > 10 or S2 > 10 or S3 > 10 or S4 > 10 or S5 > 10) {
            std::cout << (t - _last_time) << " ";
            if (S1 > 10) {
                std::cout << "S1";
            }
            if (S2 > 10) {
                std::cout << "S2";
            }
            if (S3 > 10) {
                std::cout << "S3";
            }
            if (S4 > 10) {
                std::cout << "S4";
            }
            if (S5 > 10) {
                std::cout << "S5";
            }
            std::cout << std::endl;
        }

        return common::Bag < common::DoubleTime >();
    }

private:
    double S1, S2, S3, S4, S5;
    double speed1, speed2, speed3, speed4, speed5;

    typename common::DoubleTime::type _last_time;
};

} } } // namespace paradevs tests pdevs

#endif
