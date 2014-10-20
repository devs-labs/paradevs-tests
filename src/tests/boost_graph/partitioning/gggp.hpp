/**
 * @file tests/boost_graph/partitioning/gggp.hpp
 * @author The PARADEVS Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * PARADEVS - the multimodeling and simulation environment
 * This file is a part of the PARADEVS environment
 *
 * Copyright (C) 2013 ULCO http://www.univ-litoral.fr
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

#ifndef TESTS_BOOST_GRAPH_PARTITIONING_GGGP_H
#define TESTS_BOOST_GRAPH_PARTITIONING_GGGP_H 1

#include <tests/boost_graph/partitioning/utils.hpp>
#include <boost/graph/copy.hpp>

namespace paradevs { namespace tests { namespace boost_graph {

void ggp(UnorientedGraph *g, Entiers *sommetsSource,
         Entiers *sommetsDestination, EntiersEntiers &Partition, int rand , int &index_partition, int distance);

void gggp_pond(UnorientedGraph *g, Entiers *sommetsSource,
               Entiers *sommetsDestination, EntiersEntiers &Partition, int rand, int &index_partition, const std::string &name , int distance);

void Iter_2l(EntiersEntiers &part, int nbr_parties, UnorientedGraph *g,
             const std::string &nom_cut, int nbr_tirage, const std::string &nom_strat, int distance = -1);

void bissectionRec(UnorientedGraph *g, EntiersEntiers &Partition,
                   int nbr_parties, const std::string &nom_cut, int nbr_tirage, const std::string &nom_strat, int distance = -1);

void Pseudo_random_partitioning(UnorientedGraph *g, EntiersEntiers &Partition,
                                uint nbr_parties);

EntiersEntiers Random_partitioning(UnorientedGraph *g,
                                uint parts_number);

OrientedGraphs Multiniveau(uint niveau_contraction,
                           OrientedGraph *go, int nbr_parties, int nbr_tirage,
                           const std::vector<std::string> &parameters, 
                           /*std::string contraction,
                           std::string type_methode,
                           std::string choix_affinage,
                           std::string type_cut,*/
                           Edges &edge_partie,
                           OutputEdgeList &outputedgeslist,
                           InputEdgeList &inputedgelist,
                           Connections &connections,
                           bool rec,
                           int distance = -1);
void Optimisation_method_neighbour(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition, int nbr_tirage, const std::string &name_cut, const std::string &name_strat);
void Optimisation_method_neighbour_distance(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition, int nbr_tirage, int distance, const std::string &name_cut, 
	const std::string &name_strat);
void Optimisation_method_neighbour_degree(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition,const std::string &name_cut, const std::string &name_strat);
void Optimisation_method_neighbour_minweight(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition,
	const std::string &name_cut, const std::string &name_strat);
void tirage_distance(UnorientedGraph *g, int tirage, std::list<int> &vertex_list, int distance);

} } } // namespace paradevs tests boost_graph

#endif
