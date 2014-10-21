/**
 * @file tests/boost_graph/partitioning/graph_build.cpp
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

#include <tests/boost_graph/partitioning/graph_build.hpp>
#include <iostream>

//#include <tests/corsen/lib/Corsen.hpp>

namespace paradevs { namespace tests { namespace boost_graph {

void build_graph(OrientedGraph& og, unsigned int edge_number)
{

	if(edge_number == 11){
		vertex_to v0 = boost::add_vertex(og);
		vertex_to v1 = boost::add_vertex(og);
		vertex_to v2 = boost::add_vertex(og);
		vertex_to v3 = boost::add_vertex(og);
		vertex_to v4 = boost::add_vertex(og);
		vertex_to v5 = boost::add_vertex(og);
		vertex_to v6 = boost::add_vertex(og);
		vertex_to v7 = boost::add_vertex(og);
		vertex_to v8 = boost::add_vertex(og);
		vertex_to v9 = boost::add_vertex(og);
		vertex_to v10 = boost::add_vertex(og);

		add_edge(v1, v0, EdgeProperties(2.), og);
		add_edge(v2, v0, EdgeProperties(1.5), og);
		add_edge(v3, v0, EdgeProperties(1.5), og);
		add_edge(v1, v2, EdgeProperties(3.), og);
		add_edge(v4, v1, EdgeProperties(3.), og);
		add_edge(v5, v1, EdgeProperties(3.), og);
		add_edge(v6, v1, EdgeProperties(3.), og);
		add_edge(v6, v2, EdgeProperties(2.), og);
		add_edge(v2, v3, EdgeProperties(1.), og);
		add_edge(v9, v3, EdgeProperties(1.), og);
		add_edge(v10, v3, EdgeProperties(1.), og);
		add_edge(v4, v5, EdgeProperties(1.5), og);
		add_edge(v5, v6, EdgeProperties(1.5), og);
		add_edge(v7, v4, EdgeProperties(1.), og);
		add_edge(v8, v4, EdgeProperties(1.), og);
		add_edge(v7, v8, EdgeProperties(1.5), og);
		add_edge(v9, v10, EdgeProperties(2.), og);

		og[v6] = VertexProperties(6, 1.5, NORMAL_PIXEL);
		og[v8] = VertexProperties(8, 1., NORMAL_PIXEL);
		og[v10] = VertexProperties(10, 1.5, NORMAL_PIXEL);
		og[v0] = VertexProperties(0, 3, NORMAL_PIXEL);
		og[v1] = VertexProperties(1, 2, NORMAL_PIXEL);
		og[v2] = VertexProperties(2, 2, NORMAL_PIXEL);
		og[v3] = VertexProperties(3, 2, NORMAL_PIXEL);
		og[v4] = VertexProperties(4, 1.5, NORMAL_PIXEL);
		og[v5] = VertexProperties(5, 1.5, NORMAL_PIXEL);
		og[v7] = VertexProperties(7, 1., TOP_PIXEL);
		og[v9] = VertexProperties(9, 1.5, TOP_PIXEL);
		
	} else if(edge_number == 38){
		
		vertex_to v0 = boost::add_vertex(og);
		vertex_to v1 = boost::add_vertex(og);
		vertex_to v2 = boost::add_vertex(og);
		vertex_to v3 = boost::add_vertex(og);
		vertex_to v4 = boost::add_vertex(og);
		vertex_to v5 = boost::add_vertex(og);
		vertex_to v6 = boost::add_vertex(og);
		vertex_to v7 = boost::add_vertex(og);
		vertex_to v8 = boost::add_vertex(og);
		vertex_to v9 = boost::add_vertex(og);
		vertex_to v10 = boost::add_vertex(og);
		vertex_to v11 = boost::add_vertex(og);
		vertex_to v12 = boost::add_vertex(og);
		vertex_to v13 = boost::add_vertex(og);
		vertex_to v14 = boost::add_vertex(og);
		vertex_to v15 = boost::add_vertex(og);
		vertex_to v16 = boost::add_vertex(og);
		vertex_to v17 = boost::add_vertex(og);
		vertex_to v18 = boost::add_vertex(og);
		vertex_to v19 = boost::add_vertex(og);
		vertex_to v20 = boost::add_vertex(og);
		vertex_to v21 = boost::add_vertex(og);
		vertex_to v22 = boost::add_vertex(og);
		vertex_to v23 = boost::add_vertex(og);
		vertex_to v24 = boost::add_vertex(og);
		vertex_to v25 = boost::add_vertex(og);
		vertex_to v26 = boost::add_vertex(og);
		vertex_to v27 = boost::add_vertex(og);
		vertex_to v28 = boost::add_vertex(og);
		vertex_to v29 = boost::add_vertex(og);
		vertex_to v30 = boost::add_vertex(og);
		vertex_to v31 = boost::add_vertex(og);
		vertex_to v32 = boost::add_vertex(og);
		vertex_to v33 = boost::add_vertex(og);
		vertex_to v34 = boost::add_vertex(og);
		vertex_to v35 = boost::add_vertex(og);
		vertex_to v36 = boost::add_vertex(og);
		vertex_to v37 = boost::add_vertex(og);
		
		add_edge(v1, v0, EdgeProperties(1.), og);
		add_edge(v2, v0, EdgeProperties(1.), og);
		add_edge(v3, v0, EdgeProperties(1.), og);
		add_edge(v1, v2, EdgeProperties(1.), og);
		add_edge(v4, v1, EdgeProperties(1.), og);
		add_edge(v5, v1, EdgeProperties(1.), og);
		add_edge(v6, v1, EdgeProperties(1.), og);
		add_edge(v6, v2, EdgeProperties(1.), og);
		add_edge(v2, v3, EdgeProperties(1.), og);
		add_edge(v9, v3, EdgeProperties(1.), og);
		add_edge(v10, v3, EdgeProperties(1.), og);
		add_edge(v4, v5, EdgeProperties(1.), og);
		add_edge(v5, v6, EdgeProperties(1.), og);
		add_edge(v7, v4, EdgeProperties(1.), og);
		add_edge(v8, v4, EdgeProperties(1.), og);
		add_edge(v7, v8, EdgeProperties(1.), og);
		add_edge(v9, v10, EdgeProperties(1.), og);
		add_edge(v8, v11, EdgeProperties(1.), og);
		add_edge(v11, v4, EdgeProperties(1.), og);
		add_edge(v12, v13, EdgeProperties(1.), og);
		add_edge(v12, v7, EdgeProperties(1.), og);
		add_edge(v13, v7, EdgeProperties(1.), og);
		add_edge(v14, v15, EdgeProperties(1.), og);
		add_edge(v14, v12, EdgeProperties(1.), og);
		add_edge(v15, v16, EdgeProperties(1.), og);
		add_edge(v15, v12, EdgeProperties(1.), og);
		add_edge(v16, v17, EdgeProperties(1.), og);
		add_edge(v16, v12, EdgeProperties(1.), og);
		add_edge(v17, v12, EdgeProperties(1.), og);
		add_edge(v18, v19, EdgeProperties(1.), og);
		add_edge(v18, v8, EdgeProperties(1.), og);
		add_edge(v19, v20, EdgeProperties(1.), og);
		add_edge(v19, v8, EdgeProperties(1.), og);
		add_edge(v20, v8, EdgeProperties(1.), og);
		add_edge(v21, v22, EdgeProperties(1.), og);
		add_edge(v21, v19, EdgeProperties(1.), og);
		add_edge(v22, v23, EdgeProperties(1.), og);
		add_edge(v22, v19, EdgeProperties(1.), og);
		add_edge(v23, v19, EdgeProperties(1.), og);
		add_edge(v24, v25, EdgeProperties(1.), og);
		add_edge(v24, v6, EdgeProperties(1.), og);
		add_edge(v25, v6, EdgeProperties(1.), og);
		add_edge(v26, v11, EdgeProperties(1.), og);
		add_edge(v26, v27, EdgeProperties(1.), og);
		add_edge(v26, v24, EdgeProperties(1.), og);
		add_edge(v27, v24, EdgeProperties(1.), og);
		add_edge(v28, v25, EdgeProperties(1.), og);
		add_edge(v29, v27, EdgeProperties(1.), og);
		add_edge(v29, v28, EdgeProperties(1.), og);
		add_edge(v30, v27, EdgeProperties(1.), og);
		add_edge(v30, v28, EdgeProperties(1.), og);
		add_edge(v31, v32, EdgeProperties(1.), og);
		add_edge(v31, v10, EdgeProperties(1.), og);
		add_edge(v32, v33, EdgeProperties(1.), og);
		add_edge(v32, v10, EdgeProperties(1.), og);
		add_edge(v33, v10, EdgeProperties(1.), og);
		add_edge(v34, v31, EdgeProperties(1.), og);
		add_edge(v34, v35, EdgeProperties(1.), og);
		add_edge(v35, v31, EdgeProperties(1.), og);
		add_edge(v35, v32, EdgeProperties(1.), og);
		add_edge(v36, v32, EdgeProperties(1.), og);
		add_edge(v36, v37, EdgeProperties(1.), og);
		add_edge(v36, v33, EdgeProperties(1.), og);
		add_edge(v37, v33, EdgeProperties(1.), og);

		og[v6] = VertexProperties(6, 2.5, NORMAL_PIXEL);
		og[v8] = VertexProperties(8, 2., NORMAL_PIXEL);
		og[v10] = VertexProperties(10, 2.5, NORMAL_PIXEL);
		og[v0] = VertexProperties(0, 4, NORMAL_PIXEL);
		og[v1] = VertexProperties(1, 3, NORMAL_PIXEL);
		og[v2] = VertexProperties(2, 3, NORMAL_PIXEL);
		og[v3] = VertexProperties(3, 3, NORMAL_PIXEL);
		og[v4] = VertexProperties(4, 2.5, NORMAL_PIXEL);
		og[v5] = VertexProperties(5, 2.5, NORMAL_PIXEL);
		og[v7] = VertexProperties(7, 2., NORMAL_PIXEL);
		og[v9] = VertexProperties(9, 2.5, TOP_PIXEL);
		og[v11] = VertexProperties(11, 2, NORMAL_PIXEL);
		og[v12] = VertexProperties(12, 1.5, NORMAL_PIXEL);
		og[v13] = VertexProperties(13, 1.5, NORMAL_PIXEL);
		og[v14] = VertexProperties(14, 1, TOP_PIXEL);
		og[v15] = VertexProperties(15, 1, NORMAL_PIXEL);
		og[v16] = VertexProperties(16, 1, NORMAL_PIXEL);
		og[v17] = VertexProperties(17, 1, NORMAL_PIXEL);
		og[v18] = VertexProperties(18, 1.5, TOP_PIXEL);
		og[v19] = VertexProperties(19, 1.5, NORMAL_PIXEL);
		og[v20] = VertexProperties(20, 1.5, NORMAL_PIXEL);
		og[v21] = VertexProperties(21, 1, TOP_PIXEL);
		og[v22] = VertexProperties(22, 1, NORMAL_PIXEL);
		og[v23] = VertexProperties(23, 1, NORMAL_PIXEL);
		og[v24] = VertexProperties(24, 2, NORMAL_PIXEL);
		og[v25] = VertexProperties(25, 2, NORMAL_PIXEL);
		og[v26] = VertexProperties(26, 1.5, TOP_PIXEL);
		og[v27] = VertexProperties(27, 1.5, NORMAL_PIXEL);
		og[v28] = VertexProperties(28, 1.5, NORMAL_PIXEL);
		og[v29] = VertexProperties(29, 1, TOP_PIXEL);
		og[v30] = VertexProperties(30, 1, TOP_PIXEL);
		og[v31] = VertexProperties(31, 2, NORMAL_PIXEL);
		og[v32] = VertexProperties(32, 2, NORMAL_PIXEL);
		og[v33] = VertexProperties(33, 2, NORMAL_PIXEL);
		og[v34] = VertexProperties(34, 1.5, TOP_PIXEL);
		og[v35] = VertexProperties(35, 1.5, NORMAL_PIXEL);
		og[v36] = VertexProperties(36, 1.5, TOP_PIXEL);
		og[v37] = VertexProperties(37, 1.5, NORMAL_PIXEL);
		
	}else{
		std::cout<<"Le type de artificiel graphe choisi n'existe pas ! "<<std::endl;
	}
}

void modify_file_paths(std::string& absolutePath,
                       std::vector < std::string* >& files)
{
    for (std::vector < std::string* >::iterator it = files.begin();
         it != files.end(); it++) {
        (*it)->insert(0, absolutePath);
    }
}

void build_graph_cyclique(OrientedGraph& og)
{
		vertex_to v0 = boost::add_vertex(og);
		vertex_to v1 = boost::add_vertex(og);
		vertex_to v2 = boost::add_vertex(og);
		vertex_to v3 = boost::add_vertex(og);
		vertex_to v4 = boost::add_vertex(og);
		vertex_to v5 = boost::add_vertex(og);
		vertex_to v6 = boost::add_vertex(og);
		vertex_to v7 = boost::add_vertex(og);
		vertex_to v8 = boost::add_vertex(og);
		vertex_to v9 = boost::add_vertex(og);
		vertex_to v10 = boost::add_vertex(og);

		add_edge(v0, v1, EdgeProperties(1.), og);
		add_edge(v0, v2, EdgeProperties(1.), og);
		add_edge(v0, v3, EdgeProperties(1.), og);
		add_edge(v1, v3, EdgeProperties(1.), og);
		add_edge(v2, v1, EdgeProperties(1.), og);
		add_edge(v2, v3, EdgeProperties(1.), og);
		add_edge(v2, v4, EdgeProperties(1.), og);
		add_edge(v2, v5, EdgeProperties(1.), og);
		add_edge(v2, v7, EdgeProperties(1.), og);
		add_edge(v2, v9, EdgeProperties(1.), og);
		add_edge(v3, v4, EdgeProperties(1.), og);
		add_edge(v3, v5, EdgeProperties(1.), og);
		add_edge(v3, v7, EdgeProperties(1.), og);
		add_edge(v3, v9, EdgeProperties(1.), og);
		add_edge(v4, v6, EdgeProperties(1.), og);
		add_edge(v5, v4, EdgeProperties(1.), og);
		add_edge(v5, v6, EdgeProperties(1.), og);
		add_edge(v6, v7, EdgeProperties(1.), og);
		add_edge(v7, v8, EdgeProperties(1.), og);
		add_edge(v7, v10, EdgeProperties(1.), og);
		add_edge(v8, v9, EdgeProperties(1.), og);
		add_edge(v8, v10, EdgeProperties(1.), og);
		add_edge(v9, v10, EdgeProperties(1.), og);
		add_edge(v10, v0, EdgeProperties(1.), og);

		og[v6] = VertexProperties(6, 1, NORMAL_PIXEL);
		og[v8] = VertexProperties(8, 1, NORMAL_PIXEL);
		og[v10] = VertexProperties(10, 1, NORMAL_PIXEL);
		og[v0] = VertexProperties(0, 1, NORMAL_PIXEL);
		og[v1] = VertexProperties(1, 1, NORMAL_PIXEL);
		og[v2] = VertexProperties(2, 1, NORMAL_PIXEL);
		og[v3] = VertexProperties(3, 1, NORMAL_PIXEL);
		og[v4] = VertexProperties(4, 1, NORMAL_PIXEL);
		og[v5] = VertexProperties(5, 1, NORMAL_PIXEL);
		og[v7] = VertexProperties(7, 1, NORMAL_PIXEL);
		og[v9] = VertexProperties(9, 1, NORMAL_PIXEL);
}
/*void build_corsen_graph(OrientedGraph& graph)
{
    std::string absolutePath(
        "/home/eric/vle/vle-labs/paradevs/src/tests/corsen/data_s/");
    std::string modeFile(".mode");
    std::string parametersFile("par.txt");
    std::string elevationFile("alt");
    std::string outletFile("arbre");
    std::string layersFile("couche");
    std::string contextFile("contexte_yar_scenario.xml");
    std::string slopeFile("pav");
    std::vector < std::string* > files;
    Corsen c;

    files.push_back(&parametersFile);
    files.push_back(&modeFile);
    files.push_back(&elevationFile);
    files.push_back(&outletFile);
    files.push_back(&slopeFile);
    files.push_back(&contextFile);
    files.push_back(&layersFile);
    modify_file_paths(absolutePath, files);

    c.read(files, absolutePath);
    c.buildGraph();

    const DirectedGraph& g = c.getGraph().graph();
    std::vector < vertex_t > og_vertex_list;
    std::vector < vertex_t > dg_vertex_list;
    std::vector < int > dg_in_vertex_list;
    DirectedGraph::vertex_iterator it_dg, end_dg;

    tie(it_dg, end_dg) = vertices(g);
    for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
        og_vertex_list.push_back(add_vertex(graph));
        dg_vertex_list.push_back(*it_dg);
        dg_in_vertex_list.push_back(0);
    }

    tie(it_dg, end_dg) = vertices(g);
    for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
        DirectedGraph::adjacency_iterator neighbour_it, neighbour_end;

        tie(neighbour_it, neighbour_end) = adjacent_vertices(*it_dg, g);
        for (; neighbour_it != neighbour_end; ++neighbour_it) {
            uint index = 0;

            while (dg_vertex_list[index] != *neighbour_it) {
                ++index;
            }
            ++dg_in_vertex_list[index];
            boost::add_edge(og_vertex_list[i], og_vertex_list[index],
                            EdgeProperties(1.), graph);
        }
    }

    tie(it_dg, end_dg) = vertices(g);
    for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
        if (dg_in_vertex_list[i] == 0) {
            graph[og_vertex_list[i]] = VertexProperties(i, 1., TOP_PIXEL);
        } else {
            graph[og_vertex_list[i]] = VertexProperties(i, 1., NORMAL_PIXEL);
        }
    }
}*/

