/**
 * @file tests/boost_graph/partitioning/main.cpp
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

#include <tests/boost_graph/partitioning/gggp.hpp>
#include <tests/boost_graph/partitioning/graph_build.hpp>
#include <boost/graph/copy.hpp>
#include <boost/timer.hpp>
#include <fstream>

#include <iostream>

using namespace paradevs::tests::boost_graph;

UnorientedGraph::vertex_iterator vertexIt, vertexEnd;
UnorientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

OrientedGraph::vertex_iterator vertexIto, vertexEndo;
OrientedGraph::adjacency_iterator neighbourIto, neighbourEndo;

int main()
{
	boost::timer t;
    srand((unsigned)time(NULL));

	/*** Génération du graphe ***/

    OrientedGraph *go = new OrientedGraph();
    std::string type_graph = "parcellaire";
    std::pair<bool,bool> Spectrale = {false,false};

    if(type_graph == "grid"){
		int side = floor(sqrt(100));
		std::vector<std::pair<int,int>> vertex_selection;
	    std::pair<int,int> tmp;
	    tmp.first = 0;
		tmp.second = 3;
	    vertex_selection.push_back(tmp);
	    Entiers weight_vertex;
	    weight_vertex.push_back(1);
	    const char *texte = "../../sortie_graphe/tests_grid.txt";
	    build_graph_grid(go, side, vertex_selection, weight_vertex,texte,true);
	    Plot_OrientedGraph(go,"../../sortie_graphe/Tests/Graphes/Multiniveau/txt/grid_500.txt");
	}else if (type_graph == "tree"){
		int nbr_sommets = 70;
		int nbr_sources = 2;
		Entiers niveau = {2,2};
		const char *texte;
        //texte = "file/data_base/tree/tree_20000.txt";
		build_generator_graph(go, nbr_sommets, nbr_sources, 2,
												3, niveau);
		Plot_OrientedGraph(go,"../../sortie_graphe/Tests/Graphes/Multiniveau/txt/grid_500.txt");
		//Text_generator_graph(texte, go);
        //Graph_constructor_txt(texte,go);
	}else if (type_graph == "linked"){
		int nbr_sommets = 10;
		int nbr_couches = 4;
		build_example_linked9(*go);
		//build_generator_graph_linked(go, nbr_sommets, nbr_couches , 2, 3);
		//Text_generator_graph("file/data_base/linked/linked_50.txt",go);
		Plot_OrientedGraph(go,"../../sortie_graphe/Tests/Graphes/Multiniveau/txt/grid_500.txt");
	}else if(type_graph == "parcellaire")
	{
		build_parcellaire_graph(go, 10000, "mono");
	}
	else{
		build_example_grid(*go);
		//build_graph(*go, 11);
	}

	/*** Comparaison des méthodes par étude du ratio de coupe ***/

	if(Spectrale.first){
		UnorientedGraph *g = new UnorientedGraph();
		make_unoriented_graph(*go, *g);
		EntiersEntiers Partition2;

		Affichage_UnorientedGraph(g);

		Partition2 = Spectral_Partition("../../Classif_R/Graphe/txt/Partition2.txt");
		Plot_OrientedGraph_All(go,Partition2,"../../Classif_R/Graphe/txt/Spectral_Partition.txt", true);
		Plot_OrientedGraph(go,"../../Classif_R/Graphe/txt/toto.txt");

		delete g;
	}

	/*** Paramétrage du Multiniveau ***/
	std::vector<uint> numeric_parameters = {(uint)(num_vertices(*go)/50), 4, 10};
	std::vector<std::string> parameters = {"HEM", "gggp", "diff", "ratio"};

	uint nbr_tirage = 1;

	for(uint i = 0 ; i < nbr_tirage ; i++){
		Edges edge_partie;
		OutputEdgeList outputedgeslist(numeric_parameters.at(1));
		InputEdgeList inputedgelist;
		Connections connections;
		if(!Spectrale.first){
			if(Spectrale.second){
				UnorientedGraph *g = new UnorientedGraph();
				make_unoriented_graph(*go, *g);
				Adjacent_Matrix_Txt(g,"../../Classif_R/Graphe/txt/Madj.txt");
				Weight_Matrix_Txt(g,"../../Classif_R/Graphe/txt/Mwei.txt");
				delete g;
			}

			std::cout<<"Multiniveau"<<std::endl;
			OrientedGraphs graphs = Multiniveau(go, numeric_parameters,
												parameters, edge_partie ,
												outputedgeslist, inputedgelist,
												connections,false, 2);
												std::cout<<std::endl;
		}else{
			OrientedGraphs graphs = Multiniveau(go, numeric_parameters,
												parameters, edge_partie ,
												outputedgeslist, inputedgelist,
												connections,true, 2);
												std::cout<<std::endl;
		}
	}

	delete go;

	std::cout << "Duration : " << t.elapsed()/nbr_tirage << " seconds" << std::endl;
	return 0;
}
