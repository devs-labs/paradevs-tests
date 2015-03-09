/**
 * @file tests/boost_graph/graph_defs.hpp
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

#ifndef __TESTS_PLOT_GRAPH_DEFS_HPP
#define __TESTS_PLOT_GRAPH_DEFS_HPP 1

#include <boost/graph/adjacency_list.hpp>

namespace paradevs { namespace tests { namespace boost_graph {

struct VertexProperties
{
    int    _index;
    double _weight;
    double _x;
    double _y;

    VertexProperties() : _index(0), _weight(0), _x(0.), _y(0)
    { }

    VertexProperties(int index, double weight, double x, double y) :
        _index(index), _weight(weight), _x(x), _y(y)
    { }

    VertexProperties(const VertexProperties& vp) :
        _index(vp._index), _weight(vp._weight), _x(vp._x), _y(vp._y)
    { }
};

struct EdgeProperties
{
    double _weight;

    EdgeProperties() : _weight(0)
    { }

    EdgeProperties(double weight) : _weight(weight)
    { }
};

typedef boost::adjacency_list < boost::vecS, boost::vecS,
                                boost::bidirectionalS,
                                VertexProperties,
                                EdgeProperties > OrientedGraph;
typedef std::vector < OrientedGraph > OrientedGraphs;

typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
                                VertexProperties,
                                EdgeProperties> UnorientedGraph;

typedef std::vector < UnorientedGraph* > Base_Graph;

typedef boost::graph_traits < UnorientedGraph >::vertex_descriptor vertex_t;
typedef boost::graph_traits < UnorientedGraph >::edge_descriptor edge_t;

typedef OrientedGraph::vertex_descriptor vertex_to;
typedef OrientedGraph::edge_descriptor edge_to;

typedef std::pair < int, int > Edge;
typedef std::vector < Edge > Edges;
typedef Edges OutputEdges;
typedef Edges InputEdges;
typedef std::vector < OutputEdges > OutputEdgeList;
typedef std::vector < InputEdges > InputEdgeList;

typedef std::pair < int, int > Port;
typedef std::pair < Port, Port > Connection;
typedef std::vector < Connection > Connections;

} } } // namespace paradevs tests boost_graph

#endif
