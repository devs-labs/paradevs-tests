/**
 * @file tests/boost_graph/partitioning/gggp.cpp
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
#include <boost/timer.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace paradevs { namespace tests { namespace boost_graph {

extern UnorientedGraph::vertex_iterator vertexIt, vertexEnd;
extern UnorientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

extern OrientedGraph::vertex_iterator vertexIto, vertexEndo;
extern OrientedGraph::adjacency_iterator neighbourIto, neighbourEndo;

void ggp(UnorientedGraph *g, Entiers *sommetsSource,
               Entiers *sommetsDestination, EntiersEntiers &Partition, int rand, int &index_partition, int distance)
{
    Entiers sommets_adj;
    if(sommetsSource->size()==1){
        Entiers tailles;
        for(uint i=0;i<Partition.size();i++){
            tailles.push_back(Partition.at(i)->size());
        }
        uint tmp=*max_element(tailles.begin(),tailles.end());
        for(uint i=0; i<Partition.size();i++){
            if(Partition.at(i)->size() == tmp)
            {
				sommetsSource->clear();
				for(int id = 0; id < Partition.at(i)->size();id++){
					sommetsSource->push_back(Partition.at(i)->at(id));
				}
				index_partition = i;
				if(distance != -1){
					ggp(g, sommetsSource, sommetsDestination, Partition, Partition.at(i)->at(rand_fini(0,Partition.at(i)->size())), 
						index_partition, distance);
				}else{
					ggp(g, sommetsSource, sommetsDestination, Partition, rand_fini(0,Partition.at(i)->size()),
						index_partition, distance);
				}
                return;
            }
        }
    }
    
    double poids_max=0;
    for(uint i=0;i<sommetsSource->size();i++){
        poids_max+=(*g)[sommetsSource->at(i)]._weight;
    }
    poids_max/=2.;

	double poids;
    if(distance == -1){
		poids=(*g)[sommetsSource->at(rand)]._weight;
		sommetsDestination->push_back(sommetsSource->at(rand));
		sommetsSource->erase(sommetsSource->begin() + rand);
		
	}else{
		poids=(*g)[rand]._weight;
		sommetsDestination->push_back(rand);
		suprim_val(*sommetsSource,rand);
	}
	
    int cpt = 0;
    while(poids<poids_max && sommetsSource->size()>1)
    {
        if(cpt<sommetsDestination->size()){
            adjacence_ggp(sommetsDestination->at(cpt),sommets_adj,g);
		} else{
            int val=rand_fini(0,sommetsSource->size()-1);
            sommetsDestination->push_back(sommetsSource->at(val));
            sommetsSource->erase(sommetsSource->begin() + val);
            adjacence_ggp(sommetsDestination->at(cpt),sommets_adj,g);
        }
        if(sommets_adj.size() == 0)
        {
            sort(sommetsDestination->begin(), sommetsDestination->end());
            return;
        }
        else{
            for(uint i =0; i<sommets_adj.size(); i++){
                if(In_tab(*sommetsDestination,sommets_adj.at(i)) != 1 && sommetsSource->size()!=1){
                    sommetsDestination->push_back(sommets_adj.at(i));
                    poids+=(*g)[sommets_adj.at(i)]._weight;
                    suprim_val(*sommetsSource, sommets_adj[i]);
                }
                else if(poids>=poids_max || sommetsSource->size() == 1){
					sort(sommetsDestination->begin(), sommetsDestination->end());
					return;
				}
            }

            /*std::cout<<"Sommets_source :"<<std::endl;
            for(uint i =0; i<sommetsSource->size();i++){
                std::cout<<sommetsSource->at(i)<<",";
            }
            std::cout<<std::endl;
            std::cout<<"Sommets_destination :"<<std::endl;
            for(uint i =0; i<sommetsDestination->size();i++){
                std::cout<<sommetsDestination->at(i)<<",";
            }
            std::cout<<std::endl;*/
        }
        sommets_adj.clear();
        cpt++;

    }
    
    sort(sommetsDestination->begin(), sommetsDestination->end());
}