void brhtg_source(OrientedGraph *go,int nbr_vertex, int nbr_source, int nbr_v_min, int nbr_v_max, const std::vector<int> &niveau,
					std::vector<int> &Ram, std::vector<int> &Exu, const std::vector<vertex_to> &Vertexs,
					int nbr_passe, int nbr_npb){

	uint nbr_voisin;

	int niv=1;
	for(uint j =0; j<niveau.size()-1; j++){
		niv *= niveau.at(j);
	}

	int niv_tot=1;
	for(uint j =0; j<niveau.size(); j++){
		niv_tot *= niveau.at(j);
	}

	for(int b = 0; b<niv; b++){
		std::vector<uint> branche;
		int cpt = 0;
		while(cpt != niveau.at(niveau.size()-1)+1){
			branche.push_back((b*niveau.at(niveau.size()-1)+cpt)*nbr_npb);
			cpt++;
		}

		for(uint i =0; i<branche.size()-1;i++){
			for(uint j = branche.at(i); j<branche.at(i+1); j++){
				if(j==branche.at(branche.size()-1)-1){
					break;
				}
				else{
					nbr_voisin = rand_fini(nbr_v_min, nbr_v_max+1);
					uint cpt=0;
					Entiers ensemble;
					int cpt_nbr_s = 0;
					while(cpt != nbr_voisin){
						int val;
						if(j<branche.at(i)+nbr_source){ // Les sommets sont des sources donc imossible de recevoir des données
							val = rand_fini(j + 4, j + nbr_v_max + nbr_passe);
						}
						else if(j>=branche.at(i)+nbr_npb-nbr_passe && i!= (branche.size()-2)){ // Les sommets sont en fin de branche, nécessité de raccordement avec les autres branches
							val = rand_fini(branche.at(branche.size()-1)-4, branche.at(branche.size()-1));
						}
						else if(j>=branche.at(i)+nbr_npb-nbr_passe && i== (branche.size()-2)){ // Les sommets sont proche de l'exutoire
							val = rand_fini(j+1,branche.at(branche.size()-1));
						}
						else{ // Les sommets sont en plein milieu d'une branche
							val = rand_fini(j+1, j+nbr_passe);
						}

						if(In_tab(ensemble,val) != 1){
							ensemble.push_back(val);
							boost::add_edge(Vertexs.at(j), Vertexs.at(val), EdgeProperties(1.), *go);
							cpt++;
						}
						else{
							if((nbr_vertex-j) == cpt)
								break;
							else
								cpt_nbr_s++;
						}

						if(cpt_nbr_s>2*nbr_passe)
							break;
					}

				}
			}
		}

		/*for(int i =0; i<branche.size()-1; i++){
			for(int j = branche.at(i)+nbr_source; j<branche.at(i+1); j++){
				(*go)[Vertexs.at(j)] = VertexProperties(j, 1, NORMAL_PIXEL);
			}
		}

		for(int i =0; i<branche.size()-1; i++){
			for(int j = branche.at(i); j<branche.at(i)+nbr_source; j++){
				(*go)[Vertexs.at(j)] = VertexProperties(j, 1, TOP_PIXEL);
			}
		}*/

		Ram.push_back((niv_tot+b)*nbr_npb);
		Exu.push_back(branche.at(branche.size()-1)-1);
	}

	Ram.push_back(Ram.at(Ram.size()-1)+nbr_npb);

	if(niveau.size()>1){
		nbr_voisin = 2;
		for(uint k =0; k<Exu.size(); k++){
			for(uint l = Exu.at(k)-2; l<Exu.at(k)+1; l++){
				int cpt=0;
				Entiers ensemble;
				int cpt_nbr_s = 0;
				while(cpt!=nbr_voisin){
					int val = rand_fini(Ram.at(k),Ram.at(k)+2);

					if(In_tab(ensemble,val)!=1){
						ensemble.push_back(val);
						boost::add_edge(Vertexs.at(l), Vertexs.at(val), EdgeProperties(1.), *go);
						cpt++;
					}
					else{
						if((nbr_vertex-l)==cpt)
							break;
						else
							cpt_nbr_s++;
					}

					if(cpt_nbr_s>2*nbr_passe)
						break;
				}
			}
		}
	}
}

