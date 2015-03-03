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
#include <boost/timer.hpp>

#include <tests/boost_graph/graph_generator.hpp>
#include <tests/boost_graph/graph_partitioning.hpp>

using namespace paradevs::tests::boost_graph;

int main()
{
	boost::timer t;
	srand((unsigned)time(NULL));
	/** Grid **/
	/*int side = floor(sqrt(4900));
	std::vector<std::pair<int,int>> vertex_selection;
    std::pair<int,int> tmp;
    tmp.first = 0;
	tmp.second = 3;
    vertex_selection.push_back(tmp);
    Entiers weight_vertex;
    weight_vertex.push_back(1);
    const char *texte = "../../sortie_graphe/tests_grid.txt";
    RandomGridGraphGenerator generator(side, vertex_selection,
									weight_vertex, texte, true);*/

	/** Tree **/
	/*int nbr_sommets = 20000;
	int sources = nbr_sommets/100*1;
	Entiers levels = {5,4,3,2};
	//Text_generator_graph("file/data_base/linked/linked_10000.txt", &gtmp);
	RandomGraphGenerator generator(nbr_sommets, levels, sources, 2, 3);*/

	/** Linked **/
	//OrientedGraph gtmp;
	//int nbr_sommets = 10000;
	//int nbr_couches = 60;
	//RandomLinkedGraphGenerator generator(nbr_sommets, nbr_couches, 2, 3);
	//generator.generate(gtmp);
	//Text_generator_graph("file/data_base/linked/linked_10000.txt", &gtmp);

	//ArtificialGraphGenerator generator(38);

	ParcelGraphGenerator generator(10000, "mono");
	std::cout << "Duration : " << t.elapsed() << " seconds" << std::endl;

	OrientedGraph::vertex_iterator vertexIt, vertexEnd;

	/*UnorientedGraph gi;
	OrientedGraph go;
	generator.generate(go);
	std::ofstream flat("file/data_base/linked/flat.tgf");
	std::map < int, int > indexs;
	int k1 = 1;

	boost::tie(vertexIt, vertexEnd) = boost::vertices(go);
	for (; vertexIt != vertexEnd; ++vertexIt) {
		indexs[go[*vertexIt]._index] = k1;
		switch (go[*vertexIt]._type) {
		case TOP_PIXEL:
			flat << "top " << std::endl;
			break;
		case NORMAL_PIXEL:
			flat << "normal " << std::endl;
			break;
		};
		++k1;
	}
	boost::tie(vertexIt, vertexEnd) = boost::vertices(go);
	for (; vertexIt != vertexEnd; ++vertexIt)
	{
		OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

		boost::tie(neighbourIt, neighbourEnd) =
			boost::adjacent_vertices(*vertexIt, go);
		for (; neighbourIt != neighbourEnd; ++neighbourIt) {
			int a = *vertexIt + 1;
			int b = *neighbourIt + 1

			flat << a << " " << b << " 0 0" << std::endl;
		}
	}*/

	/**Liste des méthodes possible :
	 * gggp
	 * ggp
	 * rand
	 **/
    PartitioningGraphBuilder graph_builder(4, "gggp", 200,
                                           false, generator);

    OrientedGraphs graphs;
    InputEdgeList  input_edges;
    OutputEdgeList output_edges;
    Connections    parent_connections;

    graph_builder.build(graphs, input_edges, output_edges,
                        parent_connections);

    std::cout << "Duration : " << t.elapsed() << " seconds" << std::endl;

    /*std::vector<const char*> name;
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_0.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_1.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_2.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_3.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_4.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_5.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_6.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_7.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_8.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_9.txt");
    name.push_back("../../sortie_graphe/Tests/Graphes/Multiniveau/txt/partie_10.txt");*/

    for (unsigned int i = 0; i < graphs.size(); ++i) {
        OrientedGraph& g = graphs[i];
        Edges& inputs = input_edges[i];
        Edges& outputs = output_edges[i];
        std::map < int, int > indexes;

        //std::cout<<num_vertices(graphs[i])<<std::endl;
        //Plot_OrientedGraph(&graphs[i],name.at(i));
        std::ofstream f((boost::format("file/data_base/linked/rand/S%1%.tgf") % i).str());

       // OrientedGraph::vertex_iterator vertexIt, vertexEnd;

        int k = 1;

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt) {
            indexes[g[*vertexIt]._index] = k;
            switch (g[*vertexIt]._type) {
            case 0 /*TOP_PIXEL*/:
                f << "top " << std::endl;
                break;
            case 1 /*NORMAL_PIXEL*/:
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

    std::ofstream f("file/data_base/linked/rand/ROOT.tgf");

    for (unsigned int i = 0; i < graphs.size(); ++i) {
        f << "coupled" << std::endl;
    }
    f << "#" << std::endl;

    //std::vector < std::pair < int, int > > outs;
    std::vector<std::pair<int, int>> Outs;

    for (Connections::const_iterator it = parent_connections.begin();
         it != parent_connections.end(); ++it) {
       const Connection& connection = *it;
	   std::pair<int, int> tmp_outs;
	   tmp_outs.first = connection.first.second;
	   tmp_outs.second = connection.second.first;
       //if (std::find(outs.begin(), outs.end(), connection.) == outs.end()) {
       if(std::find(Outs.begin(), Outs.end(), tmp_outs) == Outs.end()){
	   //} else{
            f << connection.first.first << " "
              << connection.second.first << " "
              << connection.first.second << " "
              << connection.first.second << std::endl;
            //outs.push_back(connection.second.first);
            //outs.push_back(connection.second.first);
            Outs.push_back(tmp_outs);
		}
    }

    f.close();

    return 0;
}
