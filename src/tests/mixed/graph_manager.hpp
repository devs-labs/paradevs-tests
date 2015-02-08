/**
 * @file tests/mixed/graph_manager.hpp
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

#ifndef TESTS_MIXED_GRAPH_MANAGER_HPP
#define TESTS_MIXED_GRAPH_MANAGER_HPP 1

#include <tests/mixed/models.hpp>

#include <paradevs/kernel/dtss/Coordinator.hpp>
#include <paradevs/kernel/dtss/GraphManager.hpp>
#include <paradevs/kernel/dtss/Simulator.hpp>

#include <paradevs/kernel/pdevs/Coordinator.hpp>
#include <paradevs/kernel/pdevs/GraphManager.hpp>
#include <paradevs/kernel/pdevs/Simulator.hpp>

namespace paradevs { namespace tests { namespace mixed {

struct LastBagPolicy
{
    const common::Bag < common::DoubleTime >& bag() const
    { return _bag; }

    virtual void operator()(
        typename common::DoubleTime::type /* t */,
        const common::ExternalEvent < common::DoubleTime >& event,
        typename common::DoubleTime::type /* tl */,
        typename common::DoubleTime::type /* tn */)
    {
        _bag.clear();
        _bag.push_back(event);
    }

private:
    common::Bag < common::DoubleTime > _bag;
};

struct IgnorePolicy
{
    const common::Bag < common::DoubleTime >& bag() const
    { return _bag; }

    virtual void operator()(
        typename common::DoubleTime::type /* t */,
        const common::ExternalEvent < common::DoubleTime >& /* event */,
        typename common::DoubleTime::type /* tl */,
        typename common::DoubleTime::type /* tn */)
    { }

private:
    common::Bag < common::DoubleTime > _bag;
};

class S1GraphManager :
        public pdevs::GraphManager < common::DoubleTime,
                                     paradevs::common::NoParameters >
{
public:
    S1GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        pdevs::GraphManager < common::DoubleTime,
                              paradevs::common::NoParameters >(
                                  coordinator, parameters),
        a("a1", paradevs::common::NoParameters()),
        b("b1", paradevs::common::NoParameters())
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

    virtual ~S1GraphManager()
    { }

private:
    pdevs::Simulator < common::DoubleTime,
                       A1,
                       paradevs::common::NoParameters > a;
    pdevs::Simulator < common::DoubleTime,
                       B1,
                       paradevs::common::NoParameters > b;
};

class S2GraphManager :
        public dtss::GraphManager < common::DoubleTime >
{
public:
    S2GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        dtss::GraphManager < common::DoubleTime >(coordinator, parameters),
        a("a2", 20, paradevs::common::NoParameters()),
        b("b2", 20, paradevs::common::NoParameters())
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

    virtual ~S2GraphManager()
    { }

private:
    dtss::Simulator < common::DoubleTime,
                      A2 > a;
    dtss::Simulator < common::DoubleTime,
                      B2 > b;
};

class RootGraphManager :
        public pdevs::GraphManager < common::DoubleTime,
                                     paradevs::common::NoParameters >
{
public:
    RootGraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                     const paradevs::common::NoParameters& parameters) :
        pdevs::GraphManager < common::DoubleTime,
                              paradevs::common::NoParameters >(
                                  coordinator, parameters),
        S1("S1", paradevs::common::NoParameters(),
           paradevs::common::NoParameters()),
        S2("S2", paradevs::dtss::Parameters < common::DoubleTime >(20),
           paradevs::common::NoParameters())
    {
        add_child(&S1);
        add_child(&S2);
        add_link(&S1, "out", &S2, "in");
    }

    virtual ~RootGraphManager()
    { }

private:
    paradevs::pdevs::Coordinator <
        common::DoubleTime,
        S1GraphManager > S1;
    paradevs::dtss::Coordinator <
        common::DoubleTime,
        LastBagPolicy,
        S2GraphManager,
        paradevs::dtss::Parameters < common::DoubleTime >
    > S2;
};

