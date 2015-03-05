/**
 * @file tests/multithreading/lifegame/graph_builder.hpp
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

#ifndef TESTS_MULTITHREADING_LIFEGAME_GRAPH_BUILDER_HPP
#define TESTS_MULTITHREADING_LIFEGAME_GRAPH_BUILDER_HPP 1

#include <tests/boost_graph/graph_defs.hpp>
#include <tests/boost_graph/partitioning/gggp.hpp>

using namespace paradevs::tests::boost_graph;

namespace paradevs { namespace tests {
        namespace multithreading { namespace lifegame {

const int NCOLUMNS = 5;
const int NLINES = 5;

class GraphBuilder
{
public:
    GraphBuilder(int cluster_number) :
        _cluster_number(cluster_number)
    { }

    void build(OrientedGraphs& graphs,
               InputEdgeList& input_edges,
               OutputEdgeList& output_edges,
               Connections& parent_connections)
    {
        //std::vector < std::string > parameters = {
        //    "HEM", "rande", "diff", "ratio"
        //};
        std::vector < std::string > parameters = {
             "HEM", "gggp", "diff", "ratio"
        };

        bool contraction_coef_flag = false;
        uint cluster_number = _cluster_number;
        OrientedGraph go;
        Edges edge_partie;
        Connections connections;

        generate(go);
        std::cout << "Nbrs models : " << num_vertices(go) << std::endl;
        uint contraction_coef = 20;//num_vertices(go);
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
        std::vector < OrientedGraph::vertex_descriptor > vertices;
        for (int i = 0; i < NLINES; ++i) {
            for (int j = 0; j < NCOLUMNS; ++j) {
                vertices.push_back(boost::add_vertex(g));
                g[vertices.back()] = VertexProperties(i * NCOLUMNS + j, 1., 0);
            }
        }

        for (int i = 0; i < NLINES; ++i) {
            for (int j = 0; j < NCOLUMNS; ++j) {
                int index = i * NCOLUMNS + j;

                if (i > 0) {
                    boost::add_edge(vertices[(i - 1) * NCOLUMNS + j],
                                    vertices[index], 1., g);
                }
                if (i < NLINES - 1) {
                    boost::add_edge(vertices[(i + 1) * NCOLUMNS + j],
                                    vertices[index], 1., g);
                }
                if (j > 0) {
                    boost::add_edge(vertices[i * NCOLUMNS + j - 1],
                                    vertices[index], 1., g);
                }
                if (j < NCOLUMNS - 1) {
                    boost::add_edge(vertices[i * NCOLUMNS + j + 1],
                                    vertices[index], 1., g);
                }
            }
        }

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
};

} } } } // namespace paradevs tests multithreading lifegame

#endif
