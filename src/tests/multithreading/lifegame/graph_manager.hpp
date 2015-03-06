/**
 * @file tests/multithreading/lifegame/graph_manager.cpp
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

#ifndef TESTS_MULTITHREADING_LIFEGAME_GRAPH_MANAGER_HPP
#define TESTS_MULTITHREADING_LIFEGAME_GRAPH_MANAGER_HPP 1

#include <tests/multithreading/lifegame/models.hpp>
#include <tests/multithreading/lifegame/graph_builder.hpp>

#include <paradevs/kernel/pdevs/multithreading/Coordinator.hpp>
#include <paradevs/kernel/pdevs/GraphManager.hpp>
#include <paradevs/kernel/pdevs/Simulator.hpp>

namespace paradevs { namespace tests {
        namespace multithreading { namespace lifegame {

struct GraphParameters
{
    OrientedGraph _graph;
    InputEdges    _input_edges;
    OutputEdges   _output_edges;

    GraphParameters(const OrientedGraph& graph,
                    const InputEdges& input_edges,
                    const OutputEdges& output_edges) :
        _graph(graph), _input_edges(input_edges), _output_edges(output_edges)
    { }
};

template < class Parameters >
class FlatGraphManager :
                public paradevs::pdevs::GraphManager < common::DoubleTime,
                                                       Parameters >
{
public:
    FlatGraphManager(common::Coordinator < common::DoubleTime >* coordinator,
                     const Parameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime, Parameters >(
            coordinator, parameters)
    { }

    virtual ~FlatGraphManager()
    {
        for (typename Simulators::const_iterator it =
                 _simulators.begin(); it != _simulators.end();
             ++it) {
            delete it->second;
        }
    }

    void build_flat_graph(const OrientedGraph& g, const InputEdges& inputs)
    {
        OrientedGraph::vertex_iterator vertexIt, vertexEnd;

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt) {
            std::ostringstream ss;
            int i = g[*vertexIt]._index / NCOLUMNS;
            int j = g[*vertexIt]._index - i * NCOLUMNS;
            int neighbour_number = 0;

            if (i > 0) {
                ++neighbour_number;
            }
            if (i < NLINES - 1) {
                ++neighbour_number;
            }
            if (j > 0) {
                ++neighbour_number;
            }
            if (j < NCOLUMNS - 1) {
                ++neighbour_number;
            }

            ss << "a" << g[*vertexIt]._index;
            _simulators[g[*vertexIt]._index] =
                new pdevs::Simulator <
                    common::DoubleTime,
                Cell,
                CellParameters >(ss.str(),
                                 CellParameters(neighbour_number));
            _simulators[g[*vertexIt]._index]->add_out_port("out");
            _simulators[g[*vertexIt]._index]->add_in_port("in");
            FlatGraphManager < Parameters >::add_child(
                _simulators[g[*vertexIt]._index]);
        }

        boost::tie(vertexIt, vertexEnd) = boost::vertices(g);
        for (; vertexIt != vertexEnd; ++vertexIt)
        {
            OrientedGraph::adjacency_iterator neighbourIt, neighbourEnd;

            boost::tie(neighbourIt, neighbourEnd) =
                boost::adjacent_vertices(*vertexIt, g);
            for (; neighbourIt != neighbourEnd; ++neighbourIt) {
                paradevs::common::Model < common::DoubleTime >* a = 0;
                paradevs::common::Model < common::DoubleTime >* b = 0;

                a = _simulators[g[*vertexIt]._index];
                b = _simulators[g[*neighbourIt]._index];
                FlatGraphManager < Parameters >::add_link(a, "out",
                                                          b, "in");
            }
        }
    }

protected:
    typedef std::map < int, pdevs::Simulator <
                                common::DoubleTime,
                                Cell,
                                CellParameters >* > Simulators;

    Simulators _simulators;
};

class BuiltFlatGraphManager :
        public FlatGraphManager < GraphParameters >
{
public:
    BuiltFlatGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const GraphParameters& parameters) :
        FlatGraphManager < GraphParameters >(
            coordinator, parameters)
    {
        BuiltFlatGraphManager::build_flat_graph(
            parameters._graph, parameters._input_edges);

        // input
        for (Edges::const_iterator it = parameters._input_edges.begin();
             it != parameters._input_edges.end(); ++it) {
            std::ostringstream ss_in;

            ss_in << "in_" << it->first;
            if (not coordinator->exist_in_port(ss_in.str())) {
                coordinator->add_in_port(ss_in.str());
            }
            BuiltFlatGraphManager::add_link(
                coordinator, ss_in.str(),
                BuiltFlatGraphManager::_simulators[it->second], "in");
        }

        // output
        for (Edges::const_iterator it = parameters._output_edges.begin();
             it != parameters._output_edges.end(); ++it) {
            std::ostringstream ss_out;

            ss_out << "out_" << it->first;
            if (not coordinator->exist_out_port(ss_out.str())) {
                coordinator->add_out_port(ss_out.str());
            }
            if (not BuiltFlatGraphManager::exist_link(
                    BuiltFlatGraphManager::_simulators[it->first],
                    "out", coordinator, ss_out.str())) {
                BuiltFlatGraphManager::add_link(
                    BuiltFlatGraphManager::_simulators[it->first],
                    "out", coordinator, ss_out.str());
            }
        }
    }

    virtual ~BuiltFlatGraphManager()
    { }
};

class ParallelBuiltFlatGraphManager :
        public FlatGraphManager < GraphParameters >
{
public:
    ParallelBuiltFlatGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const GraphParameters& parameters) :
        FlatGraphManager < GraphParameters >(
            coordinator, parameters)
    {
        ParallelBuiltFlatGraphManager::build_flat_graph(
            parameters._graph, parameters._input_edges);

        // input
        for (Edges::const_iterator it = parameters._input_edges.begin();
             it != parameters._input_edges.end(); ++it) {
            std::ostringstream ss_in;

            ss_in << "in_" << it->first;
            if (not coordinator->exist_in_port(ss_in.str())) {
                coordinator->add_in_port(ss_in.str());
            }
            ParallelBuiltFlatGraphManager::add_link(
                coordinator, ss_in.str(),
                ParallelBuiltFlatGraphManager::_simulators[it->second],
                "in");
        }

        // output
        for (Edges::const_iterator it = parameters._output_edges.begin();
             it != parameters._output_edges.end(); ++it) {
            std::ostringstream ss_out;

            ss_out << "out_" << it->first;
            if (not coordinator->exist_out_port(ss_out.str())) {
                coordinator->add_out_port(ss_out.str());
            }
            if (not ParallelBuiltFlatGraphManager::exist_link(
                    ParallelBuiltFlatGraphManager::_simulators[it->first],
                    "out", coordinator, ss_out.str())) {
                ParallelBuiltFlatGraphManager::add_link(
                    ParallelBuiltFlatGraphManager::_simulators[it->first],
                    "out", coordinator, ss_out.str());
            }
        }
    }

    void init()
    { }

    void start(common::DoubleTime::type /* t */)
    { }

    void transition(
        const common::Models < common::DoubleTime >& /* receivers */,
        common::DoubleTime::type /* t */)
    { }

    virtual ~ParallelBuiltFlatGraphManager()
    { }
};

