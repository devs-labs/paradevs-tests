/**
 * @file tests/multithreading/main.cpp
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

#include <tests/boost_graph/models.hpp>
#include <tests/boost_graph/graph_builder.hpp>
#include <tests/boost_graph/graph_generator.hpp>
#include <tests/boost_graph/graph_manager.hpp>
#include <tests/boost_graph/graph_partitioning.hpp>

#include <chrono>

using namespace paradevs::common;
using namespace std::chrono;

double grid_monothreading()
{
    unsigned int side = 20;
    std::vector<std::pair<int,int>> vertex_selection;
    std::pair<int,int> tmp;
    tmp.first = 0;
    tmp.second = 3;
    vertex_selection.push_back(tmp);
    paradevs::tests::boost_graph::Entiers weight_vertex;
    weight_vertex.push_back(1);
    const char *edge_weight;
    edge_weight = "../../sortie_graphe/tests_grid.txt";
    bool rec = false;

    paradevs::tests::boost_graph::RandomGridGraphGenerator generator(
        side, vertex_selection,  weight_vertex, edge_weight, rec);

    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::Coordinator <
            DoubleTime,
            paradevs::tests::boost_graph::HeapHierarchicalGraphManager <
                paradevs::tests::boost_graph::PartitioningGraphBuilder >,
            paradevs::common::NoParameters,
            paradevs::tests::boost_graph::PartitioningParameters >
        > rc(0, 10, "root", paradevs::common::NoParameters(),
             paradevs::tests::boost_graph::PartitioningParameters(4,
                                                                  "gggp",
                                                                  20,
                                                                  false,
                                                                  generator));

    steady_clock::time_point t1 = steady_clock::now();

    rc.run();

    steady_clock::time_point t2 = steady_clock::now();

    duration < double > time_span = duration_cast <
        duration < double > >(t2 - t1);

    return time_span.count();
}

double grid_multithreading(int cluster_number)
{
    unsigned int side = 20;
    std::vector<std::pair<int,int>> vertex_selection;
    std::pair<int,int> tmp;
    tmp.first = 0;
    tmp.second = 3;
    vertex_selection.push_back(tmp);
    paradevs::tests::boost_graph::Entiers weight_vertex;
    weight_vertex.push_back(1);
    const char *edge_weight;
    edge_weight = "../../sortie_graphe/tests_grid.txt";
    bool rec = false;

    paradevs::tests::boost_graph::RandomGridGraphGenerator generator(
        side, vertex_selection,  weight_vertex, edge_weight, rec);

    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::multithreading::Coordinator <
            DoubleTime,
            paradevs::tests::boost_graph::ParallelHeapHierarchicalGraphManager <
                paradevs::tests::boost_graph::PartitioningGraphBuilder >,
            paradevs::common::NoParameters,
            paradevs::tests::boost_graph::PartitioningParameters >
        > rc(0, 10, "root", paradevs::common::NoParameters(),
             paradevs::tests::boost_graph::PartitioningParameters(
                 cluster_number, "rand", 20, false, generator));

    steady_clock::time_point t1 = steady_clock::now();

    rc.run();

    steady_clock::time_point t2 = steady_clock::now();

    duration < double > time_span = duration_cast <
        duration < double > >(t2 - t1);

    return time_span.count();
}

double tree_monothreading()
{
    std::vector < int > levels = { 4, 3, 2 };
    int nbr_sommets = 1000;
    int sources = nbr_sommets/100*1;
    paradevs::tests::boost_graph::RandomGraphGenerator generator(nbr_sommets,
                                                                 levels,
                                                                 sources, 2, 3);

    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::Coordinator <
            DoubleTime,
            paradevs::tests::boost_graph::HeapHierarchicalGraphManager <
                paradevs::tests::boost_graph::PartitioningGraphBuilder >,
            paradevs::common::NoParameters,
            paradevs::tests::boost_graph::PartitioningParameters >
        > rc(0, 10, "root", paradevs::common::NoParameters(),
             paradevs::tests::boost_graph::PartitioningParameters(
                 4, "gggp", 200, false, generator));

    steady_clock::time_point t1 = steady_clock::now();

    rc.run();

    steady_clock::time_point t2 = steady_clock::now();

    duration < double > time_span = duration_cast <
        duration < double > >(t2 - t1);

    return time_span.count();
}

double tree_multithreading(int cluster_number)
{
    std::vector < int > levels = { 4, 3, 2 };
    int nbr_sommets = 1000;
    int sources = nbr_sommets/100*1;
    paradevs::tests::boost_graph::RandomGraphGenerator generator(nbr_sommets,
                                                                 levels,
                                                                 sources, 2, 3);

    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::multithreading::Coordinator <
            DoubleTime,
            paradevs::tests::boost_graph::ParallelHeapHierarchicalGraphManager <
                paradevs::tests::boost_graph::PartitioningGraphBuilder >,
            paradevs::common::NoParameters,
            paradevs::tests::boost_graph::PartitioningParameters >
        > rc(0, 10, "root", paradevs::common::NoParameters(),
             paradevs::tests::boost_graph::PartitioningParameters(
                 cluster_number, "gggp", 200, false, generator));

    steady_clock::time_point t1 = steady_clock::now();

    rc.run();

    steady_clock::time_point t2 = steady_clock::now();

    duration < double > time_span = duration_cast <
        duration < double > >(t2 - t1);

    return time_span.count();
}

void grid(int n)
{
    if (n == 1) {
        std::cout << grid_monothreading() << std::endl;
    } else {
        std::cout << grid_multithreading(n) << std::endl;
        // grid_multithreading(n);
    }
}

void tree(int n)
{
    if (n == 1) {
        std::cout << tree_monothreading() << std::endl;
    } else {
        std::cout << tree_multithreading(n) << std::endl;
        // tree_multithreading(n);
    }
}


int main(int argc, char** argv)
{
    if (argc > 1) {
        // grid(atoi(argv[1]));
        tree(atoi(argv[1]));
    }
    return 0;
}
