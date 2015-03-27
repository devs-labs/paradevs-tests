/**
 * @file tests/plot/models.hpp
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

#ifndef TESTS_PLOT_MODELS_HPP
#define TESTS_PLOT_MODELS_HPP 1

#include <paradevs/common/time/DoubleTime.hpp>
#include <paradevs/kernel/pdevs/Dynamics.hpp>

#include <tests/plot/graph_defs.hpp>
#include <tests/plot/dispersion.hpp>
#include <tests/plot/milsol.hpp>

#include <shapefil.h>

#include <cmath>

#define WIND_SPEED 6
#define WIND_DIRECTION 0
#define PLOT_ID 297

namespace paradevs { namespace tests { namespace plot {

struct PlotParameters
{
    PlotParameters(int index,
                   const paradevs::tests::boost_graph::Points& points,
                   int neighbour_number)
        : _index(index), _points(points), _neighbour_number(neighbour_number)
    { }

    int _index;
    paradevs::tests::boost_graph::Points _points;
    int _neighbour_number;
};

typedef std::vector < double > Doubles;

struct PlotData
{
    PlotData() : index(-1)
    { }

    PlotData(int index, const Points& points, Doubles ready_spore_numbers) :
        index(index), points(points), ready_spore_numbers(ready_spore_numbers)
    { }

    int index;
    Points points;
    Doubles ready_spore_numbers;
};

class Plot : public paradevs::pdevs::Dynamics < common::DoubleTime,
                                                PlotParameters >
{
public:
    Plot(const std::string& name, const PlotParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime, PlotParameters >(
            name, parameters),
        _index(parameters._index),
        _points(parameters._points),
        _neighbour_number(parameters._neighbour_number)
    {
        _data.index = _index;
        _data.points = parameters._points;

        for (Points::const_iterator it = parameters._points.begin();
             it != parameters._points.end(); ++it) {
            _milsol.push_back(Milsol());
            _data.ready_spore_numbers.push_back(0.);
        }

        // std::cout << "(" << _centroid._x << "," << _centroid._y << "): ";
        // for (Points::const_iterator it = _neighbour_centroids.begin();
        //      it != _neighbour_centroids.end(); ++it) {
        //     double distance = std::sqrt(
        //         (_centroid._x - it->_x) * (_centroid._x - it->_x) +
        //         (_centroid._y - it->_y) * (_centroid._y - it->_y));

        //     std::cout << "(" << it->_x << "," << it->_y << ") -> " << distance
        //               << " ";
        // }
        // std::cout << std::endl;

        _handle = DBFOpen("/home/eric/tmp/parcelle/test/parcellaire.dbf",
                          "rb+");

        if (!_handle) {
            std::cout << "Error" << std::endl;
        }

    }

    virtual ~Plot()
    {
        DBFClose(_handle);
    }

    void dint(typename common::DoubleTime::type t)
    {
        // std::cout << _index << " " << t << ": dint - BEFORE - "
        //           << _phase << std::endl;

        if (_phase == SEND) {
            if (_neighbour_number > 0) {
                _phase = WAIT;
                _sigma = common::DoubleTime::infinity;
            } else {
                _phase  = NEWSTATE;
                _sigma = 0;
            }
        } else if (_phase == NEWSTATE) {
            // cohorte update
            int i = 0;

            for (std::vector < Milsol >::iterator it = _milsol.begin();
                 it != _milsol.end(); ++it) {
                (*it)(t);
                _data.ready_spore_numbers[i] = it->get_ready_spore_number();
                ++i;
            }

            // compute
            // neighbour -> local
            {
                for (int k = 0; k < _data.points.size(); ++k) {
                    double sum = 0;

                    for (std::vector < PlotData >::iterator it =
                             _neighbour_data.begin();
                         it != _neighbour_data.end(); ++it) {
                        for (int i = 0; i < it->points.size(); ++i) {
                            if (it->ready_spore_numbers[i] > 0) {
                                sum += _dispersion_function(
                                    t,
                                    paradevs::tests::boost_graph::Point(
                                        it->points[i]._x, it->points[i]._y),
                                    _data.points[k],
                                    WIND_SPEED, WIND_DIRECTION) *
                                    it->ready_spore_numbers[i];
                            }
                        }
                    }
                    _milsol[k].add_zoospore_number(sum);
                }
            }

            // local -> local
            {
                for (int k = 0; k < _data.points.size(); ++k) {
                    double sum = 0;

                    for (int i = 0; i < _data.points.size(); ++i) {
                        if (i != k and _data.ready_spore_numbers[i] > 0) {
                            sum += _dispersion_function(
                                t, _data.points[i], _data.points[k],
                                WIND_SPEED, WIND_DIRECTION) *
                                _data.ready_spore_numbers[i];
                        }
                    }
                    _milsol[k].add_zoospore_number(sum);
                }
            }

            // sum += _dispersion_function(
            //     t, _centroid,
            //     paradevs::tests::boost_graph::Point(_centroid._x + 10,
            //                                         _centroid._y), 6., 0.) *
            //     _data.ready_spore_number;

            {
                double sum = 0;
                double p = 0;

                for (int k = 0; k < _data.points.size(); ++k) {
                    sum += _milsol[k].get_zoospore_number();
                }
                sum /= _data.points.size();
                if (sum > 0) {
                    p = std::log10(sum);
                    if (p < 0) {
                        p = 0;
                    }
                }

            // std::cout << t << "\t" << _index << "\t"
            //           << _data.ready_spore_number << "\t"
            //           << _milsol.get_zoospore_number() << "\t"
            //           << sum << "\t"
            //           << p << std::endl;

                DBFWriteDoubleAttribute(_handle, _index, 0, p);
            }

            _phase  = SEND;
            _sigma = 1;
            _received = 0;
            _neighbour_data.clear();
        }

        // std::cout << _index << " " << t << ": dint - AFTER - "
        //           << _phase << std::endl;

    }

    void dext(typename common::DoubleTime::type t,
              typename common::DoubleTime::type /* e */,
              const common::Bag < common::DoubleTime >& bag)
    {
        for (common::Bag < common::DoubleTime >::const_iterator it =
                 bag.begin(); it != bag.end(); ++it) {
            _neighbour_data.push_back(*(PlotData*)(it->get_content()));
            ++_received;
        }

        if (_received == _neighbour_number) {
            _phase = NEWSTATE;
            _sigma = 0;
        } else {
            _phase = WAIT;
            _sigma = common::DoubleTime::infinity;
        }

        // std::cout << _index << " " << t << ": dext - "
        //           << _received << "/"
        //           << _neighbour_centroids.size()
        //           << " " << _phase << std::endl;
    }

    void dconf(typename common::DoubleTime::type t,
               typename common::DoubleTime::type e,
               const common::Bag < common::DoubleTime >& bag)
    {

        // std::cout << t << "[" << get_name() << "]: dconf" << std::endl;

        dext(t, e, bag);
    }

    typename common::DoubleTime::type start(
        typename common::DoubleTime::type /* t */)
    {
        for (std::vector < Milsol >::iterator it = _milsol.begin();
             it != _milsol.end(); ++it) {
            it->set_inoculum_primaire_number(0);
        }

        if (_index == PLOT_ID) {
            _milsol[0].set_inoculum_primaire_number(1e6);
        }

        _phase = SEND;
        if (_neighbour_number > 0) {
            _sigma = 0;
        } else {
            _sigma = 1;
        }
        _received = 0;
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type /* t */) const
    { return _sigma; }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {
        common::Bag < common::DoubleTime > bag;

        if (_phase == SEND) {
            bag.push_back(common::ExternalEvent < common::DoubleTime >(
                              "out", (void*)(&_data)));
        }
        return bag;
    }

    void observation(std::ostream& /* file */) const
    { }

private:
    enum Phase { WAIT, SEND, NEWSTATE, NEXT };

    // parameters
    int _index;
    paradevs::tests::boost_graph::Points _points;
    int _neighbour_number;

    DBFHandle _handle;

    // data
    PlotData                 _data;
    std::vector < PlotData > _neighbour_data;

    // submodels
    Plume2dDispersionFunction _dispersion_function;
    // KleinDispersionFunction _dispersion_function;
    std::vector < Milsol >  _milsol;
    Climate                 _climate;

    // state
    Phase _phase;
    common::DoubleTime::type _sigma;
    int _received;
};

} } } // namespace paradevs tests plot

#endif