template < int size >
class LinearGraphManager :
        public pdevs::GraphManager < common::DoubleTime,
                                     paradevs::common::NoParameters >
{
public:
    LinearGraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                       const paradevs::common::NoParameters& parameters) :
        pdevs::GraphManager < common::DoubleTime,
                              paradevs::common::NoParameters >(
                                  coordinator, parameters)
    {
        for (unsigned int i = 1; i <= size; ++i) {
            std::ostringstream ss;

            ss << "a" << i;
            _models.push_back(
                new pdevs::Simulator < common::DoubleTime,
                                       Beep,
                                       paradevs::common::NoParameters >(
                                           ss.str(),
                                           paradevs::common::NoParameters()));
        }
        for (unsigned int i = 0; i < size; ++i) {
            LinearGraphManager < size >::add_child(_models[i]);

            _models[i]->add_in_port("in");
            _models[i]->add_out_port("out");
        }
        for (unsigned int i = 0; i < size - 1; ++i) {
            LinearGraphManager < size >::add_link(_models[i],
                                                             "out",
                                                             _models[i + 1],
                                                             "in");
        }
    }

    virtual ~LinearGraphManager()
    {
        for (unsigned int i = 0; i < size; ++i) {
            delete _models[i];
        }
    }

private:
    std::vector <
    pdevs::Simulator < common::DoubleTime,
                       Beep,
                       paradevs::common::NoParameters >* > _models;
};

class Linear2GraphManager :
        public pdevs::GraphManager < common::DoubleTime,
                                     paradevs::common::NoParameters >
{
public:
    Linear2GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                        const paradevs::common::NoParameters& parameters) :
        pdevs::GraphManager < common::DoubleTime,
                              paradevs::common::NoParameters >(
                                  coordinator, parameters)
    {
        for (unsigned int i = 1; i <= 100; ++i) {
            std::ostringstream ss;

            ss << "a" << i;
            _models.push_back(
                new pdevs::Simulator < common::DoubleTime,
                                       Beep,
                                       paradevs::common::NoParameters >(
                                           ss.str(),
                                           paradevs::common::NoParameters()));
        }
        for (unsigned int i = 0; i < 100; ++i) {
            add_child(_models[i]);

            _models[i]->add_in_port("in");
            _models[i]->add_out_port("out");
        }
        for (unsigned int i = 0; i < 99; ++i) {
            add_link(_models[i], "out", _models[i + 1], "in");
        }

        coordinator->add_in_port("in");
        coordinator->add_out_port("out");

        add_link(coordinator, "in", _models[0], "in");
        add_link(_models[49], "out", coordinator, "out");
    }

    virtual ~Linear2GraphManager()
    {
        for (unsigned int i = 0; i < 100; ++i) {
            delete _models[i];
        }
    }

private:
    std::vector <
        pdevs::Simulator < common::DoubleTime, Beep,
                           paradevs::common::NoParameters >* > _models;
};

class Root2GraphManager :
        public pdevs::GraphManager < common::DoubleTime,
                                     paradevs::common::NoParameters >
{
public:
    Root2GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                      const paradevs::common::NoParameters& parameters) :
        pdevs::GraphManager < common::DoubleTime,
                              paradevs::common::NoParameters >(
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

    virtual ~Root2GraphManager()
    { }

private:
    paradevs::pdevs::Coordinator <
        common::DoubleTime,
        Linear2GraphManager > S1;
    paradevs::pdevs::Coordinator <
        common::DoubleTime,
        Linear2GraphManager > S2;
};

class Root3GraphManager :
        public pdevs::GraphManager <
            common::DoubleTime,
            paradevs::common::NoParameters >
{
public:
    Root3GraphManager(
        common::Coordinator <
            common::DoubleTime >* coordinator,
        const paradevs::common::NoParameters& parameters) :
        pdevs::GraphManager < common::DoubleTime,
                              paradevs::common::NoParameters >(
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

    virtual ~Root3GraphManager()
    { }

private:
    pdevs::Coordinator <
        common::DoubleTime,
        Linear2GraphManager > S1;
    pdevs::Coordinator <
        common::DoubleTime,
        Linear2GraphManager > S2;
};

} } } // namespace paradevs tests mixed

#endif
