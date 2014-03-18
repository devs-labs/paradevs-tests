/**
 * @file tests/boost_graph/partitioning/utils.cpp
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

#include <tests/boost_graph/partitioning/utils.hpp>

#include <algorithm>
#include <iostream>
#include <fstream>

namespace paradevs { namespace tests { namespace boost_graph {

UnorientedGraph::vertex_iterator vertexIt, vertexEnd;
UnorientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

OrientedGraph::vertex_iterator vertexIto, vertexEndo;
OrientedGraph::adjacency_iterator neighbourIto, neighbourEndo;

struct myclass
{
    bool operator() (Entiers *i, Entiers *j)
    { return i->at(0) < j->at(0); }
} myobject;

struct myclass2
{
    bool operator() (Entiers *i, Entiers *j, UnorientedGraph *g)
    { return Calcul_poids(i,g) < Calcul_poids(j,g); }
} mon_tri;

struct myclass3
{
	bool operator() (Entiers *i, Entiers *j)
	{ return i->size() > j->size(); }
} myobject_taille;

/**
 * Fonction de verification de la connexité d'un graphe
 * @param *g : adresse d'un graphe de type boost graphe undirected
 * @param Partition : vecteur contenant des vecteurs d'entiers [tableau contenant les parties de la partition]
 * @param part : vecteur d'entier (une partie de la partition)
 * @return un booleen
 */
bool Est_connexe(UnorientedGraph *g, EntiersEntiers Partition, Entiers &part)
{
    /*
     * Copie du graphe contenu par l'adresse *g
     */
    UnorientedGraph copie_g;
    copie_g = *g;

    /*
     * Modification du graphe copié afin de générer des sous graphes liés aux différentes parties
     */
    for (uint i=0; i<Partition.size()-1;i++)
    {
        for (uint j=1+i; j<Partition.size();j++)
        {
            for (uint k=0; k<Partition.at(i)->size();k++)
            {
                for (uint y=0; y<Partition.at(j)->size();y++)
                {
                    remove_edge(Partition.at(i)->at(k),Partition.at(j)->at(y),copie_g); //suppression de certains arcs
                }
            }
        }
    }

    /*
     * Objectif : déterminer s'il existe un chemin reliant tous les noeuds d'une même partie
     * Méthode : partir d'un sommet de départ tiré aléatoirement dans la partie "part" et parcourir l'ensemble de ces voisins.
     * Si le voisin recontré n'est pas contenu dans le vecteur "sommets" il est ajouté. Le processus est répété pour chaque
     * nouveau sommet ajouté au vecteur.
     * Résultat : si le nombre de sommets contenu dans le vecteur "part" est égale au nombre de sommets du vecteur "sommets"
     * alors le graphe est connexe
     */

    int val;
    Entiers sommets;

    if(part.size()==1)
        val = 0;
    else
        val=rand_fini(0,part.size()-1); //tirage aléatoire d'un sommets

    int vertex = part.at(val);
    sommets.push_back(vertex); //ajout du sommets à la lsite des sommets parcouru

    /*
     * Recherche de voisins n'appartenant pas à la liste des sommets parcourus
     */
    for(uint i = 0;i<sommets.size();i++){
        tie(neighbourIt, neighbourEnd) = adjacent_vertices(sommets.at(i),copie_g);
        for (; neighbourIt != neighbourEnd; ++neighbourIt){
            if(In_tab(sommets,*neighbourIt)!=1)
                sommets.push_back(*neighbourIt);
        }
    }

    /*
     * Retour de la réponse vrai ou faux
     */
    if(part.size()!=sommets.size())
        return false;
    else
        return true;

}

/**
 * Fonction de projection
 * @param Partition : vecteur contenant des vecteurs d'entiers [tableau contenant les parties de la partition]
 * @param lit : itérateur sur une liste contenant une vecteur de vecteur d'entier
 * @return
 */

/*
 * Objectif : obtenir la correspondance entre les sommets d'un graphe Gn et celui de Gn-1
 * Méthode : modification des sommets contenus dans "Partition" à l'aide de la liste de correspondance *lit
 */

void projection(EntiersEntiers &Partition,ListEntiersEntiers::iterator lit)
{

    /*
     * Création d'un nouveau vecteur contenant les adresses d'autres vecteur d'entiers.
     * Celui-ci est conçu pour recevoir les sommets contenus dans la liste de correspondance,
     * correspondant à la projection des sommets du graphe Gn à celui de Gn-1
     */
    EntiersEntiers new_partition;
    for(uint i = 0; i< Partition.size() ; i++)
    {
        Entiers *new_part = new Entiers();
        for(uint j = 0 ; j<Partition.at(i)->size() ; j++)
        {
            for(uint k = 0; k<((*lit)->at(Partition.at(i)->at(j)))->size();k++){
                new_part->push_back((*lit)->at(Partition.at(i)->at(j))->at(k));
            }

        }
        new_partition.push_back(new_part);
    }


    /*
     * Désalocation mémoire des pointeurs que contient "Partition"
     */
    for(EntiersEntiers::iterator it = Partition.begin(); it != Partition.end(); it++)
    {
        delete *it;
        *it = NULL;
    }

    Partition = new_partition; // copie de new_partition dans Partition
    for(uint i =0; i<Partition.size(); i++) {
        // permet de trier chaque  sous vecteur de "Partition"
        std::sort(Partition[i]->begin(),Partition[i]->end());
    }

    new_partition.clear();
}

/**
 * Fonction qui calcul le poids d'une partie
 * @param * part : adresse d'un vecteur d'entier, ici une partie de la partition
 * @param *g : adresse d'un graphe de type boost graphe undirected
 * @return un type double contenant le poids associé à la partie
 */
double Calcul_poids(Entiers *partie, UnorientedGraph *g)
{
    double poids=0; // initialisation du poids à 0

    /*
     * Pour chaque sommet de la partie concerné on ajoute son poids au poids total
     */
    for(uint j = 0; j<partie->size();j++){
        poids+=(*g)[partie->at(j)]._weight;
    }

    return poids;
}

/**
 * Fonction d'affinage suivant un critère de poids
 * @param *g : adresse d'un graphe de type boost graphe undirected
 * @param Partition : vecteur contenant des vecteurs d'entiers [tableau contenant les parties de la partition]
 * @return modification de la partition
 */
void Affinage_equilibrage_charge(UnorientedGraph *g, EntiersEntiers &Partition)
{
    /*
     * Calcule de la somme des poids du graphe et le poids moyen à atteindre
     */
    double poids_moyen = 0.;

    for(uint i = 0; i < num_vertices(*g); i++) {
        poids_moyen += (*g)[i]._weight;
    }

    // détermination du poids moyen à atteindre pour chaque partie
    poids_moyen /= Partition.size();

    std::vector < double > poids_parties;

    /*
     * Calcul du poids de chaque partie de la partition
     */
    for (uint i = 0; i < Partition.size(); i++) {
        double tmp = Calcul_poids(Partition.at(i),g);

        poids_parties.push_back(tmp);
    }

    std::clog << "Poids initial des parties : " << std::endl;

    // for (uint i = 0; i < poids_parties.size(); i++){
    //     std::cout << poids_parties.at(i) << " ";
    // }
    // std::cout << "\n" << std::endl;

    /*
     * Le critère d'amélioration consiste à faire tendre vers 0 la somme
     * des écarts entre le poids des parties et le poids moyen
     * le "critere" est la somme pour chaque partie de la différence
     * en valeurs absolues du poids
     * d'une partie moins le poids moyen divisé par le nombre de parties
     */

    double critere = 0.;

    for (uint i = 0; i < poids_parties.size(); i++){
        critere += abs(poids_parties.at(i) - poids_moyen);
    }
    critere /= Partition.size();

    // on conserve le poids maximum
    double p_max = *max_element(poids_parties.begin(), poids_parties.end());

    // std::cout << "Valeurs du criètre de départ : " << critere << std::endl;

    // création d'un second critère légérement plsu faible que le premier
    double best_critere = critere - 1e-7;
    uint nbr_passage = 1; // initialisation du nombre de passages à 1

    /*
     * Tant que le critère n'est pas amélioré etque le nombre de
     * passage est inférieur au nombre de parties on réalise
     * des modifications sur la partition
     */
    while (best_critere < critere or nbr_passage < Partition.size()) {
        critere = best_critere; //critere devient best_critere

        // recherche la partie associé au poids maximum
        int cpt = recherche_val_double(poids_parties,p_max);
        bool decision = false; //initialisatio d'un booleen a false
        int nbr_pass_interne = 0;

        /*
         * tirage aléatoire des sommets de la partie de poids maximum
         */
        Entiers random_orders(Partition.at(cpt)->size());
        for (uint i=0 ; i<Partition.at(cpt)->size() ; i++)
            random_orders.at(i)=Partition.at(cpt)->at(i);

        for (uint j=0 ; j<Partition.at(cpt)->size()-1 ; j++) {
            int rand_pos = (rand() % Partition.at(cpt)->size()-j)+j;
            int tmp = random_orders[j];
            random_orders[j] = random_orders[rand_pos];
            random_orders[rand_pos] = tmp;
        }

        /*
         * Si le nombre de sommets d'une partie excéde les 400, on tire au hasar 400 sommets sans remise
         * et on effectue les modifications (ceci permet d'eviter une explosion des temps de calcul)
         */
        int size;

        if(Partition.at(cpt)->size()>400)
            size = 400;
        else
            size = Partition.at(cpt)->size();

        /*
         * Seconde boucle Tant que sur les sommets d'une partie.
         * Tant que le critere booleen est vrai et que le nombre de passe interne est inférieur au seuil size faire
         */
        while(decision!=true && nbr_pass_interne < size){
            int vertex = random_orders.at(nbr_pass_interne); //tirage d'un sommets dans la parite de poids maximum
            Entiers community = Neigh_community(g,Partition,vertex,cpt); // recherche des communautés voisines à ce sommet (s'il est au bord)
            if(community.size()!=0) // s'il existe au moins une communauté voisine
            {
                std::vector<double> new_poids_parties; // initialisation d'un nouveau vecteur contenant des doubles
                std::vector<double> tmp_critere; // initialisation d'un nouveau vecteur contenant des doubles

                /*
                 * Pour chacune des parties (communauté) voisine au sommet vertexs faire
                 */
                for(uint k = 0; k < community.size();k++)
                {
                    new_poids_parties = poids_parties; //copie du tableau de poids des parties dans new_poids_parties

                    /*
                     * Modification des poids des parties :
                     * on ajoute le poids du sommets à la partie voisine
                     * et on soustrait son poids à sa partie d'origine
                     */
                    new_poids_parties.at(community.at(k))+=(*g)[vertex]._weight;
                    new_poids_parties.at(cpt)-=(*g)[vertex]._weight;

                    /*
                     * Calcul ldu nouveau critère associé à cette modification
                     */
                    double new_critere = 0.;

                    for(uint i = 0; i<poids_parties.size();i++){
                        new_critere += abs(new_poids_parties.at(i)-poids_moyen);
                    }
                    new_critere/=Partition.size();
                    tmp_critere.push_back(new_critere); // enregistrement du résutlat dans le tableau tmp_critere
                }
                double val_min = *min_element(tmp_critere.begin(),tmp_critere.end()); // enregistrement de la valeur minimale du tableau tmp_critere
                int val = recherche_val_double(tmp_critere,val_min); // recherche de la communauté correspondant à cette valeur

                /*
                 * Si la valeur associé est plus petite et que la partie selectionné n'est pas vérouillée faire
                 */
                if(val_min<critere && poids_parties.at(val)!=-1)
                {
                    /*
                     * On change le sommet vertex de partie, il est déplacé vers la partie
                     * qui permet la meilleure amélioration du critère
                     */
                    Partition.at(community.at(val))->push_back(vertex);
                    suprim_val(*Partition.at(cpt),vertex);
                    std::sort(Partition.at(community.at(val))->begin(), Partition.at(community.at(val))->end());

                    /*
                     * Vérification de la sauvegarde de la connexsité,
                     * si se n'est pas le cas retour à l'état précédent
                     */
                    if(Est_connexe(g,Partition,*Partition.at(cpt))!=1)
                    {
                        suprim_val(*Partition.at(community.at(val)),vertex);
                        Partition.at(cpt)->push_back(vertex);
                        std::sort(Partition.at(cpt)->begin(), Partition.at(cpt)->end());
                        // std::cout<<" C'EST MORT RETOUR EN ARRIERE ! "<<std::endl
                            ;
                    }
                    else
                    {
                        poids_parties = new_poids_parties;
                        decision = true;
                        // std::cout<<" Modification reussi ! "<<std::endl;
                    }
                }
            }
            nbr_pass_interne++;
        }
        /*
         * Si aucune modification n'a été réalisé pour cett partie de poids maximum
         */
        if(decision==false)
        {
            nbr_passage++; // augmentation du nombre de passage
            poids_parties.at(cpt)=-1; // vérrouillage de la partie
            std::clog<<"nouveau passag ! "<<std::endl;
        }
        else
        {
            best_critere = 0.;

            for(uint i = 0; i<poids_parties.size();i++){
                best_critere += abs(poids_parties.at(i)-poids_moyen);
            }
            best_critere/=Partition.size();
            nbr_passage = 0;
        }

        // std::clog<<"Poids des parties modifié : "<<std::endl;
        // for(uint i = 0; i<poids_parties.size(); i++){
        //     std::cout<<poids_parties.at(i)<<" ";
        // }
        // std::cout<<"\n"<<std::endl;
        p_max = *max_element(poids_parties.begin(),poids_parties.end());
        // std::cout<<"Valeurs du criètre : "<<critere<<std::endl;
        // std::cout<<"Valeurs du best_criètre : "<<best_critere<<std::endl;
        // std::cout<<"Nombre de passage : "<<nbr_passage<<std::endl;
        // std::cout<<"\n"<<std::endl;

    }
}

