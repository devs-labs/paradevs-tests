/**
 * @file tests/boost_graph/tests.cpp
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

#include <boost/timer.hpp>

#include <paradevs/common/RootCoordinator.hpp>

#include <tests/boost_graph/models.hpp>
#include <tests/boost_graph/graph_defs.hpp>
#include <tests/boost_graph/graph_builder.hpp>
#include <tests/boost_graph/graph_generator.hpp>
#include <tests/boost_graph/graph_manager.hpp>
#include <tests/boost_graph/graph_partitioning.hpp>

using namespace paradevs::common;
using namespace paradevs::common::scheduler;
using namespace paradevs::pdevs;
using namespace paradevs::tests::boost_graph;

#include <fstream>

std::ofstream log_file("log");

/* flat models */

template < class GraphBuilder >
void flat_heap_test(double duration)
{
    RootCoordinator <
        DoubleTime,
        paradevs::pdevs::Coordinator <
            DoubleTime,
            InBuildFlatGraphManager < GraphBuilder > >
        > rc(0, duration, "root");

    rc.run();
}

// template < class GraphBuilder >
// void flat_vector_test(double duration)
// {
//     RootCoordinator <
//         DoubleTime,
//         paradevs::pdevs::Coordinator <
//             DoubleTime,
//             paradevs::common::scheduler::VectorScheduler <
//                 paradevs::common::DoubleTime >,
//             paradevs::common::scheduler::NoSchedulerHandle,
//             InBuildFlatGraphManager <
//                 paradevs::common::scheduler::NoSchedulerHandle,
//                 GraphBuilder > >
//     > rc(0, duration, "root");

//     rc.run();
// }

/* partitionning models */

template < class GraphGenerator >
void partitionning_heap_test(double duration,
                             int cluster_number,
                             std::string partitioning_method_name,
                             int contraction_coef,
                             bool contraction_coef_flag,
                             GraphGenerator& g)
{
    RootCoordinator <
        DoubleTime,
        paradevs::pdevs::Coordinator <
            DoubleTime,
            HeapHierarchicalGraphManager < PartitioningGraphBuilder>,
            paradevs::common::NoParameters,
            PartitioningParameters >
        > rc(0, duration, "root", NoParameters(),
             PartitioningParameters(cluster_number, partitioning_method_name,
                                    contraction_coef, contraction_coef_flag,
                                    g));

    rc.run();
}

// template < class GraphGenerator >
// void partitionning_vector_test(double duration,
//                                int cluster_number,
//                                std::string partitioning_method_name,
//                                int contraction_coef,
//                                bool contraction_coef_flag,
//                                GraphGenerator& g)
// {
//     RootCoordinator <
//         DoubleTime,
//         paradevs::pdevs::Coordinator <
//             DoubleTime,
//             paradevs::common::scheduler::VectorScheduler <
//                 paradevs::common::DoubleTime >,
//             paradevs::common::scheduler::NoSchedulerHandle,
//             VectorHierarchicalGraphManager <
//                 paradevs::common::scheduler::NoSchedulerHandle,
//                 PartitioningGraphBuilder >,
//             paradevs::common::NoParameters,
//             PartitioningParameters >
//         > rc(0, duration, "root", NoParameters(),
//              PartitioningParameters(cluster_number, partitioning_method_name,
//                                     contraction_coef, contraction_coef_flag, g));

//     rc.run();
// }

/* iteration for partitionning methods */

