/**
 * @file tests/dtss/graph_manager.hpp
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

#ifndef TESTS_DTSS_GRAPH_MANAGER_HPP
#define TESTS_DTSS_GRAPH_MANAGER_HPP 1

#include <tests/dtss/models.hpp>

#include <paradevs/kernel/dtss/Coordinator.hpp>
#include <paradevs/kernel/dtss/GraphManager.hpp>
#include <paradevs/kernel/dtss/Simulator.hpp>

namespace paradevs { namespace tests { namespace dtss {

struct Policy
{
    const common::Bag < common::DoubleTime >& bag() const
    { return _bag; }

    virtual void operator()(
        common::DoubleTime::type /* t */,
        const common::ExternalEvent < common::DoubleTime >& event,
        common::DoubleTime::type /* tl */,
        common::DoubleTime::type /* tn */)
    {
        _bag.clear();
        _bag.push_back(event);
    }

private:
    common::Bag < common::DoubleTime > _bag;
};

class OnlyOneGraphManager :
        public paradevs::dtss::GraphManager < common::DoubleTime,
                                              paradevs::common::NoParameters >
{
public:
    OnlyOneGraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                        const paradevs::common::NoParameters& parameters) :
        paradevs::dtss::GraphManager < common::DoubleTime,
                                       paradevs::common::NoParameters >(
                                           coordinator, parameters),
        a("a", 1., common::NoParameters())
    {
        add_child(&a);
    }

    virtual ~OnlyOneGraphManager()
    { }

private:
    paradevs::dtss::Simulator < common::DoubleTime, A > a;
};

class TwoGraphManager :
        public paradevs::dtss::GraphManager < common::DoubleTime,
                                              paradevs::common::NoParameters >
{
public:
    TwoGraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                    const paradevs::common::NoParameters& parameters) :
        paradevs::dtss::GraphManager < common::DoubleTime,
                                       paradevs::common::NoParameters >(
                                           coordinator, parameters),
        a("a", 1., common::NoParameters()), b("b", 1., common::NoParameters())
    {
        add_child(&a);
        add_child(&b);
        a.add_out_port("out");
        b.add_in_port("in");
        add_link(&a, "out", &b, "in");
    }

    virtual ~TwoGraphManager()
    { }

private:
    paradevs::dtss::Simulator < common::DoubleTime, A > a;
    paradevs::dtss::Simulator < common::DoubleTime, B > b;
};

} } } // namespace paradevs tests dtss

#endif