Entiers Neigh_community(UnorientedGraph *g, EntiersEntiers &Partition, int vertex, int comm_in)
{
    Entiers community;
    int comm;
    tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex,*g);
    for (; neighbourIt != neighbourEnd; ++neighbourIt){
        comm = In_community_dichotomie(Partition,*neighbourIt);
        if(In_tab(community,comm)!=1 && comm!=comm_in)
            community.push_back(comm);
    }
    return community;
}

void Affinage_recherche_locale(UnorientedGraph *g, EntiersEntiers &Partition, double &cut, std::string name)
{
    Entiers random_orders(num_vertices(*g)); //gestion d'un tableau contenant tout les sommets et ranger de façon aléatoire

    for (uint i=0 ; i<num_vertices(*g) ; i++)
        random_orders.at(i)=i;

    for (uint j=0 ; j<num_vertices(*g)-1 ; j++) {
        int rand_pos = (rand() % num_vertices(*g)-j)+j;
        int tmp = random_orders[j];
        random_orders[j] = random_orders[rand_pos];
        random_orders[rand_pos] = tmp;
    }
    uint size = random_orders.size();

    if(num_vertices(*g)>500)
        size=500;

    std::vector<std::vector<double> > tabe_cut;

    //std::cout<<"Passage : "<<Partition.size()<<std::endl;
    for(uint k = 0; k < Partition.size();k++){
        std::vector<double> tmp;
        double vol = 0.;
        double cut = Modif_Cut_one_cluster(*Partition.at(k), *g, vol, name);
        tmp.push_back(cut);
        tmp.push_back(vol);
        tabe_cut.push_back(tmp);
    }

    for(uint i = 0; i < size; i++){
        if(random_orders.at(i)!=-1){
            int vertex = random_orders.at(i);
            //std::cout<<vertex<<std::endl;
            int comm = In_community_dichotomie(Partition, vertex);
            Entiers community = Neigh_community(g,Partition,vertex,comm);
            std::vector<double> tmp_cut;

            if(community.size()!=0 && Partition.at(comm)->size()!=1){
                tmp_cut = modif_cut_tmp(g,Partition,tabe_cut,vertex,comm,community,cut,name);
                /*for(uint z = 0; z<tmp_cut.size(); z++){
                    std::cout<<tmp_cut.at(z)<<std::endl;
                }
                std::cout<<"\n"<<std::endl;*/
                double cut_min = *min_element(tmp_cut.begin(),tmp_cut.end());
                //std::cout<<"cout de coupe minimum de la liste : "<<cut_min<<std::endl;
                if(cut_min<cut){
                    // std::clog<<"Changement ! "<<std::endl;
                    int tmp = recherche_val_double(tmp_cut,cut_min);
                    cut=cut_min;
                    Partition.at(community.at(tmp))->push_back(vertex);
                    suprim_val(*Partition.at(comm),vertex);
                    std::sort(Partition.at(community.at(tmp))->begin(), Partition.at(community.at(tmp))->end());
                    tabe_cut.clear();
                    for(uint k = 0; k < Partition.size();k++){
                        std::vector<double> tmp;
                        double vol = 0.;
                        double cut = Modif_Cut_one_cluster(*Partition.at(k), *g, vol, name);
                        tmp.push_back(cut);
                        tmp.push_back(vol);
                        tabe_cut.push_back(tmp);
                    }

                }
            }

            Modif_fonction_Gain_Cut(Partition,g,community,vertex,cut,name);


            /*if(Est_connexe(g,Partition,*Partition.at(comm))!=1)
              {
              suprim_val(*Partition.at(community.at(tmp)),vertex);
              Partition.at(comm)->push_back(vertex);
              std::sort(*Partition.at(comm));
              std::cout<<" C'EST MORT RETOUR EN ARRIERE ! "<<std::endl;
              }*/

        }
    }
}

double Modif_Cut_one_cluster(Entiers &cluster, UnorientedGraph &g, double &vol, std::string name)
{
	edge_t e1;
	bool found;
	double cpt= 0.;
	
	if(name == "norm"){
		for(uint i=0;i<cluster.size();i++){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(cluster.at(i), g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(cluster[i],g),vertex(*neighbourIt,g),g);
				if(In_tab(cluster,*neighbourIt)!=1){
					cpt+=g[e1]._weight;
				}
			}
		}
		vol = Cluster_Degree(g,cluster);
	} else if(name == "ratio"){
		for(uint i=0;i<cluster.size();i++){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(cluster.at(i), g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(cluster[i],g),vertex(*neighbourIt,g),g);
				if(In_tab(cluster,*neighbourIt)!=1){
					cpt+=g[e1]._weight;
				}
			}
		}
		vol = Cluster_Weight(g,cluster);
	}
	return cpt;

}

std::vector<double> modif_cut_tmp(UnorientedGraph *g, EntiersEntiers &Partition, std::vector<std::vector<double> > tabe_cut, int vertexs, int comm_in, Entiers community, double cut,std::string name){
	edge_t e1;
	bool found;
	//std::cout<<"le sommet tiré est : "<<vertexs<<std::endl;

	if(name == "cut")
	{
		std::vector<double> modif_cut(community.size());
		double cpt_comm_in;
		double cpt_comm_out;
		for(uint i =0; i<community.size(); i++){
			double tmp_cut = cut;
			cpt_comm_in=0;
			cpt_comm_out=0;

			tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertexs,*g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(vertexs,*g),vertex(*neighbourIt,*g),*g);
				if(In_tab(*Partition.at(comm_in),*neighbourIt)==1)
					cpt_comm_in+=(*g)[e1]._weight;
				else if(In_tab(*Partition.at(community.at(i)),*neighbourIt)==1)
					cpt_comm_out+=(*g)[e1]._weight;
			}

			tmp_cut+=cpt_comm_in;
			tmp_cut-=cpt_comm_out;

			modif_cut.at(i)=tmp_cut;
		}
		return modif_cut;
	}
	else if(name == "norm"){
		std::vector<double> modif_cut(community.size());
		double cpt_comm_in;
		double cpt_comm_out;
		double tmp_cut;

		for(uint i =0; i<community.size(); i++){
			std::vector<std::vector<double> > tab_cut = tabe_cut;

			tmp_cut =0.;
			cpt_comm_in=0.;
			cpt_comm_out=0.;

			tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertexs,*g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(vertexs,*g),vertex(*neighbourIt,*g),*g);
				if(In_tab(*Partition.at(comm_in),*neighbourIt)==1)
					cpt_comm_in+=(*g)[e1]._weight;
				else if(In_tab(*Partition.at(community.at(i)),*neighbourIt)==1)
					cpt_comm_out+=(*g)[e1]._weight;
			}

			cpt_comm_in/=2.;
			cpt_comm_out/=2.;

			tab_cut.at(comm_in).at(0)+=cpt_comm_in;
			tab_cut.at(comm_in).at(0)-=cpt_comm_out;
			tab_cut.at(comm_in).at(1)-= Degree(*g ,vertexs);

			tab_cut.at(community.at(i)).at(0)+=cpt_comm_in;
			tab_cut.at(community.at(i)).at(0)-=cpt_comm_out;
			tab_cut.at(community.at(i)).at(1)+= Degree(*g ,vertexs);

			for(uint j = 0; j < tab_cut.size();j++){
				tmp_cut+=((tab_cut.at(j).at(0))/(tab_cut.at(j).at(1)));
			}

			modif_cut.at(i)=tmp_cut;
		}
	}else if(name == "ratio"){
		std::vector<double> modif_cut(community.size());
		double cpt_comm_in;
		double cpt_comm_out;
		double tmp_cut;

		for(uint i =0; i<community.size(); i++){
			std::vector<std::vector<double> > tab_cut = tabe_cut;

			tmp_cut =0.;
			cpt_comm_in=0.;
			cpt_comm_out=0.;

			tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertexs,*g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(vertexs,*g),vertex(*neighbourIt,*g),*g);
				if(In_tab(*Partition.at(comm_in),*neighbourIt)==1)
					cpt_comm_in+=(*g)[e1]._weight;
				else if(In_tab(*Partition.at(community.at(i)),*neighbourIt)==1)
					cpt_comm_out+=(*g)[e1]._weight;
			}

			/*cpt_comm_in/=2.;
			cpt_comm_out/=2.;*/

			tab_cut.at(comm_in).at(0)+=cpt_comm_in;
			tab_cut.at(comm_in).at(0)-=cpt_comm_out;
			tab_cut.at(comm_in).at(1)-= (*g)[vertexs]._weight;

			tab_cut.at(community.at(i)).at(0)+=cpt_comm_in;
			tab_cut.at(community.at(i)).at(0)-=cpt_comm_out;
			tab_cut.at(community.at(i)).at(1)+= (*g)[vertexs]._weight;

			for(uint j = 0; j < tab_cut.size();j++){
				tmp_cut+=((tab_cut.at(j).at(0))/(tab_cut.at(j).at(1)));
			}

			modif_cut.at(i)=tmp_cut;
		}
		return modif_cut;
	}
}
void Modif_fonction_Gain_Cut(EntiersEntiers &Partition,UnorientedGraph *g, Entiers &community, int val, double &cut,std::string name)
{
	/*std::cout<<"Nombre de communauté voisine : "<<community.size()<<std::endl;
	std::cout<<"\n"<<std::endl;*/
	for(uint i = 0; i<community.size();i++){
		EntiersEntiers new_partition;
		for(uint k = 0; k < Partition.size();k++){
			Entiers * tmp = new Entiers();
			for(uint j = 0;j<Partition.at(k)->size();j++){
				tmp->push_back(Partition.at(k)->at(j));
			}
			new_partition.push_back(tmp);
		}

		/*std::cout<<"Avant Modification partition"<<std::endl;
		std::cout<<"************"<<std::endl;
		for(uint t = 0; t< new_partition.size() ; t++)
				{
					for(uint j = 0 ; j<new_partition.at(t)->size() ; j++)
					{
						std::cout<<new_partition.at(t)->at(j)<<std::endl;
					}
					std::cout<<"\n"<<std::endl;
				}
		std::cout<<"************"<<std::endl;*/


		new_partition.at(community.at(i))->push_back(val);
		suprim_val(*new_partition.at(In_community_dichotomie(Partition,val)),val);
		std::sort(new_partition.at(community.at(i))->begin(),
                    new_partition.at(community.at(i))->end());

		/*std::cout<<"Modification partition"<<std::endl;
		std::cout<<"************"<<std::endl;
		for(uint t= 0; t< new_partition.size() ; t++)
		{
			for(uint j = 0 ; j<new_partition.at(t)->size() ; j++)
			{
				std::cout<<new_partition.at(t)->at(j)<<std::endl;
			}
			std::cout<<"\n"<<std::endl;
		}
		std::cout<<"************"<<std::endl;*/

		double coupe = Cut_cluster(new_partition,*g,name);

		//std::cout<<"cout de coupe : "<<coupe<<std::endl;
		if(coupe<cut)
		{
			for(EntiersEntiers::iterator it = Partition.begin(); it != Partition.end(); it++)
			{
				delete *it;
				*it = NULL;
			}
			Partition = new_partition;
			cut = coupe;
		}
		else
		{
			for(EntiersEntiers::iterator it = new_partition.begin(); it != new_partition.end(); it++)
			{
				delete *it;
				*it = NULL;
			}
		}
	}
}

