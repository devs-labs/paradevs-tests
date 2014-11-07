/**
 * @file tests/boost_graph/hierarchical_graph_manager.hpp
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

#ifndef __TESTS_BOOST_GRAPH_HIERARCHICAL_GRAPH_MANAGER_HPP
#define __TESTS_BOOST_GRAPH_HIERARCHICAL_GRAPH_MANAGER_HPP 1

#include <paradevs/common/scheduler/HeapScheduler.hpp>

#include <paradevs/kernel/pdevs/Coordinator.hpp>
#include <paradevs/kernel/pdevs/GraphManager.hpp>
#include <paradevs/kernel/pdevs/Simulator.hpp>

#include <tests/boost_graph/models.hpp>
#include <tests/boost_graph/graph_manager.hpp>

namespace paradevs { namespace tests { namespace boost_graph {

struct RootHierarchicalParameters
{
    int level_number;
    GraphGenerator& generator;

    RootHierarchicalParameters(int level, GraphGenerator& generator) :
        level_number(level), generator(generator)
    { }
};

struct HierarchicalParameters
{
    int index;
    int level_number;
    std::map < int, int >& cluster;
    OrientedGraph& graph;

    HierarchicalParameters(int index,
                           int level,
                           std::map < int, int >& cluster,
                           OrientedGraph& graph) :
        index(index), level_number(level), cluster(cluster), graph(graph)
    { }
};

template < class SchedulerHandle >
class LeafGraphManager :
        public paradevs::pdevs::GraphManager < common::DoubleTime,
                                               SchedulerHandle,
                                               HierarchicalParameters >
{
public:
    LeafGraphManager(
        common::Coordinator < common::DoubleTime,
                              SchedulerHandle >* coordinator,
        const HierarchicalParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime, SchedulerHandle,
                                        HierarchicalParameters >(
            coordinator, parameters)
    {
        build_flat_graph(parameters.index, parameters.graph,
                         parameters.cluster);

        // input
        {
            OrientedGraph::vertex_iterator vertexIt, vertexEnd;

            boost::tie(vertexIt, vertexEnd) = boost::vertices(parameters.graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                if (parameters.cluster.at(parameters.graph[*vertexIt]._index) !=
                    parameters.index) {
                    OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                    boost::tie(neighbourIt, neighbourEnd) =
                        boost::adjacent_vertices(*vertexIt, parameters.graph);
                    for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                        std::map < int, int >::const_iterator it =
                            parameters.cluster.find(
                                parameters.graph[*neighbourIt]._index);

                        if (it != parameters.cluster.end() and
                            it->second == parameters.index) {
                            std::ostringstream ss_in;

                            ss_in << "in_"
                                  << parameters.graph[*vertexIt]._index;
                            if (not coordinator->exist_in_port(ss_in.str())) {
                                coordinator->add_in_port(ss_in.str());
                            }
                            if (not LeafGraphManager <
                                    SchedulerHandle >::exist_link(
                                        coordinator, ss_in.str(),
                                        _normal_simulators[it->second], "in")) {
                                LeafGraphManager < SchedulerHandle>::add_link(
                                    coordinator, ss_in.str(),
                                    _normal_simulators[it->second], "in");
                            }

                            std::cout << "DEBUG: "
                                      << parameters.graph[*neighbourIt]._index
                                      << " input -> "
                                      << parameters.graph[*vertexIt]._index
                                      << " "
                                      << parameters.index << ":" << ss_in.str()
                                      << " -> "
                                      << parameters.graph[*neighbourIt]._index
                                      << ":in"
                                      << std::endl;
                        }
                    }
                }
            }
        }

        // output
        {
            OrientedGraph::vertex_iterator vertexIt, vertexEnd;

            boost::tie(vertexIt, vertexEnd) = boost::vertices(parameters.graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                if (parameters.cluster.at(parameters.graph[*vertexIt]._index) ==
                    parameters.index) {
                    OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                    boost::tie(neighbourIt, neighbourEnd) =
                        boost::adjacent_vertices(*vertexIt, parameters.graph);
                    for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                        std::map < int, int >::const_iterator it =
                            parameters.cluster.find(
                                parameters.graph[*neighbourIt]._index);

                        if (it != parameters.cluster.end() and
                            it->second != parameters.index) {
                            std::ostringstream ss_out;

                            ss_out << "out_"
                                   << parameters.graph[*vertexIt]._index;
                            if (not coordinator->exist_out_port(ss_out.str())) {
                                coordinator->add_out_port(ss_out.str());
                            }
                            if (_normal_simulators.find(it->first) !=
                                _normal_simulators.end()) {
                                if (not LeafGraphManager <
                                        SchedulerHandle >::exist_link(
                                            _normal_simulators[it->first],
                                            "out", coordinator, ss_out.str())) {
                                    LeafGraphManager <
                                        SchedulerHandle >::add_link(
                                            _normal_simulators[it->first],
                                            "out", coordinator, ss_out.str());
                                }

                                std::cout << "DEBUG: "
                                          << parameters.graph[*vertexIt]._index
                                          << " output -> "
                                          << parameters.graph[*neighbourIt]._index
                                          << " "
                                          << parameters.graph[*vertexIt]._index
                                          << ":out -> "
                                          << parameters.index << ":"
                                          << ss_out.str()
                                          << std::endl;

                            } else {
                                if (not LeafGraphManager <
                                        SchedulerHandle >::exist_link(
                                            _top_simulators[it->first],
                                            "out", coordinator, ss_out.str())) {
                                    LeafGraphManager <
                                        SchedulerHandle>::add_link(
                                            _top_simulators[it->first],
                                            "out", coordinator, ss_out.str());
                                }

                                std::cout << "DEBUG: "
                                          << parameters.graph[*vertexIt]._index
                                          << " output -> "
                                          << parameters.graph[*neighbourIt]._index
                                          << " "
                                          << parameters.graph[*vertexIt]._index
                                          << ":out -> "
                                          << parameters.index << ":"
                                          << ss_out.str()
                                          << std::endl;

                            }
                        }
                    }
                }
            }
        }
    }

    virtual ~LeafGraphManager()
    {
        for (typename TopSimulators::const_iterator it =
                 _top_simulators.begin(); it != _top_simulators.end();
             ++it) {
            delete it->second;
        }
        for (typename NormalSimulators::const_iterator it =
                 _normal_simulators.begin(); it != _normal_simulators.end();
             ++it) {
            delete it->second;
        }
    }

private:
    void build_flat_graph(int index, const OrientedGraph& g,
                          const std::map < int, int >& cluster)
    {
        OrientedGraph::vertex_iterator vertexIt, vertexEnd;

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt) {
            std::ostringstream ss;

            if (cluster.at(g[*vertexIt]._index) == index) {
                ss << "a" << g[*vertexIt]._index;

                std::cout << "DEBUG: " << g[*vertexIt]._index << " => "
                          << index << std::endl;

                switch (g[*vertexIt]._type) {
                case TOP_PIXEL:
                    _top_simulators[g[*vertexIt]._index] =
                        new pdevs::Simulator <
                            common::DoubleTime, TopPixel < SchedulerHandle >,
                            SchedulerHandle,
                            TopPixelParameters >(ss.str(),
                                                 TopPixelParameters());
                    _top_simulators[g[*vertexIt]._index]->add_out_port("out");
                    LeafGraphManager < SchedulerHandle >::add_child(
                        _top_simulators[g[*vertexIt]._index]);
                    break;
                case NORMAL_PIXEL:
                    unsigned int n = 0;
                    OrientedGraph::vertex_iterator vertexIt2, vertexEnd2;

                    std::cout << "DEBUG: " << g[*vertexIt]._index << " = [ ";
                    boost::tie(vertexIt2, vertexEnd2) = boost::vertices(g);
                    for (; vertexIt2 != vertexEnd2; ++vertexIt2) {
                        OrientedGraph::adjacency_iterator neighbourIt,
                            neighbourEnd;

                        boost::tie(neighbourIt, neighbourEnd) =
                            boost::adjacent_vertices(*vertexIt2, g);
                        for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                            if (g[*neighbourIt]._index == g[*vertexIt]._index) {
                                ++n;
                                std::cout << g[*vertexIt2]._index << " ";
                            }
                        }
                    }
                    std::cout << "]" << std::endl;

                    std::cout << "DEBUG: " << g[*vertexIt]._index << " => N = "
                              << n << std::endl;

                    _normal_simulators[g[*vertexIt]._index] =
                        new pdevs::Simulator <
                            common::DoubleTime, NormalPixel < SchedulerHandle >,
                            SchedulerHandle, NormalPixelParameters >(
                                ss.str(), NormalPixelParameters(n));
                    _normal_simulators[g[*vertexIt]._index]->add_in_port("in");
                    _normal_simulators[g[*vertexIt]._index]->add_out_port("out");
                    LeafGraphManager < SchedulerHandle >::add_child(
                        _normal_simulators[g[*vertexIt]._index]);
                    break;
                };
            }
        }

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt)
        {
            if (cluster.at(g[*vertexIt]._index) == index) {
                OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                std::cout << "DEBUG: " << g[*vertexIt]._index << " = [ ";
                boost::tie(neighbourIt, neighbourEnd) =
                    boost::adjacent_vertices(*vertexIt, g);
                for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                    std::map < int, int >::const_iterator it =
                        cluster.find(g[*neighbourIt]._index);
                    std::cout << g[*neighbourIt]._index << "/"
                              << it->second << " ";
                }
                std::cout << "]" << std::endl;

                boost::tie(neighbourIt, neighbourEnd) =
                    boost::adjacent_vertices(*vertexIt, g);
                for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                    std::map < int, int >::const_iterator it =
                        cluster.find(g[*neighbourIt]._index);

                    if (it != cluster.end() and it->second == index) {
                        paradevs::common::Model < common::DoubleTime,
                                                  SchedulerHandle >* a = 0;
                        paradevs::common::Model < common::DoubleTime,
                                                  SchedulerHandle >* b = 0;

                        if (g[*vertexIt]._type == TOP_PIXEL) {
                            a = _top_simulators[g[*vertexIt]._index];
                        } else {
                            a = _normal_simulators[g[*vertexIt]._index];
                        }
                        if (g[*neighbourIt]._type == TOP_PIXEL) {
                            b = _top_simulators[g[*neighbourIt]._index];
                        } else {
                            b = _normal_simulators[g[*neighbourIt]._index];
                        }
                        if (LeafGraphManager <
                                SchedulerHandle >::exist_link(a, "out",
                                                              b, "in")) {
                            std::cout << "== BUG == : "
                                      << g[*vertexIt]._index
                                      << ":out -> "
                                      << g[*neighbourIt]._index
                                      << ":in"
                                      << std::endl;
                        }


                            std::cout << "DEBUG: "
                                      << g[*vertexIt]._index
                                      << " internal -> "
                                      << g[*neighbourIt]._index
                                      << " "
                                      << g[*vertexIt]._index
                                      << ":out -> "
                                      << g[*neighbourIt]._index
                                      << ":in"
                                      << std::endl;

                            LeafGraphManager < SchedulerHandle >::add_link(
                                a, "out", b, "in");
                    }
                }
            }
        }
    }

    typedef std::map < int, pdevs::Simulator <
                                common::DoubleTime,
                                TopPixel < SchedulerHandle >,
                                SchedulerHandle,
                                TopPixelParameters >* > TopSimulators;
    typedef std::map < int, pdevs::Simulator <
                                common::DoubleTime,
                                NormalPixel < SchedulerHandle >,
                                SchedulerHandle,
                                NormalPixelParameters >* > NormalSimulators;

    TopSimulators    _top_simulators;
    NormalSimulators _normal_simulators;
};

template < class SchedulerHandle >
class HierarchicalGraphManager :
    public paradevs::pdevs::GraphManager < common::DoubleTime,
                                           SchedulerHandle,
                                           HierarchicalParameters >
{
public:
    HierarchicalGraphManager(
        common::Coordinator < common::DoubleTime,
                              SchedulerHandle >* coordinator,
        const HierarchicalParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime, SchedulerHandle,
                                        HierarchicalParameters >(
                                            coordinator, parameters)
    {
        std::map < int, int > cluster = parameters.cluster;

        int n1 = 1;
        int n2 = 0;
        for (int i = 1; i <= parameters.level_number; ++i) {
            n2 += 1 << i;
        }
        n2 >>= 1;
        n2++;

        OrientedGraph::vertex_iterator vertexIt, vertexEnd;
        boost::tie(vertexIt, vertexEnd) = boost::vertices(parameters.graph);
        for (; vertexIt != vertexEnd; ++vertexIt) {
            if (parameters.index ==
                parameters.cluster[parameters.graph[*vertexIt]._index]) {
                if (rand() % 2) {
                    parameters.cluster[parameters.graph[*vertexIt]._index] += n1;
                } else {
                    parameters.cluster[parameters.graph[*vertexIt]._index] += n2;
                }
            }
        }

        std::map < int, int > copy_cluster = parameters.cluster;

        // build coordinators
        for (unsigned int i = 0; i < 2; ++i) {
            unsigned index = parameters.index + ((i == 0) ? n1 : n2);

            if (parameters.level_number > 1) {
                Coordinator* coordinator = 0;
                std::ostringstream ss;

                ss << "S" << index;
                coordinator =
                    new Coordinator(ss.str(),
                                    paradevs::common::NoParameters(),
                                    HierarchicalParameters(
                                        index,
                                        parameters.level_number - 1,
                                        parameters.cluster,
                                        parameters.graph));
                _coordinators.push_back(coordinator);
                HierarchicalGraphManager < SchedulerHandle >::add_child(
                    coordinator);
            } else {
                LeafCoordinator* coordinator = 0;
                std::ostringstream ss;

                ss << "S" << index;
                coordinator =
                    new LeafCoordinator(ss.str(),
                                        paradevs::common::NoParameters(),
                                        HierarchicalParameters(
                                            index,
                                            -1,
                                            parameters.cluster,
                                            parameters.graph));
                _leaf_coordinators.push_back(coordinator);
                HierarchicalGraphManager < SchedulerHandle >::add_child(
                    coordinator);
            }
        }

        // build ports and connections
        // input
        for (unsigned int i = 0; i < 2; ++i) {
            unsigned index = (i == 0) ? n1 : n2;
            OrientedGraph::vertex_iterator vertexIt, vertexEnd;

            boost::tie(vertexIt, vertexEnd) = boost::vertices(parameters.graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                if (cluster.at(parameters.graph[*vertexIt]._index) !=
                    parameters.index) {
                    OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                    boost::tie(neighbourIt, neighbourEnd) =
                        boost::adjacent_vertices(*vertexIt, parameters.graph);
                    for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                        std::map < int, int >::const_iterator it =
                            copy_cluster.find(
                                parameters.graph[*neighbourIt]._index);

                        if (it != copy_cluster.end() and
                            it->second == parameters.index + index) {
                            std::ostringstream ss_in_a;
                            std::ostringstream ss_in_b;

                            ss_in_a << "in_"
                                    << parameters.graph[*vertexIt]._index;
                            ss_in_b << "in_"
                                    << parameters.graph[*vertexIt]._index;

                            std::cout << "DEBUG: ="
                                      << parameters.index
                                      << " "
                                      << (parameters.index + index)
                                      << " "
                                      << parameters.graph[*vertexIt]._index
                                      << " "
                                      << parameters.index << ":"
                                      << ss_in_a.str() << " -> "
                                      << (parameters.index + index) << ":"
                                      << ss_in_b.str()
                                      << std::endl;

                            if (not coordinator->exist_in_port(
                                    ss_in_a.str())) {
                                coordinator->add_in_port(
                                    ss_in_a.str());
                            }
                            if (parameters.level_number > 1) {
                                if (not _coordinators[i]->exist_in_port(
                                        ss_in_b.str())) {
                                    _coordinators[i]->add_in_port(
                                        ss_in_b.str());
                                }
                                if (not HierarchicalGraphManager <
                                    SchedulerHandle >::exist_link(
                                        coordinator, ss_in_a.str(),
                                        _coordinators[i], ss_in_b.str())) {
                                    HierarchicalGraphManager <
                                        SchedulerHandle >::add_link(
                                            coordinator, ss_in_a.str(),
                                            _coordinators[i], ss_in_b.str());
                                }
                            } else {
                                if (not _leaf_coordinators[i]->exist_in_port(
                                        ss_in_b.str())) {
                                    _leaf_coordinators[i]->add_in_port(
                                        ss_in_b.str());
                                }
                                if (not HierarchicalGraphManager <
                                        SchedulerHandle >::exist_link(
                                            coordinator, ss_in_a.str(),
                                            _leaf_coordinators[i], ss_in_b.str())) {
                                    HierarchicalGraphManager <
                                        SchedulerHandle >::add_link(
                                            coordinator, ss_in_a.str(),
                                            _leaf_coordinators[i], ss_in_b.str());
                                }
                            }
                        }
                    }
                }
            }
        }

        // output
        for (unsigned int i = 0; i < 2; ++i) {
            unsigned index = (i == 0) ? n1 : n2;
            OrientedGraph::vertex_iterator vertexIt, vertexEnd;

            boost::tie(vertexIt, vertexEnd) = boost::vertices(parameters.graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                if (copy_cluster.at(parameters.graph[*vertexIt]._index) ==
                    parameters.index + index) {
                    OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                    boost::tie(neighbourIt, neighbourEnd) =
                        boost::adjacent_vertices(*vertexIt, parameters.graph);
                    for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                        std::map < int, int >::const_iterator it =
                            cluster.find(
                                parameters.graph[*neighbourIt]._index);

                        if (it != cluster.end() and
                            it->second != parameters.index) {
                            std::ostringstream ss_out_a;
                            std::ostringstream ss_out_b;

                            ss_out_a << "out_"
                                     << parameters.graph[*vertexIt]._index;
                            ss_out_b << "out_"
                                     << parameters.graph[*vertexIt]._index;
                            if (not coordinator->exist_out_port(
                                    ss_out_b.str())) {
                                coordinator->add_out_port(
                                    ss_out_b.str());
                            }

                            std::cout << "DEBUG: ="
                                      << parameters.index
                                      << " "
                                      << (parameters.index + index)
                                      << " "
                                      << parameters.graph[*vertexIt]._index
                                      << " "
                                      << parameters.graph[*neighbourIt]._index
                                      << " "
                                      << (parameters.index + index) << ":"
                                      << ss_out_a.str() << " -> "
                                      << parameters.index << ":"
                                      << ss_out_b.str()
                                      << std::endl;

                            if (parameters.level_number > 1) {
                                if (not _coordinators[i]->exist_out_port(
                                        ss_out_a.str())) {
                                    _coordinators[i]->add_out_port(
                                        ss_out_a.str());
                                }
                                if (not HierarchicalGraphManager <
                                        SchedulerHandle >::exist_link(
                                            _coordinators[i],
                                            ss_out_a.str(),
                                            coordinator,
                                            ss_out_b.str())) {
                                    HierarchicalGraphManager <
                                        SchedulerHandle >::add_link(
                                            _coordinators[i],
                                            ss_out_a.str(),
                                            coordinator,
                                            ss_out_b.str());
                                }
                            } else {
                                if (not _leaf_coordinators[i]->exist_out_port(
                                        ss_out_a.str())) {
                                    _leaf_coordinators[i]->add_out_port(
                                        ss_out_a.str());
                                }
                                if (not HierarchicalGraphManager <
                                        SchedulerHandle >::exist_link(
                                            _leaf_coordinators[i],
                                            ss_out_a.str(),
                                            coordinator,
                                            ss_out_b.str())) {
                                    HierarchicalGraphManager <
                                        SchedulerHandle >::add_link(
                                            _leaf_coordinators[i],
                                            ss_out_a.str(),
                                            coordinator,
                                            ss_out_b.str());
                                }
                            }
                        }
                    }
                }
            }
        }

        // internal
        for (unsigned int i = 0; i < 2; ++i) {
            unsigned index1 = (i == 0) ? n1 : n2;
            unsigned index2 = (i == 0) ? n2 : n1;
            OrientedGraph::vertex_iterator vertexIt, vertexEnd;

            boost::tie(vertexIt, vertexEnd) = boost::vertices(parameters.graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                if (copy_cluster.at(parameters.graph[*vertexIt]._index) ==
                    parameters.index + index1) {
                    OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                    boost::tie(neighbourIt, neighbourEnd) =
                        boost::adjacent_vertices(*vertexIt, parameters.graph);
                    for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                        std::map < int, int >::const_iterator it =
                            copy_cluster.find(
                                parameters.graph[*neighbourIt]._index);

                        if (it != copy_cluster.end() and
                            it->second == parameters.index + index2) {
                            std::ostringstream ss_out_a;
                            std::ostringstream ss_out_b;

                            ss_out_a << "out_"
                                     << parameters.graph[*vertexIt]._index;
                            ss_out_b << "in_"
                                     << parameters.graph[*vertexIt]._index;

                            std::cout << "DEBUG: ="
                                      << parameters.index
                                      << " "
                                      << (parameters.index + index1)
                                      << " "
                                      << parameters.graph[*vertexIt]._index
                                      << " "
                                      << (parameters.index + index2)
                                      << " "
                                      << parameters.graph[*neighbourIt]._index
                                      << " "
                                      << (parameters.index + index1) << ":"
                                      << ss_out_a.str()
                                      << " -> "
                                      << (parameters.index + index2) << ":"
                                      << ss_out_b.str()
                                      << std::endl;

                            if (parameters.level_number > 1) {
                                if (not _coordinators[i]->exist_out_port(
                                        ss_out_a.str())) {
                                    _coordinators[i]->add_out_port(
                                        ss_out_a.str());
                                }
                                if (not _coordinators[(i==0)?1:0]->exist_in_port(
                                            ss_out_b.str())) {
                                    _coordinators[(i==0)?1:0]->add_in_port(
                                        ss_out_b.str());
                                }
                                if (not HierarchicalGraphManager <
                                        SchedulerHandle >::exist_link(
                                            _coordinators[i],
                                            ss_out_a.str(),
                                            _coordinators[(i==0)?1:0],
                                            ss_out_b.str())) {
                                    HierarchicalGraphManager <
                                        SchedulerHandle >::add_link(
                                            _coordinators[i],
                                            ss_out_a.str(),
                                            _coordinators[(i==0)?1:0],
                                            ss_out_b.str());
                                }
                            } else {
                                if (not _leaf_coordinators[i]->exist_out_port(
                                        ss_out_a.str())) {
                                    _leaf_coordinators[i]->add_out_port(
                                        ss_out_a.str());
                                }
                                if (not _leaf_coordinators[(i==0)?1:0]->exist_in_port(
                                        ss_out_b.str())) {
                                    _leaf_coordinators[(i==0)?1:0]->add_in_port(
                                        ss_out_b.str());
                                }
                                if (not HierarchicalGraphManager <
                                        SchedulerHandle >::exist_link(
                                            _leaf_coordinators[i],
                                            ss_out_a.str(),
                                            _leaf_coordinators[(i==0)?1:0],
                                            ss_out_b.str())) {
                                    HierarchicalGraphManager <
                                        SchedulerHandle >::add_link(
                                            _leaf_coordinators[i],
                                            ss_out_a.str(),
                                            _leaf_coordinators[(i==0)?1:0],
                                            ss_out_b.str());
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    virtual ~HierarchicalGraphManager()
    {
        for (typename Coordinators::const_iterator it = _coordinators.begin();
             it != _coordinators.end(); ++it) {
            delete *it;
        }
    }

private:
    // INTERNODE
    typedef paradevs::pdevs::Coordinator <
        common::DoubleTime,
        SchedulerType,
        SchedulerHandle,
        HierarchicalGraphManager < SchedulerHandle >,
        common::NoParameters,
        HierarchicalParameters > Coordinator;
    typedef std::vector < Coordinator* > Coordinators;

    Coordinators _coordinators;

    // LEAF
    typedef paradevs::pdevs::Coordinator <
        common::DoubleTime,
        SchedulerType,
        SchedulerHandle,
        LeafGraphManager < SchedulerHandle >,
        common::NoParameters,
        HierarchicalParameters > LeafCoordinator;
    typedef std::vector < LeafCoordinator* > LeafCoordinators;

    LeafCoordinators _leaf_coordinators;
};

template < class SchedulerHandle, class GraphBuilder >
class RootHierarchicalGraphManager :
    public paradevs::pdevs::GraphManager < common::DoubleTime,
                                           SchedulerHandle,
                                           RootHierarchicalParameters >
{
public:
    RootHierarchicalGraphManager(
        common::Coordinator < common::DoubleTime,
                              SchedulerHandle >* coordinator,
        const RootHierarchicalParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime, SchedulerHandle,
                                        RootHierarchicalParameters >(
                                            coordinator, parameters)
    {
        GraphBuilder graph_builder(parameters.generator);
        OrientedGraph graph;

        graph_builder.build(graph);

        if (parameters.level_number == 1) {
            std::map < int, int > cluster;

            OrientedGraph::vertex_iterator vertexIt, vertexEnd;
            boost::tie(vertexIt, vertexEnd) = boost::vertices(graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                cluster[graph[*vertexIt]._index] = 1;
            }
            {
                LeafCoordinator* coordinator = 0;

                coordinator =
                    new LeafCoordinator("S1", paradevs::common::NoParameters(),
                                        HierarchicalParameters(
                                            1,
                                            -1,
                                            cluster,
                                            graph));
                RootHierarchicalGraphManager < SchedulerHandle,
                                               GraphBuilder >::add_child(
                                                   coordinator);
            }
        } else {
            int n1 = 1;
            int n2 = 0;
            for (int i = 1; i <= parameters.level_number; ++i) {
                n2 += 1 << i;
            }
            n2 >>= 1;
            n2++;

            std::map < int, int > cluster;
            std::map < int, int > copy_cluster;

            OrientedGraph::vertex_iterator vertexIt, vertexEnd;
            boost::tie(vertexIt, vertexEnd) = boost::vertices(graph);
            for (; vertexIt != vertexEnd; ++vertexIt) {
                if (rand() % 2) {
                    cluster[graph[*vertexIt]._index] = n1;
                } else {
                    cluster[graph[*vertexIt]._index] = n2;
                }
            }
            copy_cluster = cluster;

            // build coordinators (graphs)
            for (unsigned int i = 0; i < 2; ++i) {
                Coordinator* coordinator = 0;
                std::ostringstream ss;
                unsigned index = (i == 0) ? n1 : n2;

                ss << "S" << index;
                coordinator =
                    new Coordinator(ss.str(), paradevs::common::NoParameters(),
                                    HierarchicalParameters(
                                        index,
                                        parameters.level_number - 1,
                                        cluster,
                                        graph));
                _coordinators.push_back(coordinator);
                RootHierarchicalGraphManager < SchedulerHandle,
                                               GraphBuilder >::add_child(
                                                   coordinator);

            }

            // build ports and connections
            // internal
            for (unsigned int i = 0; i < 2; ++i) {
                unsigned index = (i == 0) ? n1 : n2;
                OrientedGraph::vertex_iterator vertexIt, vertexEnd;

                boost::tie(vertexIt, vertexEnd) = boost::vertices(graph);
                for (; vertexIt != vertexEnd; ++vertexIt) {
                    if (copy_cluster.at(graph[*vertexIt]._index) != index) {
                        OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

                        boost::tie(neighbourIt, neighbourEnd) =
                            boost::adjacent_vertices(*vertexIt, graph);
                        for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                            std::map < int, int >::const_iterator it =
                                copy_cluster.find(graph[*neighbourIt]._index);

                            if (it != copy_cluster.end() and it->second == index) {

                                std::ostringstream ss_in_a;
                                std::ostringstream ss_in_b;

                                ss_in_a << "out_" << graph[*vertexIt]._index;
                                ss_in_b << "in_" << graph[*vertexIt]._index;

                                std::cout << "DEBUG: =>"
                                          << index
                                          << " "
                                          << graph[*vertexIt]._index
                                          << " "
                                          << ((i == 0) ? n2 : n1)
                                          << " "
                                          << ((i == 0) ? n2 : n1) << ":"
                                          << ss_in_a.str()
                                          << " -> " << index << ":"
                                          << ss_in_b.str()
                                          << std::endl;

                                if (not _coordinators[i]->exist_in_port(
                                        ss_in_b.str())) {
                                    _coordinators[i]->add_in_port(
                                        ss_in_b.str());
                                }
                                if (not _coordinators[(i==0)?1:0]->exist_out_port(
                                        ss_in_a.str())) {
                                    _coordinators[(i==0)?1:0]->add_out_port(
                                        ss_in_a.str());
                                }
                                if (not RootHierarchicalGraphManager <
                                        SchedulerHandle, GraphBuilder >::exist_link(
                                            _coordinators[(i==0)?1:0],
                                            ss_in_a.str(),
                                            _coordinators[i],
                                            ss_in_b.str())) {
                                    RootHierarchicalGraphManager <
                                        SchedulerHandle, GraphBuilder >::add_link(
                                            _coordinators[(i==0)?1:0],
                                            ss_in_a.str(),
                                            _coordinators[i],
                                            ss_in_b.str());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    virtual ~RootHierarchicalGraphManager()
    {
        for (typename Coordinators::const_iterator it = _coordinators.begin();
             it != _coordinators.end(); ++it) {
            delete *it;
        }
    }

private:
    typedef paradevs::pdevs::Coordinator <
        common::DoubleTime,
        SchedulerType,
        SchedulerHandle,
        HierarchicalGraphManager < SchedulerHandle >,
        common::NoParameters,
        HierarchicalParameters > Coordinator;
    typedef std::vector < Coordinator* > Coordinators;

    // LEAF
    typedef paradevs::pdevs::Coordinator <
        common::DoubleTime,
        SchedulerType,
        SchedulerHandle,
        LeafGraphManager < SchedulerHandle >,
        common::NoParameters,
        HierarchicalParameters > LeafCoordinator;

    Coordinators _coordinators;
};

class RootHierarchicalGraphBuilder
{
public:
    RootHierarchicalGraphBuilder(GraphGenerator& g) : generator(g)
    { }

    void build(OrientedGraph& graph)
    { generator.generate(graph); }

private:
    GraphGenerator& generator;
};

} } } // namespace paradevs tests boost_graph

#endif
