/**
 * @file tests/mpi/graph_manager.cpp
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

#ifndef TESTS_PDEVS_MPI_GRAPH_MANAGER_HPP
#define TESTS_PDEVS_MPI_GRAPH_MANAGER_HPP 1

#include <tests/pdevs/models.hpp>
#include <tests/pdevs/graph_manager.hpp>

#include <paradevs/kernel/pdevs/mpi/Coordinator.hpp>
#include <paradevs/kernel/pdevs/mpi/ModelProxy.hpp>
#include <paradevs/kernel/pdevs/mpi/GraphManager.hpp>
#include <paradevs/kernel/pdevs/Simulator.hpp>

// #include <tests/boost_graph/graph_defs.hpp>

namespace paradevs { namespace tests { namespace mpi {

class S1GraphManager :
        public paradevs::pdevs::mpi::GraphManager < common::DoubleTime >
{
public:
    S1GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::mpi::GraphManager < common::DoubleTime >(coordinator,
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
        public paradevs::pdevs::mpi::GraphManager < common::DoubleTime >
{
public:
    S2GraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                   const paradevs::common::NoParameters& parameters) :
        paradevs::pdevs::mpi::GraphManager < common::DoubleTime >(coordinator,
                                                                  parameters),
        a("a2", common::NoParameters()),
        b("b2", common::NoParameters())
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

struct RootGraphManagerParameters
{
    int S1_rank;
    int S2_rank;
};

class RootGraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime,
                                               RootGraphManagerParameters >
{
public:
    RootGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const RootGraphManagerParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime,
                                        RootGraphManagerParameters >(
                                            coordinator, parameters),
        S1("a", parameters.S1_rank, false),
        S2("b", parameters.S2_rank, false)
    {
        add_child(&S1);
        add_child(&S2);

        S1.add_out_port("out");
        S2.add_in_port("in");
        add_link(&S1, "out", &S2, "in");
    }

    virtual ~RootGraphManager()
    { }

private:
    paradevs::pdevs::mpi::ModelProxy < common::DoubleTime > S1;
    paradevs::pdevs::mpi::ModelProxy < common::DoubleTime > S2;
};

// struct MPIHierarchicalGraphManagerParameters
// {
//     paradevs::tests::boost_graph::OrientedGraphs graphs;
//     paradevs::tests::boost_graph::InputEdgeList input_edges;
//     paradevs::tests::boost_graph::OutputEdgeList output_edges;
//     paradevs::tests::boost_graph::Connections parent_connections;
//     std::vector < int > ranks;
// };

// class MPIHierarchicalGraphManager :
//         public paradevs::pdevs::GraphManager <
//     common::DoubleTime, MPIHierarchicalGraphManagerParameters >
// {
// public:
//     MPIHierarchicalGraphManager(
//         common::Coordinator < common::DoubleTime >* coordinator,
//         const MPIHierarchicalGraphManagerParameters& parameters) :
//         paradevs::pdevs::GraphManager <
//             common::DoubleTime,
//             MPIHierarchicalGraphManagerParameters >
//         (coordinator, parameters)
//     {
//         // build model proxies (graphs)
//         for (unsigned int i = 0; i < parameters.graphs.size(); ++i) {
//             ModelProxy* model = 0;
//             std::ostringstream ss;

//             ss << "S" << i;
//             model = new ModelProxy(ss.str(), parameters.ranks[i], false);
//             _models.push_back(model);
//             add_child(model);
//         }

//         // // builds internal connections (edges)
//         // for (Connections::const_iterator it = parent_connections.begin();
//         //      it != parent_connections.end(); ++it) {
//         //     const Connection& connection = *it;
//         //     std::ostringstream ss_out;
//         //     std::ostringstream ss_in;

//         //     ss_out << "out_" << connection.first.second;
//         //     ss_in << "in_" << connection.first.second;

//         //     if (not ParallelHeapHierarchicalGraphManager <
//         //             GraphBuilder >::exist_link(
//         //                 _coordinators[connection.first.first - 1],
//         //                 ss_out.str(),
//         //                 _coordinators[connection.second.first - 1],
//         //                 ss_in.str())) {
//         //         ParallelHeapHierarchicalGraphManager <
//         //             GraphBuilder >::add_link(
//         //                 _coordinators[connection.first.first - 1],
//         //                 ss_out.str(),
//         //                 _coordinators[connection.second.first - 1],
//         //                 ss_in.str());
//         //     }
//         // }
//     }

//     virtual ~MPIHierarchicalGraphManager()
//     {
//         for (typename ModelProxies::const_iterator it = _models.begin();
//              it != _models.end(); ++it) {
//             delete *it;
//         }
//     }

// private:
//     typedef paradevs::pdevs::mpi::ModelProxy < common::DoubleTime > ModelProxy;
//     typedef std::vector < ModelProxy* > ModelProxies;

//     ModelProxies _models;
// };

} } } // namespace paradevs tests mpi

#endif