void brhtg_ramification(OrientedGraph *go, int nbr_vertex, int nbr_v_min, int nbr_v_max, const std::vector<int> &niveau,
					std::vector<int> &Ram, std::vector<int> &Exu, const std::vector<vertex_to> &Vertexs,
					int nbr_passe, int nbr_npb){

	int nbr_voisin;
	int cpt_ram = 1;
	while(cpt_ram != niveau.size()-1){
		int niv=1;
		for(int j =0; j<niveau.size()-1-cpt_ram; j++){
			niv *= niveau.at(j);
		}

		int niv_tot=0;
		for(int i =0; i<=cpt_ram; i++){
			int tmp_niv_tot=1;
			for(int j =0; j<niveau.size()-i; j++){
				tmp_niv_tot *= niveau.at(j);
			}
			niv_tot+=tmp_niv_tot;
		}

		std::vector<std::vector<int> > branche;
		int cpt_b=0;
		for(int j = 0; j<(Ram.size()-1)/niveau.at(niveau.size()-1-cpt_ram); j++){
			std::vector<int> tmp_branche;
			for(int i =0; i< niveau.at(niveau.size()-1-cpt_ram); i++){
				tmp_branche.push_back(Ram.at(i+cpt_b));
			}
			tmp_branche.push_back(tmp_branche.at(tmp_branche.size()-1)+nbr_npb);
			branche.push_back(tmp_branche);
			cpt_b+=niveau.at(niveau.size()-1-cpt_ram);
		}

		Ram.clear();
		Exu.clear();

		for(int b = 0; b<niv; b++){

			for(int i =0; i<branche.at(b).size()-1;i++){
				for(int j = branche.at(b).at(i); j<branche.at(b).at(i+1); j++){
					if(j==branche.at(b).at(branche.at(b).size()-1)-1){
						break;
					}
					else{
						nbr_voisin = rand_fini(nbr_v_min, nbr_v_max+1);
						int cpt=0;
						Entiers ensemble;
						int cpt_nbr_s = 0;
						while(cpt!=nbr_voisin){
							int val;

							if(j>=branche.at(b).at(i)+nbr_npb-nbr_passe && i!= (branche.at(b).size()-2)){ // Les sommets sont en fin de branche, nécessité de raccordement avec les autres branches
								val = rand_fini(branche.at(b).at(branche.at(b).size()-1)-4, branche.at(b).at(branche.at(b).size()-1));
							}
							else if(j>=branche.at(b).at(i)+nbr_npb-nbr_passe && i== (branche.at(b).size()-2)){ // Les sommets sont proche de l'exutoire
								val = rand_fini(j+1,branche.at(b).at(branche.at(b).size()-1));
							}
							else{ // Les sommets sont en plein milieu d'une branche
								val = rand_fini(j+1, j+nbr_passe);
							}

							if(In_tab(ensemble,val)!=1){
								ensemble.push_back(val);
								boost::add_edge(Vertexs.at(j), Vertexs.at(val), EdgeProperties(1.), *go);
								cpt++;
							}
							else{
								if((nbr_vertex-j)==cpt)
									break;
								else
									cpt_nbr_s++;
							}

							if(cpt_nbr_s>2*nbr_passe)
								break;
						}

					}
				}
			}
			Ram.push_back((niv_tot+b)*nbr_npb);
			Exu.push_back(branche.at(b).at(branche.at(b).size()-1)-1);

			/*for(int y =0; y<branche.at(b).size()-1; y++){
				for(int x = branche.at(b).at(y); x<branche.at(b).at(y+1); x++){
					(*go)[Vertexs.at(x)] = VertexProperties(x, 1, NORMAL_PIXEL);
				}
			}*/
		}

		Ram.push_back(Ram.at(Ram.size()-1)+nbr_npb);

		nbr_voisin = 2;
		for(int k =0; k<Exu.size(); k++){
			for(int l = Exu.at(k)-1; l<Exu.at(k)+1; l++){
				int cpt=0;
				Entiers ensemble;
				int cpt_nbr_s = 0;
				while(cpt!=nbr_voisin){
					int val = rand_fini(Ram.at(k),Ram.at(k)+2);

					if(In_tab(ensemble,val)!=1){
						ensemble.push_back(val);
						boost::add_edge(Vertexs.at(l), Vertexs.at(val), EdgeProperties(1.), *go);
						cpt++;
					}
					else{
						if((nbr_vertex-l)==cpt)
							break;
						else
							cpt_nbr_s++;
					}

					if(cpt_nbr_s>2*nbr_passe)
						break;
				}
			}
		}
		cpt_ram++;
	}
}