bool contraction_HEM(UnorientedGraph *g, Base_Graph &baseg, ListEntiersEntiers &liste_corr, int val_reduc, int &val_cpt){
	UnorientedGraph *gtmp = new UnorientedGraph(*g);
	Entiers Random_list_vertices; // Initialisation du tableau de sommets rangés aléatoirements
	EntiersEntiers *tableau_de_correspondance = new EntiersEntiers();
	edge_t e1,e2; // Iterateurs sur les arcs
	bool found;
	uint nbr_vertex = num_vertices(*gtmp);
	Entiers sommets_a_detruire; // Initialisation d'un tableau pret à recevoir les "sommets à détruire"
	/*
	 * Création d'un vecteur contenant l'ensemble des sommets du graphe. Ces sommets sont rangés
	 * aléatoirement afin de simuler un tirage aléatoire
	 */
	for (uint i=0 ; i<nbr_vertex ; i++)
		Random_list_vertices.push_back(i);
	for (uint j=0 ; j<nbr_vertex-1 ; j++) {
		int rand_pos = rand()%(nbr_vertex-j)+j;
		int tmp      = Random_list_vertices[j];
		Random_list_vertices[j] = Random_list_vertices[rand_pos];
		Random_list_vertices[rand_pos] = tmp;
	}

	/*
	 * Pour chaque sommet non verrouiller faire ....
	 */
	for(uint i=0; i<nbr_vertex; i++){
		int vertexs = Random_list_vertices[i];
		//std::cout<<"Le sommet tiré est : "<<vertexs<<std::endl;
		if(vertexs!=-1){
			Entiers liste_voisin = Liste_adjacence(*gtmp,vertexs,Random_list_vertices); // Recherche des sommets adjacents au sommets  tiré
			if(liste_voisin.size()!=0){
				/*
				 * S'il en existe au mois un sommet adjacent au sommet tiré qui n'est pas verrouillé, on
				 * choisi celui dont l'arc les reliants est le plus fort. Dans le cas où les arcs ont tous
				 * le même poids, on selectionne le sommet d'index le plus petit
				 */
				double poids_a = 0.;
				int best_vertexs = -1;
				for(uint j=0;j<liste_voisin.size();j++){
					tie(e1,found)=edge(vertex(vertexs,*gtmp),vertex(liste_voisin[j],*gtmp),*gtmp);
					if((*gtmp)[e1]._weight>poids_a){
						best_vertexs = liste_voisin[j];
						poids_a = (*gtmp)[e1]._weight;
					}
				}


				Entiers * couple = new Entiers(); // Initialisation du vecteur contenant le couple de sommet fusionné
				int vertex_delete = std::max(vertexs, best_vertexs); // Sommet d'index le plus grand (qui sera détruit)
				//std::cout<<"sommet détruit : "<<vertex_delete<<std::endl;
				int vertex_save = std::min(vertexs,best_vertexs); // Sommet d'identifiant le plus petit (qui sera conservé)
				//std::cout<<"sommet sauvé : "<<vertex_save<<std::endl;

				sommets_a_detruire.push_back(vertex_delete); // On ajoute le sommet détruit au tableau des sommets à détruire
				/*
				 * On ajoute au tableau "couple" le couple de sommet à fusionner
				 */
				couple->push_back(vertex_save);
				couple->push_back(vertex_delete);
				tableau_de_correspondance->push_back(couple); // Ajout du "couple" à la liste de correspondance

				remove_edge(vertex_save,vertex_delete,*gtmp); // Suppression de l'arc reliant le couple de sommets

				Entiers neigh_vertex_save; // Initialisation du vecteur contenant les sommets adjacents au "sommet sauvegardé"
				Entiers neigh_vertex_delete; // Initialisation du vecteur contenant les somemts adjacents au "sommet à détruire"
				tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex_save,*gtmp);

				/*
				 * Remplissage de ces deux tableaux à l'aide de la fonction adjacent_vertices de boost graph
				 * [La création de ces tableaux est nécéssaire du fait que certains arcs sont détruit au cours
				 * du processus]
				 */
				for (; neighbourIt != neighbourEnd; ++neighbourIt){
					neigh_vertex_save.push_back(*neighbourIt);
				}

				tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex_delete,*gtmp);
				for (; neighbourIt != neighbourEnd; ++neighbourIt){
					neigh_vertex_delete.push_back(*neighbourIt);
				}

				/*
				 * Recherche de sommets communs entre le "sommet sauvegardé" et le "sommet à détruire"
				 * S'il existe un tel sommet "v" alors on ajoute le poids de l'arcs (vertex_delet,v)
				 * à celui de l'arcs (vertex_save,v) et on détruit l'arcs reliant "v" au "sommet à détruire"
				 */
				for(uint j=0;j<neigh_vertex_delete.size();j++){
					if(In_tab(neigh_vertex_save,neigh_vertex_delete[j])==1){
						tie(e2,found)=edge(vertex(vertex_save,*gtmp),vertex(neigh_vertex_delete[j],*gtmp),*gtmp);
						tie(e1,found)=edge(vertex(vertex_delete,*gtmp),vertex(neigh_vertex_delete[j],*gtmp),*gtmp);
						(*gtmp)[e2]._weight+=(*gtmp)[e1]._weight;
						remove_edge(vertex_delete,neigh_vertex_delete[j],*gtmp);
					}
					else
					{
						tie(e1,found)=edge(vertex(vertex_delete,*gtmp),vertex(neigh_vertex_delete[j],*gtmp),*gtmp);
						add_edge(vertex_save,neigh_vertex_delete[j],EdgeProperties((*gtmp)[e1]._weight),*gtmp);
						remove_edge(vertex_delete,neigh_vertex_delete[j],*gtmp);
					}
				}

				(*gtmp)[vertex_save]._weight+=(*gtmp)[vertex_delete]._weight; // ajout du poids du sommet détruit au sommet conservé
				/*
				 * Vérouillage du "sommet sauvegardé" et du "sommet à détruire"
				 */
				Random_list_vertices[i]=-1;
				Random_list_vertices[recherche_val(Random_list_vertices,best_vertexs)]=-1;
				val_cpt--;
				// std::cout<<val_cpt<<std::endl;
			}
			else{
				/*
				 * Et si le sommet tiré ne possède pas de sommet adjacent non verrouillé
				 * alors on l'ajoute à la liste de correspondance des sommets et on
				 * le verrouille
				 */
				Entiers *couple = new Entiers();
				couple->push_back(Random_list_vertices.at(i));
				tableau_de_correspondance->push_back(couple);
				Random_list_vertices[i]=-1;
			}



			/*std::cout<<"Graphe contracté : "<<std::endl;
			tie(vertexIt, vertexEnd) = vertices(*gtmp);
			for (; vertexIt != vertexEnd; ++vertexIt) {
				std::cout << (*gtmp)[*vertexIt]._index
						<< " est connecté avec ";
				tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt,
						*gtmp);
				for (; neighbourIt != neighbourEnd; ++neighbourIt)
					std::cout << (*gtmp)[*neighbourIt]._index << " ";
				std::cout << " et son poids est de "
						<< (*gtmp)[*vertexIt]._weight<<std::endl;
			}
			std::cout << std::endl;*/
		}
		if(val_cpt == val_reduc){
			for(uint j=i+1; j <nbr_vertex; j++){
				if(Random_list_vertices[j] !=-1){
				Entiers *couple = new Entiers();
				couple->push_back(Random_list_vertices.at(j));
				tableau_de_correspondance->push_back(couple);}
			}
			break;
		}
	}

	std::sort(sommets_a_detruire.begin(), sommets_a_detruire.end()); // Trie dans l'ordre croissant des "sommets à détruire"
	// std::cout<<"\n"<<std::endl;
	/*
	 * Suppression des sommets de la liste "sommets à détruire". Cette suppression est
	 * effectuée dans l'ordre décroissant afin à maintenir à jour la renumérotation
	 * des sommets
	 */
	for(int j=(sommets_a_detruire.size()-1);j>-1;j--){
		//std::cout<<"Noeuds a supprimer : "<<sommets_a_detruire.at(j)<<std::endl;
		remove_vertex(sommets_a_detruire[j],*gtmp);
	}

	// std::clog<<"Affichage avant tri "<<std::endl;
	// for(uint k = 0;k<tableau_de_correspondance->size();k++){
	// 	for(uint v = 0; v<tableau_de_correspondance->at(k)->size();v++){
	// 		std::cout<<tableau_de_correspondance->at(k)->at(v)<<" ";
	// 	}
	// 	std::cout<<"\n"<<std::endl;
	// }
	std::sort(tableau_de_correspondance->begin(),tableau_de_correspondance->end(),myobject); // Trie dans l'ordre croissant des couples de sommets de la liste de correspondance

	// std::clog<<"Tableau de correspondance "<<std::endl;
	// for(uint k = 0;k<tableau_de_correspondance->size();k++){
	// 	for(uint v = 0; v<tableau_de_correspondance->at(k)->size();v++){
	// 		std::cout<<tableau_de_correspondance->at(k)->at(v)<<" ";
	// 	}
	// 	std::cout<<"\n"<<std::endl;
	// }

	liste_corr.push_back(tableau_de_correspondance);
	// std::cout<<"\n"<<std::endl;
	baseg.push_back(gtmp); // Ajout du graphe modifié à la "base des graphe"

	if(val_cpt == val_reduc)
		return true;
	else
		return false;

}

