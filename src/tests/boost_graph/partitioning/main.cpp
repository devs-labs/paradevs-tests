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

	/*std::vector<std::pair < double, int >> tab_diff;
	
	for(int i =0; i<5; i++){
		std::pair<double, int> diff(rand_fini(0,15)+0.426,i);
		tab_diff.push_back(diff);
		std::cout<<tab_diff.at(i).first<<" "<<tab_diff.at(i).second<<std::endl;
	}
	std::cout<<std::endl;
	sort(tab_diff.begin(),tab_diff.end());
	for(int i =0; i<5; i++){
		std::cout<<tab_diff.at(i).first<<" "<<tab_diff.at(i).second<<std::endl;
	}
	
	std::vector<int> Diff;
	for(int i =0; i<tab_diff.size(); i++){
		Diff.push_back(tab_diff.at(i).second);
	}
	
	for(int i =0; i<tab_diff.size(); i++){
		std::cout<<Diff.at(i)<<std::endl;
	}*/
	
    OrientedGraph* go = new OrientedGraph();
    /*UnorientedGraph graph_origin;
    build_graph(*go, 11);
    
    UnorientedGraph *g = new UnorientedGraph();
	make_unoriented_graph(*go, *g);
	
    EntiersEntiers Partition;
    Entiers *part1 = new Entiers();
    Entiers *part2 = new Entiers();
    for(uint i = 0; i<11; i ++){
		part1->push_back(i);
	}
    Partition.push_back(part1);
    
	int node = 1;
	int index = 0;
    gggp_pond(g, part1, part2, Partition, node, index, "cut", 2);
    Partition.push_back(part2);
    
    double cut = Cut_cluster(Partition, *g, "cut");
    
    for(int x =0; x<Partition.size(); x++){
		std::cout<<"** ";
		for(int y =0; y<Partition.at(x)->size();y++){
			std::cout<<Partition.at(x)->at(y)<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
	
	std::cout<<"cut : "<<cut<<std::endl;
	
	for(EntiersEntiers::iterator it = Partition.begin(); it != Partition.end(); it++)
    {
        delete *it;
        *it = NULL;
    }
    
    EntiersEntiers Partition2;
	Entiers *part3 = new Entiers();
    Entiers *part4 = new Entiers();
    
    for(uint i = 0; i<11; i ++){
		part3->push_back(i);
	}
    Partition2.push_back(part3);

    gggp_pond(g, part3, part4, Partition2, node, index, "ratio", 2);
    Partition2.push_back(part4);
    
	cut = Cut_cluster(Partition2, *g, "ratio");
    
    for(int x =0; x<Partition2.size(); x++){
		std::cout<<"** ";
		for(int y =0; y<Partition2.at(x)->size();y++){
			std::cout<<Partition2.at(x)->at(y)<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
	
	std::cout<<"cut : "<<cut<<std::endl;
	
	for(EntiersEntiers::iterator it = Partition2.begin(); it != Partition2.end(); it++)
    {
        delete *it;
        *it = NULL;
    }*/

    std::vector<std::string> color;
    color.push_back("node [color=lightblue2, style=filled];");
    color.push_back("node [color=red, style=filled];");
    color.push_back("node [color=limegreen, style=filled];");
    color.push_back("node [color=plum, style=filled];");
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
    
    
    int nbr_sommets = 10000;
    int nbr_sources = nbr_sommets/100*1; // 1% des sommets sont des sources
    Entiers niveau = {5,4,3,2};
    
    //build_graph(*go, 38);
    //build_generator_graph(go, nbr_sommets, nbr_sources , 2 , 3 ,niveau);
    build_generator_graph_linked(go, nbr_sommets, 100 , 2, 3);
    //build_graph_cyclique(*go);
    //const char *texte = new const char();
    //texte = "../../sortie_graphe/enregistrement_100.txt";
    //Text_generator_graph(texte,go);
    //Graph_constructor_txt(texte, go);
    
    std::ofstream fichier ("../../sortie_graphe/Tests/Graphes/txt/graph_linked_200_30.txt", std::ios::out);
    fichier<<"digraph G {"<<std::endl;
    tie(vertexIto, vertexEndo) = vertices(*go);
    for (; vertexIto != vertexEndo; ++vertexIto) {
    	fichier<<(*go)[*vertexIto]._index<<"-> {";
    	//std::cout<<(*go)[*vertexIto]._index<<" -> ";
    	tie(neighbourIto, neighbourEndo) = adjacent_vertices(*vertexIto,
    			*go);
    	for (; neighbourIto != neighbourEndo; ++neighbourIto){
    		fichier<<(*go)[*neighbourIto]._index<<";";
    		//std::cout<<(*go)[*neighbourIto]._index<<" ";
    	}
    	fichier<<"}"<<std::endl;
    	//std::cout<<std::endl;
    }
	fichier<<"}";
	fichier.close();
	//std::cout<<std::endl;
	

	int nbr_parties = 50;
	int niveau_contraction = num_vertices(*go)/50;
	std::vector<double> Cut;
	int nbr_tirage = 20;
	
	for(uint i =0; i<nbr_tirage; i++){
	Edges edge_partie;
	OutputEdgeList outputedgeslist(nbr_parties);
	InputEdgeList inputedgelist;
	Connections connections;
	UnorientedGraph *g = new UnorientedGraph();
	UnorientedGraph graph_origin;
	make_unoriented_graph(*go, *g);
	boost::copy_graph(*g, graph_origin);

	OrientedGraphs graphs = Multiniveau(niveau_contraction, g, &graph_origin, go, nbr_parties, 10,"HEM", "gggp",
									"diff", "ratio", edge_partie ,
									outputedgeslist, inputedgelist,
									connections,false,Cut,2);  	
									//std::cout<<std::endl;
									//std::cout<<std::endl;				
	}
	
	//std::ofstream fichier_cut ("../../sortie_graphe/Tests/Cut/cut_s200_nc10_d0_c1_t10_p8_gggp.txt", std::ios::out);
	double moy = 0.;
	
	for(int cpt = 0; cpt <Cut.size(); cpt ++){
		//if(cpt!=Cut.size()-1)
			//fichier_cut<<Cut.at(cpt)<<";";
		//else
			//fichier_cut<<Cut.at(cpt);
		moy += Cut.at(cpt);
		//std::cout<<Cut.at(cpt)<<std::endl;
	}
	//fichier_cut.close();
	
	moy/= Cut.size();
	std::cout<<moy<<std::endl;
	
    /*int nbr_parties = 20;
    int nbr_sommets = 200;
    int nbr_sources = nbr_sommets/100*1; // 1% des sommets sont des sources
    const char *texte = new const char();
    texte = "../../sortie_graphe/enregistrement_200.txt";
    


    //build_graph(*go, 38);
    Entiers niveau = {2,2};
    
    //build_generator_graph(go, nbr_sommets, nbr_sources , 2 , 3 ,niveau);
    

    //Text_generator_graph(texte,go);
    Graph_constructor_txt(texte, go);
	
    //make_unoriented_graph(*go, *g);
    
    std::ofstream fichier ("../../sortie_graphe/Tests/arbre_s200_n22_tests.txt", std::ios::out);
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
	int niveau_contraction = num_vertices(*go)/1;
	
	for(uint i =0; i<500; i++){
		Edges edge_partie;
		OutputEdgeList outputedgeslist(nbr_parties);
		InputEdgeList inputedgelist;
		Connections connections;
		UnorientedGraph *g = new UnorientedGraph();
		make_unoriented_graph(*go, *g);
		boost::copy_graph(*g, graph_origin);
    
		OrientedGraphs graphs = Multiniveau(niveau_contraction, g, &graph_origin, go, nbr_parties, 20,"HEM", "ggp",
                                        "cut", "ratio", edge_partie ,
                                        outputedgeslist, inputedgelist,
                                        connections,Cut,4);  
                                        
	}
	
	std::ofstream fichier_cut ("../../sortie_graphe/Tests/Cut/cut_s200_n22_d4-20_c1_t500_p20_ggp.txt", std::ios::out);
	double moy = 0.;
	
	for(int cpt = 0; cpt <Cut.size(); cpt ++){
		if(cpt!=Cut.size()-1)
			fichier_cut<<Cut.at(cpt)<<";";
		else
			fichier_cut<<Cut.at(cpt);
		moy += Cut.at(cpt);
		//std::cout<<Cut.at(cpt)<<std::endl;
	}
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
	std::cout << std::endl;*/
	
	     
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
	


	delete go;*/
	
	std::cout << "Duration : " << t.elapsed()/nbr_tirage << " seconds" << std::endl;
	return 0;
}