void brhtg_exutoire(OrientedGraph *go,int nbr_vertex,int nbr_v_min, int nbr_v_max,
		std::vector<int> &Ram, const std::vector<vertex_to> &Vertexs,
		int nbr_passe){

	int nbr_voisin;
	Ram.at(Ram.size()-1)=nbr_vertex;

	for(int i =0; i<Ram.size()-1;i++){
		for(int j = Ram.at(i); j<Ram.at(i+1); j++){
			if(j==Ram.at(Ram.size()-1)-1){
				//(*go)[Vertexs.at(j)] = VertexProperties(j, 1, NORMAL_PIXEL);
				break;
			}
			else{
				if(Ram.at(i+1)-j<nbr_passe)
					nbr_voisin = 2;
				else
					nbr_voisin = rand_fini(nbr_v_min, nbr_v_max+1);
				int cpt=0;
				Entiers ensemble;
				int cpt_nbr_s = 0;
				while(cpt!=nbr_voisin){
					int val;

					if(j>=Ram.at(i+1)-nbr_passe && i!=Ram.size()-2){ // Les sommets sont proche de l'exutoire
						val = rand_fini(Ram.at(Ram.size()-1)-4,Ram.at(Ram.size()-1));
					}
					else if(j>=Ram.at(i+1)-nbr_passe && i==Ram.size()-2){ // Les sommets sont proche de l'exutoire
						val = rand_fini(j+1,Ram.at(Ram.size()-1));
					}
					else{ // Les sommets sont en plein milieu d'une branche
						val = rand_fini(j+1, j+nbr_passe);
					}

					if(In_tab(ensemble,val)!=1&& j!=val){
						ensemble.push_back(val);
						boost::add_edge(Vertexs.at(j), Vertexs.at(val), EdgeProperties(1.), *go);
						cpt++;
					}
					else{
						if((nbr_vertex-j)==cpt)
							break;
						else
							cpt_nbr_s++;
					}

					if(cpt_nbr_s>2*nbr_passe)
						break;
				}

			}
			//(*go)[Vertexs.at(j)] = VertexProperties(j, 1, NORMAL_PIXEL);
		}
	}
}