template < class GraphGenerator >
void test(double duration, std::string partitioning_method_name,
          unsigned int min, unsigned int max, unsigned int step,
          bool vector, bool heap,
          int contraction_coef,
          bool contraction_coef_flag,
          GraphGenerator& g)
{
    boost::timer t;
	int nbr_ite = 10;

    log_file << "==== " << partitioning_method_name << " with ";
    if (contraction_coef_flag) {
        log_file << "/";
    }
    log_file << contraction_coef << " ===" << std::endl;
    log_file << "n";
    if (heap) {
        log_file << "\t heap";
    }
    if (vector) {
        log_file << "\t vector";
    }
    log_file << std::endl;

    if (contraction_coef_flag) {
        for (unsigned int n = min; n <= max; n += step) {
            log_file << n << "\t";
            if (heap) {
                double t1 = t.elapsed();

                for (unsigned int i = 0; i < nbr_ite; ++i) {
                    partitionning_heap_test(duration, n,
                                            partitioning_method_name,
                                            contraction_coef,
                                            contraction_coef_flag, g);
                }

                double t2 = t.elapsed();

                log_file << (t2 - t1) / nbr_ite << "\t";
            }
            if (vector) {
                double t1 = t.elapsed();

                for (unsigned int i = 0; i < nbr_ite; ++i) {
                    // partitionning_vector_test(duration, n,
                    //                           partitioning_method_name,
                    //                           contraction_coef,
                    //                           contraction_coef_flag, g);
                }

                double t2 = t.elapsed();

                log_file << (t2 - t1) / nbr_ite << std::endl;
            } else {
                log_file << std::endl;
            }

        }
    } else {
        log_file << contraction_coef << "\t";
        if (heap) {
            double t1 = t.elapsed();

            for (unsigned int i = 0; i < nbr_ite; ++i) {
                partitionning_heap_test(duration, contraction_coef,
                                        partitioning_method_name,
                                        contraction_coef,
                                        contraction_coef_flag, g);
            }

            double t2 = t.elapsed();

            log_file << (t2 - t1) / nbr_ite << "\t";
        }
        if (vector) {
            double t1 = t.elapsed();

            for (unsigned int i = 0; i < nbr_ite; ++i) {
                // partitionning_vector_test(duration, contraction_coef,
                //                           partitioning_method_name,
                //                           contraction_coef,
                //                           contraction_coef_flag, g);
            }

            double t2 = t.elapsed();

            log_file << (t2 - t1) / nbr_ite << std::endl;
        } else {
            log_file << std::endl;
        }
    }
}

/* 38 nodes graph */

const double duration_38 = 20;

void test_flat_38()
{
    boost::timer t;

    log_file << "== 38 nodes ==" << std::endl;
    log_file << "flat graph with heap = ";
    for (unsigned int i = 0; i < 20; ++i) {
        flat_heap_test< FlatGraphBuilder >(duration_38);
    }

    double t2 = t.elapsed();

    log_file << t2 / 20 << std::endl;

    log_file << "flat graph with vector = ";
    for (unsigned int i = 0; i < 20; ++i) {
        // flat_vector_test< FlatGraphBuilder >(duration_38);
    }

    double t3 = t.elapsed();

    log_file << (t3 - t2) / 20 << std::endl;
}

void test_partitioning_38()
{
    ArtificialGraphGenerator g(38);

    for (int i = 1; i <= 2; ++i) {
		/*std::cout<<"Méthode ggp"<<std::endl;
        test < ArtificialGraphGenerator >(duration_38, "ggp", 2, 16, 2,
                                          true, true, i, true, g);*/
        std::cout<<"Méthode gggp"<<std::endl;
        test < ArtificialGraphGenerator >(duration_38, "gggp_pond", 2, 16, 2,
                                          true, true, i, true, g);
        std::cout<<"Méthode random"<<std::endl;
        test < ArtificialGraphGenerator >(duration_38, "random", 2, 16, 2,
                                          true, true, i, true, g);
    }

    for (int i = 2; i <= 8; i += 2) {
        test < ArtificialGraphGenerator >(duration_38, "gggp_pond", 0, 0, 0,
                                          true, true, i, false, g);
    }
}

/* random graph */

const double duration_random = 0;

void test_flat_random()
{
    boost::timer t;
    uint nbr_ite = 20;

    log_file << "== Random Graph ==" << std::endl;
    log_file << "flat graph with heap = ";
    for (unsigned int i = 0; i < nbr_ite; ++i) {
        flat_heap_test< RandomFlatGraphBuilder >(duration_random);
    }

    double t2 = t.elapsed();

    /*log_file << t2 / 10 << std::endl;

    log_file << "flat graph with vector = ";
    for (unsigned int i = 0; i < 10; ++i) {
        flat_vector_test< FlatGraphBuilder >(duration_random);
    }

    double t3 = t.elapsed();*/

    //log_file << (t3 - t2) / 10 << std::endl;
    log_file << t2 / nbr_ite << std::endl;
}

