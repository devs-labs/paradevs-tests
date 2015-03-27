/**
 * @file tests/plot/graph_defs.hpp
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
#include <vector>

namespace paradevs { namespace tests { namespace boost_graph {

struct Point
{
    double _x;
    double _y;

    Point() : _x(0), _y(0)
    { }

    Point(double x, double y) : _x(x), _y(y)
    { }

    Point(const Point& pt) : _x(pt._x), _y(pt._y)
    { }
};

typedef std::vector < Point > Points;

struct VertexProperties
{
    int    _index;
    double _weight;
    Points _points;
    int    _neighbour_number;

    VertexProperties() : _index(0), _weight(0), _neighbour_number(0)
    { }

    VertexProperties(int index, double weight, const Points& points,
                     int neighbour_number) :
        _index(index), _weight(weight), _points(points),
        _neighbour_number(neighbour_number)
    { }

    VertexProperties(const VertexProperties& vp) :
        _index(vp._index), _weight(vp._weight), _points(vp._points),
        _neighbour_number(vp._neighbour_number)
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

typedef boost::adjacency_list < boost::vecS,
                                boost::vecS,
                                boost::bidirectionalS,
                                VertexProperties,
                                EdgeProperties > OrientedGraph;

typedef boost::adjacency_list < boost::vecS,
                                boost::vecS,
                                boost::undirectedS,
                                VertexProperties,
                                EdgeProperties > UnorientedGraph;

} } } // namespace paradevs tests boost_graph

#endif