void build_generator_graph(OrientedGraph *go, int nbr_vertex, int nbr_source, int nbr_v_min, int nbr_v_max,const std::vector<int> &niveau){
	std::vector<vertex_to> Vertexs;

	int nbr_npb;
	int nbr_branche = 0;
	int nbr_passe = 4;

	for(int i =0; i<nbr_vertex; i++){
		vertex_to vo = boost::add_vertex(*go);
		Vertexs.push_back(vo);
	}

	// Calcul du nombre de branches à partir de l'information des niveaux
	for(int i =0; i< niveau.size(); i++){
		int nbr_tmp =1;
		for(int j =0; j<=i; j++){
			nbr_tmp *= niveau.at(j);
		}
		nbr_branche += nbr_tmp;
	}

	nbr_npb = nbr_vertex/(nbr_branche);

	std::vector<int> Ram;
	std::vector<int> Exu;

	if(niveau.size()>1){
	/*
	 * *** Code Source ***
	 */

	brhtg_source(go,nbr_vertex,nbr_source,nbr_v_min,nbr_v_max,niveau,Ram,Exu,Vertexs,nbr_passe,nbr_npb);

	/*
	 * *** Code Ramifications ***
	 */

	brhtg_ramification(go,nbr_vertex,nbr_v_min,nbr_v_max,niveau,
			Ram,Exu,Vertexs,nbr_passe,nbr_npb);

	/*
	 * *** Code Exutoire ***
	 */

	brhtg_exutoire(go,nbr_vertex,nbr_v_min,nbr_v_max,
				Ram,Vertexs,nbr_passe);
				
	} else {
			brhtg_source(go,nbr_vertex,nbr_source,nbr_v_min,nbr_v_max,niveau,Ram,Exu,Vertexs,nbr_passe,nbr_npb);
	}
	
	std::vector < int > dg_in_vertex_list;
	std::vector <vertex_to> dg_vertex_list;
	OrientedGraph::vertex_iterator it_dg, end_dg;

	tie(it_dg, end_dg) = vertices(*go);
	for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
		dg_in_vertex_list.push_back(0);
		dg_vertex_list.push_back(*it_dg);
	}

	tie(it_dg, end_dg) = vertices(*go);
	for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
		OrientedGraph::adjacency_iterator neighbour_it, neighbour_end;

		tie(neighbour_it, neighbour_end) = adjacent_vertices(*it_dg, *go);
		for (; neighbour_it != neighbour_end; ++neighbour_it) {
			uint index = 0;

			while (dg_vertex_list[index] != *neighbour_it) {
				++index;
			}
			++dg_in_vertex_list[index];
		}
	}
	
	for(uint i = 0; i<num_vertices(*go); i++){
		if(dg_in_vertex_list.at(i) == 0){
			(*go)[i] = VertexProperties(i, 1, TOP_PIXEL);
		}else{
			(*go)[i] = VertexProperties(i, 1, NORMAL_PIXEL);
		}
	}

}