void test_partitioning_random()
{
    std::vector < int > levels = {/*5,4,*/ 3, 2  };
    int nbr_sommets = 200;
    int sources = nbr_sommets/100*1;

    RandomGraphGenerator g(nbr_sommets, levels, sources, 2, 3);

    /*test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 200, 2,
                                  false, true, 5, true, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 200, 2,
                                  false, true, 10, true, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 200, 2,
                                  false, true, 20, true, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 42, 42, 2,
                                  false, true, 40, true, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 50, 2,
                                  false, true, 60, true, g);*/
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 200, 2,
                                  false, true, 1 , true, g);
   /* test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 100, true, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 30, 2,
                                  false, true, 200, true, g);*/

    /*test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 15, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 20, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 25, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 30, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 35, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 40, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 45, false, g);
    test < RandomGraphGenerator >(duration_random, "gggp_pond", 2, 32, 2,
                                  false, true, 50, false, g);*/

    /*test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 5, true, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 10, true, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 20, true, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 40, true, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 60, true, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 80, true, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 30, 2,
                                  false, true, 100, true, g);*/

    /*test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 15, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 20, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 25, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 30, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 35, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 40, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 45, false, g);
    test < RandomGraphGenerator >(duration_random, "ggp", 2, 32, 2,
                                  false, true, 50, false, g);

    test < RandomGraphGenerator >(duration_random, "random", 2, 32, 2,
                                  false, true, 5, true, g);
    test < RandomGraphGenerator >(duration_random, "random", 2, 32, 2,
                                  false, true, 10, true, g);
    test < RandomGraphGenerator >(duration_random, "random", 2, 32, 2,
                                  false, true, 20, true, g);
    test < RandomGraphGenerator >(duration_random, "random", 2, 32, 2,
                                  false, true, 40, true, g);
    test < RandomGraphGenerator >(duration_random, "random", 2, 22, 2,
                                  false, true, 100, true, g);*/

}

const double duration_random_grid = 40;

void test_flat_random_grid()
{
    boost::timer t;
    uint nbr_ite = 10;

    log_file << "== Random Graph ==" << std::endl;
    log_file << "flat graph with heap = ";
    for (unsigned int i = 0; i < nbr_ite; ++i) {
        flat_heap_test< RandomGridFlatGraphBuilder >(duration_random_grid);
    }

    double t2 = t.elapsed();

    /*log_file << t2 / 10 << std::endl;

    log_file << "flat graph with vector = ";
    for (unsigned int i = 0; i < 10; ++i) {
        flat_vector_test< FlatGraphBuilder >(duration_random);
    }

    double t3 = t.elapsed();*/

    //log_file << (t3 - t2) / 10 << std::endl;
    log_file << t2 / nbr_ite << std::endl;
}

void test_partitioning_random_grid()
{
	unsigned int side = 90;
	std::vector<std::pair<int,int>> vertex_selection;
	std::pair<int,int> tmp;
	tmp.first = 0;
	tmp.second = 3;
	vertex_selection.push_back(tmp);
	Entiers weight_vertex;
	weight_vertex.push_back(1);
	const char *edge_weight;
	edge_weight = "../../sortie_graphe/tests_grid.txt";
	bool rec = false;

    RandomGridGraphGenerator g(side, vertex_selection,  weight_vertex, edge_weight, rec);

    test < RandomGridGraphGenerator >(duration_random_grid, "gggp_pond", 2, 100, 2,
                                  false, true, 80 , true, g);

    test < RandomGridGraphGenerator >(duration_random_grid, "gggp_pond", 2, 100, 2,
                                  false, true, 40 , true, g);


}

const double duration_random_linked = 30;

