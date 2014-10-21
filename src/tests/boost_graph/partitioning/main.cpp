/**
 * @file tests/boost_graph/partitioning/main.cpp
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
    std::string type_graph = "tree";
    std::pair<bool,bool> Spectrale = {false,false};
    
    if(type_graph == "grid"){
		int side = floor(sqrt(5000));
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
		int nbr_sommets = 5000;
		int nbr_sources = nbr_sommets/100*1; 
		Entiers niveau = {5,4,3,2};
		build_generator_graph(go, nbr_sommets, nbr_sources , 2 , 4 ,niveau);
	}else if (type_graph == "linked"){
		int nbr_sommets = 1000;
		int nbr_couches = 150;
		build_generator_graph_linked(go, nbr_sommets, nbr_couches , 2, 3);
	}else{
		build_graph(*go, 38);
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
	std::vector<uint> numeric_parameters = {num_vertices(*go)/20, 8, 10};
	std::vector<std::string> parameters = {"HEM", "rand", "diff", "cut"};
	
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
			OrientedGraphs graphs = Multiniveau(go, numeric_parameters,
												parameters, edge_partie ,
												outputedgeslist, inputedgelist,
												connections,true, i);  	
												std::cout<<std::endl;
		}else{
			OrientedGraphs graphs = Multiniveau(go, numeric_parameters,
												parameters, edge_partie ,
												outputedgeslist, inputedgelist,
												connections,true, i); 
												std::cout<<std::endl;
		}
	}
	
	delete go;
	
	std::cout << "Duration : " << t.elapsed()/nbr_tirage << " seconds" << std::endl;
	return 0;
}