void build_generator_graph_linked(OrientedGraph *go, int nbr_vertex, int nbr_couche, int nbr_v_min, int nbr_v_max){
	std::vector<vertex_to> Vertexs;
	for(int i =0; i<nbr_vertex; i++){
		vertex_to vo = boost::add_vertex(*go);
		Vertexs.push_back(vo);
	}
	
	/*Création du vecteur contenant le nombre de sommets par couche*/
	int midel_couche = nbr_couche-2;
	int top_couche;
	if(nbr_couche>5){
		top_couche = floor(nbr_vertex/floor(midel_couche/2));
	}else{
		top_couche = floor(nbr_vertex/2);
	}
	int nbr_vertex_midel_couche = nbr_vertex - top_couche -1;
	int tmp = 0;
	std::vector<int> nbr_som_couche;
	nbr_som_couche.push_back(1);
	
	for(int i = 1; i <= midel_couche; i++){
		tmp += i;
	}
	
	for(int i = 0; i < midel_couche; i++){
		nbr_som_couche.push_back(floor((i+1)*nbr_vertex_midel_couche/tmp));
	}
	
	nbr_som_couche.push_back(top_couche);
	
	int sum = 0;
	for(int i = 0; i < nbr_som_couche.size(); i++){
		sum += nbr_som_couche.at(i);
	}
	int reste = nbr_vertex - sum;
	
	/*std::cout<<"sum : "<<sum<<std::endl;
	std::cout<<"midel_couche : "<<midel_couche<<std::endl;
	std::cout<<"top_couche : "<<top_couche<<std::endl;
	std::cout<<"nbr_vertex_midel_couche : "<<nbr_vertex_midel_couche<<std::endl;
	std::cout<<"tmp : "<<tmp<<std::endl;
	std::cout<<"reste : "<<reste<<std::endl;
	
	for(int i =0; i < nbr_som_couche.size(); i++){
		std::cout<<nbr_som_couche.at(i)<<" ";
	}
	std::cout<<std::endl;*/
	
	while(reste != 0){
		for(int i = 1; i <nbr_som_couche.size(); i++){
			nbr_som_couche.at(i) += 1;
			reste --;
			if(reste == 0){
				break;
			}
		}
	}
	
	/*for(int i =0; i < nbr_som_couche.size(); i++){
		std::cout<<nbr_som_couche.at(i)<<" ";
	}
	std::cout<<std::endl;*/
	
	/*Génération du graphe*/
	std::vector<int> tab_couche_som;
	tab_couche_som.push_back(0);
	
	for(int i =1; i<nbr_som_couche.size(); i++){
		tab_couche_som.push_back(tab_couche_som.at(i-1)+nbr_som_couche.at(i));
	}
	
	/*for(int i =0; i < tab_couche_som.size(); i++){
		std::cout<<tab_couche_som.at(i)<<" ";
	}
	std::cout<<std::endl;*/
	int nbr_vois = nbr_v_max+2;
	
	int cpt = 0;
	for(int i = tab_couche_som.at(tab_couche_som.size()-2)+1; i <= tab_couche_som.at(tab_couche_som.size()-1); i++){
		std::vector<int> vertex_tmp;
		int rand = rand_fini(nbr_v_min,nbr_v_max+1);
		int neigh_cpt = 0;
		while(neigh_cpt != rand){
			int val;
			if(cpt<nbr_vois){
				val = rand_fini(tab_couche_som.at(tab_couche_som.size()-3)+cpt+1,tab_couche_som.at(tab_couche_som.size()-3)+cpt+1+nbr_vois);
				//std::cout<<"val1 : "<<val<<std::endl;
			}else if(cpt>nbr_vois && cpt<(tab_couche_som.at(tab_couche_som.size()-2)-tab_couche_som.at(tab_couche_som.size()-3)+1-nbr_vois)){
				val = rand_fini(tab_couche_som.at(tab_couche_som.size()-3)+cpt+1-nbr_vois,tab_couche_som.at(tab_couche_som.size()-3)+cpt+1+nbr_vois);
				//std::cout<<"val2 : "<<val<<std::endl;
			}else{
				val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2)+1-2*nbr_vois,tab_couche_som.at(tab_couche_som.size()-2)+1);
				//std::cout<<"val3 : "<<val<<std::endl;
			}
			if(In_tab(vertex_tmp,val) != 1){
				boost::add_edge(Vertexs.at(i), Vertexs.at(val), EdgeProperties(1.), *go);
				vertex_tmp.push_back(val);
				neigh_cpt ++;
			}
		}
		cpt ++;
	}
	//std::cout<<"Passage !"<<std::endl;
	
	for(int j = 2; j < tab_couche_som.size()-1; j++){
		cpt = 0;
		for(int i = tab_couche_som.at(tab_couche_som.size()-1-j)+1; i <= tab_couche_som.at(tab_couche_som.size()-1-(j-1)); i++){
			std::vector<int> vertex_tmp;
			int rand = rand_fini(nbr_v_min,nbr_v_max+1);
			//std::cout<<"rand : "<<rand<<std::endl;
			int neigh_cpt = 0;
			while(neigh_cpt != rand){
				int val;
				if((tab_couche_som.at(tab_couche_som.size()-2-(j-1))-tab_couche_som.at(tab_couche_som.size()-2-j))>2*nbr_vois){
					if(cpt<nbr_vois){
						val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2-j)+cpt+1,tab_couche_som.at(tab_couche_som.size()-2-j)+cpt+1+nbr_vois);
						//std::cout<<"val1 : "<<val<<std::endl;
					}else if(cpt>nbr_vois && cpt<(tab_couche_som.at(tab_couche_som.size()-2-(j-1))-tab_couche_som.at(tab_couche_som.size()-2-j)+1-nbr_vois)){
						val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2-j)+cpt+1-nbr_vois,tab_couche_som.at(tab_couche_som.size()-2-j)+cpt+1+nbr_vois);
						//std::cout<<"val2 : "<<val<<std::endl;
					}else{
						if(2*nbr_vois<(tab_couche_som.at(tab_couche_som.size()-2-(j-1))-tab_couche_som.at(tab_couche_som.size()-2-j)+1)){
							val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2-(j-1))+1-2*nbr_vois,tab_couche_som.at(tab_couche_som.size()-2-(j-1))+1);
						}else{
							val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2-(j-1))+1-nbr_vois,tab_couche_som.at(tab_couche_som.size()-2-(j-1))+1);
						}
						//std::cout<<"val3 : "<<val<<std::endl;
					}
				}else{
					val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2-j)+1,tab_couche_som.at(tab_couche_som.size()-2-(j-1))+1);
				}
				//int val = rand_fini(tab_couche_som.at(tab_couche_som.size()-2-j)+1,tab_couche_som.at(tab_couche_som.size()-2-(j-1))+1);
				if(In_tab(vertex_tmp,val) != 1){
					boost::add_edge(Vertexs.at(i), Vertexs.at(val), EdgeProperties(1.), *go);
					vertex_tmp.push_back(val);
					neigh_cpt ++;	
				}
			}
			cpt ++;
		}
	}
	//std::cout<<"Passage2 !"<<std::endl;
	
	for(int i = tab_couche_som.at(0)+1; i <= tab_couche_som.at(1); i++){
		//int val = rand_fini(0,2);
		//if(val == 0){
			boost::add_edge(Vertexs.at(i), Vertexs.at(0), EdgeProperties(1.), *go);
		//}
	}
	//std::cout<<"Passage3 !"<<std::endl;
	
	std::vector < int > dg_in_vertex_list;
	std::vector <vertex_to> dg_vertex_list;
	OrientedGraph::vertex_iterator it_dg, end_dg;

	tie(it_dg, end_dg) = vertices(*go);
	for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
		dg_in_vertex_list.push_back(0);
		dg_vertex_list.push_back(*it_dg);
	}

	tie(it_dg, end_dg) = vertices(*go);
	for (uint i = 0; it_dg != end_dg; ++it_dg, ++i) {
		OrientedGraph::adjacency_iterator neighbour_it, neighbour_end;

		tie(neighbour_it, neighbour_end) = adjacent_vertices(*it_dg, *go);
		for (; neighbour_it != neighbour_end; ++neighbour_it) {
			uint index = 0;

			while (dg_vertex_list[index] != *neighbour_it) {
				++index;
			}
			++dg_in_vertex_list[index];
		}
	}
	
	for(uint i = 0; i<num_vertices(*go); i++){
		if(dg_in_vertex_list.at(i) == 0){
			(*go)[i] = VertexProperties(i, 1, TOP_PIXEL);
		}else{
			(*go)[i] = VertexProperties(i, 1, NORMAL_PIXEL);
		}
	}
}