void test_flat_random_linked()
{
    boost::timer t;
    uint nbr_ite = 20;

    log_file << "== Random Linked Graph ==" << std::endl;
    log_file << "flat graph with heap = ";
    for (unsigned int i = 0; i < nbr_ite; ++i) {
        flat_heap_test< RandomLinkedFlatGraphBuilder >(duration_random_linked);
    }

    double t2 = t.elapsed();

    /*log_file << t2 / 10 << std::endl;

    log_file << "flat graph with vector = ";
    for (unsigned int i = 0; i < 10; ++i) {
        flat_vector_test< FlatGraphBuilder >(duration_random);
    }

    double t3 = t.elapsed();*/

    //log_file << (t3 - t2) / 10 << std::endl;
    log_file << t2 / nbr_ite << std::endl;
}

void test_partitioning_random_linked()
{
    unsigned int levels = 60;
    int nbr_sommets = 5000;
    RandomLinkedGraphGenerator g(nbr_sommets, levels, 2, 3);

	// gggp
    /*test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 5, true, g);*/
    test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 10, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 20, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 40, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 60, true, g);
    /*test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 80, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 100, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "gggp_pond", 2, 50, 2,
                                  false, true, 200, true, g);*/

    // ggp
    /*test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 5, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 10, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 20, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 40, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 60, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 80, true, g);
    test < RandomLinkedGraphGenerator >(duration_random_linked, "ggp", 2, 50, 2,
                                  false, true, 100, true, g);*/

}

/* corsen graph */

const double corsen_duration = 20;

void test_flat_corsen()
{
    boost::timer t;

    log_file << "== CORSEN == " << std::endl;
    log_file << "flat graph with heap = ";
    for (unsigned int i = 0; i < 20; ++i) {
        flat_heap_test< CorsenFlatGraphBuilder >(corsen_duration);
    }

    double t2 = t.elapsed();

    log_file << t2 / 20 << std::endl;

    log_file << "flat graph with vector = ";
    for (unsigned int i = 0; i < 20; ++i) {
        // flat_vector_test< CorsenFlatGraphBuilder >(corsen_duration);
    }

    double t3 = t.elapsed();

    log_file << (t3 - t2) / 20 << std::endl;
}

void test_partitioning_corsen()
{
    CorsenGraphGenerator g;

    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 10, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 20, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 40, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 80, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 22, 2,
                                  false, true, 200, true, g);

    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 30, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 40, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 50, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 60, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 70, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 80, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 90, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "gggp_pond", 2, 32, 2,
                                  false, true, 100, false, g);

    /*test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 10, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 20, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 40, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 80, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 22, 2,
                                  false, true, 200, true, g);

    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 30, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 40, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 50, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 60, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 70, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 80, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 90, false, g);
    test < CorsenGraphGenerator >(corsen_duration, "ggp", 2, 32, 2,
                                  false, true, 100, false, g);

    test < CorsenGraphGenerator >(corsen_duration, "random", 2, 32, 2,
                                  false, true, 10, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "random", 2, 32, 2,
                                  false, true, 20, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "random", 2, 32, 2,
                                  false, true, 40, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "random", 2, 32, 2,
                                  false, true, 80, true, g);
    test < CorsenGraphGenerator >(corsen_duration, "random", 2, 22, 2,
                                  false, true, 200, true, g);*/
}

/* three graph types : 38 nodes, corsen and random */

void test_38()
{
    test_flat_38();
    test_partitioning_38();
}

void test_corsen()
{
    test_flat_corsen();
    test_partitioning_corsen();
}

void test_random()
{
    test_flat_random();
    test_partitioning_random();
}

void test_random_linked()
{
    test_flat_random_linked();
    test_partitioning_random_linked();
}

void test_random_grid()
{
    test_flat_random_grid();
    test_partitioning_random_grid();
}

int main()
{
    srand(7262);

	//std::cout<<"Simulation pour graphe 38"<<std::endl;
    //test_38();
    //std::cout<<std::endl;
    //std::cout<<"Simulation pour graphe RANDOM 4000"<<std::endl;
    //test_random();
    std::cout<<"Simulation pour graphe RANDOM_TREE 200"<<std::endl;
    test_random_grid();
    //std::cout<<"Simulation pour graphe RANDOM_LINKED 6000"<<std::endl;
    //test_random_linked();
    //std::cout<<std::endl;
    //std::cout<<"Simulation pour graphe CORSEN"<<std::endl;
    //test_corsen();

    return 0;
}
