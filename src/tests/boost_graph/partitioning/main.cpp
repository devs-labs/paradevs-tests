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

    OrientedGraph *go = new OrientedGraph();
    std::string type_cut = "ratio";
    
    int nbr_sommets = 1000;
    int nbr_sources = nbr_sommets/100*1; // 1% des sommets sont des sources
    Entiers niveau = {4,3,2};
    //int nbr_couches = 150;//nbr_sommets*3/100;
    
    //build_graph(*go, 38);
    build_generator_graph(go, nbr_sommets, nbr_sources , 2 , 4 ,niveau);
    /*std::vector<std::pair<int,int>> vertex_selection;
    std::pair<int,int> tmp;
    tmp.first = 0;
	tmp.second = 3;
    vertex_selection.push_back(tmp);
    Entiers weight_vertex;
    weight_vertex.push_back(1);
    const char *texte = new const char();
    texte = "../../sortie_graphe/tests_grid.txt";
    build_graph_grid(go, floor(sqrt(1000)), vertex_selection, weight_vertex,texte,true);*/
    bool Spectrale = false;
    
    /*EntiersEntiers toto;
    Affichage_OrientedGraph(go);
    Plot_OrientedGraph_All(go,toto,"../../sortie_graphe/Tests/Graphes/Contraction/txt/graphe.txt", false);
    UnorientedGraph *g = new UnorientedGraph();
	make_unoriented_graph(*go, *g);
	UnorientedGraph *copy_graphe = new UnorientedGraph();
	boost::copy_graph(*g,*copy_graphe);
	Base_Graph baseg1, baseg2;
	ListEntiersEntiers liste_corr1, liste_corr2;
	int val_cpt = num_vertices(*g);
	int val_cpt2 = num_vertices(*g);
	baseg1.push_back(g);
	baseg2.push_back(copy_graphe);*/
	
	/*std::vector<const char* > nom;
	const char * tmp_nom1 = new const char();
	tmp_nom1 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_0.txt";
	nom.push_back(tmp_nom1);
	const char * tmp_nom2 = new const char();
	tmp_nom2 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_1.txt";
	nom.push_back(tmp_nom2);
	const char * tmp_nom3 = new const char();
	tmp_nom3 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_2.txt";
	nom.push_back(tmp_nom3);
	const char * tmp_nom4 = new const char();
	tmp_nom4 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_3.txt";
	nom.push_back(tmp_nom4);
	const char * tmp_nom5 = new const char();
	tmp_nom5= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_4.txt";
	nom.push_back(tmp_nom5);
	const char * tmp_nom6 = new const char();
	tmp_nom6= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_5.txt";
	nom.push_back(tmp_nom6);
	const char * tmp_nom7 = new const char();
	tmp_nom7= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_6.txt";
	nom.push_back(tmp_nom7);
	const char * tmp_nom8 = new const char();
	tmp_nom8= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_7.txt";
	nom.push_back(tmp_nom8);
	const char * tmp_nom9 = new const char();
	tmp_nom9= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_8.txt";
	nom.push_back(tmp_nom9);
	const char * tmp_nom10 = new const char();
	tmp_nom10= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_9.txt";
	nom.push_back(tmp_nom10);
	
	std::vector<const char* > noma;
	const char * tmp_noma1 = new const char();
	tmp_noma1 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_0n.txt";
	noma.push_back(tmp_noma1);
	const char * tmp_noma2 = new const char();
	tmp_noma2 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_1n.txt";
	noma.push_back(tmp_noma2);
	const char * tmp_noma3 = new const char();
	tmp_noma3 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_2n.txt";
	noma.push_back(tmp_noma3);
	const char * tmp_noma4 = new const char();
	tmp_noma4 = "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_3n.txt";
	noma.push_back(tmp_noma4);
	const char * tmp_noma5 = new const char();
	tmp_noma5= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_4n.txt";
	noma.push_back(tmp_noma5);
	const char * tmp_noma6 = new const char();
	tmp_noma6= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_5n.txt";
	noma.push_back(tmp_noma6);
	const char * tmp_noma7 = new const char();
	tmp_noma7= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_6n.txt";
	noma.push_back(tmp_noma7);
	const char * tmp_noma8 = new const char();
	tmp_noma8= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_7n.txt";
	noma.push_back(tmp_noma8);
	const char * tmp_noma9 = new const char();
	tmp_noma9= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_8n.txt";
	noma.push_back(tmp_noma9);
	const char * tmp_noma10 = new const char();
	tmp_noma10= "../../sortie_graphe/Tests/Graphes/Contraction/txt/niveau_9n.txt";
	noma.push_back(tmp_noma10);
	
	std::cout<<"Contraction "<<std::endl;
    double t1 = t.elapsed();
    std::cout<<"temps construction : "<<t1<<std::endl;
    
	bool truc1 = false;
	int cpt = 0;
    while(truc1 != true)
    {
    	truc1 = contraction_HEM(baseg2.at(cpt), baseg2, liste_corr2, 100, val_cpt2);
        cpt++;
        std::cout<<num_vertices(*baseg2.at(cpt))<<std::endl;
        //Plot_UnorientedGraph_All(baseg2.at(cpt),toto,noma.at(cpt), true);
        double poids_total = Total_weight_edges(baseg2.at(cpt));
        std::cout<<"Contraction "<<cpt<<"  Poids Total : "<<poids_total<<std::endl;
    }
   
    double t2 = t.elapsed();
    std::cout<<"temps HEM : "<<t2-t1<<std::endl;
    
    bool truc2 = false;
	cpt = 0;
	
	while(truc2 != true)
    {
    	truc2 = contraction_HEM_degree(baseg1.at(cpt), baseg1, liste_corr1, 100, val_cpt);
        cpt++;
        std::cout<<num_vertices(*baseg1.at(cpt))<<std::endl;
        //Plot_UnorientedGraph_All(baseg1.at(cpt),toto,nom.at(cpt), true);
        double poids_total = Total_weight_edges(baseg1.at(cpt));
        std::cout<<"Contraction "<<cpt<<"  Poids Total : "<<poids_total<<std::endl;
    }
    
    double t3 = t.elapsed();
    std::cout<<"temps HEM tests: "<<t3-t2<<std::endl; 
    
    int size = baseg1.size();
    std::cout<<"taille de la base de graphe : "<<size<<std::endl;
    
    for(Base_Graph::iterator it = baseg1.begin(); it != baseg1.end(); it++)
    {
        delete *it;
        *it = NULL;
    }
    
    for(Base_Graph::iterator it2 = baseg2.begin(); it2 != baseg2.end(); it2++)
    {
        delete *it2;
        *it2 = NULL;
    } 	
    
    //build_generator_graph_linked(go, nbr_sommets, nbr_couches , 2, 3);
    //build_graph_cyclique(*go);
    //const char *texte = new const char();
    //texte = "../../sortie_graphe/enregistrement_100.txt";
    //Text_generator_graph(texte,go);
    //Graph_constructor_txt(texte, go);
    
   	/*bool found;
	edge_to e1;
    
    tie(vertexIto, vertexEndo) = vertices(*go);
    for (; vertexIto != vertexEndo; ++vertexIto) {
    	std::cout<<(*go)[*vertexIto]._index<<" -> ";
    	tie(neighbourIto, neighbourEndo) = adjacent_vertices(*vertexIto,
    			*go);
    	for (; neighbourIto != neighbourEndo; ++neighbourIto){
			tie(e1,found)=edge(vertex(*vertexIto,*go),vertex(*neighbourIto,*go),*go);
    		std::cout<<(*go)[*neighbourIto]._index<<" p = "<<(*go)[e1]._weight<<std::endl;	
    	}
    }
	std::cout<<std::endl;*/
	
	/*** Comparaison des méthodes par étude du ratio de coupe ***/
	/*if(Spectrale == true){
	UnorientedGraph *g = new UnorientedGraph();
	make_unoriented_graph(*go, *g);
	EntiersEntiers Partition2;
	
	Affichage_UnorientedGraph(g);
	
	Partition2 = Spectral_Partition("../../Classif_R/Graphe/txt/Partition2.txt");
	Plot_OrientedGraph(go,Partition2,"../../Classif_R/Graphe/txt/Spectral_Partition.txt", true);
	Plot_OrientedGraph(go,Partition2,"../../Classif_R/Graphe/txt/toto.txt", false);*/
	
	/*std::cout<<"Partition2 : "<<std::endl;
	for(int x =0; x<Partition2.size(); x++){
		std::cout<<"*"<<x<<"* ";
		for(int y =0; y<Partition2.at(x)->size();y++){
			std::cout<<Partition2.at(x)->at(y)<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;*/
	
	/*std::string type_cut = "ratio";
    double best_cut = Cut_cluster(Partition2,*g,type_cut);
    std::cout<<"Ratio cut Spectrale : "<<best_cut<<std::endl;
    std::cout<<std::endl;
	}*/
	/*** ***/

	int nbr_parties = 4;
	int niveau_contraction = num_vertices(*go)/10;
	int nbr_tirage = 2;
	
	std::vector<std::string> parameters;
	parameters.push_back("HEM");
	parameters.push_back("rand");
	parameters.push_back("diff");
	parameters.push_back(type_cut);
	
	for(uint i =1; i<nbr_tirage; i++){
		Edges edge_partie;
		OutputEdgeList outputedgeslist(nbr_parties);
		InputEdgeList inputedgelist;
		Connections connections;
		if(Spectrale == false){
			UnorientedGraph *g = new UnorientedGraph();
			make_unoriented_graph(*go, *g);
			Adjacent_Matrix_Txt(g,"../../Classif_R/Graphe/txt/Madj.txt");
			Weight_Matrix_Txt(g,"../../Classif_R/Graphe/txt/Mwei.txt");
			delete g;
			OrientedGraphs graphs = Multiniveau(niveau_contraction, go, nbr_parties,
										10 , parameters, edge_partie ,
										outputedgeslist, inputedgelist,
										connections,true,2);  	
										std::cout<<std::endl;
		}else
			OrientedGraphs graphs = Multiniveau(niveau_contraction, go, nbr_parties, 
										num_vertices(*go)*2/100, 
										parameters, edge_partie ,
										outputedgeslist, inputedgelist,
										connections,true,2); 
	}
	
	delete go;
	
	std::cout << "Duration : " << t.elapsed()/nbr_tirage << " seconds" << std::endl;
	return 0;
}
