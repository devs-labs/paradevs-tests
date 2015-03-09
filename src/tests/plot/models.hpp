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

namespace paradevs { namespace tests { namespace plot {

struct PlotParameters
{
    PlotParameters(double x, double y, int n) : x(x), y(y), neighbour_number(n)
    { }

    double x;
    double y;
    int neighbour_number;
};

struct PlotData
{
    PlotData() : x(-1), y(-1), quantity(0)
    { }

    PlotData(double x, double y, double quantity) : x(x), y(y),
                                                    quantity(quantity)
    { }

    double x;
    double y;
    double quantity;
};

class Plot : public paradevs::pdevs::Dynamics < common::DoubleTime,
                                                PlotParameters >
{
public:
    Plot(const std::string& name, const PlotParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime, PlotParameters >(
            name, parameters),
        _neighbour_number(parameters.neighbour_number)
    {
        _data.x = parameters.x;
        _data.y = parameters.y;
    }

    virtual ~Plot()
    { }

    void dint(typename common::DoubleTime::type /* t */)
    {
    }

    void dext(typename common::DoubleTime::type /* t */,
              typename common::DoubleTime::type /* e */,
              const common::Bag < common::DoubleTime >& /* bag */)
    {
    }

    void dconf(typename common::DoubleTime::type /* t */,
               typename common::DoubleTime::type /* e */,
               const common::Bag < common::DoubleTime >& /* bag */)
    {
        // dext(t, e, bag);
    }

    typename common::DoubleTime::type start(
        typename common::DoubleTime::type /* t */)
    {
        _data.quantity = 0;
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type /* t */) const
    { return common::DoubleTime::infinity; }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type /* t */) const
    {
        common::Bag < common::DoubleTime > bag;

        bag.push_back(common::ExternalEvent < common::DoubleTime >(
                          "out", (void*)(&_data)));
        return bag;
    }

    void observation(std::ostream& /* file */) const
    { }

private:
    // parameters
    unsigned int _neighbour_number;

    // data
    PlotData _data;
};

} } } // namespace paradevs tests plot

#endif
