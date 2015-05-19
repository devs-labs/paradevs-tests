/**
 * @file tests/plot/graph_builder.hpp
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

#ifndef TESTS_PLOT_GRAPH_BUILDER_HPP
#define TESTS_PLOT_GRAPH_BUILDER_HPP 1

#include <tests/boost_graph/partitioning/gggp.hpp>

#include <shapefil.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/simplify/DouglasPeuckerSimplifier.h>
#include <geos/algorithm/CentroidArea.h>
#include <geos/triangulate/DelaunayTriangulationBuilder.h>
#include <geos/triangulate/quadedge/QuadEdgeSubdivision.h>

#include <map>

using namespace geos;
using namespace geos::geom;
using namespace geos::simplify;
using namespace paradevs::tests::boost_graph;

#define DISTANCE 1000.
#define SCALE 4.
#define DELTA 20 // 2.5

namespace paradevs { namespace tests { namespace plot {

class GraphBuilder
{
public:
    GraphBuilder(const std::string& file_name, int cluster_number) :
        _cluster_number(cluster_number), _file_name(file_name)
    { }

    void build(OrientedGraphs& graphs,
               InputEdgeList& input_edges,
               OutputEdgeList& output_edges,
               Connections& parent_connections)
    {
        std::vector < std::string > parameters = {
             "HEM", "rande", "diff", "ratio"
        };

        bool contraction_coef_flag = false;
        uint cluster_number = _cluster_number;
        OrientedGraph go;
        Edges edge_partie;
        Connections connections;

        generate(go);
        uint contraction_coef = 200;//num_vertices(go);
        output_edges = OutputEdgeList(cluster_number);
        if (contraction_coef_flag) {
            uint coars = num_vertices(go) / contraction_coef;
            std::vector < uint > numeric_parameters = { coars,
                                                        cluster_number,
                                                        10 };

            graphs = Multiniveau(&go, numeric_parameters,
                                 parameters, edge_partie,
                                 output_edges, input_edges,
                                 parent_connections,
                                 false, 2);
        } else {
            std::vector < uint > numeric_parameters = { contraction_coef,
                                                        cluster_number,
                                                        10 };

            graphs = Multiniveau(&go, numeric_parameters,
                                 parameters, edge_partie ,
                                 output_edges, input_edges,
                                 parent_connections, false, 2);
        }
    }

private:
    void build_neighbourhood(
        OrientedGraph& g,
        std::map < int, OrientedGraph::vertex_descriptor >& vertices,
        const std::map < int, std::pair < Polygon*, Points > >& polygons,
        const std::map < int, bool >& select)
    {
        // build vertices
        std::map < int, std::pair < Polygon*, Points > >::const_iterator it =
            polygons.begin();

        while (it != polygons.end()) {
            vertices[it->first] = boost::add_vertex(g);
            if (select.find(it->first)->second) {
                g[vertices[it->first]] = VertexProperties(it->first, 1.,
                                                      it->second.second, 0);
            } else {
                g[vertices[it->first]] = VertexProperties(it->first, 1.,
                                                          Points(), 0);
            }
            ++it;
        }

        // build edges
        it = polygons.begin();
        while (it != polygons.end()) {
            if (select.find(it->first)->second) {
                std::map < int,
                           std::pair < Polygon*,
                                       Points > >::const_iterator it2 =
                    polygons.begin();

                while (it2 != polygons.end()) {
                    if (select.find(it2->first)->second) {
                        if (it->first != it2->first) {
                            if (it->second.first->distance(
                                    it2->second.first) < DISTANCE) {
                                boost::add_edge(vertices[it->first],
                                                vertices[it2->first], 1., g);
                                g[vertices[it2->first]]._neighbour_number++;
                            }
                        }
                    }
                    ++it2;
                }
            }
            ++it;
        }

            // compute neighbour
        // int buffer = 10;
        // std::map < int, Polygon* >::const_iterator it = polygons.begin();

        // while (it != polygons.end()) {
        //     Geometry* bufferedPolygon = it->second->buffer(buffer);
        //     std::map < int, Polygon* >::const_iterator it2 = polygons.begin();

        //     while (it2 != polygons.end()) {
        //         if (it->first != it2->first) {
        //             if (bufferedPolygon->intersects(it2->second)) {
        //                 boost::add_edge(vertices[it->first],
        //                                 vertices[it2->first], 1., g);

        //                 g[vertices[it2->first]]._neighbour_centroids.push_back(
        //                     paradevs::tests::boost_graph::Point(
        //                         g[vertices[it->first]]._centroid));
        //             }
        //         }
        //         ++it2;
        //     }
        //     delete bufferedPolygon;
        //     ++it;
        // }

    }

    void generate(OrientedGraph& g)
    {
        SHPHandle hSHP;

        hSHP = SHPOpen(_file_name.c_str(), "rb" );

        int nShapeType, nEntities;
        double adfMinBound[4], adfMaxBound[4];

        SHPGetInfo(hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

        PrecisionModel precisionModel(100., 0, 0);
        GeometryFactory geomFactory(&precisionModel);
        const CoordinateSequenceFactory* seqFactory =
            CoordinateArraySequenceFactory::instance();
        std::vector < Geometry* > holes;
        std::map < int, std::pair < Polygon*, Points > > polygons;
        std::map < int, OrientedGraph::vertex_descriptor > vertices;

        // read shapefile file and build polygons
        for (int i = 0; i < nEntities; i++) {
            SHPObject	*psShape;

            psShape = SHPReadObject(hSHP, i);

            int n = psShape->nVertices;

            if (psShape->padfX[0] != psShape->padfX[psShape->nVertices - 1] or
                psShape->padfY[0] != psShape->padfY[psShape->nVertices - 1])
                ++n;

            CoordinateSequence* seq = seqFactory->create(n, 2);
            Coordinate first;

            for (int j = 0; j < n; j++) {
                Coordinate pt(psShape->padfX[j],
                              psShape->padfY[j]);

                if (j == 0) first = pt;
                seq->setAt(pt, j);
            }
            if (first != seq->back()) {
                seq->setAt(first, n-1);
            }

            LinearRing* ring(geomFactory.createLinearRing(seq));
            Polygon* polygon = geomFactory.createPolygon(*ring, holes);

            std::auto_ptr < Geometry > object =
                DouglasPeuckerSimplifier::simplify(polygon, 1);
            Points list;

            {
                GeometryFactory factory;

                for (double x = (*object->getEnvelope()
                                 ->getCoordinates())[0].x + DELTA / 2;
                     x < (*object->getEnvelope()->getCoordinates())[1].x;
                     x += DELTA) {
                    for (double y = (*object->getEnvelope()
                                     ->getCoordinates())[0].y + DELTA / 2;
                         y < (*object->getEnvelope()->getCoordinates())[2].y;
                         y += DELTA) {
                        Coordinate c;

                        c.x = x;
                        c.y = y;

                        geos::geom::Point* pt = factory.createPoint(c);

                        if (object->contains(pt->clone())) {
                            list.push_back(
                                paradevs::tests::boost_graph::Point(x, y));
                        }
                    }
                }
            }

            polygons[i] = std::make_pair(
                dynamic_cast < Polygon* >(object->clone()), list);

            delete ring;
            SHPDestroyObject(psShape);
        }

        std::map < int, bool > select;

        select_polygons(polygons, select);
        build_neighbourhood(g, vertices, polygons, select);

        std::map < int, std::pair < Polygon*, Points > >::const_iterator it =
            polygons.begin();

        while (it != polygons.end()) {
            delete it->second.first;
            ++it;
        }

        SHPClose(hSHP);

        // OrientedGraph::vertex_iterator it_og, end_og;

        // boost::tie(it_og, end_og) = boost::vertices(g);
        // for (; it_og != end_og; ++it_og) {
        //     OrientedGraph::adjacency_iterator neighbour_it, neighbour_end;

        //     std::cout << g[*it_og]._index << " -> { ";
        //     tie(neighbour_it, neighbour_end) =
        //         boost::adjacent_vertices(*it_og, g);
        //     for (; neighbour_it != neighbour_end; ++neighbour_it) {
        //         std::cout << g[*neighbour_it]._index << " ";
        //     }
        //     std::cout << "}" << std::endl;
        // }

    }

    void select_polygons(const std::map < int, std::pair < Polygon*, Points > >&
                         polygons, std::map < int, bool >& select)
    {
        double sum = 0;

        std::map < int, std::pair < Polygon*, Points > >::const_iterator it =
            polygons.begin();

        while (it != polygons.end()) {
            sum += it->second.first->getArea() * SCALE * SCALE / 1e4;
            select[it->first] = false;
            ++it;
        }

        std::cout << "Total area = " << sum << " Ha" << std::endl;

        int n = 0;

        sum = 0;
        while (sum < 150) {
            int index = rand() % polygons.size();

            if (not select[index] and
                polygons.find(index)->second.first->getArea() *
                SCALE * SCALE  > 10 * 1e4) {
                select[index] = true;
                sum += polygons.find(index)->second.first->getArea() *
                    SCALE * SCALE / 1e4;
                ++n;
            }
        }

        std::cout << "Select number = " << n << " for " << sum << std::endl;

    }

    int _cluster_number;
    std::string _file_name;
};

} } } // namespace paradevs tests plot

#endif
