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

#include <tests/multithreading/lifegame/models.hpp>
#include <tests/pdevs/graph_manager.hpp>
#include <tests/pdevs/models.hpp>

#include <paradevs/kernel/pdevs/mpi/Coordinator.hpp>
#include <paradevs/kernel/pdevs/mpi/ModelProxy.hpp>
#include <paradevs/kernel/pdevs/mpi/GraphManager.hpp>
#include <paradevs/kernel/pdevs/Simulator.hpp>

#include <sstream>

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
    std::vector < int > ranks;
};

typedef paradevs::pdevs::mpi::ModelProxy < common::DoubleTime > ModelProxy;
typedef std::vector < ModelProxy* > ModelProxies;

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
                                            coordinator, parameters)
    {
        ModelProxy* previous = 0;

        for (std::vector < int >::const_iterator it = parameters.ranks.begin();
             it != parameters.ranks.end(); ++it) {
            std::stringstream ss;
            ModelProxy* model = 0;

            ss << "S" << *it;
            model = new ModelProxy(ss.str(), *it, false);
            models.push_back(model);
            add_child(model);
            model->add_out_port("out");
            model->add_in_port("in");
            if (it != parameters.ranks.begin()) {
                add_link(previous, "out", model, "in");
            }
            previous = model;
        }
    }

    virtual ~RootGraphManager()
    {
        std::for_each(models.begin(), models.end(),
                      std::default_delete < ModelProxy >());
    }

private:
    ModelProxies models;
};

struct GridGraphManagerParameters
{
    unsigned int global_size;
    unsigned int sub_grid_size;
};

class GridGraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime,
                                               GridGraphManagerParameters >
{
public:
    GridGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const GridGraphManagerParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime,
                                        GridGraphManagerParameters >(
                                            coordinator, parameters)
    {
        int index = 1;
        unsigned int size = parameters.global_size / parameters.sub_grid_size;

        // models
        for (unsigned int j = 0; j < size; ++j) {
            for (unsigned int i = 0; i < size; ++i) {
                std::stringstream ss;
                ModelProxy* model = 0;

                ss << "S_" << i << "_" << j;
                model = new ModelProxy(ss.str(), index++, false);
                models.push_back(model);
                add_child(model);
            }
        }

        // input ports
        for (unsigned int i = 0; i < size; ++i) {
            for (unsigned int j = 0; j < size; ++j) {
                ModelProxy* model = models[i + j * size];

                // top and bottom
                for (unsigned int x = i * parameters.sub_grid_size;
                     x < (i + 1) * parameters.sub_grid_size; ++x) {
                    // top
                    if (j != 0) {
                        std::stringstream ss;

                        ss << "in_" << x << "_"
                           << (j * parameters.sub_grid_size - 1);

                        // std::cout << "ADD INPUT PORT "
                        //           << model->get_name() << " "
                        //           << ss.str() << std::endl;

                        model->add_in_port(ss.str());
                    }
                    // bottom
                    if (j != size - 1) {
                        std::stringstream ss;

                        ss << "in_" << x << "_"
                           << ((j + 1) * parameters.sub_grid_size);

                        // std::cout << "ADD INPUT PORT "
                        //           << model->get_name() << " "
                        //           << ss.str() << std::endl;

                        model->add_in_port(ss.str());
                    }
                }
                // left and right
                for (unsigned int y = j * parameters.sub_grid_size;
                     y < (j + 1) * parameters.sub_grid_size; ++y) {

                    // left
                    if (i != 0) {
                        std::stringstream ss;

                        ss << "in_" << (i * parameters.sub_grid_size - 1)
                           << "_" << y;

                        // std::cout << "ADD INPUT PORT "
                        //           << model->get_name() << " "
                        //           << ss.str() << std::endl;

                        model->add_in_port(ss.str());
                    }
                    // right
                    if (i != size - 1) {
                        std::stringstream ss;

                        ss << "in_" << ((i + 1) * parameters.sub_grid_size)
                           << "_" << y;

                        // std::cout << "ADD INPUT PORT "
                        //           << model->get_name() << " "
                        //           << ss.str() << std::endl;

                        model->add_in_port(ss.str());
                    }
                }
            }
        }

        // output ports
        for (unsigned int i = 0; i < size; ++i) {
            for (unsigned int j = 0; j < size; ++j) {
                ModelProxy* model = models[i + j * size];

                // top and bottom
                for (unsigned int x = i * parameters.sub_grid_size;
                     x < (i + 1) * parameters.sub_grid_size; ++x) {
                    // top
                    if (j != 0) {
                        std::stringstream ss;

                        ss << "out_" << x << "_"
                           << (j * parameters.sub_grid_size);

                        // std::cout << "ADD OUTPUT PORT "
                        //           << model->get_name() << " "
                        //           << ss.str() << std::endl;

                        model->add_out_port(ss.str());
                    }
                    // bottom
                    if (j != size - 1) {
                        std::stringstream ss;

                        ss << "out_" << x << "_"
                           << ((j + 1) * parameters.sub_grid_size - 1);

                        // std::cout << "ADD OUTPUT PORT "
                        //           << model->get_name() << " "
                        //           << ss.str() << std::endl;

                        model->add_out_port(ss.str());
                    }
                }
                // left and right
                for (unsigned int y = j * parameters.sub_grid_size;
                     y < (j + 1) * parameters.sub_grid_size; ++y) {
                    // left
                    if (i != 0) {
                        std::stringstream ss;

                        ss << "out_" << (i * parameters.sub_grid_size)
                           << "_" << y;
                        if (not model->exist_out_port(ss.str())) {

                            // std::cout << "ADD OUTPUT PORT "
                            //           << model->get_name() << " "
                            //           << ss.str() << std::endl;

                            model->add_out_port(ss.str());
                        }
                    }
                    // right
                    if (i != size - 1) {
                        std::stringstream ss;

                        ss << "out_" << ((i + 1) * parameters.sub_grid_size - 1)
                           << "_" << y;
                        if (not model->exist_out_port(ss.str())) {

                            // std::cout << "ADD OUTPUT PORT "
                            //           << model->get_name() << " "
                            //           << ss.str() << std::endl;

                            model->add_out_port(ss.str());
                        }
                    }
                }
            }
        }

        // connections
        for (unsigned int i = 0; i < size; ++i) {
            for (unsigned int j = 0; j < size; ++j) {
                ModelProxy* model = models[i + j * size];

                // top
                if (j != 0) {
                    for (unsigned int x = i * parameters.sub_grid_size;
                         x < (i + 1) * parameters.sub_grid_size; ++x) {
                        std::stringstream out_ss;
                        std::stringstream in_ss;

                        out_ss << "out_" << x << "_"
                               << (j * parameters.sub_grid_size - 1);
                        in_ss << "in_" << x << "_"
                              << (j * parameters.sub_grid_size - 1);

                        // std::cout << "LINK " << model->get_name()
                        //           << "::" << in_ss.str()
                        //           << " <= "
                        //           << models[i + (j - 1) * size]->get_name()
                        //           << "::"
                        //           << out_ss.str()
                        //           << std::endl;

                        add_link(models[i + (j - 1) * size], out_ss.str(),
                                 model, in_ss.str());
                    }
                }
                // bottom
                if (j != size - 1) {
                    for (unsigned int x = i * parameters.sub_grid_size;
                         x < (i + 1) * parameters.sub_grid_size; ++x) {
                        std::stringstream out_ss;
                        std::stringstream in_ss;

                        out_ss << "out_" << x << "_"
                               << ((j + 1) * parameters.sub_grid_size);
                        in_ss << "in_" << x << "_"
                              << ((j + 1) * parameters.sub_grid_size);

                        // std::cout << "LINK " << model->get_name()
                        //           << "::" << in_ss.str()
                        //           << " <= "
                        //           << models[i + (j + 1) * size]->get_name()
                        //           << "::"
                        //           << out_ss.str()
                        //           << std::endl;

                        add_link(models[i + (j + 1) * size], out_ss.str(),
                                 model, in_ss.str());
                    }
                }
                // left
                if (i != 0) {
                    for (unsigned int y = j * parameters.sub_grid_size;
                         y < (j + 1) * parameters.sub_grid_size; ++y) {
                        std::stringstream out_ss;
                        std::stringstream in_ss;

                        out_ss << "out_" << (i * parameters.sub_grid_size - 1)
                               << "_" << y;
                        in_ss << "in_" << (i * parameters.sub_grid_size - 1)
                              << "_" << y;

                        // std::cout << "LINK " << model->get_name()
                        //           << "::" << in_ss.str()
                        //           << " <= "
                        //           << models[i - 1 + j * size]->get_name()
                        //           << "::"
                        //           << out_ss.str()
                        //           << std::endl;

                        add_link(models[i - 1 + j * size], out_ss.str(),
                                 model, in_ss.str());
                    }
                }
                // right
                if (i != size - 1) {
                    for (unsigned int y = j * parameters.sub_grid_size;
                         y < (j + 1) * parameters.sub_grid_size; ++y) {
                        std::stringstream out_ss;
                        std::stringstream in_ss;

                        out_ss << "out_"
                               << ((i + 1) * parameters.sub_grid_size)
                               << "_" << y;
                        in_ss << "in_"
                              << ((i + 1) * parameters.sub_grid_size)
                              << "_" << y;

                        // std::cout << "LINK " << model->get_name()
                        //           << "::" << in_ss.str()
                        //           << " <= "
                        //           << models[i + 1 + j * size]->get_name()
                        //           << "::"
                        //           << out_ss.str()
                        //           << std::endl;

                        add_link(models[i + 1 + j * size], out_ss.str(),
                                 model, in_ss.str());
                    }
                }
            }
        }
    }

    virtual ~GridGraphManager()
    {
        std::for_each(models.begin(), models.end(),
                      std::default_delete < ModelProxy >());
    }

private:
    ModelProxies models;
};

