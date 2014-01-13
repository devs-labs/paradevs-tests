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

	
    OrientedGraph* go = new OrientedGraph();
    UnorientedGraph graph_origin;
    std::vector<std::string> color;
    color.push_back("node [color=lightblue2, style=filled];");
    color.push_back("node [color=red, style=filled];");
    color.push_back("node [color=limegreen, style=filled];");
    color.push_back("node [color=orange, style=filled];");
    color.push_back("node [color=yellow2, style=filled];");
    color.push_back("node [color=slateblue1, style=filled];");
    color.push_back("node [color=plum, style=filled];");
    color.push_back("node [color=black, style=filled];");
    color.push_back("node [color=dimgrey, style=filled];");
    color.push_back("node [color=gold, style=filled];");
    color.push_back("node [color=indigo, style=filled];");
    color.push_back("node [color=turquoise, style=filled];");
    color.push_back("node [color=olive, style=filled];");
    color.push_back("node [color=cyan, style=filled];");
    color.push_back("node [color=crimson, style=filled];");
    color.push_back("node [color=lightsalmon, style=filled];");
    int nbr_parties = 4;
    


    //build_graph(*go, 38);
    Entiers niveau;
    niveau.push_back(2);
    niveau.push_back(3);   
    niveau.push_back(2);
	//niveau.push_back(2);
    //niveau.push_back(2);
    
    build_generator_graph(go, 2000, 5 , 2 , 5 ,niveau);
    //make_unoriented_graph(*go, *g);
    
    std::ofstream fichier ("../../sortie_graphe/graph_38.txt", std::ios::out);
    fichier<<"digraph G {"<<std::endl;
    tie(vertexIto, vertexEndo) = vertices(*go);
    for (; vertexIto != vertexEndo; ++vertexIto) {
    	fichier<<(*go)[*vertexIto]._index<<"-> {";
    	tie(neighbourIto, neighbourEndo) = adjacent_vertices(*vertexIto,
    			*go);
    	for (; neighbourIto != neighbourEndo; ++neighbourIto){
    		fichier<<(*go)[*neighbourIto]._index<<";";
    	}
    	fichier<<"}"<<std::endl;
    }
	fichier<<"}";
	fichier.close();
	
	std::vector<double> Cut;
	int niveau_contraction = num_vertices(*go)/10;
	
	for(uint i =0; i<100; i++){
		Edges edge_partie;
		OutputEdgeList outputedgeslist(nbr_parties);
		InputEdgeList inputedgelist;
		Connections connections;
		UnorientedGraph* g = new UnorientedGraph();
		make_unoriented_graph(*go, *g);
		boost::copy_graph(*g, graph_origin);
    
		OrientedGraphs graphs = Multiniveau(niveau_contraction, g, &graph_origin, go, nbr_parties, 8,"HEM", "gggp",
                                        "cut", "ratio", edge_partie ,
                                        outputedgeslist, inputedgelist,
                                        connections,Cut,3);  
                                        
	}
	
	std::ofstream fichier_cut ("../../sortie_graphe/graphe_cut.txt", std::ios::out);
	fichier_cut<<"x<-c(";
	double moy = 0.;
	
	for(int cpt = 0; cpt <Cut.size(); cpt ++){
		if(cpt!=Cut.size()-1)
			fichier_cut<<Cut.at(cpt)<<",";
		else
			fichier_cut<<Cut.at(cpt);
		moy += Cut.at(cpt);
		std::cout<<Cut.at(cpt)<<std::endl;
	}
	fichier_cut<<")"<<std::endl;
	fichier_cut.close();
	
	moy/= Cut.size();

	
	double etendu = *max_element(Cut.begin(),Cut.end()) - *min_element(Cut.begin(),Cut.end());
	
	std::cout<<std::endl;
	std::cout<<" *** Partitionnement en "<<nbr_parties<<" parties d'un graphe de taille "<<2000<<" *** "<<std::endl;
	std::cout<<std::endl;
	std::cout<<" === 1 tirage === "<<std::endl;
	std::cout<<"Cout de coupe min : "<<*min_element(Cut.begin(),Cut.end())<<" "<<"Cout de coupe max : "<<*max_element(Cut.begin(),Cut.end())<<std::endl;
	std::cout<<"Cout de coupe normalisé moyen : "<<moy<<std::endl;
	std::cout<<"Étendu du coût de coupe : "<<etendu<<std::endl;
    std::cout << "Duration : " << t.elapsed() << " seconds" << std::endl;
	std::cout << std::endl;
	
	     
   /*for(int i =0; i<graphs.size(); i++){
	    fichier2<<color.at(i)<<std::endl; C
		tie(vertexIto, vertexEndo) = vertices(graphs.at(i));
		for (; vertexIto != vertexEndo; ++vertexIto) {
			fichier2<<(graphs.at(i))[*vertexIto]._index<<"-> {";
			//std::cout<<(graphs.at(i))[*vertexIto]._index<<" est connecté avec : ";
			tie(neighbourIto, neighbourEndo) = adjacent_vertices(*vertexIto,
					graphs.at(i));
			for (; neighbourIto != neighbourEndo; ++neighbourIto){
				fichier2<<(graphs.at(i))[*neighbourIto]._index<<";";
				//std::cout<<(graphs.at(i))[*neighbourIto]._index<<" ";
			}
			fichier2<<"}"<<std::endl;
			//std::cout<<std::endl;
		}
		fichier2<<std::endl;
		//std::cout<<std::endl;
	}
	fichier2<<"}"<<std::endl;
	fichier2.close();
	*/


	delete go;
	
	std::cout << "Duration : " << t.elapsed()/100 << " seconds" << std::endl;
}
