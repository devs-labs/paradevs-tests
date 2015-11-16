/**
 * @file tests/mpi/main.cpp
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

#include <paradevs/common/RootCoordinator.hpp>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

// #include <tests/boost_graph/graph_defs.hpp>
// #include <tests/boost_graph/graph_builder.hpp>
// #include <tests/boost_graph/graph_generator.hpp>
// #include <tests/boost_graph/graph_partitioning.hpp>

#include <paradevs/common/time/DoubleTime.hpp>
#include <paradevs/kernel/pdevs/mpi/LogicalProcessor.hpp>
#include <tests/mpi/graph_manager.hpp>

#include <chrono>

using namespace paradevs::tests::mpi;
using namespace paradevs::common;
using namespace boost::mpi;
using namespace std::chrono;

void example_simple(int argc, char *argv[])
{
    environment env(argc, argv);
    communicator world;

    if (world.rank() == 0) {
        paradevs::tests::mpi::RootGraphManagerParameters parameters;

        parameters.ranks.push_back(1);
        parameters.ranks.push_back(2);
        parameters.ranks.push_back(3);
        parameters.ranks.push_back(4);
        parameters.ranks.push_back(5);
        parameters.ranks.push_back(6);
        parameters.ranks.push_back(7);
        parameters.ranks.push_back(8);

        paradevs::common::RootCoordinator <
            DoubleTime,
            paradevs::pdevs::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::RootGraphManager,
                paradevs::common::NoParameters,
                paradevs::tests::mpi::RootGraphManagerParameters >
            > rc(0, 20, "root", paradevs::common::NoParameters(), parameters);


        steady_clock::time_point t1 = steady_clock::now();

        rc.run();

        steady_clock::time_point t2 = steady_clock::now();

        duration < double > time_span = duration_cast <
            duration < double > >(t2 - t1);

        std::cout << "CHAIN = " << time_span.count() << std::endl;

    } else {
        std::stringstream ss;

        ss << "S" << world.rank();
        if (world.rank() == 1) {
            paradevs::pdevs::mpi::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::S1GraphManager > model(
                    ss.str(), paradevs::common::NoParameters(),
                    paradevs::common::NoParameters());
            paradevs::pdevs::mpi::LogicalProcessor <
                DoubleTime > LP(&model, world.rank(), 0);

            model.set_logical_processor(&LP);
            LP.loop();
        } else {
            paradevs::pdevs::mpi::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::S2GraphManager > model(
                    ss.str(), paradevs::common::NoParameters(),
                    paradevs::common::NoParameters());
            paradevs::pdevs::mpi::LogicalProcessor <
                DoubleTime > LP(&model, world.rank(), 0);

            model.set_logical_processor(&LP);
            LP.loop();
        }
    }
}

#define SUB_GRID_SIZE 32
#define SUB_GRID_NUMBER 8

void example_grid(int argc, char *argv[])
{
    environment env(argc, argv);
    communicator world;

    if (world.rank() == 0) {
        paradevs::tests::mpi::GridGraphManagerParameters parameters;

        parameters.global_size = SUB_GRID_SIZE * SUB_GRID_NUMBER;
        parameters.sub_grid_size = SUB_GRID_SIZE;

        paradevs::common::RootCoordinator <
            DoubleTime,
            paradevs::pdevs::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::GridGraphManager,
                paradevs::common::NoParameters,
                paradevs::tests::mpi::GridGraphManagerParameters >
            > rc(0, 20, "root", paradevs::common::NoParameters(), parameters);

        rc.run();
    } else {
        std::stringstream ss;
        unsigned int x = (world.rank() - 1) % SUB_GRID_NUMBER;
        unsigned int y = (world.rank() - 1) / SUB_GRID_NUMBER;
        paradevs::tests::mpi::SubGridGraphManagerParameters parameters;

        ss << "GC_" << x << "_" << y;

        parameters.global_size = SUB_GRID_SIZE * SUB_GRID_NUMBER;
        parameters.begin_column = x * SUB_GRID_SIZE;
        parameters.end_column = (x + 1) * SUB_GRID_SIZE - 1;
        parameters.begin_line = y * SUB_GRID_SIZE;
        parameters.end_line = (y + 1) * SUB_GRID_SIZE - 1;
        paradevs::pdevs::mpi::Coordinator <
            DoubleTime,
            paradevs::tests::mpi::SubGridGraphManager,
            paradevs::common::NoParameters,
            paradevs::tests::mpi::SubGridGraphManagerParameters
            > model(ss.str(), paradevs::common::NoParameters(), parameters);
        paradevs::pdevs::mpi::LogicalProcessor <
            DoubleTime > LP(&model, world.rank(), 0);

        model.set_logical_processor(&LP);
        LP.loop();
    }
}

// void example_grid(int argc, char *argv[])
// {
//     environment env(argc, argv);
//     communicator world;

//     unsigned int side = 100;
//     std::vector<std::pair<int,int>> vertex_selection;
//     std::pair<int,int> tmp;
//     tmp.first = 0;
//     tmp.second = 3;
//     vertex_selection.push_back(tmp);
//     paradevs::tests::boost_graph::Entiers weight_vertex;
//     weight_vertex.push_back(1);
//     const char *edge_weight;
//     edge_weight = "../../sortie_graphe/tests_grid.txt";
//     bool rec = false;

//     paradevs::tests::boost_graph::RandomGridGraphGenerator generator(
//         side, vertex_selection,  weight_vertex, edge_weight, rec);
//     paradevs::tests::boost_graph::PartitioningGraphBuilder builder(
//         4, "gggp", 20, false, generator);
//     paradevs::tests::mpi::MPIHierarchicalGraphManagerParameters parameters;

//     builder.build(parameters.graphs, parameters.input_edges,
//                   parameters.output_edges, parameters.parent_connections);
//     if (world.rank() == 0) {
//         paradevs::common::RootCoordinator <
//             DoubleTime, paradevs::pdevs::mpi::Coordinator <
//                 DoubleTime,
//                 paradevs::tests::mpi::MPIHierarchicalGraphManager,
//                 paradevs::common::NoParameters,
//                 paradevs::tests::mpi::MPIHierarchicalGraphManagerParameters >
//             > rc(0, 10, "root", paradevs::common::NoParameters(), parameters);

//         steady_clock::time_point t1 = steady_clock::now();

//         rc.run();

//         steady_clock::time_point t2 = steady_clock::now();

//         duration < double > time_span = duration_cast <
//             duration < double > >(t2 - t1);

//         std::cout << "MULTI = " << time_span.count() << std::endl;
//     } else {
//     }
// }

int main(int argc, char *argv[])
{
    example_simple(argc, argv);
//    example_grid(argc, argv);
    return 0;
}