struct SubGridGraphManagerParameters
{
    unsigned int global_size;
    unsigned int begin_line;
    unsigned int end_line;
    unsigned int begin_column;
    unsigned int end_column;
};

class SubGridGraphManager :
        public paradevs::pdevs::mpi::GraphManager <
    common::DoubleTime, SubGridGraphManagerParameters >
{
public:
    SubGridGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const SubGridGraphManagerParameters& parameters) :
        paradevs::pdevs::mpi::GraphManager <
    common::DoubleTime, SubGridGraphManagerParameters >(coordinator,
                                                        parameters)
    {
        unsigned int size = parameters.end_column - parameters.begin_column + 1;

        for (unsigned int y = parameters.begin_line;
             y <= parameters.end_line; ++y) {
            for (unsigned int x = parameters.begin_column;
                 x <= parameters.end_column; ++x) {
                std::stringstream ss;
                Simulator* simulator = 0;
                paradevs::tests::multithreading::lifegame::CellParameters
                    cell_parameters(
                        ((x != 0) ? 1 : 0) +
                        ((x != parameters.global_size - 1) ? 1 : 0) +
                        ((y != 0) ? 1 : 0) +
                        ((y != parameters.global_size - 1) ? 1 : 0));

                ss << "C_" << x << "_" << y;
                simulator = new Simulator(ss.str(), cell_parameters);
                simulators.push_back(simulator);
                add_child(simulator);
                simulator->add_out_port("out");
                simulator->add_in_port("in");
            }
        }
        // internal connections
        for (unsigned int x = parameters.begin_column;
             x <= parameters.end_column; ++x) {
            for (unsigned int y = parameters.begin_line;
                 y <= parameters.end_line; ++y) {
                unsigned int i = x - parameters.begin_column;
                unsigned int j = y - parameters.begin_line;
                Simulator* simulator = simulators[i + j * size];

                if (i != 0) {
                    add_link(simulators[(i - 1) + j * size], "out",
                             simulator, "in");

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << ((i - 1) + j * size) << " -> "
                    //           << (i + j * size) << std::endl;

                }
                if (i != size - 1) {
                    add_link(simulators[(i + 1) + j * size], "out",
                             simulator, "in");

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << ((i + 1) + j * size) << " -> "
                    //           << (i + j * size) << std::endl;

                }
                if (j != 0) {
                    add_link(simulators[i + (j - 1) * size], "out",
                             simulator, "in");

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << (i + (j - 1) * size) << " -> "
                    //           << (i + j * size) << std::endl;

                }
                if (j != size - 1) {
                    add_link(simulators[i + (j + 1 ) * size], "out",
                             simulator, "in");

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << (i + (j + 1) * size) << " -> "
                    //           << (i + j * size) << std::endl;

                }
            }
        }
        // input ports
        {
            // top and bottom
            for (unsigned int x = parameters.begin_column;
                 x <= parameters.end_column; ++x) {
                unsigned int i = x - parameters.begin_column;

                // top
                if (parameters.begin_line != 0) {
                    std::stringstream ss;

                    ss << "in_" << x << "_" << (parameters.begin_line - 1);
                    coordinator->add_in_port(ss.str());
                    add_link(coordinator, ss.str(), simulators[i], "in");
                }
                // bottom
                if (parameters.end_line != parameters.global_size - 1) {
                    std::stringstream ss;

                    ss << "in_" << x << "_" << (parameters.end_line + 1);
                    coordinator->add_in_port(ss.str());
                    add_link(coordinator, ss.str(),
                             simulators[i + size * (size - 1)], "in");
                }
            }
            // left and right
            for (unsigned int y = parameters.begin_line;
                 y <= parameters.end_line; ++y) {
                unsigned int j = y - parameters.begin_line;

                // left
                if (parameters.begin_column != 0) {
                    std::stringstream ss;

                    ss << "in_" << (parameters.begin_column - 1) << "_" << y;
                    coordinator->add_in_port(ss.str());
                    add_link(coordinator, ss.str(), simulators[j * size], "in");
                }
                // right
                if (parameters.end_column != parameters.global_size - 1) {
                    std::stringstream ss;

                    ss << "in_" << (parameters.end_column + 1) << "_" << y;
                    coordinator->add_in_port(ss.str());
                    add_link(coordinator, ss.str(),
                             simulators[size - 1 + j * size], "in");

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << ss.str() << " -> "
                    //           << (size - 1 + j * size) << std::endl;

                }
            }
        }
        // output ports
        {
            // top and bottom
            for (unsigned int x = parameters.begin_column;
                 x <= parameters.end_column; ++x) {
                unsigned int i = x - parameters.begin_column;

                // top
                {
                    std::stringstream ss;

                    ss << "out_" << x << "_" << parameters.begin_line;
                    coordinator->add_out_port(ss.str());
                    add_link(simulators[i], "out", coordinator, ss.str());

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << simulators[i]->get_name()
                    //           << "::out => " << ss.str() << std::endl;

                }
                // bottom
                {
                    std::stringstream ss;

                    ss << "out_" << x << "_" << parameters.end_line;
                    coordinator->add_out_port(ss.str());
                    add_link(simulators[i + size * (size - 1)], "out",
                             coordinator, ss.str());

                    // std::cout << "subgrid [ " << parameters.begin_column
                    //           << " " << parameters.begin_line << " ] => "
                    //           << simulators[i + size * (size - 1)]->get_name()
                    //           << "::out => " << ss.str() << std::endl;

                }
            }
            // left and right
            for (unsigned int y = parameters.begin_line;
                 y <= parameters.end_line; ++y) {
                unsigned int j = y - parameters.begin_line;

                // left
                {
                    std::stringstream ss;

                    ss << "out_" << parameters.begin_column << "_" << y;
                    if (not coordinator->exist_out_port(ss.str())) {
                        coordinator->add_out_port(ss.str());
                        add_link(simulators[j * size], "out",
                                 coordinator, ss.str());

                        // std::cout << "subgrid [ " << parameters.begin_column
                        //       << " " << parameters.begin_line << " ] => "
                        //       << simulators[j * size]->get_name()
                        //       << "::out => " << ss.str() << std::endl;

                    }
                }
                // right
                {
                    std::stringstream ss;

                    ss << "out_" << parameters.end_column << "_" << y;
                    if (not coordinator->exist_out_port(ss.str())) {
                        coordinator->add_out_port(ss.str());
                        add_link(simulators[size - 1 + j * size], "out",
                                 coordinator, ss.str());
                    }
                }
            }
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

    virtual ~SubGridGraphManager()
    {
        std::for_each(simulators.begin(), simulators.end(),
                      std::default_delete < Simulator >());
    }

private:
    typedef paradevs::pdevs::Simulator <
    common::DoubleTime,
    multithreading::lifegame::Cell,
    multithreading::lifegame::CellParameters > Simulator;

    typedef std::vector < Simulator* > Simulators;

    Simulators simulators;
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