bool contraction_Random_Maching(UnorientedGraph *g, Base_Graph &baseg, ListEntiersEntiers &liste_corr, int val_reduc, int &val_cpt){
	UnorientedGraph *gtmp = new UnorientedGraph();
	*gtmp=*g;
	Entiers Random_list_vertices; // Initialisation du tableau de sommets rangés aléatoirements
	EntiersEntiers *tableau_de_correspondance = new EntiersEntiers();
	edge_t e1,e2; // Iterateurs sur les arcs
	bool found;
	uint nbr_vertex = num_vertices(*gtmp);
	Entiers sommets_a_detruire; // Initialisation d'un tableau pret à recevoir les "sommets à détruire"
	/*
	 * Création d'un vecteur contenant l'ensemble des sommets du graphe. Ces sommets sont rangés
	 * aléatoirement afin de simuler un tirage aléatoire
	 */
	for (uint i=0 ; i<nbr_vertex ; i++)
		Random_list_vertices.push_back(i);
	for (uint j=0 ; j<nbr_vertex-1 ; j++) {
		int rand_pos = rand()%(nbr_vertex-j)+j;
		int tmp      = Random_list_vertices[j];
		Random_list_vertices[j] = Random_list_vertices[rand_pos];
		Random_list_vertices[rand_pos] = tmp;
	}

	/*
	 * Pour chaque sommet non verrouiller faire ....
	 */
	for(uint i=0; i<nbr_vertex; i++){
		int vertexs = Random_list_vertices[i];
		if(vertexs!=-1){
			Entiers liste_voisin = Liste_adjacence(*gtmp,vertexs,Random_list_vertices); // Recherche des sommets adjacents au sommets  tiré
			if(liste_voisin.size()!=0){
				/*
				 * S'il en existe au mois un sommet adjacent au sommet tiré qui n'est pas verrouillé, on
				 * choisi celui dont l'arc les reliants est le plus fort. Dans le cas où les arcs ont tous
				 * le même poids, on selectionne le sommet d'identifiant le plus petit
				 */
				int tmp;
				if(liste_voisin.size()==1)
					tmp = 0;
				else
					tmp = rand_fini(0,liste_voisin.size()-1);

				int best_vertexs = liste_voisin.at(tmp);

				Entiers * couple = new Entiers(); // Initialisation du vecteur contenant le couple de sommet fusionné
				int vertex_delete = std::max(vertexs, best_vertexs); // Sommet d'indentifiant le plus grand (qui sera détruit)
				//std::cout<<"sommet détruit : "<<vertex_delete<<std::endl;
				int vertex_save = std::min(vertexs,best_vertexs); // Sommet d'identifiant le plus petit (qui sera conservé)
				//std::cout<<"sommet sauvé : "<<vertex_save<<std::endl;

				sommets_a_detruire.push_back(vertex_delete); // On ajoute le sommet détruit au tableau des sommets à détruire
				/*
				 * On ajoute au tableau "couple" le couple de sommet à fusionner
				 */
				couple->push_back(vertex_save);
				couple->push_back(vertex_delete);
				tableau_de_correspondance->push_back(couple); // Ajout du "couple" à la liste de correspondance

				remove_edge(vertex_save,vertex_delete,*gtmp); // Suppression de l'arc reliant le couple de sommets

				Entiers neigh_vertex_save; // Initialisation du vecteur contenant les somemts adjacents au "sommet sauvegardé"
				Entiers neigh_vertex_delete; // Initialisation du vecteur contenant les somemts adjacents au "sommet à détruire"
				tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex_save,*gtmp);

				/*
				 * Remplissage de ces deux tableaux à l'aide de la fonction adjacent_vertices de boost graph
				 * [La création de ces tableaux est nécéssaire du fait que certains arcs sont détruit au cours
				 * du processus]
				 */
				for (; neighbourIt != neighbourEnd; ++neighbourIt){
					neigh_vertex_save.push_back(*neighbourIt);
				}

				tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex_delete,*gtmp);
				for (; neighbourIt != neighbourEnd; ++neighbourIt){
					neigh_vertex_delete.push_back(*neighbourIt);
				}

				/*
				 * Recherche de sommets communs entre le "sommet sauvegardé" et le "sommet à détruire"
				 * S'il existe un tel sommet "v" alors on ajoute le poids de l'arcs (vertex_delet,v)
				 * à celui de l'arcs (vertex_save,v) et on détruit l'arcs reliant "v" au "sommet à détruire"
				 */
				for(uint j=0;j<neigh_vertex_delete.size();j++){
					if(In_tab(neigh_vertex_save,neigh_vertex_delete[j])==1){
						tie(e2,found)=edge(vertex(vertex_save,*gtmp),vertex(neigh_vertex_delete[j],*gtmp),*gtmp);
						tie(e1,found)=edge(vertex(vertex_delete,*gtmp),vertex(neigh_vertex_delete[j],*gtmp),*gtmp);
						(*gtmp)[e2]._weight+=(*gtmp)[e1]._weight;
						remove_edge(vertex_delete,neigh_vertex_delete[j],*gtmp);
					}
					else
					{
						tie(e1,found)=edge(vertex(vertex_delete,*gtmp),vertex(neigh_vertex_delete[j],*gtmp),*gtmp);
						add_edge(vertex_save,neigh_vertex_delete[j],EdgeProperties((*gtmp)[e1]._weight),*gtmp);
						remove_edge(vertex_delete,neigh_vertex_delete[j],*gtmp);
					}
				}

				(*gtmp)[vertex_save]._weight+=(*gtmp)[vertex_delete]._weight; // ajout du poids du sommet détruit au sommet conservé
				/*
				 * Vérouillage du "sommet sauvegardé" et du "sommet à détruire"
				 */
				Random_list_vertices[i]=-1;
				Random_list_vertices[recherche_val(Random_list_vertices,best_vertexs)]=-1;
				val_cpt--;
				// std::cout<<val_cpt<<std::endl;
			}
			else{
				/*
				 * Et si le sommet tiré ne possède pas de sommet adjacent non verrouillé
				 * alors on l'ajoute à la liste de correspondance des sommets et on
				 * le verrouille
				 */
				Entiers *couple = new Entiers();
				couple->push_back(Random_list_vertices.at(i));
				tableau_de_correspondance->push_back(couple);
				Random_list_vertices[i]=-1;
			}
		}
		if(val_cpt == val_reduc){
			for(uint j=i+1; j <nbr_vertex; j++){
				if(Random_list_vertices[j] !=-1){
				Entiers *couple = new Entiers();
				couple->push_back(Random_list_vertices.at(j));
				tableau_de_correspondance->push_back(couple);}
			}
			break;
		}
	}

	std::sort(sommets_a_detruire.begin(), sommets_a_detruire.end()); // Trie dans l'ordre croissant des "sommets à détruire"
	//std::cout<<"\n"<<std::endl;
	/*
	 * Suppression des sommets de la liste "sommets à détruire". Cette suppression est
	 * effectuée dans l'ordre décroissant afin à maintenir à jour la renumérotation
	 * des sommets
	 */
	for(int j=(sommets_a_detruire.size()-1);j>-1;j--){
		//std::cout<<"Noeuds a supprimer : "<<sommets_a_detruire.at(j)<<std::endl;
		remove_vertex(sommets_a_detruire[j],*gtmp);
	}

	/**std::clog<<"Affichage avant tri "<<std::endl;
	for(uint k = 0;k<tableau_de_correspondance->size();k++){
		for(uint v = 0; v<tableau_de_correspondance->at(k)->size();v++){
			std::cout<<tableau_de_correspondance->at(k)->at(v)<<" ";
		}
		std::cout<<"\n"<<std::endl;
	}*/
	std::sort(tableau_de_correspondance->begin(),tableau_de_correspondance->end(),myobject); // Trie dans l'ordre croissant des couples de sommets de la liste de correspondance

	// std::clog<<"Tableau de correspondance "<<std::endl;
	// for(uint k = 0;k<tableau_de_correspondance->size();k++){
	// 	for(uint v = 0; v<tableau_de_correspondance->at(k)->size();v++){
	// 		std::cout<<tableau_de_correspondance->at(k)->at(v)<<" ";
	// 	}
	// 	std::cout<<"\n"<<std::endl;
	// }

	liste_corr.push_back(tableau_de_correspondance);
	// std::cout<<"\n"<<std::endl;
	baseg.push_back(gtmp); // Ajout du graphe modifié à la "base des graphe"


	if(val_cpt == val_reduc)
		return true;
	else
		return false;

}

Entiers Liste_adjacence(UnorientedGraph &g, int vertexs,const Entiers &random_vertices){ // a revoir !!!!
	Entiers liste_voisin;
	tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertexs, g);
	for (; neighbourIt != neighbourEnd; ++neighbourIt){
		if(In_tab(random_vertices,*neighbourIt)==1)
			liste_voisin.push_back(*neighbourIt);
	}
	return liste_voisin;
}

int rand_fini(int a, int b){
	return rand()%(b-a)+a;
}

/**
 * Fonction de recherche d'une valeur dans un tableau.
 * @param tab
 * @param val
 * @return
 */
int recherche_val2(const std::vector<float> &tab,float val){
	int cpt=0;
	while(tab[cpt]!=val)
		cpt++;
	return cpt;
}

int recherche_val_double(const std::vector<double> &tab,double val){
	int cpt=0;
	while(tab[cpt]!=val)
		cpt++;
	return cpt;
}

int recherche_val(const Entiers &tab,int val){
	int cpt=0;
	while(tab[cpt]!=val)
		cpt++;
	return cpt;
}
/**
 * @param tab
 * @param i
 * @return
 */
int dichotomie(const Entiers &tab, int i){

	/* déclaration des variables locales à la fonction */
	int id;  //indice de début
	int ifin;  //indice de fin
	int im;  //indice de "milieu"

	/* initialisation de ces variables avant la boucle de recherche */
	id = 0;  //intervalle de recherche compris entre 0...
	ifin = tab.size();  //...et nbVal

	/* boucle de recherche */
	while ((ifin - id) > 1){
		im = (id + ifin)/2;  //on détermine l'indice de milieu
		if(tab[im] > i) ifin = im;  //si la valeur qui est à la case "im" est supérieure à la valeur recherchée, l'indice de fin "ifin" << devient >> l'indice de milieu, ainsi l'intervalle de recherche est restreint lors du prochain tour de boucle
		else id = im;  //sinon l'indice de début << devient >> l'indice de milieu et l'intervalle est de la même façon restreint
	}

	/* test conditionnant la valeur que la fonction va renvoyer */
	if (tab[id] == i) return id;  //si on a trouvé la bonne valeur, on retourne l'indice
	else return -1;  //sinon on retourne -1

}

