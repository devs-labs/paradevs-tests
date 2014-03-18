/**
 * @file tests/boost_graph/partitioning/graph_build.hpp
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

#ifndef TESTS_BOOST_GRAPH_PARTITIONING_GRAPH_BUILD_H
#define TESTS_BOOST_GRAPH_PARTITIONING_GRAPH_BUILD_H

#include <tests/boost_graph/partitioning/defs.hpp>
#include <tests/boost_graph/partitioning/utils.hpp>
namespace paradevs { namespace tests { namespace boost_graph {

void build_graph(OrientedGraph& og, unsigned int edge_number);
void build_generator_graph(OrientedGraph *go, int nbr_vertex, int nbr_source, int nbr_v_min, int nbr_v_max,const std::vector<int> &niveau);
void build_generator_graph_linked(OrientedGraph *go, int nbr_vertex, int nbr_couche, int nbr_v_min, int nbr_v_max);
void build_graph_cyclique(OrientedGraph& og);
//void build_corsen_graph(OrientedGraph& graph);

} } } // namespace paradevs tests boost_graph;

#endif
