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

#include <algorithm>
#include <iostream>
#include <fstream>

namespace paradevs { namespace tests { namespace boost_graph {

extern UnorientedGraph::vertex_iterator vertexIt, vertexEnd;
extern UnorientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

extern OrientedGraph::vertex_iterator vertexIto, vertexEndo;
extern OrientedGraph::adjacency_iterator neighbourIto, neighbourEndo;

void ggp(UnorientedGraph *g, Entiers *sommetsSource,
               Entiers *sommetsDestination, EntiersEntiers &Partition, int rand, int distance)
{
    //std::cout<<""<<std::endl;
    //int val;
    Entiers sommets_adj;
    if(sommetsSource->size()==1){
        //val=0;
        //std::cout<<"Entré dans le debug ! "<<std::endl;
        Entiers tailles;
        for(uint i=0;i<Partition.size();i++){
            tailles.push_back(Partition.at(i)->size());
        }
        uint tmp=*max_element(tailles.begin(),tailles.end());
        for(uint i=0; i<Partition.size();i++){
            if(Partition.at(i)->size()==tmp)
            {
                ggp(g, Partition[i], sommetsDestination, Partition, rand_fini(0,Partition.at(i)->size()),
					distance);
                return;
            }
        }
    }
   // else
       // val=rand_fini(0,sommetsSource->size()-1);//Tirage aléatoire de l'indice du premier sommet entre 0 et taille du tableau -1
        //std::cout<<"val : "<<sommetsSource->at(val)<<std::endl;
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
   // std::cout<<"taille sommetsSource avant le while : "<<sommetsSource->size()<<std::endl;
    while(poids<poids_max && sommetsSource->size()>1)
    {
        //std::cout<<"taille sommetsSource dans le while "<<sommetsSource->size()<<std::endl;
        if(cpt<sommetsDestination->size() )
            adjacence_ggp(sommetsDestination->at(cpt),sommets_adj,g);
        else{
            int val=rand_fini(0,sommetsSource->size()-1);
            sommetsDestination->push_back(sommetsSource->at(val));
            sommetsSource->erase(sommetsSource->begin() + val);
            adjacence_ggp(sommetsDestination->at(cpt),sommets_adj,g);
        }
        /*std::cout<<"adj :"<<std::endl;
        for(uint a = 0; a<sommets_adj.size(); a++){
            std::cout<<sommets_adj.at(a)<<std::endl;
        }*/
        if(sommets_adj.size()==0)
        {
            //std::cout<<"Je suis sorti car pas de voisin !!!! "<<std::endl;
            for (uint i=0; i<sommetsSource->size();i++)
            {
                for (uint j=0; j<sommetsDestination->size();j++)
                {
                    remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
                }
            }
            sort(sommetsDestination->begin(), sommetsDestination->end());
            return;
        }
        else{
            for(uint i =0; i<sommets_adj.size(); i++){
                if(In_tab(*sommetsDestination,sommets_adj.at(i))!=1 && sommetsSource->size()!=1){
                    sommetsDestination->push_back(sommets_adj.at(i));
                    poids+=(*g)[sommets_adj.at(i)]._weight;
                    suprim_val(*sommetsSource, sommets_adj[i]);
                }
                if(poids>poids_max || sommetsSource->size()==1)
                    break;
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

            if(poids>poids_max  || sommetsSource->size()==1){
                for (uint i=0; i<sommetsSource->size();i++)
                {
                    for (uint j=0; j<sommetsDestination->size();j++)
                    {
                        remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
                    }
                }
                sort(sommetsDestination->begin(), sommetsDestination->end());
                return;
            }

            sommets_adj.clear();
            cpt++;
        }

    }

    /*for (uint i=0; i<sommetsSource->size();i++)
    {
        for (uint j=0; j<sommetsDestination->size();j++)
        {
            remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
        }
    }*/
    sort(sommetsDestination->begin(), sommetsDestination->end());
}


void Iter_2l(EntiersEntiers &part, int nbr_parties, UnorientedGraph *g,
             const std::string &nom_cut, int nbr_tirage, const std::string &nom_strat, int distance)
{
			
	for(int i = 0; i<floor(log(nbr_parties)/log(2)); i++)
    {
		for(int j = 0; j< pow(2,i);j++)
        {
			if(distance == -1){
				Optimisation_method_neighbour(g,part,j,nbr_tirage, nom_cut, nom_strat);
			}else{
				Optimisation_method_neighbour_distance(g,part,j,nbr_tirage, distance, nom_cut, nom_strat);
			}
			
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
				Optimisation_method_neighbour(g,Partition,j,nbr_tirage, nom_cut, nom_strat);
			}else{
				Optimisation_method_neighbour_distance(g,Partition,j,nbr_tirage, distance, nom_cut, nom_strat);
			}
        }
    }
}

void gggp_pond(UnorientedGraph *g, Entiers *sommetsSource,
               Entiers *sommetsDestination, EntiersEntiers &Partition, int rand, int distance)
{
    Entiers sommets_adj;
    if(sommetsSource->size()==1){
        Entiers tailles;
        for(uint i=0;i<Partition.size();i++){
            tailles.push_back(Partition.at(i)->size());
        }
        uint tmp=*max_element(tailles.begin(),tailles.end());
        for(uint i=0; i<Partition.size();i++){
            if(Partition.at(i)->size()==tmp)
            {
                gggp_pond(g,Partition[i],sommetsDestination,Partition,rand_fini(0,Partition.at(i)->size()),distance);
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
    std::vector<float> sommets_cut;
    float cut;
    if(distance == -1){
		poids=(*g)[sommetsSource->at(rand)]._weight;
		cut = Degree(*g,sommetsSource->at(rand));
		//std::cout<<"verif rand : "<<rand<<std::endl;
		sommetsDestination->push_back(sommetsSource->at(rand));
		sommetsSource->erase(sommetsSource->begin() + rand);
		
	}else{
		poids=(*g)[rand]._weight;
		cut = Degree(*g,rand);
		sommetsDestination->push_back(rand);
		suprim_val(*sommetsSource,rand);
	}

    while(poids<poids_max && sommetsSource->size()>1)
    {
        Liste_Voisin(*sommetsDestination,sommets_adj,*g);
        if(sommets_adj.size()==0)
        {
            for (uint i=0; i<sommetsSource->size();i++)
            {
                for (uint j=0; j<sommetsDestination->size();j++)
                {
                    remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
                }
            }
            sort(sommetsDestination->begin(), sommetsDestination->end());
            return;
        }
        else{
            sort(sommets_adj.begin(), sommets_adj.end());

            for(uint i=0;i<sommets_adj.size();i++)
            {
                sommets_cut.push_back(modif_Cout_coupe(*sommetsDestination,sommets_adj.at(i),cut,g));
            }

            cut = *min_element(sommets_cut.begin(),sommets_cut.end());
            sommetsDestination->push_back(sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
            poids+=(*g)[sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]]._weight;
            suprim_val(*sommetsSource, sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
            suprim_val(sommets_adj, sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);

            sommets_cut.clear();

        }
    }
    
    sort(sommetsDestination->begin(), sommetsDestination->end());

    /*for (uint i=0; i<sommetsSource->size();i++)
    {
        for (uint j=0; j<sommetsDestination->size();j++)
        {
            remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
        }
    }*/
    
}

// void gggp_pond(UnorientedGraph *g, Entiers *sommetsSource,
//                Entiers *sommetsDestination, EntiersEntiers &Partition)
// {
//     int val;
//     Entiers sommets_adj;
//     if(sommetsSource->size()==1){
//         val=0;
//         //std::cout<<"Entré dans le debug ! "<<std::endl;
//         Entiers tailles;
//         for(uint i=0;i<Partition.size();i++){
//             tailles.push_back(Partition.at(i)->size());
//         }
//         uint tmp=*max_element(tailles.begin(),tailles.end());
//         for(uint i=0; i<Partition.size();i++){
//             if(Partition.at(i)->size()==tmp)
//             {
//                 gggp_pond(g,Partition[i],sommetsDestination,Partition);
//             	return;
//             }
//         }
//     }
//     else
//         val=rand_fini(0,sommetsSource->size()-1);//Tirage aléatoire de l'indice du premier sommet entre 0 et taille du tableau -1
//     	//std::cout<<"val : "<<sommetsSource->at(val)<<std::endl;
//     double poids_max=0;
//     for(uint i=0;i<sommetsSource->size();i++){
//         poids_max+=(*g)[sommetsSource->at(i)]._weight;
//     }
//     poids_max/=2.;
//     double poids=(*g)[sommetsSource->at(val)]._weight;
//     std::vector<float> sommets_cut;
//     float  cut = Degree(*g,sommetsSource->at(val));

//     sommetsDestination->push_back(sommetsSource->at(val));
//     sommetsSource->erase(sommetsSource->begin() + val);

//    // std::cout<<"taille sommetsSource avant le while : "<<sommetsSource->size()<<std::endl;
//     while(poids<poids_max && sommetsSource->size()>1)
//     {
//     	//std::cout<<"taille sommetsSource dans le while "<<sommetsSource->size()<<std::endl;
//         Liste_Voisin(*sommetsDestination,sommets_adj,*g);
//         if(sommets_adj.size()==0)
//         {
//             //std::cout<<"Je suis sorti car pas de voisin !!!! "<<std::endl;
//             for (uint i=0; i<sommetsSource->size();i++)
//             {
//                 for (uint j=0; j<sommetsDestination->size();j++)
//                 {
//                     remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
//                 }
//             }
//             sort(sommetsDestination->begin(), sommetsDestination->end());
//             return;
//         }
//         else{
//             sort(sommets_adj.begin(), sommets_adj.end());

//             /*std::cout<<"adj :"<<std::endl;
//             for(uint a = 0; a<sommets_adj.size(); a++){
//             	std::cout<<sommets_adj.at(a)<<std::endl;
//             }
//             std::cout<<std::endl;*/

//             for(uint i=0;i<sommets_adj.size();i++)
//             {
//                 sommets_cut.push_back(modif_Cout_coupe(*sommetsDestination, sommets_adj.at(i), cut, g));
//                 // sommets_cut.push_back(Cout_coupe_pond(*sommetsDestination,sommets_adj[i],*g));
//             }
//             /*std::cout<<"cut :"<<std::endl;
//             for(uint a = 0; a<sommets_cut.size(); a++){
//             	std::cout<<sommets_cut.at(a)<<std::endl;
//             }
//             std::cout<<std::endl;*/
//             sommetsDestination->push_back(sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
//             //std::cout<<"Sommet deplacé : "<<sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]<<std::endl;
//             poids+=(*g)[sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]]._weight;
//             suprim_val(*sommetsSource, sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);
//             suprim_val(sommets_adj, sommets_adj[recherche_val2(sommets_cut,*min_element(sommets_cut.begin(),sommets_cut.end()))]);

//             sommets_cut.clear();

//         }

//         /*for(uint i =0; i<sommetsSource->size();i++){
//         	std::cout<<sommetsSource->at(i)<<",";
//         }
//         std::cout<<std::endl;
//         for(uint i =0; i<sommetsDestination->size();i++){
//         	std::cout<<sommetsDestination->at(i)<<",";
//         }
//         std::cout<<std::endl;*/

//     }

//     for (uint i=0; i<sommetsSource->size();i++)
//     {
//         for (uint j=0; j<sommetsDestination->size();j++)
//         {
//             remove_edge(sommetsSource->at(i),sommetsDestination->at(j),*g);
//         }
//     }
//     sort(sommetsDestination->begin(), sommetsDestination->end());

//     //std::cout<<"fin du gggp_pond"<<std::endl;
// }

// void Iter_2l(EntiersEntiers &part, int nbr_parties, UnorientedGraph *g,
//              const std::string &nom)
// {
//     if (nom!="gggp_pond"){
//         //std::cout<<"je jsuis dans gggp"<<std::endl;

//         for(int i = 0; i<floor(log(nbr_parties)/log(2)); i++)
//         {
//             //std::cout<<"Et un tours de plus !!!! "<<std::endl;
//             for(int j = 0; j< pow(2,i);j++)
//             {
//                 Entiers *Q = new Entiers();
//                 gggp(g,part[j],Q,part);
//                 part.push_back(Q);
//             }

//         }
//     } else {
//         //std::cout<<"je jsuis dans gggp_pond"<<std::endl;

//         for(int i = 0; i<floor(log(nbr_parties)/log(2)); i++)
//         {
//             //std::cout<<"Et un tours de plus !!!! "<<std::endl;
//             for(int j = 0; j< pow(2,i);j++)
//             {
//                 Entiers *Q = new Entiers();
//                 gggp_pond(g,part.at(j),Q,part);
//                 //std::clog<<"sortie du gggp_pond"<<std::endl;
//                 part.push_back(Q);
//             }
//             //std::cout<<"\n"<<std::endl;

//         }
//     }
// }

// void bissectionRec(UnorientedGraph *g, EntiersEntiers &Partition,
//                    int nbr_parties, const std::string &nom)
// {
//     if((nbr_parties&(nbr_parties-1))==0)
//     {
//         //std::cout<<"C'est de la forme 2l : "<<nbr_parties<<std::endl;
//         Iter_2l(Partition,nbr_parties,g,nom);
//     }
//     else
//     {
//         int puissance_2=0;

//         Entiers tailles;

//         while(pow(2,puissance_2)<nbr_parties)
//             puissance_2++;

//         Iter_2l(Partition,pow(2,puissance_2-1),g,nom);

//         for(unsigned int i = 0; i< Partition.size() -1 ; i++)
//         {
//             for(EntiersEntiers::iterator it1 = Partition.begin() + i ; it1!=Partition.end(); it1++)
//             {
//                 if((*it1)->size() > Partition.at(i)->size())
//                     Partition.at(i)->swap(**it1);
//             }
//         }

//         for(int j = 0; j<nbr_parties-pow(2,puissance_2-1);j++)
//         {
//             Entiers *Q = new Entiers();
//             if(nom!="gggp_pond")
//                 gggp(g,Partition.at(j),Q,Partition);
//             else
//                 gggp_pond(g,Partition.at(j),Q,Partition);
//             Partition.push_back(Q);
//         }
//     }
//     // std::cout<<"Partition avant affinage "<<std::endl;
//     // for(uint i = 0 ; i<Partition.size(); i++){
//     // 	for(uint j = 0 ; j<Partition.at(i)->size(); j++){
//     // 		std::cout<<Partition.at(i)->at(j)<<" ";
//     // 	}
//     // 	std::cout<<"\n"<<std::endl;
//     // }
// }

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
                           Connections& connections,
                           std::vector<double> &Cut, int distance)
{
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
    
    for(int i = 0; i<num_vertices(*baseg.at(baseg.size() - 1)); i++){
		part->push_back(i);
	}
	Partition.push_back(part);
	
	UnorientedGraph copy_graph;
	boost::copy_graph(*baseg.at(baseg.size()-1),copy_graph);
    // std::cout<<"Graphe contracté : "<<std::endl;
    // for (uint i = 0; i< baseg.size(); i++) {
    //     tie(vertexIt, vertexEnd) = vertices(*baseg[i]);
    //     for (; vertexIt != vertexEnd; ++vertexIt) {
    //         std::cout << (*baseg[i])[*vertexIt]._index
    //                   << " est connecté avec ";
    //         tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
    //         		*baseg[i]);
    //         for (; neighbourIt != neighbourEnd; ++neighbourIt)
    //             std::cout << (*baseg[i])[*neighbourIt]._index << " ";
    //         std::cout << " et son poids est de "
    //                   << (*baseg[i])[*vertexIt]._weight<<std::endl;
    //     }
    //     std::cout << std::endl;
    // }
    
    if(type_methode == "gggp" || type_methode == "ggp"){
		bissectionRec(baseg.at(baseg.size()-1),Partition,nbr_parties,type_cut,nbr_tirage,type_methode, distance);
		double best_cut = Cut_cluster(Partition,copy_graph,type_cut);
		Cut.push_back(best_cut);
		//std::cout<<"Meilleur coût de coupe : "<<best_cut<<std::endl;
		//std::cout<<std::endl;
    }
    else
    	Partition = Random_partitioning(baseg.at(baseg.size()-1),nbr_parties);
    // std::cout<<std::endl;

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

    for(uint y =0; y<taille_list;y++){
    	if(proj != true){
    		// std::cout<<"Projection "<<std::endl;
    		projection(Partition,lit);
    		// std::cout<<std::endl;
    		double cut = Cut_cluster(Partition,*baseg.at(baseg.size()-2-y),type_cut);
    		//std::cout<<"Cout de coupe avant affinage : "<<cut<<std::endl;
    		//std::cout<<std::endl;
    		//std::cout<<"Affinage "<<std::endl;
    		if(choix_affinage=="charge")
    			Affinage_equilibrage_charge(baseg.at(baseg.size()-2-y),Partition);
    		else
    			Affinage_recherche_locale(baseg.at(baseg.size()-2-y),Partition,cut,type_cut);

    		lit--;
    	}
    	else{
    		//std::cout<<"Pas de projection "<<std::endl;
    		// std::cout<<std::endl;

    		/*if(nbr_parties != num_vertices(*g)){
    			// std::cout<<"Affinage "<<std::endl;
        		double cut = Cut_cluster(Partition,*graph_origin,type_cut);
        		std::cout<<"Cout de coupe avant affinage : "<<cut<<std::endl;
    			if(choix_affinage=="charge")
    				Affinage_equilibrage_charge(graph_origin,Partition);
    			else{
    				Affinage_recherche_locale(graph_origin,Partition,cut,type_cut);
    				std::cout<<"Cout de coupe après affinage : "<<cut<<std::endl;
    			}
    		}*/
    		// else
    			// std::cout<<"Pas d'affinage "<<std::endl;
    	}

    }

    OrientedGraphs Graphes = Graph_Partition(Partition, go, graph_origin, outputedgelist,
                                             inputedgelist, connections);
	std::vector<std::string> color;
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
    
   /*std::ofstream fichier2 ("../../sortie_graphe/graph_partition_38_4_1.txt", std::ios::out);
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
	fichier2.close();*/

	//double cut = Cut_cluster(Partition,*graph_origin,"cut");
	// std::cout<<"Cout de coupe engendré par le partitionnement: "<<cut<<std::endl;


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
	double cut=1000000000.;
	//boost::copy_graph(*g,*copy_g_ref);

	for (int i=0 ; i<Partition.at(index_partition)->size() ; i++)
		Random_list_vertices.push_back(i);
	for (int j=0 ; j<Partition.at(index_partition)->size()-1 ; j++) {
		int rand_pos = rand()%(Partition.at(index_partition)->size()-j)+j;
		int tmp      = Random_list_vertices[j];
		Random_list_vertices[j] = Random_list_vertices[rand_pos];
		Random_list_vertices[rand_pos] = tmp;
	}
	
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
		
		/*std::cout<<"Ensembles verification  !!! "<<std::endl;
		std::cout<<"Ensemble 1 : ";
		for(uint i = 0; i < tmp_part->size(); i++){
			std::cout<<tmp_part->at(i)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<"Ensemble 2 : ";
		for(uint j = 0; j < part2->size(); j++){
			std::cout<<part2->at(j)<<" ";
		}
		std::cout<<std::endl;*/
		
		//gggp_pond(copy_g,tmp_part,part2,Partition);

		if(name_strat == "gggp"){
			gggp_pond(g,tmp_part,part2,Partition,Random_list_vertices.at(k),-1);
		} else if(name_strat == "ggp"){
			ggp(g,tmp_part,part2,Partition,Random_list_vertices.at(k),-1);
		} else {
			std::cout<<"Nom de stratégie de partitionnement non valide ! "<<std::endl;
		}
		
		/*std::cout<<"Ensemble 1 : ";
		for(uint i = 0; i < tmp_part->size(); i++){
			std::cout<<tmp_part->at(i)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<"Ensemble 2 : ";
		for(uint j = 0; j < part2->size(); j++){
			std::cout<<part2->at(j)<<" ";
		}
		std::cout<<std::endl;
		std::cout<<std::endl;*/
		new_cut = Best_Cut_cluster(Partition, tmp_part, part2, index_partition,*g,name_cut);
		//std::cout<<"Cout de coupe initial : "<<cut<<" "<<"Résutlat : "<<new_cut<<std::endl;
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
	/*Modification des informations*/
	Partition.at(index_partition)=part_cour_cons;
	Partition.push_back(part2_cons);
	//g=copy_g_ref;
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
			gggp_pond(g,tmp_part,part2,Partition,val,distance);
		} else if(name_strat == "ggp"){
			ggp(g,tmp_part,part2,Partition,val,distance);
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