/**
 * Fonction permettant de supprimer une case d'un tableau.
 * @param tab une référence sur un tableau d'entiers
 * @param i un indice dans tab
 */
void suprim_val(Entiers &tab,int i) {
	tab.erase(tab.begin() + dichotomie(tab,i));
}

/**
 * Détermine si une valeur se trouve dans un tableau.
 * @param tab une référence sur un tableau d'entiers
 * @param val une valeur
 * @return true si la valeur a été trouvée, false sinon
 */
bool In_tab(const Entiers &tab, int val)
{
	for (uint i=0; i < tab.size(); i++)
		if(tab[i]==val)
			return true;
	return false;
}

bool In_tab_dichotomie(const Entiers &tab, int val)
{
	if(dichotomie(tab,val)!=-1)
		return true;
	else
		return false;
}


void Liste_Voisin(const Entiers &P,Entiers &tab,const UnorientedGraph &g)
{
	tie(neighbourIt, neighbourEnd) = adjacent_vertices(P.at(P.size()-1), g);
	for (; neighbourIt != neighbourEnd; ++neighbourIt)
	{
		if((In_tab(tab,*neighbourIt) == false ) && (In_tab(P,*neighbourIt) == false ))
			tab.push_back(*neighbourIt);
	}
}

int Cout_coupe(Entiers P,int val, UnorientedGraph &g)
{
	int cpt=0;
	P.push_back(val);
	for(uint i=0;i<P.size();i++){
		tie(neighbourIt, neighbourEnd) = adjacent_vertices(P[i], g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt){
			if(In_tab(P,*neighbourIt)!=1){
				cpt++;
			}
		}
	}
	return cpt;
}

double Cut_one_cluster(const Entiers &cluster, UnorientedGraph &g, std::string name)
{
	if(name=="norm"){
		edge_t e1;
		bool found;
		double cpt=0.;
		for(uint i=0;i<cluster.size();i++){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(cluster.at(i), g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(cluster[i],g),vertex(*neighbourIt,g),g);
				if(In_tab(cluster,*neighbourIt)!=1){
					cpt+=g[e1]._weight;
				}
			}
		}
		double deg = Cluster_Degree(g,cluster);
		return cpt/deg;
	}
	else if(name == "cut"){
		edge_t e1;
		bool found;
		double cpt=0.;
		for(uint i=0;i<cluster.size();i++){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(cluster.at(i), g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(cluster.at(i),g),vertex(*neighbourIt,g),g);
				if(In_tab(cluster,*neighbourIt)!=1){
					cpt+=g[e1]._weight;
				}
			}
		}
		return cpt/2.;
	}
	else if(name == "ratio"){
		edge_t e1;
		bool found;
		double cpt=0.;
		for(uint i=0;i<cluster.size();i++){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(cluster.at(i), g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(cluster.at(i),g),vertex(*neighbourIt,g),g);
				if(In_tab(cluster,*neighbourIt)!=1){
					cpt+=g[e1]._weight;
				}
			}
		}
		double vol = Cluster_Weight(g,cluster);
		return (cpt/2.)/vol;
	}
	
	/*Vérification de la formule : doute sur le /2.*/
}

double Cut_cluster(const EntiersEntiers &tab_cluster,UnorientedGraph &g,std::string name)
{
	double cpt=0.;
	for(uint i=0;i<tab_cluster.size();i++){
		cpt+=Cut_one_cluster(*tab_cluster[i],g,name);
	}
	return cpt;
}

double Cout_coupe_pond(Entiers P, int val, UnorientedGraph &g)
{
    edge_t e1;
    bool found;
    double cpt=0;

    P.push_back(val);
    for(uint i=0;i<P.size();i++){
        tie(neighbourIt, neighbourEnd) = adjacent_vertices(P[i], g);
        for (; neighbourIt != neighbourEnd; ++neighbourIt){
            tie(e1,found)=edge(vertex(P[i],g),vertex(*neighbourIt,g),g);
            if(In_tab(P,*neighbourIt)!=1){
                cpt+=g[e1]._weight;
            }
        }
    }
    return cpt;
}


int In_community_dichotomie(const EntiersEntiers &part, int val)
{
    for (uint i = 0; i < part.size() ; i++) {
        if (In_tab_dichotomie(*part[i], val)) {
            return i;
        }
    }
    return -1;
}

double Degree(UnorientedGraph& g, int node)
{
    edge_t e1;
    bool found;
    double val = 0.;

    tie(neighbourIt, neighbourEnd) = adjacent_vertices(node, g);
    for (; neighbourIt != neighbourEnd; ++neighbourIt) {
        tie(e1, found) = edge(vertex(node, g), vertex(*neighbourIt, g), g);
        val += g[e1]._weight;
    }
    return val;
}

double Cluster_Degree(UnorientedGraph &g , const Entiers &cluster)
{
    double val = 0.;

    for(uint i = 0; i < cluster.size(); i++){
        val += Degree(g, cluster.at(i));
    }
    return val;
}

double Cluster_Weight(UnorientedGraph &g , const Entiers &cluster)
{
    double val = 0.;

    for(uint i = 0; i < cluster.size(); i++){
        val += g[cluster.at(i)]._weight;;
    }
    return val;
}

void List_edge_partie(Entiers *Partie, OrientedGraph *go, Edges &edge_partie,
                      OutputEdges &outputedgespartie){
    edge_to e1;
    //bool found;

    for(uint i = 0; i < Partie->size(); i++) {
        tie(neighbourIto, neighbourEndo) = adjacent_vertices(Partie->at(i),
                                                             *go);
        for (; neighbourIto != neighbourEndo; ++neighbourIto) {
            if(In_tab_dichotomie(*Partie,*neighbourIto)) {
                Edge new_edge;

                new_edge.first = Partie->at(i);
                new_edge.second = *neighbourIto;
                edge_partie.push_back(new_edge);
            } else {
                Edge new_edge;

                new_edge.first = Partie->at(i);
                new_edge.second = *neighbourIto;
                outputedgespartie.push_back(new_edge);
            }
        }
    }
}

void Global_Neigh_community(UnorientedGraph *g,
                            const EntiersEntiers &Partition,
                            Entiers *community, int vertex, int comm_in)
{
    int comm;

    tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex, *g);
    for (; neighbourIt != neighbourEnd; ++neighbourIt){
        comm = In_community_dichotomie(Partition, *neighbourIt);
        if (In_tab(*community,comm) != 1 and comm != comm_in)
            community->push_back(comm);
    }
}

OrientedGraphs Graph_Partition(const EntiersEntiers& Partition,
                               OrientedGraph *go,
                               UnorientedGraph *g,
                               OutputEdgeList &outputedgelist,
                               InputEdgeList &inputedgelist,
                               Connections &connections)
{
    OrientedGraphs graph_partie;
    EntiersEntiers neigh_community;

    for (uint i = 0; i < Partition.size();i++){
        Edges edge_partie;
        List_edge_partie(Partition.at(i),go,edge_partie,outputedgelist.at(i));
        OrientedGraph graph;
        std::vector<vertex_to> tab_vertex_to;
        Entiers *community = new Entiers();

        for (uint j = 0; j < Partition.at(i)->size(); j++) {
            Global_Neigh_community(g, Partition, community,
                                   Partition.at(i)->at(j),i);
            vertex_to v = add_vertex(graph);

            tab_vertex_to.push_back(v);
            graph[v] = VertexProperties((*go)[Partition.at(i)->at(j)]._index,
                                        (*go)[Partition.at(i)->at(j)]._weight,
                                        (*go)[Partition.at(i)->at(j)]._type);
        }
        neigh_community.push_back(community);

        for(uint k = 0; k < edge_partie.size(); k++) {
            add_edge(tab_vertex_to.at(recherche_val(*Partition.at(i),
                                                    edge_partie.at(k).first)),
                     tab_vertex_to.at(recherche_val(*Partition.at(i),
                                                    edge_partie.at(k).second)),
                     graph);
        }
        graph_partie.push_back(graph);
    }

    for (uint i = 0; i < neigh_community.size(); i++) {
        InputEdges inputedges;

        for (uint j = 0; j < neigh_community.at(i)->size(); j++) {
            for (uint k = 0;
                 k < outputedgelist.at(neigh_community.at(i)->at(j)).size();
                 k++) {
                if (In_tab_dichotomie(
                        *Partition.at(i),
                        outputedgelist.at(
                            neigh_community.at(i)->at(j)).at(k).second))
                    inputedges.push_back(
                        outputedgelist.at(
                            neigh_community.at(i)->at(j)).at(k));
            }
        }
        inputedgelist.push_back(inputedges);
    }

    for (uint i = 0; i < outputedgelist.size(); i++){
        Connection connec;

        for(uint j = 0; j < outputedgelist.at(i).size(); j++){
            Port port1;

            port1.first = i + 1;
            port1.second = outputedgelist.at(i).at(j).first;

            Port port2;

            port2.first = In_community_dichotomie(
                Partition,outputedgelist.at(i).at(j).second) + 1;
            port2.second = outputedgelist.at(i).at(j).second;

            connec.first = port1;
            connec.second = port2;

            connections.push_back(connec);
        }
    }

    for (EntiersEntiers::iterator it = neigh_community.begin();
        it != neigh_community.end(); it++) {
        delete *it;
        *it = NULL;
    }

    return graph_partie;
}

double Best_Cut_cluster(EntiersEntiers &tab_cluster,Entiers *cluster1, Entiers *cluster2, int index_cluster1, UnorientedGraph &g,std::string name)
{
	tab_cluster.push_back(cluster2);
	
	double cpt=0.;
	for(int i=0;i<tab_cluster.size();i++){
		if(i!=index_cluster1){
		cpt+=Cut_one_cluster(*tab_cluster[i],g,name);
		}
	}
	cpt+=Cut_one_cluster(*cluster1,g,name);
	tab_cluster.pop_back();
	return cpt;
}

/*double In_modularity(UnorientedGraph &g , const Entiers &cluster){
	property_map<UnorientedGraph,edge_weight_t>::type poids_arc=get(edge_weight_t(),g);
	edge_t e1;
	bool found;
	int val=0;

	for(uint i=0;i<cluster.size();i++){
		tie(neighbourIt, neighbourEnd) = adjacent_vertices(cluster[i],g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt){
			tie(e1,found)=edge(vertex(cluster[i],g),vertex(*neighbourIt,g),g);
			if(In_tab(cluster,*neighbourIt)==1)
				val+=get(poids_arc,e1);
		}
	}
	return val/2.;
}*/

/**
 *
 * @param g
 * @param cluster
 * @return
 */



/**
 *
 * @param g
 * @param part
 * @return
 */

/*double Modularity(UnorientedGraph &g,const EntiersEntiers &part) {
  double q  = 0.;
  int tmp=num_edges(g);
  for(uint i=0;i<part.size();i++){
	  q+=In_modularity(g,*part[i])/tmp-(Cluster_Degree(g,*part[i])/(2*tmp))*(Cluster_Degree(g,*part[i])/(2*tmp));
  	}

  return q;
}*/

