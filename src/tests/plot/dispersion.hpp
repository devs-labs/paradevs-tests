/**
 * @file tests/plot/dispersion.hpp
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

#ifndef TESTS_PLOT_DISPERSION_HPP
#define TESTS_PLOT_DISPERSION_HPP 1

#include <cmath>

#define SCALE 4

namespace paradevs { namespace tests { namespace plot {

class KleinDispersionFunction
{
public:
    KleinDispersionFunction()
    {
    }

    virtual double operator()(
        typename common::DoubleTime::type t,
        const paradevs::tests::boost_graph::Point& source,
        const paradevs::tests::boost_graph::Point& destination,
        double wind_speed,
        double wind_direction) const
    {
        double distance = std::sqrt(
            (destination._x - source._x) * (destination._x - source._x) +
            (destination._y - source._y) * (destination._y - source._y));
        double r, rt;

        r = distance * SCALE / 100;
        if (r <= 1.5) {
            rt = 0.340 - 0.405 * r + 0.128 * std::pow (r, 2.0);
        } else if (r <= 50) {
            rt = 0.03985 / (1.0 + std::pow (r, 3.12) / 3.80);
        } else { // r > 50m
            double rr, gamma, K;

            rr = 50;
            gamma = -2.29;
            //valeur la plus lourde -2.14, + legere - 2.56
            K = (0.03985 / (1.0 + std::pow (rr, 3.12) / 3.80)) / std::pow (1.0 + rr, gamma);
            rt = K * std::pow (1.0 + r, gamma);

            // std::cout << _index << "\t" << t << "\t" << r << "\t" << K << "\t" << rr << "\t" << rt << "\t" << value << std::endl;

        }
        return rt;
    }
};

class Plume2dDispersionFunction
{
public:
    Plume2dDispersionFunction()
    {
    }

    virtual double operator()(
        typename common::DoubleTime::type t,
        const paradevs::tests::boost_graph::Point& source,
        const paradevs::tests::boost_graph::Point& destination,
        double wind_speed,
        double wind_direction) const
    {
        paradevs::tests::boost_graph::Point destination1(destination);
        paradevs::tests::boost_graph::Point destination2;

        //Changement de repère centré en (source._x, source._y)
        destination1._x -= source._x;
        destination1._y -= source._y;

        //Rotation des axes d'angle wind_direction
        destination2._x = destination1._x * std::cos(wind_direction) +
            destination1._y * std::sin(wind_direction);
        destination2._y = -destination1._x * std::sin(wind_direction) +
            destination1._y * std::cos(wind_direction);


        if (destination2._x > 0) {
            // double sigma_y = 0.0787*x/(std::pow(1+x/707,0.135));
            // double sigma_z = 0.0475*x/(std::pow(1+x/707, 0.465));

            double sigma_y = 0.08 * destination2._x * SCALE *
                std::pow(1 + 0.0001 * destination2._x * SCALE, -0.5);
            double sigma_z = 0.06 * destination2._x * SCALE *
                std::pow(1 + 0.0015 * destination2._x * SCALE , -0.5);

            double h = 20;
            double disp = 1. / (2 * M_PI * wind_speed * sigma_y * sigma_z) *
                std::exp(-0.5 * std::pow(h / sigma_z, 2)) *
                std::exp(-0.5 * std::pow(destination2._y * SCALE / sigma_y, 2));

            return disp * 3600 * 24 * 1e4;
        } else {
            return 0.0;
        }
    }
};

} } } // namespace paradevs tests plot

#endif
