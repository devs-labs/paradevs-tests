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
#include <geos/geom/PrecisionModel.h>
#include <geos/simplify/DouglasPeuckerSimplifier.h>
#include <geos/algorithm/CentroidArea.h>

#include <map>

using namespace geos;
using namespace geos::geom;
using namespace geos::simplify;
using namespace paradevs::tests::boost_graph;

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
        std::map < int, Polygon* > polygons;
        std::vector < OrientedGraph::vertex_descriptor > vertices;

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

            polygons[i] = dynamic_cast < Polygon* >(object->clone());

            geos::algorithm::CentroidArea centroid;

            centroid.add(object.get());

            vertices.push_back(boost::add_vertex(g));
            g[vertices.back()] = VertexProperties(i, 1.,
                                                  centroid.getCentroid()->x,
                                                  centroid.getCentroid()->y);

            delete ring;
            SHPDestroyObject(psShape);
        }

        // compute neighbour
        int buffer = 10;
        std::map < int, Polygon* >::const_iterator it = polygons.begin();

        while (it != polygons.end()) {
            Geometry* bufferedPolygon = it->second->buffer(buffer);
            std::map < int, Polygon* >::const_iterator it2 = polygons.begin();

            while (it2 != polygons.end()) {
                if (bufferedPolygon->intersects(it2->second)) {
                    boost::add_edge(vertices[it->first],
                                    vertices[it2->first], 1., g);
                }
                ++it2;
            }
            delete bufferedPolygon;
            ++it;
        }

        it = polygons.begin();
        while (it != polygons.end()) {
            delete it->second;
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

    int _cluster_number;
    std::string _file_name;
};

} } } // namespace paradevs tests plot

#endif