void Transfert_vertex_bissection(UnorientedGraph *g, Entiers *sommetsSource, Entiers *sommetsDestination, Entiers &sommets_adj, const std::string &name, double &poids, double &cut){
	std::vector<double> sommets_cut;
	sort(sommets_adj.begin(), sommets_adj.end());
	for(uint i=0;i<sommets_adj.size();i++)
	{
		double tmp_cut = cut;
		if(name == "cut"){
			sommets_cut.push_back(modif_Cout_coupe(*sommetsDestination,sommets_adj.at(i),tmp_cut,g)); 
		}else if(name == "ratio"){
			//std::cout<<"adj : "<<(*g)[sommets_adj.at(i)]._index<<" cut = "<<Modif_ratio_cut(g, sommetsSource, sommetsDestination, sommets_adj.at(i), tmp_cut)<<std::endl;
			sommets_cut.push_back(Modif_ratio_cut(g, sommetsSource, sommetsDestination, sommets_adj.at(i), tmp_cut));
		}
	}
	
	cut = *min_element(sommets_cut.begin(),sommets_cut.end());
	//std::cout<<"*** Passage 3 cut ***** : "<<cut<<std::endl;
	//std::cout<<"Meilleur voisin : "<<(*g)[sommets_adj.at(recherche_val_double(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end())))]._index<<std::endl;
	sommetsDestination->push_back(sommets_adj[recherche_val_double(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
	//std::cout<<"*** Passage 3 cuta ***"<<std::endl;
	poids+=(*g)[sommets_adj[recherche_val_double(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]]._weight;
	//std::cout<<"*** Passage 3 cutb ***"<<std::endl;
	suprim_val(*sommetsSource, sommets_adj[recherche_val_double(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
	suprim_val(sommets_adj, sommets_adj[recherche_val_double(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
	
	//std::cout<<"*** Passage 3 fin ***"<<std::endl;
	
}

void Tirage_transfert_vertex_bissection(UnorientedGraph *g, Entiers *sommetsSource, Entiers *sommetsDestination, const std::string &name, double &poids, double &cut){
	double diff = -100.;
	int tmp_tir;
	while(diff != 0){
		tmp_tir = sommetsSource->at(rand_fini(0,sommetsSource->size()));
		diff = Diff_cut_ratio_bissection(g, sommetsSource, tmp_tir, name);
	}
	poids += (*g)[tmp_tir]._weight;
	if(name == "cut"){
		cut = modif_Cout_coupe(*sommetsDestination,tmp_tir,cut,g);
	}else{
		cut = Modif_ratio_cut(g, sommetsSource, sommetsDestination, tmp_tir, cut);
	}
	sommetsDestination->push_back(tmp_tir);
	suprim_val(*sommetsSource,tmp_tir);
}

bool Best_transfert_vertex_bissection(UnorientedGraph *g, Entiers *sommetsSource, Entiers *sommetsDestination, Entiers &sommets_adj,
									  const std::string &name, double &poids, double poids_max, double PC, int stop, int &cpt, double &cut){
    Liste_Voisin(*sommetsDestination,sommets_adj,*g);
    //sort(sommets_adj.begin(), sommets_adj.end());
	if(sommets_adj.size() == 0 & cpt<stop & sommetsSource->size()>1)
	{
		cpt++;
		//std::cout<<"*** Passage 1 ***"<<std::endl;
		if(poids < (poids_max - poids_max*PC/100)){
			//std::cout<<"*** Passage 1a ***"<<std::endl;
			Tirage_transfert_vertex_bissection(g, sommetsSource, sommetsDestination, name, poids, cut);
			if(poids < (poids_max - poids_max*PC/100) & sommetsSource->size()>1){
				Best_transfert_vertex_bissection(g, sommetsSource, sommetsDestination, sommets_adj,
												name, poids, poids_max, PC, stop, cpt, cut);	
			}else{			
				//std::cout<<"*** Passage 1ac ***"<<std::endl;
				return true;
			}			       
		}else{	
			//std::cout<<"*** Passage 1b ***"<<std::endl;
			return true;
		}
		
	}
	else if (sommets_adj.size() == 0 & cpt>=stop){
		//std::cout<<"*** Passage 2 ***"<<std::endl;
		return true;
	}else if (sommetsSource->size()==1){
		//std::cout<<"*** Passage 4 ***"<<std::endl;
		return true;
	}else{
		//std::cout<<"*** Passage 3 *** : "<<cut<<std::endl;
		Transfert_vertex_bissection(g, sommetsSource, sommetsDestination, sommets_adj, name, poids, cut);
		return false;
	}
}

void gggp_pond(UnorientedGraph *g, Entiers *sommetsSource, Entiers *sommetsDestination,
				EntiersEntiers &Partition, int rand, int &index_partition, const std::string &name, int distance)
{
	//std::cout<<"bissec"<<std::endl;
    Entiers sommets_adj;
    double poids_max = 0;
    double poids, cut;
    
    if(sommetsSource->size()==1){
		//std::cout<<"Changement de Partition"<<std::endl;
        Entiers tailles;
        for(uint i=0;i<Partition.size();i++){
            tailles.push_back(Partition.at(i)->size());
        }
        uint tmp=*max_element(tailles.begin(),tailles.end());
        for(int i=0; i<Partition.size();i++){
            if(Partition.at(i)->size()==tmp)
            {
				sommetsSource->clear();
				for(int id = 0; id < Partition.at(i)->size();id++){
					sommetsSource->push_back(Partition.at(i)->at(id));
				}
				index_partition = i;
				if(distance != -1){
					gggp_pond(g,sommetsSource,sommetsDestination,Partition,
							  Partition.at(i)->at(rand_fini(0,Partition.at(i)->size())),index_partition ,name, distance);
				}else{
					gggp_pond(g,sommetsSource,sommetsDestination,Partition,
							  rand_fini(0,Partition.at(i)->size()),index_partition, name ,distance);
				}
                return;
            }
        }
    }

    for(uint i=0;i<sommetsSource->size();i++){
        poids_max+=(*g)[sommetsSource->at(i)]._weight;
    }
    poids_max/=2.;
    //std::cout<<"Poids total : "<<poids_max<<std::endl;
   
    if(distance == -1){
		//std::cout<<"Sommet de départ : "<<(*g)[sommetsSource->at(rand)]._index<<std::endl;
		poids=(*g)[sommetsSource->at(rand)]._weight;
		if(name == "cut"){
			cut = Degree(*g,sommetsSource->at(rand));
		}else if(name == "ratio"){
			cut = Degree(*g,sommetsSource->at(rand));
			double tmp_cut = cut/2./(*g)[sommetsSource->at(rand)]._weight 
				   + cut/2./(Cluster_Weight(*g,*sommetsSource)-(*g)[sommetsSource->at(rand)]._weight);
			cut = tmp_cut;
		}
		sommetsDestination->push_back(sommetsSource->at(rand));
		sommetsSource->erase(sommetsSource->begin() + rand);
	}else{
		//std::cout<<"Sommet de départ : "<<(*g)[rand]._index<<std::endl;
		poids=(*g)[rand]._weight;
		cut = Degree(*g,rand);
		if(name == "ratio"){
			double tmp_cut = cut/2./(*g)[rand]._weight + cut/2./(Cluster_Weight(*g,*sommetsSource)-(*g)[rand]._weight);
			cut = tmp_cut;
		}
		sommetsDestination->push_back(rand);
		suprim_val(*sommetsSource,rand);
	}

    while(poids<poids_max && sommetsSource->size()>1)
    {
		int cpt = 0;
        bool next = Best_transfert_vertex_bissection(g, sommetsSource, sommetsDestination, sommets_adj,
													name, poids, poids_max, 20, 2, cpt, cut);
		if(next == true){
			sort(sommetsDestination->begin(), sommetsDestination->end());
			return;
		}										
    }
    sort(sommetsDestination->begin(), sommetsDestination->end());
}

/*Liste_Voisin(*sommetsDestination,sommets_adj,*g);
if(sommets_adj.size()==0)
{
	std::cout<<"*** Passage ***"<<std::endl;
	if(poids < (poids_max - poids_max*40/100)){
		Tirage_transfert_vertex_bissection(g, sommetsSource, sommetsDestination, name, poids);
		
		Liste_Voisin(*sommetsDestination,sommets_adj,*g);
		if(sommets_adj.size() != 0){
			Transfert_vertex_bissection(g, sommetsSource, sommetsDestination, sommets_adj, name, poids);
		}else{
			sort(sommetsDestination->begin(), sommetsDestination->end());
			return;
		}
	}else{
		sort(sommetsDestination->begin(), sommetsDestination->end());
		return;
	}
	
}
else{
	Transfert_vertex_bissection(g, sommetsSource, sommetsDestination, sommets_adj, name, poids);
}*/


void Iter_2l(EntiersEntiers &part, int nbr_parties, UnorientedGraph *g,
             const std::string &nom_cut, int nbr_tirage, const std::string &nom_strat, int distance)
{
	/*std::vector<std::string> color;
	color.push_back("[color=blue2, fontcolor=blue2];");
	color.push_back("[color=red, fontcolor=red];");
	color.push_back("[color=green, fontcolor=green];");
	color.push_back("[color=turquoise, fontcolor=turquoise];");
	color.push_back("[color=saddlebrown, fontcolor=saddlebrown];");
	color.push_back("[color=indigo, fontcolor=indigo];");
	color.push_back("[color=yellow, fontcolor=yellow2];");
	color.push_back("[color=orange, fontcolor=orange];");
	color.push_back("[color=olivedrab, fontcolor=olivedrab];");
	color.push_back("[color=gold, fontcolor=gold];");
	color.push_back("[color=slateblue2, fontcolor=slateblue2];");
	color.push_back("[color=dimgrey, fontcolor=dimgrey];");
	color.push_back("[color=cyan, fontcolor=cyan];");
	color.push_back("[color=purple1, fontcolor=purpule1];");
	color.push_back("[color=crimson, fontcolor=crimson];");
	color.push_back("[color=black, fontcolor=black];");*/
	
	UnorientedGraph copy_graph;
	boost::copy_graph(*g,copy_graph);
	
	/*std::vector<char* > nom;
	char * tmp_nom1 = new char();
	tmp_nom1 = "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_1.txt";
	nom.push_back(tmp_nom1);
	char * tmp_nom2 = new char();
	tmp_nom2 = "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_2.txt";
	nom.push_back(tmp_nom2);
	char * tmp_nom3 = new char();
	tmp_nom3= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_3.txt";
	nom.push_back(tmp_nom3);
	char * tmp_nom4 = new char();
	tmp_nom4= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_4.txt";
	nom.push_back(tmp_nom4);
	char * tmp_nom5 = new char();
	tmp_nom5= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_5.txt";
	nom.push_back(tmp_nom5);	
	char * tmp_nom6 = new char();
	tmp_nom6= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_6.txt";
	nom.push_back(tmp_nom6);
	char * tmp_nom7 = new char();
	tmp_nom7= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_7.txt";
	nom.push_back(tmp_nom7);
	char * tmp_nom8 = new char();
	tmp_nom8= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_8.txt";
	nom.push_back(tmp_nom8);
	char * tmp_nom9 = new char();
	tmp_nom9= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_9.txt";
	nom.push_back(tmp_nom9);
	char * tmp_nom10 = new char();
	tmp_nom10= "../../sortie_graphe/Tests/Graphes/Evolution_bissection/bissection_10.txt";
	nom.push_back(tmp_nom10);
	int cpt = 0;*/
	
	for(int i = 0; i<floor(log(nbr_parties)/log(2)); i++)
    {
		for(int j = 0; j< pow(2,i);j++)
        {
			if(distance == -1){
				if(nbr_tirage != 0){
					Optimisation_method_neighbour(g,part,j,nbr_tirage, nom_cut, nom_strat);
				}else{
					Optimisation_method_neighbour_minweight(g,part,j, nom_cut, nom_strat);
				}
			}else{
				Optimisation_method_neighbour_distance(g,part,j,nbr_tirage, distance, nom_cut, nom_strat);
			}
			
	    		/*std::ofstream GRAPH2 (nom.at(cpt), std::ios::out);
				GRAPH2<<"graph G {"<<std::endl;   
				tie(vertexIt, vertexEnd) = vertices(copy_graph);
				for (; vertexIt != vertexEnd; ++vertexIt) {
					GRAPH2<<(copy_graph)[*vertexIt]._index<<" -- {";
					tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
	    			copy_graph);
					for (; neighbourIt != neighbourEnd; ++neighbourIt){
						if((copy_graph)[*neighbourIt]._index>(copy_graph)[*vertexIt]._index)
							GRAPH2<<(copy_graph)[*neighbourIt]._index<<";";
					}
					GRAPH2<<"}"<<std::endl;
				}   
				
				for(uint k=0; k<part.size(); k++){
					for(uint t=0; t<part.at(k)->size(); t++)
					{
						GRAPH2<<(copy_graph)[part.at(k)->at(t)]._index<<color.at(k)<<std::endl;
					}
				}
	    
				GRAPH2<<"}";	
				GRAPH2.close();
				
				cpt ++;*/
        }
    }
}

void bissectionRec(UnorientedGraph *g, EntiersEntiers &Partition,
                   int nbr_parties, const std::string &nom_cut, int nbr_tirage, const std::string &nom_strat, int distance)
{
	
    if((nbr_parties&(nbr_parties-1))==0)
    {
        //std::cout<<"C'est de la forme 2l : "<<nbr_parties<<std::endl;
        Iter_2l(Partition,nbr_parties,g,nom_cut,nbr_tirage,nom_strat, distance);
    }
    else
    {
        int puissance_2=0;

        Entiers tailles;

        while(pow(2,puissance_2)<nbr_parties)
            puissance_2++;

        Iter_2l(Partition,pow(2,puissance_2-1),g,nom_cut,nbr_tirage,nom_strat, distance);

        for(unsigned int i = 0; i< Partition.size() -1 ; i++)
        {
            for(EntiersEntiers::iterator it1 = Partition.begin() + i ; it1!=Partition.end(); it1++)
            {
                if((*it1)->size() > Partition.at(i)->size())
                    Partition.at(i)->swap(**it1);
            }
        }

        for(int j = 0; j<nbr_parties-pow(2,puissance_2-1);j++)
        {
			if(distance == -1){
				if(nbr_tirage != 0){
					Optimisation_method_neighbour(g,Partition,j,nbr_tirage, nom_cut, nom_strat);
				}else{
					Optimisation_method_neighbour_degree(g,Partition,j, nom_cut, nom_strat);
				}
			}else{
				Optimisation_method_neighbour_distance(g,Partition,j,nbr_tirage, distance, nom_cut, nom_strat);
			}
			
			/*for(int x =0; x<Partition.size(); x++){
				std::cout<<"** ";
				for(int y =0; y<Partition.at(x)->size();y++){
					std::cout<<Partition.at(x)->at(y)<<" ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;*/
        }
    }
}

/**
 * Fonction réalisant un partitionnement pseudo aléatoire suivant un voisinage.
 * @param *g : adresse d'un graphe de type boost graphe undirected
 * @param Partition : vecteur contenant des vecteurs d'entiers [tableau contenant les parties de la partition]
 * @param nbr_partie : entier correspondant au nombre de parties voulues pour la partition
 * @return
 */

void Pseudo_random_partitioning(UnorientedGraph *g, EntiersEntiers &Partition,
                                uint nbr_parties)
{
    /*
     * Principe : distribution des sommets de la première partie en plusieurs autres parties
     * Le partitionnement étant pseudo aléatoire il n'y a pas de contrainte stricte sur le nombre
     * de sommets par partie
     */


    uint size = Partition.at(0)->size();
    uint cpt_sommets=1;
    int val;
    uint cpt;

    if(nbr_parties==size){
        for(uint i = 0; i < nbr_parties;i++){
            if(Partition.at(0)->size()!=1)
            {
                val=rand_fini(0,Partition.at(0)->size()-1);//tirage aléatoire d'un sommets
            }
            else
                val=0;

            int vertex = Partition.at(0)->at(val);
            Entiers *part = new Entiers();
            part->push_back(vertex);// ajout du sommet tiré
            suprim_val(*Partition.at(0),vertex);//suppression du sommet dans la premiere partie

        }
    }
    /*
     * Boucle sur le nombre de partie à réaliser
     */
    for(uint i = 0; i < nbr_parties-1;i++){
        if(Partition.at(0)->size()!=1)
        {
            val=rand_fini(0,Partition.at(0)->size()-1);//tirage aléatoire d'un sommets
        }
        else
            val=0;
        int vertex = Partition.at(0)->at(val);
        /*
         * Initialisation d'un pointeur sur un vecteur d'entier, dans notre cas
         * la n+1 ième partie de la partition
         */
        Entiers *part = new Entiers();
        part->push_back(vertex);// ajout du sommet tiré
        suprim_val(*Partition.at(0),vertex);//suppression du sommet dans la premiere partie
        cpt=1;

        /*
         * Pour chaque element de la nouvelle partie faire
         */
        for(uint j = 0; j<part->size();j++){
            /*
             * Détermination des voisins de chacun des sommets de cette nouvelle
             * partie et ajoue de ce voisin si celui-ci est présent dans la première partie (Partition[0])
             */
            tie(neighbourIt, neighbourEnd) = adjacent_vertices(part->at(j),*g);
            for (; neighbourIt != neighbourEnd; ++neighbourIt){
                if(In_tab(*Partition.at(0),*neighbourIt)==1){
                    // std::cout<<"le voisin déplacé est : "<<*neighbourIt<<std::endl;
                    part->push_back(*neighbourIt);
                    cpt_sommets++;
                    suprim_val(*Partition.at(0),*neighbourIt);
                    cpt++;
                }
                /*
                 * Si le nombre moyen de sommets est atteind dans la partie on sort de la boucle des voisins
                 * Même chose si l'on a rencontré le nombre total de sommets
                 */
                if(cpt==(size/nbr_parties)+1)
                    break;
                if(cpt_sommets==size)
                    break;
            }

            /*
             * Même chose
             */
            if(cpt==(size/nbr_parties)+1)
                break;
            if(cpt_sommets==size)
                break;
        }
        Partition.push_back(part);// ajoue de la nouvelle partie à la partition
        if(cpt_sommets==size)
            break;
    }
}

EntiersEntiers Random_partitioning(UnorientedGraph *g,
                                uint nbr_parties)
{
	EntiersEntiers Partition;
	Entiers random_order; //gestion d'un tableau contenant tout les sommets et ranger de façon aléatoire

	for (uint i=0 ; i<num_vertices(*g) ; i++)
		random_order.push_back(i);
	for (uint j=0 ; j<num_vertices(*g)-1 ; j++) {
		int rand_pos = rand()%(num_vertices(*g)-j)+j;
		int tmp      = random_order.at(j);
		random_order.at(j) = random_order.at(rand_pos);
		random_order.at(rand_pos) = tmp;
	}

	uint size = num_vertices(*g)/nbr_parties;
	for(uint j = 0 ; j < nbr_parties-1 ; j++){
		Entiers *part = new Entiers();
		for(uint i = j*size; i<(j+1)*size; i++){
			part->push_back(random_order.at(i));
		}
		Partition.push_back(part);
	}

	Entiers *part = new Entiers();
	for(uint i = (nbr_parties-1)*size; i < random_order.size(); i++){
		part->push_back(random_order.at(i));
	}
	Partition.push_back(part);

	for(uint i = 0 ; i < Partition.size() ; i++){
		sort(Partition.at(i)->begin(),Partition.at(i)->end());
	}

	return Partition;
}

OrientedGraphs Multiniveau(uint niveau_contraction,
                           UnorientedGraph *g,
                           UnorientedGraph *graph_origin,
                           OrientedGraph *go,
                           int nbr_parties,
                           int nbr_tirage,
                           std::string contraction,
                           std::string type_methode,
                           std::string choix_affinage,
                           std::string type_cut,
                           Edges& /* edge_partie */,
                           OutputEdgeList& outputedgelist,
                           InputEdgeList& inputedgelist,
                           Connections& connections, bool rec,
                           std::vector<double> &Cut, int distance)
{
	std::vector<std::string> color;
	if(rec == true){
		color.push_back("[color=blue2, fontcolor=blue2];");
		color.push_back("[color=red, fontcolor=red];");
		color.push_back("[color=green, fontcolor=green];");
		color.push_back("[color=yellow, fontcolor=yellow2];");
		color.push_back("[color=saddlebrown, fontcolor=saddlebrown];");
		color.push_back("[color=turquoise, fontcolor=turquoise];");
		color.push_back("[color=orange, fontcolor=orange];");
		color.push_back("[color=olivedrab, fontcolor=olivedrab];");
		color.push_back("[color=indigo, fontcolor=indigo];");
		color.push_back("[color=gold, fontcolor=gold];");
		color.push_back("[color=slateblue2, fontcolor=slateblue2];");
		color.push_back("[color=dimgrey, fontcolor=dimgrey];");
		color.push_back("[color=cyan, fontcolor=cyan];");
		color.push_back("[color=purple1, fontcolor=purpule1];");
		color.push_back("[color=crimson, fontcolor=crimson];");
		color.push_back("[color=black, fontcolor=black];");
    }
    
    boost::timer t; 
    
    EntiersEntiers Partition;
    Entiers *part = new Entiers();
    Base_Graph baseg;
    baseg.push_back(g);
    ListEntiersEntiers liste_corr;
    uint cpt =0;

    int val_cpt = num_vertices(*g);
    bool stop = false;

    if (niveau_contraction == val_cpt) {
        stop = true;
    }

    while(stop != true)
    {
    	if(contraction == "HEM")
    		stop = contraction_HEM(baseg.at(cpt),baseg,liste_corr,niveau_contraction,val_cpt);
    	else
    		stop = contraction_Random_Maching(baseg.at(cpt),baseg,liste_corr,niveau_contraction,val_cpt);
        cpt++;
    }
    
    if(rec == true){
		std::ofstream GRAPHp ("../../sortie_graphe/Tests/Graphes/Evolution_bissection/poids_graphe.txt", std::ios::out);
		GRAPHp<<"Poids du graphe contracté : "<<std::endl;   
		tie(vertexIt, vertexEnd) = vertices(*baseg.at(baseg.size()-1));
		for (; vertexIt != vertexEnd; ++vertexIt) {
			GRAPHp<<(*baseg.at(baseg.size()-1))[*vertexIt]._index<<" --> "<<(*baseg.at(baseg.size()-1))[*vertexIt]._weight<<std::endl;;
		}   
		
		GRAPHp<<"}";	
		GRAPHp.close();
	}

    double t2 = t.elapsed();
    std::cout << "Contraction duration : " << t2 << " seconds" << std::endl;
    
    for(int i = 0; i<num_vertices(*baseg.at(baseg.size() - 1)); i++){
		part->push_back(i);
	}
	Partition.push_back(part);
	
	UnorientedGraph copy_graph;
	boost::copy_graph(*baseg.at(baseg.size()-1),copy_graph);
	
	if(rec == true){
		std::ofstream GRAPH ("../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/contraction_final.txt", std::ios::out);
		GRAPH<<"graph G {"<<std::endl;   
		tie(vertexIt, vertexEnd) = vertices(*baseg.at(baseg.size()-1));
		for (; vertexIt != vertexEnd; ++vertexIt) {
			GRAPH<<(*baseg.at(baseg.size()-1))[*vertexIt]._index<<" -- {";
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
					*baseg.at(baseg.size()-1));
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				if((*baseg.at(baseg.size()-1))[*neighbourIt]._index>(*baseg.at(baseg.size()-1))[*vertexIt]._index)
					GRAPH<<(*baseg.at(baseg.size()-1))[*neighbourIt]._index<<";";
			}
			GRAPH<<"}"<<std::endl;
		}   
		
		GRAPH<<"}";	
		GRAPH.close();
	}
    
    if(type_methode == "gggp" || type_methode == "ggp"){
		bissectionRec(baseg.at(baseg.size()-1),Partition,nbr_parties,type_cut,nbr_tirage,type_methode, distance);
		double tcut = Cut_cluster(Partition,copy_graph,type_cut);
		/*std::cout<<tcut<<std::endl;
		std::cout<<"Meilleur coût de coupe : "<<best_cut<<std::endl;
		std::cout<<std::endl;*/
    }
    else
    	Partition = Random_partitioning(baseg.at(baseg.size()-1),nbr_parties);
    
    double t3 = t.elapsed();
    std::cout << "Partitioning duration : " << (t3 - t2) << " seconds" << std::endl;
    
    if(rec == true){
		std::ofstream GRAPH3 ("../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/contraction_final_partition.txt", std::ios::out);
		GRAPH3<<"graph G {"<<std::endl;   
		tie(vertexIt, vertexEnd) = vertices(copy_graph);
		for (; vertexIt != vertexEnd; ++vertexIt) {
			GRAPH3<<(copy_graph)[*vertexIt]._index<<" -- {";
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
			copy_graph);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				if((copy_graph)[*neighbourIt]._index>(copy_graph)[*vertexIt]._index)
				GRAPH3<<(copy_graph)[*neighbourIt]._index<<";";
			}
			GRAPH3<<"}"<<std::endl;
		}   
		
		for(uint k=0; k<Partition.size(); k++){
			for(uint j=0; j<Partition.at(k)->size(); j++)
			{
	     		GRAPH3<<(copy_graph)[Partition.at(k)->at(j)]._index<<color.at(k)<<std::endl;
			}
	    }
	
		GRAPH3<<"}";	
		GRAPH3.close();
	}
			
	double poids_moy = 0.;
	for(uint i =0; i < Partition.size(); i++){
		poids_moy += Cluster_Weight(copy_graph,*Partition.at(i));
	}
	poids_moy/=Partition.size();
	
	poids_moy = -1;
	
	/*poids_moy = -1 implique un respect du poids faible*/
	
    ListEntiersEntiers::iterator lit(liste_corr.end());
    bool proj;
    uint taille_list = liste_corr.size();
    if(liste_corr.size()==0){
    	taille_list = 1;
    	proj = true;
    }
    else{
    	lit--;
    	proj = false;
    }

	//std::cout<<"Nombre de contraction : "<<taille_list<<std::endl;
	std::vector<char* > nom;
	std::vector<char* > nom_a;
	if(rec == true){
		char * tmp_nom1 = new char();
		tmp_nom1 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_1.txt";
		nom.push_back(tmp_nom1);
		char * tmp_nom_a1 = new char();
		tmp_nom_a1 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_1.txt";
		nom_a.push_back(tmp_nom_a1);
		char * tmp_nom2 = new char();
		tmp_nom2 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_2.txt";
		nom.push_back(tmp_nom2);
		char * tmp_nom_a2 = new char();
		tmp_nom_a2 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_2.txt";
		nom_a.push_back(tmp_nom_a2);
		char * tmp_nom3 = new char();
		tmp_nom3= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_3.txt";
		nom.push_back(tmp_nom3);
		char * tmp_nom_a3 = new char();
		tmp_nom_a3 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_3.txt";
		nom_a.push_back(tmp_nom_a3);
		char * tmp_nom4 = new char();
		tmp_nom4= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_4.txt";
		nom.push_back(tmp_nom4);
		char * tmp_nom_a4 = new char();
		tmp_nom_a4 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_4.txt";
		nom_a.push_back(tmp_nom_a4);
		char * tmp_nom5 = new char();
		tmp_nom5= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_5.txt";
		nom.push_back(tmp_nom5);
		char * tmp_nom_a5 = new char();
		tmp_nom_a5 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_5.txt";
		nom_a.push_back(tmp_nom_a5);
		char * tmp_nom6 = new char();
		tmp_nom6= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_6.txt";
		nom.push_back(tmp_nom6);
		char * tmp_nom_a6 = new char();
		tmp_nom_a6 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_6.txt";
		nom_a.push_back(tmp_nom_a6);
		char * tmp_nom7 = new char();
		tmp_nom7= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_7.txt";
		nom.push_back(tmp_nom7);
		char * tmp_nom_a7 = new char();
		tmp_nom_a7 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_7.txt";
		nom_a.push_back(tmp_nom_a7);
		char * tmp_nom8 = new char();
		tmp_nom8= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_8.txt";
		nom.push_back(tmp_nom8);
		char * tmp_nom_a8 = new char();
		tmp_nom_a8 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_8.txt";
		nom_a.push_back(tmp_nom_a8);
		char * tmp_nom9 = new char();
		tmp_nom9= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_9.txt";
		nom.push_back(tmp_nom9);
		char * tmp_nom_a9 = new char();
		tmp_nom_a9 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_9.txt";
		nom_a.push_back(tmp_nom_a9);
		char * tmp_nom10 = new char();
		tmp_nom10= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_10.txt";
		nom.push_back(tmp_nom10);
		char * tmp_nom_a10 = new char();
		tmp_nom_a10 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_10.txt";
		nom_a.push_back(tmp_nom_a10);
		char * tmp_nom11 = new char();
		tmp_nom11= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_11.txt";
		nom.push_back(tmp_nom11);
		char * tmp_nom_a11 = new char();
		tmp_nom_a11 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_11.txt";
		nom_a.push_back(tmp_nom_a11);
		char * tmp_nom12 = new char();
		tmp_nom12= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_12.txt";
		nom.push_back(tmp_nom12);
		char * tmp_nom_a12 = new char();
		tmp_nom_a12 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_12.txt";
		nom_a.push_back(tmp_nom_a12);
		char * tmp_nom13 = new char();
		tmp_nom13= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_13.txt";
		nom.push_back(tmp_nom13);
		char * tmp_nom_a13 = new char();
		tmp_nom_a13 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_13.txt";
		nom_a.push_back(tmp_nom_a13);
		char * tmp_nom14 = new char();
		tmp_nom14= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_14.txt";
		nom.push_back(tmp_nom14);
		char * tmp_nom_a14 = new char();
		tmp_nom_a14 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_14.txt";
		nom_a.push_back(tmp_nom_a14);
		char * tmp_nom15 = new char();
		tmp_nom15= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_15.txt";
		nom.push_back(tmp_nom15);
		char * tmp_nom_a15 = new char();
		tmp_nom_a15 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_15.txt";
		nom_a.push_back(tmp_nom_a15);
		char * tmp_nom16 = new char();
		tmp_nom16= "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_16.txt";
		nom.push_back(tmp_nom16);
		char * tmp_nom_a16 = new char();
		tmp_nom_a16 = "../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/projection_partition_affine_16.txt";
		nom_a.push_back(tmp_nom_a16);
	}
    for(uint y =0; y<taille_list;y++){
    	if(proj != true){
    		projection(Partition,lit);
    		
    		if(rec == true){
	    		std::ofstream GRAPH2 (nom.at(y), std::ios::out);
				GRAPH2<<"graph G {"<<std::endl;   
				tie(vertexIt, vertexEnd) = vertices(*baseg.at(baseg.size()-2-y));
				for (; vertexIt != vertexEnd; ++vertexIt) {
					GRAPH2<<(*baseg.at(baseg.size()-2-y))[*vertexIt]._index<<" -- {";
					tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
	    			*baseg.at(baseg.size()-2-y));
					for (; neighbourIt != neighbourEnd; ++neighbourIt){
						if((*baseg.at(baseg.size()-2-y))[*neighbourIt]._index>(*baseg.at(baseg.size()-2-y))[*vertexIt]._index)
							GRAPH2<<(*baseg.at(baseg.size()-2-y))[*neighbourIt]._index<<";";
					}
					GRAPH2<<"}"<<std::endl;
				}   
				
				for(uint k=0; k<Partition.size(); k++){
					for(uint j=0; j<Partition.at(k)->size(); j++)
					{
						GRAPH2<<(*baseg.at(baseg.size()-2-y))[Partition.at(k)->at(j)]._index<<color.at(k)<<std::endl;
					}
				}
	    
				GRAPH2<<"}";	
				GRAPH2.close();
			}

    		double cut = Cut_cluster(Partition,*baseg.at(baseg.size()-2-y),type_cut);
    		//std::cout<<"Cout de coupe avant affinage : "<<cut<<std::endl;
    		if(choix_affinage == "charge")
    			Affinage_equilibrage_charge(baseg.at(baseg.size()-2-y),Partition);
    		else if(choix_affinage == "locale"){
    			Affinage_recherche_locale(baseg.at(baseg.size()-2-y),Partition,cut,type_cut);}
    		else	
    			Affinache_gain_diff(baseg.at(baseg.size()-2-y), Partition, cut, type_cut, poids_moy);
			//std::cout<<"Cout de coupe après affinage : "<<cut<<std::endl;
			
			if(rec == true){
	    		std::ofstream GRAPH4 (nom_a.at(y), std::ios::out);
				GRAPH4<<"graph G {"<<std::endl;   
				tie(vertexIt, vertexEnd) = vertices(*baseg.at(baseg.size()-2-y));
				for (; vertexIt != vertexEnd; ++vertexIt) {
					GRAPH4<<(*baseg.at(baseg.size()-2-y))[*vertexIt]._index<<" -- {";
					tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
	    			*baseg.at(baseg.size()-2-y));
					for (; neighbourIt != neighbourEnd; ++neighbourIt){
						if((*baseg.at(baseg.size()-2-y))[*neighbourIt]._index>(*baseg.at(baseg.size()-2-y))[*vertexIt]._index)
							GRAPH4<<(*baseg.at(baseg.size()-2-y))[*neighbourIt]._index<<";";
					}
					GRAPH4<<"}"<<std::endl;
				}   
				
				for(uint k=0; k<Partition.size(); k++){
					for(uint j=0; j<Partition.at(k)->size(); j++)
					{
						GRAPH4<<(*baseg.at(baseg.size()-2-y))[Partition.at(k)->at(j)]._index<<color.at(k)<<std::endl;
					}
				}
	    
				GRAPH4<<"}";	
				GRAPH4.close();
			}
			
    		lit--;
    	}
    	else{
    		//std::cout<<"Pas de projection "<<std::endl;
    	}

    }
    
    double t4 = t.elapsed();
    std::cout << "Affinage duration : " << (t4 - t3) << " seconds" << std::endl;
    
    /*std::cout<<"Partition : "<<std::endl;
	for(int x =0; x<Partition.size(); x++){
		std::cout<<"*"<<x<<"* ";
		for(int y =0; y<Partition.at(x)->size();y++){
			std::cout<<Partition.at(x)->at(y)<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;*/
			
	double best_cut = Cut_cluster(Partition,*graph_origin,type_cut);
	Cut.push_back(best_cut);

    OrientedGraphs Graphes = Graph_Partition(Partition, go, graph_origin, outputedgelist,
                                             inputedgelist, connections);
	
	if(rec == true){
	    std::ofstream fichier2 ("../../sortie_graphe/Tests/Graphes/txt/Niveau_contraction/résultat_partitionnement.txt", std::ios::out);
	    fichier2<<"digraph G {"<<std::endl;   
	    tie(vertexIto, vertexEndo) = vertices(*go);
	    for (; vertexIto != vertexEndo; ++vertexIto) {
			fichier2<<(*go)[*vertexIto]._index<<"-> {";
	    	tie(neighbourIto, neighbourEndo) = adjacent_vertices(*vertexIto,
	    			*go);
	    	for (; neighbourIto != neighbourEndo; ++neighbourIto){
	    		fichier2<<(*go)[*neighbourIto]._index<<";";
	    	}
	    	fichier2<<"}"<<std::endl;
		}
	    
	    for(uint k=0; k<Partition.size(); k++){
			for(uint j=0; j<Partition.at(k)->size(); j++)
			{
	     		fichier2<<Partition.at(k)->at(j)<<color.at(k)<<std::endl;
			}
	    }   
	    
		fichier2<<"}";	
		fichier2.close();
	}

    for(EntiersEntiers::iterator it = Partition.begin(); it != Partition.end(); it++)
    {
        delete *it;
        *it = NULL;
    }

    for(ListEntiersEntiers::iterator it = liste_corr.begin(); it != liste_corr.end(); it++)
    {
        for(EntiersEntiers::iterator it1 = (*it)->begin(); it1 != (*it)->end(); it1++)
        {
            delete *it1;
            *it1 = NULL;
        }
        delete *it;
        *it = NULL;
    }

    for(Base_Graph::iterator it = baseg.begin(); it != baseg.end(); it++)
    {
        delete *it;
        *it = NULL;
    }
    return Graphes;
}

void Optimisation_method_neighbour(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition, int nbr_tirage, const std::string &name_cut, const std::string &name_strat){
	/*Initialisation des parametres*/
	//UnorientedGraph *copy_g_ref = new UnorientedGraph();
	Entiers *part2_cons = new Entiers();
	Entiers *part_cour_cons = new Entiers();
	Entiers Random_list_vertices;
	double cut = 1000000000.;
	//boost::copy_graph(*g,*copy_g_ref);

	for (int i=0 ; i<Partition.at(index_partition)->size() ; i++)
		Random_list_vertices.push_back(i);
	for (int j=0 ; j<Partition.at(index_partition)->size()-1 ; j++) {
		int rand_pos = rand()%(Partition.at(index_partition)->size()-j)+j;
		int tmp      = Random_list_vertices[j];
		Random_list_vertices[j] = Random_list_vertices[rand_pos];
		Random_list_vertices[rand_pos] = tmp;
	}
	
	/*for(uint i = 0; i < Partition.at(index_partition)->size(); i++){
		std::cout<<Partition.at(index_partition)->at(i)<<" ";
	}
	std::cout<<std::endl;*/
		
	if(Partition.at(index_partition)->size()< nbr_tirage)
		nbr_tirage = Partition.at(index_partition)->size();
		
	/*Boucle de conservation de la meilleure bissection*/
	for(int k = 0; k<nbr_tirage; k++){
		//UnorientedGraph *copy_g =  new UnorientedGraph();
		Entiers *part2 = new Entiers();
		Entiers *tmp_part = new Entiers();
		double new_cut;
		//boost::copy_graph(*g,*copy_g);

		/*Recopie de la partie faisant l'objet de la bissection */
		for(int t=0; t<Partition.at(index_partition)->size();t++){
			tmp_part->push_back(Partition.at(index_partition)->at(t));
		}
		
		//std::cout<<"Sommet de départ : "<<(*g)[Random_list_vertices.at(k)]._index<<std::endl;
		if(name_strat == "gggp"){
			gggp_pond(g,tmp_part,part2,Partition,Random_list_vertices.at(k),index_partition, name_cut ,-1);
		} else if(name_strat == "ggp"){
			ggp(g,tmp_part,part2,Partition,Random_list_vertices.at(k), index_partition ,-1);
		} else {
			std::cout<<"Nom de stratégie de partitionnement non valide ! "<<std::endl;
		}
		
		new_cut = Best_Cut_cluster(Partition, tmp_part, part2, index_partition,*g,name_cut);
		
		//std::cout<<"Cout de coupe : "<<new_cut<<std::endl;
		//std::cout<<std::endl;
		if(new_cut<cut){ /*conservation de l'information en cas d'amélioration de la contrainte*/
			cut = new_cut;
			//copy_g_ref = copy_g;
			part2_cons = part2;
			part_cour_cons = tmp_part;
		}
		else{
			//delete copy_g;
			delete tmp_part;
			delete part2;
		}	
	}
	
	for (uint i=0; i<part_cour_cons->size();i++)
    {
        for (uint j=0; j<part2_cons->size();j++)
        {
            remove_edge(part_cour_cons->at(i),part2_cons->at(j),*g);
        }
    }
    
    /*std::cout<<"Ensemble 1 : ";
		for(uint i = 0; i < part_cour_cons->size(); i++){
			std::cout<<part_cour_cons->at(i)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<"Ensemble 2 : ";
		for(uint j = 0; j < part2_cons->size(); j++){
			std::cout<<part2_cons->at(j)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<std::endl;*/
	/*Modification des informations*/
	Partition.at(index_partition)=part_cour_cons;
	Partition.push_back(part2_cons);
	//g=copy_g_ref;
	
	/*std::cout<<std::endl;
	std::cout<<"Bissection réalisé"<<std::endl;
	std::cout<<std::endl;*/
}

void Optimisation_method_neighbour_distance(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition, int nbr_tirage, int distance,
	const std::string &name_cut, const std::string &name_strat){
	
	//Initialisation des parametres
	Entiers *part2_cons = new Entiers();
	Entiers *part_cour_cons = new Entiers();
	double cut=1000000000.;
	int val;

	std::list<int> vertex_list;
	for(int verx =0; verx<Partition.at(index_partition)->size(); verx++){
		vertex_list.push_back(Partition.at(index_partition)->at(verx));
	}
	
	/*std::list<int>::iterator toto;
	for(toto = vertex_list.begin(); toto != vertex_list.end(); toto ++)
		std::cout<<*toto<<" ";
		
	std::cout<<std::endl;*/
	
	if(Partition.at(index_partition)->size()< nbr_tirage)
		nbr_tirage = Partition.at(index_partition)->size();
		
	//Boucle de conservation de la meilleure bissection
	for(int k = 0; k<nbr_tirage; k++){
		//Tirage
		std::list<int>::iterator Iter;
		if(vertex_list.size()!=0){
		//std::cout<<"Il reste des sommets à tirer"<<std::endl;
		Iter = vertex_list.begin();
		for(int i = 0; i<rand_fini(0,vertex_list.size()); i++){
			Iter++;
		}
		val = *Iter;
		//std::cout<<"Sommet tiré : "<<*Iter<<std::endl;
		tirage_distance(g, *Iter, vertex_list, distance);
		}
		else{	
			//std::cout<<"Tous les sommets sont verrouillés"<<std::endl;
			break;
		}
		
		Entiers *part2 = new Entiers();
		Entiers *tmp_part = new Entiers();
		double new_cut;

		//Recopie de la partie faisant l'objet de la bissection 
		for(int t=0; t<Partition.at(index_partition)->size();t++){
			tmp_part->push_back(Partition.at(index_partition)->at(t));
		}
		
		//std::cout<<"Sommet tirée avant entré dans gggp : "<<*Iter<<std::endl;
		if(name_strat == "gggp"){
			gggp_pond(g,tmp_part,part2,Partition,val,index_partition, name_cut, distance);
		} else if(name_strat == "ggp"){
			ggp(g,tmp_part,part2,Partition,val,index_partition, distance);
		} else {
			std::cout<<"Nom de stratégie de partitionnement non valide ! "<<std::endl;
		}
		
		/*std::cout<<"tmp_part"<<std::endl;
		for(int alpha = 0; alpha<tmp_part->size(); alpha ++){
			std::cout<<tmp_part->at(alpha)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<"part2"<<std::endl;
		for(int alpho = 0; alpho<part2->size(); alpho ++){
			std::cout<<part2->at(alpho)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<std::endl;*/
		
		new_cut = Best_Cut_cluster(Partition, tmp_part, part2, index_partition, *g,name_cut);

		//std::cout<<"Cout de coupe : "<<new_cut<<std::endl;
		
		if(new_cut<cut){ //conservation de l'information en cas d'amélioration de la contrainte
			cut = new_cut;
			part2_cons = part2;
			part_cour_cons = tmp_part;
		}
		else{
			delete tmp_part;
			delete part2;
		}
		//std::cout<<"Un tour de plus dans la boucle k"<<std::endl;	
	}
	
	for (uint i=0; i<part_cour_cons->size();i++)
    {
        for (uint j=0; j<part2_cons->size();j++)
        {
			remove_edge(part_cour_cons->at(i),part2_cons->at(j),*g);
        }
    }
	//Modification des informations
	Partition.at(index_partition)=part_cour_cons;
	Partition.push_back(part2_cons);
	
	/*std::cout<<std::endl;
	std::cout<<"Bissection réalisé"<<std::endl;
	std::cout<<std::endl;*/
}

void Optimisation_method_neighbour_degree(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition,
	const std::string &name_cut, const std::string &name_strat){
	
	std::vector<double> vertex_degree;
	Entiers *part2 = new Entiers();
	
	for(uint i =0; i<Partition.at(index_partition)->size(); i++){
		vertex_degree.push_back(Degree(*g, Partition.at(index_partition)->at(i)));
	}
	
	uint best_cpt = 0;
	double  best_degree = vertex_degree.at(0);
	
	for(uint i =1; i<vertex_degree.size(); i++){
		if(vertex_degree.at(i)>best_degree){
			best_degree = vertex_degree.at(i);
			best_cpt = i;
		}
	}
	
	if(name_strat == "gggp"){
		gggp_pond(g,Partition.at(index_partition),part2,Partition,best_cpt,index_partition,name_cut,-1);
	} else if(name_strat == "ggp"){
		ggp(g,Partition.at(index_partition),part2,Partition,best_cpt,index_partition,-1);
	} else {
		std::cout<<"Nom de stratégie de partitionnement non valide ! "<<std::endl;
	}
	
	for (uint i=0; i<Partition.at(index_partition)->size();i++)
    {
        for (uint j=0; j<part2->size();j++)
        {
			remove_edge(Partition.at(index_partition)->at(i),part2->at(j),*g);
        }
    }

	Partition.push_back(part2);
	
}

void Optimisation_method_neighbour_minweight(UnorientedGraph *g, EntiersEntiers &Partition, int index_partition,
	const std::string &name_cut, const std::string &name_strat){
	
	std::vector<double> vertex_weight;
	Entiers *part2 = new Entiers();
	
	for(uint i =0; i<Partition.at(index_partition)->size(); i++){
		vertex_weight.push_back((*g)[Partition.at(index_partition)->at(i)]._weight);
	}
	
	uint best_cpt = 0;
	double  best_weight = vertex_weight.at(0);
	
	for(uint i =1; i<vertex_weight.size(); i++){
		if(vertex_weight.at(i)>best_weight){
			best_weight = vertex_weight.at(i);
			best_cpt = i;
		}
	}
	
	if(name_cut == "ratio"){
		int tmp_best_cpt;
		tmp_best_cpt = Partition.at(index_partition)->at(best_cpt);
		best_cpt = tmp_best_cpt;
	}
	
	if(name_strat == "gggp"){
		gggp_pond(g,Partition.at(index_partition),part2,Partition,best_cpt,index_partition,name_cut,-1);
	} else if(name_strat == "ggp"){
		ggp(g,Partition.at(index_partition),part2,Partition,best_cpt,index_partition,-1);
	} else {
		std::cout<<"Nom de stratégie de partitionnement non valide ! "<<std::endl;
	}
	
	for (uint i=0; i<Partition.at(index_partition)->size();i++)
    {
        for (uint j=0; j<part2->size();j++)
        {
			remove_edge(Partition.at(index_partition)->at(i),part2->at(j),*g);
        }
    }

	Partition.push_back(part2);
	
}

void tirage_distance(UnorientedGraph *g, int tirage, std::list<int> &vertex_list, int distance){
	std::vector<std::list<int> > vertex_delete;
	std::list<int> liste1;
	std::list<int> vd;

	liste1.push_back(tirage); /*modif*/
	vertex_delete.push_back(liste1);
		
	for(int i=0; i<distance; i++){
		std::list<int> liste_tmp;
		std::list<int>::iterator Ite_tmp;
		for(Ite_tmp = vertex_delete.at(i).begin(); Ite_tmp != vertex_delete.at(i).end(); Ite_tmp ++){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(*Ite_tmp,*g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				liste_tmp.push_back(*neighbourIt);/*modif*/
			}
		}
		liste_tmp.sort();
		liste_tmp.unique();
		vertex_delete.push_back(liste_tmp);
	}
	//std::cout<<std::endl;
		
	/*for(int i =0; i<vertex_delete.size(); i++){
		std::list<int>::iterator Ite_tmp;
		for(Ite_tmp = vertex_delete.at(i).begin(); Ite_tmp != vertex_delete.at(i).end(); Ite_tmp ++){
			std::cout<<*Ite_tmp<<" ";	
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;*/
	
	for(int index = 0; index < vertex_delete.size(); index ++){
		vd.merge(vertex_delete.at(index));
	}
	vd.unique();
	
	/*std::list<int>::iterator Ite_tmp;
	for(Ite_tmp = vd.begin(); Ite_tmp != vd.end(); Ite_tmp ++){
		std::cout<<*Ite_tmp<<" ";	
	}
	std::cout<<std::endl;*/
		
	std::list<int>::iterator Ite;
	for(Ite = vd.begin(); Ite != vd.end(); Ite ++){
		vertex_list.remove(*Ite);
	}
}

} } } // namespace paradevs tests boost_graph
