/**
 * @file tests/multithreading/graph_manager.cpp
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

#ifndef TESTS_PDEVS_MULTITHREADING_GRAPH_MANAGER_HPP
#define TESTS_PDEVS_MULTITHREADING_GRAPH_MANAGER_HPP 1

#include <tests/pdevs/models.hpp>
#include <tests/pdevs/graph_manager.hpp>

#include <paradevs/kernel/pdevs/multithreading/Coordinator.hpp>
#include <paradevs/kernel/pdevs/GraphManager.hpp>
#include <paradevs/kernel/pdevs/Simulator.hpp>

namespace paradevs { namespace tests { namespace multithreading {

class S1GraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime >
{
public:
    S1GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime >(coordinator,
                                                          parameters),
        a("a1", common::NoParameters()),
        b("b1", common::NoParameters())
    {
        add_child(&a);
        add_child(&b);

        a.add_out_port("out");
        b.add_in_port("in");
        b.add_out_port("out");
        coordinator->add_out_port("out");

        add_link(&a, "out", &b, "in");
        add_link(&b, "out", coordinator, "out");
    }

    void init()
    { }

    void start(common::DoubleTime::type /* t */)
    { }

    void transition(
        const common::Models < common::DoubleTime >& /* receivers */,
        common::DoubleTime::type /* t */)
    { }

    virtual ~S1GraphManager()
    { }

private:
    paradevs::pdevs::Simulator < common::DoubleTime, pdevs::A  > a;
    paradevs::pdevs::Simulator < common::DoubleTime, pdevs::B  > b;
};

class S2GraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime >
{
public:
    S2GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime >(coordinator,
                                                             parameters),
        a("a2", common::NoParameters()), b("b2", common::NoParameters())
    {
        add_child(&a);
        add_child(&b);

        a.add_in_port("in");
        a.add_out_port("out");
        b.add_in_port("in");
        coordinator->add_in_port("in");

        add_link(&a, "out", &b, "in");
        add_link(coordinator, "in", &a, "in");
    }

    void init()
    { }

    void start(common::DoubleTime::type /* t */)
    { }

    void transition(
        const common::Models < common::DoubleTime >& /* receivers */,
        common::DoubleTime::type /* t */)
    { }

    virtual ~S2GraphManager()
    { }

private:
    paradevs::pdevs::Simulator < common::DoubleTime, pdevs::A  > a;
    paradevs::pdevs::Simulator < common::DoubleTime, pdevs::B  > b;
};

class RootGraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime >
{
public:
    RootGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime >(
                                            coordinator, parameters),
        S1("S1", paradevs::common::NoParameters(),
           paradevs::common::NoParameters()),
        S2("S2", paradevs::common::NoParameters(),
           paradevs::common::NoParameters())
    {
        add_child(&S1);
        add_child(&S2);

        add_link(&S1, "out", &S2, "in");
    }

    void init()
    {
        S1.set_sender(
            dynamic_cast < paradevs::pdevs::multithreading::Coordinator <
                common::DoubleTime,
                paradevs::tests::multithreading::RootGraphManager >*
            >(get_coordinator())->get_sender());
        S2.set_sender(
            dynamic_cast < paradevs::pdevs::multithreading::Coordinator <
                common::DoubleTime,
                paradevs::tests::multithreading::RootGraphManager >*
            >(get_coordinator())->get_sender());
    }

    void start(common::DoubleTime::type t)
    {
        S1.get_sender().send(
            paradevs::pdevs::multithreading::start_message <
                common::DoubleTime >(t));
        S2.get_sender().send(
            paradevs::pdevs::multithreading::start_message <
                common::DoubleTime >(t));
    }

    void transition(const common::Models < common::DoubleTime >& receivers,
                    common::DoubleTime::type t)
    {
        if (std::find(receivers.begin(), receivers.end(),
                      &S1) != receivers.end()) {
            S1.get_sender().send(
                paradevs::pdevs::multithreading::transition_message <
                    common::DoubleTime >(t));
        }
        if (std::find(receivers.begin(), receivers.end(),
                      &S2) != receivers.end()) {
            S2.get_sender().send(
                paradevs::pdevs::multithreading::transition_message <
                    common::DoubleTime >(t));
        }
    }