struct GraphManagerParameters
{
    GraphManagerParameters(int cluster_number) : cluster_number(cluster_number)
    { }

    int cluster_number;
};

class HierarchicalGraphManager :
                public paradevs::pdevs::GraphManager < common::DoubleTime,
                                                       GraphManagerParameters >
{
public:
    HierarchicalGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const GraphManagerParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime,
                                        GraphManagerParameters >(
                                            coordinator, parameters)
    {
        GraphBuilder graph_builder(parameters.cluster_number);
        OrientedGraphs graphs;
        InputEdgeList  input_edges;
        OutputEdgeList output_edges;
        Connections    parent_connections;

        graph_builder.build(graphs, input_edges, output_edges,
                            parent_connections);

        // build coordinators (graphs)
        for (unsigned int i = 0; i < graphs.size(); ++i) {
            Coordinator* coordinator = 0;
            std::ostringstream ss;

            ss << "S" << i;
            coordinator =
                new Coordinator(ss.str(), paradevs::common::NoParameters(),
                                GraphParameters(graphs[i],
                                                input_edges[i],
                                                output_edges[i]));
            _coordinators.push_back(coordinator);
            add_child(coordinator);
        }

        // builds internal connections (edges)
        for (Connections::const_iterator it = parent_connections.begin();
             it != parent_connections.end(); ++it) {
            const Connection& connection = *it;
            std::ostringstream ss_out;
            std::ostringstream ss_in;

            ss_out << "out_" << connection.first.second;
            ss_in << "in_" << connection.first.second;

            if (not exist_link(
                    _coordinators[connection.first.first - 1],
                    ss_out.str(),
                    _coordinators[connection.second.first - 1],
                    ss_in.str())) {

                add_link(
                    _coordinators[connection.first.first - 1],
                    ss_out.str(),
                    _coordinators[connection.second.first - 1],
                    ss_in.str());
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
    typedef paradevs::pdevs::Coordinator <
        common::DoubleTime,
        BuiltFlatGraphManager,
        common::NoParameters,
        GraphParameters
    > Coordinator;
    typedef std::vector < Coordinator* > Coordinators;

    Coordinators _coordinators;
};

class ParallelHierarchicalGraphManager :
                public paradevs::pdevs::GraphManager < common::DoubleTime,
                                                       GraphManagerParameters >
{
public:
    ParallelHierarchicalGraphManager(
        common::Coordinator < common::DoubleTime >* coordinator,
        const GraphManagerParameters& parameters) :
        paradevs::pdevs::GraphManager < common::DoubleTime,
                                        GraphManagerParameters >(
                                            coordinator, parameters)
    {
        GraphBuilder   graph_builder(parameters.cluster_number);
        OrientedGraphs graphs;
        InputEdgeList  input_edges;
        OutputEdgeList output_edges;
        Connections    parent_connections;

        graph_builder.build(graphs, input_edges, output_edges,
                            parent_connections);

        // build coordinators (graphs)
        for (unsigned int i = 0; i < graphs.size(); ++i) {
            ParallelCoordinator* coordinator = 0;
            std::ostringstream ss;

            ss << "S" << i;
            coordinator =
                new ParallelCoordinator(ss.str(),
                                        paradevs::common::NoParameters(),
                                        GraphParameters(graphs[i],
                                                        input_edges[i],
                                                        output_edges[i]));
            _coordinators.push_back(coordinator);
            add_child(coordinator);
        }

        // builds internal connections (edges)
        for (Connections::const_iterator it = parent_connections.begin();
             it != parent_connections.end(); ++it) {
            const Connection& connection = *it;
            std::ostringstream ss_out;
            std::ostringstream ss_in;

            ss_out << "out_" << connection.first.second;
            ss_in << "in_" << connection.first.second;

            if (not exist_link(
                    _coordinators[connection.first.first - 1],
                    ss_out.str(),
                    _coordinators[connection.second.first - 1],
                    ss_in.str())) {
                add_link(
                    _coordinators[connection.first.first - 1],
                    ss_out.str(),
                    _coordinators[connection.second.first - 1],
                    ss_in.str());
            }
        }
    }

    virtual ~ParallelHierarchicalGraphManager()
    {
        for (typename Coordinators::const_iterator it = _coordinators.begin();
             it != _coordinators.end(); ++it) {
            delete *it;
        }
    }

    void init()
    {
        for (typename Coordinators::const_iterator it = _coordinators.begin();
             it != _coordinators.end(); ++it) {
            (*it)->set_sender(
                dynamic_cast < paradevs::pdevs::multithreading::Coordinator <
                    common::DoubleTime,
                    ParallelHierarchicalGraphManager,
                    paradevs::common::NoParameters,
                    GraphManagerParameters >*
                >(get_coordinator())->get_sender());
        }
    }

    void start(common::DoubleTime::type t)
    {
        for (typename Coordinators::const_iterator it = _coordinators.begin();
             it != _coordinators.end(); ++it) {
            (*it)->get_sender().send(
                paradevs::pdevs::multithreading::start_message <
                    paradevs::common::DoubleTime >(t));
        }
    }

    void transition(const common::Models < common::DoubleTime >& receivers,
                    paradevs::common::DoubleTime::type t)
    {
        common::Models < common::DoubleTime >::const_iterator it =
            receivers.begin();

        while (it != receivers.end()) {
            if (not (*it)->is_atomic()) {
                typename Coordinators::const_iterator itc =
                    std::find(_coordinators.begin(), _coordinators.end(), *it);

                (*itc)->get_sender().send(
                    paradevs::pdevs::multithreading::transition_message <
                        paradevs::common::DoubleTime >(t));
            }
            ++it;
        }
    }

private:
    typedef paradevs::pdevs::multithreading::Coordinator <
      common::DoubleTime,
      ParallelBuiltFlatGraphManager,
      common::NoParameters,
      GraphParameters
    > ParallelCoordinator;
    typedef std::vector < ParallelCoordinator* > Coordinators;

    Coordinators _coordinators;
};

} } } } // namespace paradevs tests multithreading lifegame

#endif
