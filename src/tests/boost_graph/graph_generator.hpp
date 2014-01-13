/**
 * @file tests/boost_graph/graph_partitioning.hpp
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

#ifndef __TESTS_BOOST_GRAPH_GRAPH_GENERATOR_HPP
#define __TESTS_BOOST_GRAPH_GRAPH_GENERATOR_HPP 1

#include <tests/boost_graph/graph_defs.hpp>
#include <tests/boost_graph/partitioning/defs.hpp>
#include <tests/boost_graph/partitioning/utils.hpp>
#include <tests/boost_graph/partitioning/graph_build.hpp>

namespace paradevs { namespace tests { namespace boost_graph {

class GraphGenerator
{
public:
    GraphGenerator()
    { }

    virtual void generate(OrientedGraph& go) = 0;
};

class RandomGraphGenerator : public GraphGenerator
{
public:
    RandomGraphGenerator(unsigned int edge_number,
                         std::vector<int> levels,
                         unsigned int source_number,
                         unsigned int min_neigh,
                         unsigned int max_neigh) :
        edge_number(edge_number), levels(levels), source_number(source_number),
        min_neigh(min_neigh), max_neigh(max_neigh)
    { }

    virtual void generate(OrientedGraph& go)
    {
        build_generator_graph(&go, edge_number, source_number, min_neigh,
                              max_neigh, levels);
    }

private:
    unsigned int edge_number;
    std::vector<int> levels;
    unsigned int source_number;
    unsigned int min_neigh;
    unsigned int max_neigh;
};

class ArtificialGraphGenerator : public GraphGenerator
{
public:
    ArtificialGraphGenerator(unsigned int edge_number) :
    edge_number(edge_number)
    { }

    virtual void generate(OrientedGraph& go)
    { build_graph(go, edge_number); }

private:
    unsigned int edge_number;
};

class CorsenGraphGenerator : public GraphGenerator
{
public:
    CorsenGraphGenerator()
    { }

    virtual void generate(OrientedGraph& go)
    {
        //build_corsen_graph(go);
    }
};

} } } // namespace paradevs tests boost_graph

#endif