    virtual ~RootGraphManager()
    { }

private:
    paradevs::pdevs::multithreading::Coordinator <
        common::DoubleTime,
        S1GraphManager > S1;
    paradevs::pdevs::multithreading::Coordinator <
        common::DoubleTime,
        S2GraphManager > S2;
};

class S3GraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime >
{
public:
    S3GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime >(coordinator,
                                                             parameters)
    {
        coordinator->add_out_port("out");
        for (unsigned int i = 0; i < 10; ++i) {
            std::ostringstream ss;
            simulator_type* s = new simulator_type(ss.str(),
                                                   common::NoParameters());

            ss << "a" << (i + 1);
            _simulators.push_back(s);
            add_child(s);
            s->add_out_port("out");
        }
    }

    void init()
    { }

    void start(common::DoubleTime::type /* t */)
    { }

    void transition(
        const common::Models < common::DoubleTime >& /* receivers */,
        common::DoubleTime::type /* t */)
    { }

    virtual ~S3GraphManager()
    {
        for (typename std::vector < simulator_type* >::const_iterator it =
                 _simulators.begin(); it != _simulators.end(); ++it) {
            delete *it;
        }
    }

private:
    typedef paradevs::pdevs::Simulator < common::DoubleTime,
                                         pdevs::A > simulator_type;

    std::vector < simulator_type* > _simulators;
};

class Root2GraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime >
{
public:
    Root2GraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime >(
                                            coordinator, parameters),
        S1("S1", paradevs::common::NoParameters(),
           paradevs::common::NoParameters()),
        S2("S2", paradevs::common::NoParameters(),
           paradevs::common::NoParameters())
    {
        add_child(&S1);
        add_child(&S2);
    }

    void init()
    {
        S1.set_sender(
            dynamic_cast < paradevs::pdevs::multithreading::Coordinator <
                common::DoubleTime,
                paradevs::tests::multithreading::Root2GraphManager >*
            >(get_coordinator())->get_sender());
        S2.set_sender(
            dynamic_cast < paradevs::pdevs::multithreading::Coordinator <
                common::DoubleTime,
                paradevs::tests::multithreading::Root2GraphManager >*
            >(get_coordinator())->get_sender());
    }

    void start(common::DoubleTime::type t)
    {
        S1.get_sender().send(
            paradevs::pdevs::multithreading::start_message <
                common::DoubleTime >(t));
        S2.get_sender().send(
            paradevs::pdevs::multithreading::start_message <
                common::DoubleTime >(t));
    }

    void transition(const common::Models < common::DoubleTime >& receivers,
                    common::DoubleTime::type t)
    {
        if (std::find(receivers.begin(), receivers.end(),
                      &S1) != receivers.end()) {
            S1.get_sender().send(
                paradevs::pdevs::multithreading::transition_message <
                    common::DoubleTime >(t));
        }
        if (std::find(receivers.begin(), receivers.end(),
                      &S2) != receivers.end()) {
            S2.get_sender().send(
                paradevs::pdevs::multithreading::transition_message <
                    common::DoubleTime >(t));
        }
    }

    virtual ~Root2GraphManager()
    { }

private:
    paradevs::pdevs::multithreading::Coordinator <
        common::DoubleTime,
        S3GraphManager > S1;
    paradevs::pdevs::multithreading::Coordinator <
        common::DoubleTime,
        S3GraphManager > S2;
};

class Root3GraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime >
{
public:
    Root3GraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime >(
                                            coordinator, parameters),
        S1("S1", paradevs::common::NoParameters(),
           paradevs::common::NoParameters()),
        S2("S2", paradevs::common::NoParameters(),
           paradevs::common::NoParameters())
    {
        add_child(&S1);
        add_child(&S2);
    }

    virtual ~Root3GraphManager()
    { }

private:
    paradevs::pdevs::Coordinator <
        common::DoubleTime,
        S3GraphManager > S1;
    paradevs::pdevs::Coordinator <
        common::DoubleTime,
        S3GraphManager > S2;
};

} } } // namespace paradevs tests multithreading

#endif
