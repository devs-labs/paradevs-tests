/**
 * @file tests/plot/climate.hpp
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

#ifndef TESTS_PLOT_CLIMATE_HPP
#define TESTS_PLOT_CLIMATE_HPP 1

namespace paradevs { namespace tests { namespace plot {

class Climate
{
public:
    Climate()
    {
    }

    virtual void get_humidities(typename common::DoubleTime::type t,
                                std::vector < double >& humidities) const
    {
        humidities.clear();
        for (unsigned int i = 0; i < 24; ++i) {
            humidities.push_back(95.);
        }
    }

    virtual void get_temperatures(typename common::DoubleTime::type t,
                                  std::vector < double >& temperatures) const
    {
        temperatures.clear();
        for (unsigned int i = 0; i < 9; ++i) {
            temperatures.push_back(10.);
        }
        for (unsigned int i = 9; i < 19; ++i) {
            temperatures.push_back(20.);
        }
        for (unsigned int i = 19; i < 24; ++i) {
            temperatures.push_back(10.);
        }
    }

    virtual void get_wind_direction(
        typename common::DoubleTime::type t,
        std::vector < double >& wind_direction) const
    {
        wind_direction.clear();
        for (unsigned int i = 0; i < 24; ++i) {
            wind_direction.push_back(0.);
        }
    }

    virtual void get_wind_speed(typename common::DoubleTime::type t,
                                std::vector < double >& wind_speed) const
    {
        wind_speed.clear();
        for (unsigned int i = 0; i < 24; ++i) {
            wind_speed.push_back(20.);
        }
    }
};

} } } // namespace paradevs tests plot

#endif