/**
 *
 * @param part
 * @param val
 * @return
 */



/**
 * Fonction de calcul du gain de modularité de déplacement d'un sommet d'une communoté à une autre !!!!!
 *  ajoute le sommet à part[val] et on calcul la nouvelle modularité
 *  on prend la différence entre la modularité et la nouvouvelle !
 * @param cur_mod
 * @param val
 * @param neight
 * @param node_comm
 * @param part
 * @param g
 */
/*double Modularity_gain(double cur_mod , int val , int neight , int node_comm , EntiersEntiers part , UnorientedGraph &g) {
	double q;
	part[neight]->push_back(val);
	std::sort(*part[neight]);
	q=Modularity(g,part);

	return q-cur_mod;
}*/

/**
 * Fonction de calcul du gain de modularité de déplacement d'un sommet d'une communoté à une autre !!!!!
 *  ajoute le sommet à part[val] et on calcul la nouvelle modularité
 *  on prend la différence entre la modularité et la nouvouvelle !
 * @param cur_mod
 * @param tmp_community
 * @param neight
 * @param node_comm
 * @param part
 * @param g
 */
/*double Modularity_gain_phase_2(double cur_mod, Entiers tmp_community, int neight, int node_comm, EntiersEntiers part, UnorientedGraph &g) {
	double q;
	for (uint i=0;i<tmp_community.size();i++)
		part[neight]->push_back(tmp_community[i]);
	std::sort(*part[neight]);
	q = Modularity(g,part);
	return q - cur_mod;
}*/

/**
 * Donne la liste des communautés voisines à celle contenant le sommet val.
 * @param part
 * @param val
 * @param g
 * @return
 */
/*Entiers Neight_community(const EntiersEntiers &part, int val , UnorientedGraph &g){
	Entiers Neight;
	tie(neighbourIt, neighbourEnd) = adjacent_vertices(val, g);
	for (; neighbourIt != neighbourEnd; ++neighbourIt){
		int tmp=In_community(part,*neighbourIt);
		if(In_tab(Neight,tmp)!=1 && In_tab(*part[In_community(part,val)],*neighbourIt)!=1)
			Neight.push_back(tmp);
	}
	std::sort(Neight);
	return Neight;
}*/

/**
 *
 * @param part
 * @param community
 * @param g
 * @return
 */
/*Entiers Part_Neight_community(const EntiersEntiers &part,int community, UnorientedGraph &g){
	Entiers Neight;
	for(uint i =0;i<part[community]->size();i++){
		tie(neighbourIt, neighbourEnd) = adjacent_vertices(part[community]->at(i), g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt){
			int tmp=In_community(part,*neighbourIt);
			if(In_tab(Neight,tmp)!=1 && tmp!=community)
				Neight.push_back(tmp);
		}
	}
	std::sort(Neight);
	return Neight;
}*/

void make_unoriented_graph(const OrientedGraph& og, UnorientedGraph& ug)
{
    std::vector < vertex_t > ug_vertex_list;
    std::vector < vertex_t > og_vertex_list;

    for (uint i = 0; i < num_vertices(og); ++i) {
        ug_vertex_list.push_back(add_vertex(ug));
    }

    OrientedGraph::vertex_iterator it_og, end_og;
    UnorientedGraph::vertex_iterator it_ug, end_ug;

    tie(it_og, end_og) = vertices(og);
    tie(it_ug, end_ug) = vertices(ug);
    for (; it_og != end_og; ++it_og, ++it_ug) {
        ug[*it_ug] = og[*it_og];
        og_vertex_list.push_back(*it_og);
    }

    OrientedGraph::edge_iterator ite_og, ende_og;

    tie(ite_og, ende_og) = edges(og);
    for (; ite_og != ende_og; ++ite_og) {
        boost::add_edge(source(*ite_og, og), target(*ite_og, og),
                        og[*ite_og], ug);
    }

    // std::cout << "Oriented graph: " << std::endl;
    // tie(it_og, end_og) = vertices(og);
    // for (; it_og != end_og; ++it_og) {
    //     OrientedGraph::adjacency_iterator neighbour_it, neighbour_end;

    //     std::cout << og[*it_og]._index << " is connected with ";
    //     tie(neighbour_it, neighbour_end) = adjacent_vertices(*it_og, og);
    //     for (; neighbour_it != neighbour_end; ++neighbour_it)
    //         std::cout << og[*neighbour_it]._index << " ";
    //     std::cout << " and weight = " << og[*it_og]._weight << std::endl;
    // }
    // std::cout << std::endl;

    // std::cout << "Unoriented graph: " << std::endl;
    // tie(it_ug, end_ug) = vertices(ug);
    // for (; it_ug != end_ug; ++it_ug) {
    //     UnorientedGraph::adjacency_iterator neighbour_it, neighbour_end;

    //     std::cout << ug[*it_ug]._index << " is connected with ";
    //     tie(neighbour_it, neighbour_end) = adjacent_vertices(*it_ug, ug);
    //     for (; neighbour_it != neighbour_end; ++neighbour_it)
    //         std::cout << ug[*neighbour_it]._index << " ";
    //     std::cout << " and weight = " << ug[*it_ug]._weight << std::endl;
    // }
    // std::cout << std::endl;
}

void adjacence_ggp(int vertex, Entiers &sommets_adj, UnorientedGraph *g)
{
    tie(neighbourIt, neighbourEnd) = adjacent_vertices(vertex, *g);
    for (; neighbourIt != neighbourEnd; ++neighbourIt)
    {
        sommets_adj.push_back(*neighbourIt);
    }

}

double modif_Cout_coupe(const Entiers &P, int val, double cut, UnorientedGraph *g)
{
    //std::cout<<"Cout de coupe initiale : "<<cut<<std::endl;
    //std::cout<<"degré du sommet tiré : "<<Degree(*g,val)<<std::endl;
    double cpt = 0.;
    double new_cut;
    bool found;
	edge_t e1;
    tie(neighbourIt, neighbourEnd) = adjacent_vertices(val, *g);
    for (; neighbourIt != neighbourEnd; neighbourIt++){
        if(In_tab(P,*neighbourIt)==1){
			tie(e1,found)=edge(vertex(val,*g),vertex(*neighbourIt,*g),*g);
            cpt += (*g)[e1]._weight;
        }
    }
    new_cut = cut + (Degree(*g,val) - 2*cpt);
    return new_cut;
}

int decimal(int valeur){
	int res;
	switch(valeur){
		case 0 ... 9 : res = 0;
		break;
		case 10 ... 99 : res = 1;
		break;
		case 100 ... 999 : res = 2;
		break;
		case 1000 ... 9999 : res = 3;
		break;
		case 10000 ... 99999 : res = 4;
		break;
		case 100000 ... 999999 : res = 5;
		break;
		case 1000000 ... 9999999 : res = 6;
		break;
		case 10000000 ... 99999999 : res = 7;
		break;
		case 100000000 ... 999999999 : res = 8;
		break;
	default : 
		std::cout<<"L'interval n'est pas de taille suffisante"<<std::endl;
		break;
	}
	return res;
}

void Graph_constructor_txt(const char* text, OrientedGraph * Og){
	
	//Traitement initial
	std::ifstream fichier (text, std::ios::in);
	int lines = std::count(std::istreambuf_iterator<char>( fichier ),
	std::istreambuf_iterator<char>(),'\n' );
	//std::cout<<"Nombre de ligne : "<<lines<<std::endl;
	
	fichier.seekg(0, std::ios::beg);
	std::string caractere;
	getline(fichier, caractere);
	int caractere_size = caractere.size()+1;
	
	fichier.seekg(0, std::ios::beg);
	int nbr_vertices;
	fichier >> nbr_vertices;
	//std::cout << "Nombre de sommets : "<< nbr_vertices<< std::endl;
	
	//Création des sommets
	
	std::vector<vertex_to> vect_vertex;
	for(int i =0; i<nbr_vertices; i++){
		vertex_to v0 = boost::add_vertex(*Og);
		vect_vertex.push_back(v0);
	}
	
	//Création des arcs
	int deplacement_sup = 0;
	for(int i = 0; i <(lines-nbr_vertices-1); i++){
	fichier.seekg((8)*i+caractere_size+deplacement_sup, std::ios::beg);
	int vertex_in, vertex_out;
	double weight;
	fichier >> vertex_in >> vertex_out >> weight ; 
	add_edge(vect_vertex.at(vertex_in), vect_vertex.at(vertex_out), EdgeProperties(weight), *Og); 
	//std::cout << vertex_in << " " << vertex_out << " " << weight << std::endl; 
	int tmp = decimal(vertex_in) + decimal(vertex_out) + decimal(floor(weight));
	deplacement_sup += tmp;
	}
	
	//Pondération des sommets
	int cpt =0;
	for(int i = lines-nbr_vertices-1; i <lines-1; i++){
	fichier.seekg((8)*(lines-nbr_vertices-1)+caractere_size+deplacement_sup, std::ios::beg);
	double poids;
	std::string txt;
	fichier >> poids >> txt ;
	DynamicsType type;
	if(txt == "NORMAL_PIXEL"){
		type = NORMAL_PIXEL; 
	}else{
		type = TOP_PIXEL;
	}
	(*Og)[vect_vertex.at(cpt)] = VertexProperties(cpt, poids, type);
	//std::cout << poids << std::endl; 
	int tmp = decimal(floor(poids)) + 17;
	deplacement_sup += tmp;
	cpt++;
	}

	fichier.close();
}

void Text_generator_graph(const char *texte, OrientedGraph *go){
	
	bool found;
	edge_to e1;
	std::ofstream fichier (texte, std::ios::out);
    fichier<<num_vertices(*go)<<std::endl;
    tie(vertexIto, vertexEndo) = vertices(*go);
    for (; vertexIto != vertexEndo; ++vertexIto) {
    	tie(neighbourIto, neighbourEndo) = adjacent_vertices(*vertexIto,
    			*go);
    	for (; neighbourIto != neighbourEndo; ++neighbourIto){
			tie(e1,found)=edge(vertex(*vertexIto,*go),vertex(*neighbourIto,*go),*go);
			if(((*go)[e1]._weight - floor((*go)[e1]._weight)) == 0 ){
				fichier<<(*go)[*vertexIto]._index<<" "<<(*go)[*neighbourIto]._index<<" "<<(*go)[e1]._weight<<".0"<<std::endl;
			}else{
				fichier<<(*go)[*vertexIto]._index<<" "<<(*go)[*neighbourIto]._index<<" "<<(*go)[e1]._weight<<std::endl;
			}
    	}
    }
    tie(vertexIto, vertexEndo) = vertices(*go);
    for (; vertexIto != vertexEndo; ++vertexIto) {
		if(((*go)[*vertexIto]._weight - floor((*go)[*vertexIto]._weight)) == 0 && (*go)[*vertexIto]._type == TOP_PIXEL){
			fichier<<(*go)[*vertexIto]._weight<<".0"<<" "<<"TOP_PIXEL"<<"   "<<std::endl;
		}else if(((*go)[*vertexIto]._weight - floor((*go)[*vertexIto]._weight)) == 0 && (*go)[*vertexIto]._type == NORMAL_PIXEL){
			fichier<<(*go)[*vertexIto]._weight<<".0"<<" "<<"NORMAL_PIXEL"<<std::endl;
		}else if(((*go)[*vertexIto]._weight - floor((*go)[*vertexIto]._weight)) != 0 && (*go)[*vertexIto]._type == TOP_PIXEL){
			fichier<<(*go)[*vertexIto]._weight<<" "<<"TOP_PIXEL"<<std::endl;
		}else{
			fichier<<(*go)[*vertexIto]._weight<<" "<<"NORMAL_PIXEL"<<std::endl;
		}
    }
    
	fichier.close();
}

