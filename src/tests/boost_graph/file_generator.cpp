/**
 * @file tests/boost_graph/file_generator.cpp
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

#include <boost/format.hpp>
#include <fstream>

#include <tests/boost_graph/graph_generator.hpp>
#include <tests/boost_graph/graph_partitioning.hpp>

using namespace paradevs::tests::boost_graph;

int main()
{
    std::vector < int > levels = {/*5,4,2,*/ 2  };
    int nbr_sommets = 10;
    int sources = nbr_sommets/100*1;

    RandomGraphGenerator generator(nbr_sommets, levels, sources, 2, 3);

    PartitioningGraphBuilder graph_builder(2,
                                           "gggp_pond",
                                           1,
                                           true,
                                           generator);
    OrientedGraphs graphs;
    InputEdgeList  input_edges;
    OutputEdgeList output_edges;
    Connections    parent_connections;

    graph_builder.build(graphs, input_edges, output_edges,
                        parent_connections);

    for (unsigned int i = 0; i < graphs.size(); ++i) {
        OrientedGraph& g = graphs[i];
        Edges& inputs = input_edges[i];
        Edges& outputs = output_edges[i];
        std::map < int, int > indexes;
        std::ofstream f((boost::format("S%1%.tfg") % i).str());

        OrientedGraph::vertex_iterator vertexIt, vertexEnd;

        int k = 1;

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt) {
            indexes[g[*vertexIt]._index] = k;
            switch (g[*vertexIt]._type) {
            case TOP_PIXEL:
                f << "top " << std::endl;
                break;
            case NORMAL_PIXEL:
                f << "normal " << std::endl;
                break;
            };
            ++k;
        }

        f << "#" << std::endl;

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt)
        {
            OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

            boost::tie(neighbourIt, neighbourEnd) =
                boost::adjacent_vertices(*vertexIt, g);
            for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                int a = *vertexIt + 1;
                int b = *neighbourIt + 1;

                f << a << " " << b << " 0 0" << std::endl;
            }
        }

        // input
        for (Edges::const_iterator it = inputs.begin(); it != inputs.end(); ++it) {
            f << "0 " << indexes[it->second] << " " << it->first << " 0" << std::endl;
        }

        // output
        std::vector < int > outs;

        for (Edges::const_iterator it = outputs.begin(); it != outputs.end(); ++it) {
            if (std::find(outs.begin(), outs.end(), it->first) == outs.end()) {
                f << indexes[it->first] << " 0 0 " << it->first << std::endl;
                outs.push_back(it->first);
            }
        }
        f.close();
    }

    std::ofstream f("ROOT.tfg");

    for (unsigned int i = 0; i < graphs.size(); ++i) {
        f << "coupled" << std::endl;
    }
    f << "#" << std::endl;

    std::vector < std::pair < int, int > > outs;

    for (Connections::const_iterator it = parent_connections.begin();
         it != parent_connections.end(); ++it) {
        const Connection& connection = *it;

        if (std::find(outs.begin(), outs.end(), connection.first) == outs.end()) {
            f << connection.first.first << " "
              << connection.second.first << " "
              << connection.first.second << " "
              << connection.first.second << std::endl;
            outs.push_back(connection.first);
        }
    }

    f.close();

    return 0;
}