void build_graph_grid(OrientedGraph *go, int side, const std::vector<std::pair<int,int>> &vertex_selection, const Entiers &weight_vertex,const char *edge_weight, bool rec){
	int nbr_vertex = side*side;
	std::vector<vertex_to> Vertexs;
	for(int i =0; i<nbr_vertex; i++){
		vertex_to vo = boost::add_vertex(*go);
		Vertexs.push_back(vo);
	}
	
	if(rec == true){
		std::ofstream fichier (edge_weight, std::ios::out);
		for(int i = 0; i<side; i++){
			for(int j = 0; j<side-1; j++){
				boost::add_edge(i*side+j, i*side+j+1,EdgeProperties(1), *go);
				fichier<<i*side+j<<" "<<i*side+j+1<<" "<<1<<" "<<std::endl;
			}
		}
		
		for(int i = 0; i<side-1; i++){
			for(int j = 0; j<side; j++){
				boost::add_edge(i*side+j, (i+1)*side+j,EdgeProperties(1), *go);
				fichier<<i*side+j<<" "<<(i+1)*side+j<<" "<<1<<" "<<std::endl;
			}
		}
		fichier.close();
	}else{
		for(int i = 0; i<side; i++){
			for(int j = 0; j<side-1; j++){
				boost::add_edge(i*side+j, i*side+j+1,EdgeProperties(1), *go);
			}
		}
		
		for(int i = 0; i<side-1; i++){
			for(int j = 0; j<side; j++){
				boost::add_edge(i*side+j, (i+1)*side+j,EdgeProperties(1), *go);
			}
		}
	}
		
	(*go)[0] = VertexProperties(0, 1, TOP_PIXEL);
	for(uint i = 1; i<num_vertices(*go); i++){
			(*go)[i] = VertexProperties(i, 1, NORMAL_PIXEL);
	}
	
	for(int ind=0; ind<vertex_selection.size(); ind++){
		for(int ind_i = vertex_selection.at(ind).first; ind_i<vertex_selection.at(ind).second+1; ind_i++){
			if(ind_i != 0)
				(*go)[ind_i] = VertexProperties(ind_i, weight_vertex.at(ind), NORMAL_PIXEL);
			else
				(*go)[ind_i] = VertexProperties(ind_i, weight_vertex.at(ind), TOP_PIXEL);
		}
	}
	
	if(rec == false){
		std::ifstream fichier (edge_weight, std::ios::in);
		
		if(fichier){
			bool found;
			edge_to e1;
			int edge1, edge2;
			double edge_weight;
			int lines = std::count(std::istreambuf_iterator<char>( fichier ),std::istreambuf_iterator<char>(),'\n' );
			//std::cout<<lines<<std::endl;
			int cpt =0;
			int length;
			fichier.seekg(0, std::ios::beg);
			while(cpt < lines){
			fichier >> edge1 >> edge2 >> edge_weight;
			tie(e1,found)=edge(vertex(edge1,*go),vertex(edge2,*go),*go);
			(*go)[e1] = EdgeProperties(edge_weight);
			//std::cout<<edge1<<" "<<edge2<<" "<<edge_weight<<std::endl;
			length = fichier.tellg();
			fichier.seekg(length+1, std::ios::beg);
			cpt++;
			}
			
		}else{
			std::cerr<<"Bugggggg du fichier txt !!!! "<<std::endl;
		}
		fichier.close();
	}
	
}

/*Vérification de la pondération + vérification des poids de la contraction + vérification de la structure de contraction*/

} } } // namespace paradevs tests boost_graph