double Diff_cut_ratio(UnorientedGraph *g, const EntiersEntiers &Partition, int partie, int node, std::string name){
	double Dif;
	double Int = 0.;
	double Ext = 0.;
	
	edge_t e1;
    bool found;

    tie(neighbourIt, neighbourEnd) = adjacent_vertices(node, *g);
    for (; neighbourIt != neighbourEnd; ++neighbourIt){
		tie(e1, found) = edge(vertex(node, *g), vertex(*neighbourIt, *g), *g);
		if(In_tab_dichotomie(*Partition.at(partie),*neighbourIt) == 1){
			Int+= (*g)[e1]._weight;
		}else{
			Ext+= (*g)[e1]._weight;
		}
    }
    
	if(name == "ratio"){
	    Int/=Cluster_Weight(*g,*Partition.at(partie));
	    Ext/=Cluster_Weight(*g,*Partition.at(partie));
	}
	
	Dif = Ext - Int;
	return Dif;
}

double Diff_cut_ratio_bissection(UnorientedGraph *g, Entiers *part, int node, std::string name){
	double Ext = 0.;
	
	edge_t e1;
    bool found;

    tie(neighbourIt, neighbourEnd) = adjacent_vertices(node, *g);
    for (; neighbourIt != neighbourEnd; ++neighbourIt){
		tie(e1, found) = edge(vertex(node, *g), vertex(*neighbourIt, *g), *g);
		if(In_tab_dichotomie(*part,*neighbourIt) != 1){
			Ext+= (*g)[e1]._weight;
		}
    }
	
	return Ext;
}

std::vector<std::vector<int>> Vector_diff_cut_ratio(UnorientedGraph *g, const EntiersEntiers &Partition, std::string name){
	std::vector<std::vector<int>> Diff_vector;
	
	for(uint i = 0; i < Partition.size(); i++){
		std::vector<std::pair<double,int>> D_vector;
		for(uint j = 0; j < Partition.at(i)->size(); j++){
			double gain_d = Diff_cut_ratio(g, Partition, i, Partition.at(i)->at(j), name);
			//std::cout<<gain_d<<std::endl;
			if(gain_d > 0){
				std::pair<double, int> D;
				D.first =  gain_d;
				D.second = Partition.at(i)->at(j);
				D_vector.push_back(D);		
			}	
		}
		sort(D_vector.begin(),D_vector.end());
		std::reverse(D_vector.begin(),D_vector.end());
		std::vector<int> index_vector;
		for(uint id = 0; id < D_vector.size(); id++){
			index_vector.push_back(D_vector.at(id).second);
		}
		Diff_vector.push_back(index_vector);
	}
	
	/*std::cout<<"Tableau des différences "<<std::endl;
	for(uint i = 0; i<Diff_vector.size(); i++){
		std::cout<<"*"<<i<<"* ";
		for(uint j = 0; j<Diff_vector.at(i).size(); j++){
			std::cout<<Diff_vector.at(i).at(j)<<" ";
		}
		std::cout<<std::endl;
	}*/
	
	return Diff_vector;

}

std::vector<int> Vector_diff_cut_ratio_2(UnorientedGraph *g, const EntiersEntiers &Partition, std::string name){
	std::vector<int> Diff_vector;
	std::vector<std::pair<double,int>> D_vector;
	for(uint i = 0; i < Partition.size(); i++){
		for(uint j = 0; j < Partition.at(i)->size(); j++){
			double gain_d = Diff_cut_ratio(g, Partition, i, Partition.at(i)->at(j), name);
			//std::cout<<gain_d<<std::endl;
			if(gain_d > 0){
				std::pair<double, int> D;
				D.first =  gain_d;
				D.second = Partition.at(i)->at(j);
				D_vector.push_back(D);		
			}	
		}
	}
	sort(D_vector.begin(),D_vector.end());
	for(uint id = 0; id < D_vector.size(); id++){
		Diff_vector.push_back(D_vector.at(id).second);
	}
	
	/*std::cout<<"Tableau des différences "<<std::endl;
	for(uint i = 0; i<Diff_vector.size(); i++){
		std::cout<<"*"<<i<<"* ";
		for(uint j = 0; j<Diff_vector.at(i).size(); j++){
			std::cout<<Diff_vector.at(i).at(j)<<" ";
		}
		std::cout<<std::endl;
	}*/
	/*for(uint j = 0; j<Diff_vector.size(); j++){
		std::cout<<Diff_vector.at(j)<<" ";
	}
	std::cout<<std::endl;*/
	
	return Diff_vector;

}

void Modif_vector_diff_cut_ratio_2(UnorientedGraph *g, const EntiersEntiers &Partition, std::vector<int> &Diff_vector, int node, std::string name){
	std::vector<std::pair<double,int>> D_vector;
	
	tie(neighbourIt, neighbourEnd) = adjacent_vertices(node, *g);
	for (; neighbourIt != neighbourEnd; ++neighbourIt){
		//std::cout<<"node : "<<node<<std::endl;
		//std::cout<<"voisin : "<<*neighbourIt<<std::endl;
		int neigh_ind = In_community_dichotomie(Partition, *neighbourIt);
		//std::cout<<"dans  : "<<neigh_ind<<std::endl;
		double gain_d = Diff_cut_ratio(g, Partition, neigh_ind, *neighbourIt, name);
		//std::cout<<"gain_d : "<<gain_d<<std::endl;
		if(gain_d > 0){
			std::pair<double, int> D;
			D.first =  gain_d;
			D.second = *neighbourIt;
			D_vector.push_back(D);		
		}
		//suprim_val(Diff_vector,*neighbourIt);
	}
	
	if(D_vector.size() == 0){
		Diff_vector.erase(Diff_vector.begin());
		return;
	}
	
	//std::cout<<"**"<<std::endl;
	sort(D_vector.begin(),D_vector.end());
	for(uint id = 0; id < D_vector.size(); id++){
		if(In_tab(Diff_vector,D_vector.at(id).second) != 1)
			Diff_vector.push_back(D_vector.at(id).second);
	}
	//std::cout<<"***"<<std::endl;
	Diff_vector.erase(Diff_vector.begin());
	//std::cout<<"**!**"<<std::endl;
	
	sort(Diff_vector.begin(),Diff_vector.end());
	
	for(uint j = 0; j<Diff_vector.size(); j++){
		std::cout<<Diff_vector.at(j)<<" ";
	}
	//std::cout<<std::endl;
}

void Modif_vector_diff_cut_ratio(UnorientedGraph *g, const EntiersEntiers &Partition, std::vector<std::vector<int>> &Diff_vector, int recalcul1, int recalcul2, std::string name){
	
	std::vector<std::pair<double,int>> D_vector;
	for(uint j = 0; j < Partition.at(recalcul1)->size(); j++){
		double gain_d = Diff_cut_ratio(g, Partition, recalcul1, Partition.at(recalcul1)->at(j), name);
		//std::cout<<gain_d<<std::endl;
		if(gain_d > 0){
			std::pair<double, int> D;
			D.first =  gain_d;
			D.second = Partition.at(recalcul1)->at(j);
			D_vector.push_back(D);		
		}	
	}
	sort(D_vector.begin(),D_vector.end());
	std::reverse(D_vector.begin(),D_vector.end());
	std::vector<int> index_vector;
	for(uint id = 0; id < D_vector.size(); id++){
		index_vector.push_back(D_vector.at(id).second);
	}
	Diff_vector.at(recalcul1) = index_vector;
	
	std::vector<std::pair<double,int>> D_vector2;
	for(uint j = 0; j < Partition.at(recalcul2)->size(); j++){
		double gain_d = Diff_cut_ratio(g, Partition, recalcul2, Partition.at(recalcul2)->at(j), name);
		//std::cout<<gain_d<<std::endl;
		if(gain_d > 0){
			std::pair<double, int> D;
			D.first =  gain_d;
			D.second = Partition.at(recalcul2)->at(j);
			D_vector2.push_back(D);		
		}	
	}
	sort(D_vector2.begin(),D_vector2.end());
	std::reverse(D_vector2.begin(),D_vector2.end());
	std::vector<int> index_vector2;
	for(uint id = 0; id < D_vector2.size(); id++){
		index_vector2.push_back(D_vector2.at(id).second);
	}
	Diff_vector.at(recalcul2) = index_vector2;
	
	/*std::cout<<"Tableau des différences modifié "<<std::endl;
	for(uint i = 0; i<Diff_vector.size(); i++){
		std::cout<<"*"<<i<<"* ";
		for(uint j = 0; j<Diff_vector.at(i).size(); j++){
			std::cout<<Diff_vector.at(i).at(j)<<" ";
		}
		std::cout<<std::endl;
	}*/
		
}

void Affinache_gain_diff(UnorientedGraph *g, EntiersEntiers &Partition, double &cut, std::string name, double poids_moy){
	double old_cut = -1.;
	
	while(old_cut != cut){
		//std::cout<<"Boucle d'ammélioration "<<std::endl;
		old_cut = cut;
		sort(Partition.begin(), Partition.end(), myobject_taille);
		std::vector<std::vector<int>> diff_vector;
		diff_vector = Vector_diff_cut_ratio(g, Partition, name);
		
		for(uint indice = 0; indice < diff_vector.size(); indice ++){
			if(diff_vector.at(indice).size() != 0 && Partition.at(indice)->size() >1){
				//for(uint i = 0; i < diff_vector.at(indice).size(); i++){
				int i =0;
				while(diff_vector.at(indice).size() != 0 && Partition.at(indice)->size() >1 && i < diff_vector.at(indice).size() && 
				Cluster_Weight(*g,*Partition.at(indice)) > (poids_moy-poids_moy/Partition.size())){
					//std::cout<<"Sommet de départ : "<< diff_vector.at(indice).at(i) <<" dans "<<indice<<std::endl;
					Entiers neigh_part;
					neigh_part = Neigh_community(g, Partition, diff_vector.at(indice).at(i), indice);
					int best_neigh_part = neigh_part.at(0);
					double gain = -10000000.;
					for(uint ind_neigh = 0; ind_neigh < neigh_part.size(); ind_neigh++){
						double tmp_gain;
						if(name == "ratio"){
							tmp_gain = Gain_ratio(g,Partition,indice,neigh_part.at(ind_neigh),diff_vector.at(indice).at(i),cut);
						}else{
							double Int = 0.;
							double Ext = 0.;
							edge_t e1;
						    bool found;
						
						    tie(neighbourIt, neighbourEnd) = adjacent_vertices(diff_vector.at(indice).at(i), *g);
						    for (; neighbourIt != neighbourEnd; ++neighbourIt){
								tie(e1, found) = edge(vertex(diff_vector.at(indice).at(i), *g), vertex(*neighbourIt, *g), *g);
								if(In_tab_dichotomie(*Partition.at(neigh_part.at(ind_neigh)),*neighbourIt) == 1){
									Ext+= (*g)[e1]._weight;
								}else if(In_tab_dichotomie(*Partition.at(indice),*neighbourIt) == 1){
									Int+= (*g)[e1]._weight;
								}
						    }
								tmp_gain = Ext - Int;
						}
						
					    if(tmp_gain > gain & tmp_gain > 0){
							gain = tmp_gain;
							best_neigh_part = neigh_part.at(ind_neigh);
						}
					}
					
					//std::cout<<" Ensemble de déstination "<<best_neigh_part<<" gain de  : "<<gain<<std::endl;
					if(gain > 0){
						//std::cout<<"Modification"<<std::endl;
						cut -= gain; /*Grosse modification a apporté de ce coté la*/
						//std::cout<<"Ratio de coupe : "<<cut<<std::endl;
						suprim_val(*Partition.at(indice),diff_vector.at(indice).at(i));
						Partition.at(best_neigh_part)->push_back(diff_vector.at(indice).at(i));
						sort(Partition.at(best_neigh_part)->begin(),Partition.at(best_neigh_part)->end());
						//double cut2 = Cut_cluster(Partition,*g,"ratio");
						//std::cout<<"Vrai ratio de coupe : "<<cut2<<std::endl;
						Modif_vector_diff_cut_ratio(g,Partition,diff_vector,best_neigh_part,indice,name);
						i = 0;
					}else{
						i++;
					}			
				}
			}
		}
		//std::cout<<cut<<std::endl;
	}
}

void Affinache_gain_diff_2(UnorientedGraph *g, EntiersEntiers &Partition, double &cut, std::string name, double poids_moy){
	double old_cut = -1.;
	
	//while(old_cut != cut){
		//std::cout<<"Boucle d'ammélioration "<<std::endl;
		//old_cut = cut;
		sort(Partition.begin(), Partition.end(), myobject_taille);
		std::vector<int> diff_vector;
		diff_vector = Vector_diff_cut_ratio_2(g, Partition, name);
		
		//for(uint indice = 0; indice < diff_vector.size(); indice ++){
		int indice = 0;
		while(/*indice < diff_vector.size() &&*/ diff_vector.size() != 0){
			int com = In_community_dichotomie(Partition,diff_vector.at(indice));
			std::cout<<" Ensemble de départ "<<com<<" sommet  : "<<diff_vector.at(indice)<<std::endl;
			if(Partition.at(com)->size() >1 && Cluster_Weight(*g,*Partition.at(com)) > (poids_moy-poids_moy/Partition.size())){
				Entiers neigh_part;
				neigh_part = Neigh_community(g, Partition, diff_vector.at(indice), com);
				int best_neigh_part = neigh_part.at(0);
				double gain = -10000000.;
				for(uint ind_neigh = 0; ind_neigh < neigh_part.size(); ind_neigh++){
					double tmp_gain;
					if(name == "ratio"){
						tmp_gain = Gain_ratio(g,Partition,com,neigh_part.at(ind_neigh),diff_vector.at(indice),cut);
					}else{
						double Int = 0.;
						double Ext = 0.;
						edge_t e1;
					    bool found;
					
					    tie(neighbourIt, neighbourEnd) = adjacent_vertices(diff_vector.at(indice), *g);
					    for (; neighbourIt != neighbourEnd; ++neighbourIt){
							tie(e1, found) = edge(vertex(diff_vector.at(indice), *g), vertex(*neighbourIt, *g), *g);
							if(In_tab_dichotomie(*Partition.at(neigh_part.at(ind_neigh)),*neighbourIt) == 1){
								Ext+= (*g)[e1]._weight;
							}else if(In_tab_dichotomie(*Partition.at(com),*neighbourIt) == 1){
								Int+= (*g)[e1]._weight;
							}
					    }
							tmp_gain = Ext - Int;
					}
					
				    if(tmp_gain > gain & tmp_gain > 0){
						gain = tmp_gain;
						best_neigh_part = neigh_part.at(ind_neigh);
					}
				}
				
				std::cout<<" Ensemble de déstination "<<best_neigh_part<<" gain de  : "<<gain<<std::endl;
				if(gain > 0){
					std::cout<<"Modification"<<std::endl;
					cut -= gain; /*Grosse modification a apporté de ce coté la*/
					std::cout<<"Ratio de coupe : "<<cut<<std::endl;
					suprim_val(*Partition.at(com),diff_vector.at(indice));
					Partition.at(best_neigh_part)->push_back(diff_vector.at(indice));
					sort(Partition.at(best_neigh_part)->begin(),Partition.at(best_neigh_part)->end());
					double cut2 = Cut_cluster(Partition,*g,"ratio");
					std::cout<<"Vrai ratio de coupe : "<<cut2<<std::endl;
					//Modif_vector_diff_cut_ratio_2(g,Partition,diff_vector,diff_vector.at(indice),name);
					//indice = 0;
					diff_vector.erase(diff_vector.begin());
				}else{
					diff_vector.erase(diff_vector.begin());
				}
			}
		}
		//std::cout<<cut<<std::endl;
//	}
}

double Gain_ratio(UnorientedGraph *g,const EntiersEntiers &Partition, int in, int out, int node, double ratio){
	double new_ratio = ratio;
	double poids_in = Cluster_Weight(*g,*Partition.at(in));
	double poids_out = Cluster_Weight(*g,*Partition.at(out));
	double tmp_poids_in = poids_in - (*g)[node]._weight;
	double tmp_poids_out = poids_out + (*g)[node]._weight;
	//std::cout<<"tmp_poids_in "<< tmp_poids_in <<std::endl;
	//std::cout<<"tmp_poids_out "<< tmp_poids_out <<std::endl;
	
	double cut_in = 0.;
	double cut_out = 0.;
	double tmp_cut_in = 0.;
	double tmp_cut_out = 0.;
	
	edge_t e1;
	bool found;
	
	for(uint i = 0; i < Partition.at(in)->size(); i++){
		tie(neighbourIt, neighbourEnd) = adjacent_vertices(Partition.at(in)->at(i), *g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt){
			tie(e1,found)=edge(vertex(Partition.at(in)->at(i),*g),vertex(*neighbourIt,*g),*g);
			if(In_tab_dichotomie(*Partition.at(in),*neighbourIt) != 1){
				if(Partition.at(in)->at(i) != node){
					tmp_cut_in += (*g)[e1]._weight;
				}
				cut_in += (*g)[e1]._weight;
			}else if(*neighbourIt == node){
				tmp_cut_in += (*g)[e1]._weight;
			}
		}
	}
	
	for(uint i = 0; i < Partition.at(out)->size(); i++){
		tie(neighbourIt, neighbourEnd) = adjacent_vertices(Partition.at(out)->at(i), *g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt){
			tie(e1,found)=edge(vertex(Partition.at(out)->at(i),*g),vertex(*neighbourIt,*g),*g);
			if(In_tab_dichotomie(*Partition.at(out),*neighbourIt) != 1){
				if(*neighbourIt != node){
					tmp_cut_out += (*g)[e1]._weight;
				}
				cut_out += (*g)[e1]._weight;
			}
		}
	}
	tie(neighbourIt, neighbourEnd) = adjacent_vertices(node, *g);
	for (; neighbourIt != neighbourEnd; ++neighbourIt){
		tie(e1,found)=edge(vertex(node,*g),vertex(*neighbourIt,*g),*g);
		if(In_tab_dichotomie(*Partition.at(out),*neighbourIt) != 1){
			tmp_cut_out += (*g)[e1]._weight;
		}
	}
	
	//std::cout<<"tmp_cut_in "<< tmp_cut_in/2. <<std::endl;
	//std::cout<<"tmp_cut_out "<< tmp_cut_out/2. <<std::endl;
	new_ratio -= cut_in/2./poids_in;
	new_ratio -= cut_out/2./poids_out;
	new_ratio += tmp_cut_in/2./tmp_poids_in;
	new_ratio += tmp_cut_out/2./tmp_poids_out;
	
	//std::cout<<"Nouveau ratio : " <<new_ratio<<std::endl;
	return ratio - new_ratio;
	
}

double Modif_ratio_cut(UnorientedGraph *g,Entiers *Ss, Entiers *Sd, int node, double ratio){/*Revoir cette fonction, modification psa forcement nécéssaire, plus simple !!!*/
	double new_ratio;
	double poids_in = Cluster_Weight(*g,*Ss);
	double poids_out = Cluster_Weight(*g,*Sd);
	double tmp_poids_in = poids_in - (*g)[node]._weight;
	double tmp_poids_out = poids_out + (*g)[node]._weight;
	//std::cout<<"tmp_poids_in "<< tmp_poids_in <<std::endl;
	//std::cout<<"tmp_poids_out "<< tmp_poids_out <<std::endl;
	
	double new_cut = 0.;
	//double new_cut_out = 0.;
	//double tmp_cut_in = 0.;
	//double tmp_cut_out = 0.;
	
	edge_t e1;
	bool found;
	
	for(uint i = 0; i < Ss->size(); i++){
		if(Ss->at(i) != node){
			tie(neighbourIt, neighbourEnd) = adjacent_vertices(Ss->at(i), *g);
			for (; neighbourIt != neighbourEnd; ++neighbourIt){
				tie(e1,found)=edge(vertex(Ss->at(i),*g),vertex(*neighbourIt,*g),*g);
				if(In_tab_dichotomie(*Ss,*neighbourIt) != 1){
					new_cut += (*g)[e1]._weight;
				}else if(*neighbourIt == node){
					new_cut += (*g)[e1]._weight;
				}
			}
		}
	}
	
	/*for(uint i = 0; i < Sd->size(); i++){
		tie(neighbourIt, neighbourEnd) = adjacent_vertices(Sd->at(i), *g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt){
			tie(e1,found)=edge(vertex(Sd->at(i),*g),vertex(*neighbourIt,*g),*g);
			if(In_tab(*Sd,*neighbourIt) != 1){
				if(*neighbourIt != node){
					tmp_cut_out += (*g)[e1]._weight;
				}
				cut_out += (*g)[e1]._weight;
			}
		}
	}
	tie(neighbourIt, neighbourEnd) = adjacent_vertices(node, *g);
	for (; neighbourIt != neighbourEnd; ++neighbourIt){
		tie(e1,found)=edge(vertex(node,*g),vertex(*neighbourIt,*g),*g);
		if(In_tab(*Sd,*neighbourIt) != 1){
			tmp_cut_out += (*g)[e1]._weight;
		}
	}*/
	
	//std::cout<<"tmp_cut_in "<< tmp_cut_in/2. <<std::endl;
	//std::cout<<"tmp_cut_out "<< tmp_cut_out/2. <<std::endl;
	new_ratio = new_cut/2./tmp_poids_in + new_cut/2./tmp_poids_out;
	/*new_ratio -= cut_out/2./poids_out;
	new_ratio += tmp_cut_in/2./tmp_poids_in;
	new_ratio += tmp_cut_out/2./tmp_poids_out;*/
	
	//std::cout<<"Nouveau ratio : " <<new_ratio<<std::endl;
	return new_ratio;
	
}

} } } // namespace paradevs tests boost_graph
