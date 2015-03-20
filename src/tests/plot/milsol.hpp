/**
 * @file tests/plot/milsol.hpp
 * @author The PARADEVS Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * PARADEVS - the multimodeling and simulation environment
 * This file is a part of the PARADEVS environment
 *
 * Copyright (C) 2013-2015 ULCO http://www.univ-litoral.fr
 * Copyright (C) 2009 INRA
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

#ifndef TESTS_PLOT_MILSOL_HPP
#define TESTS_PLOT_MILSOL_HPP 1

#include <tests/plot/climate.hpp>
#include <tests/plot/cohorte.hpp>
#include <vector>

namespace paradevs { namespace tests { namespace plot {

class Milsol
{
public:
    Milsol() : count(0), TotalSporeReady(0)
    {
        p1 = 0.5;
        p2 = 1.0;
        p3 = 10.0;
        p4 = 0.05;
        p5 = 0.025;
        p6 = 1.5;
        p7 = 75.0;
        p8 = 150.0;
        p9 = 225.0;
        p10 = 0.004;
        p11 = 0.009;
        p12 = 2.0;
        p13 = 1.0;
        p14 = 0.037;
        D0 = 100.0;
        D1 = 150.0;
        Dc = 100.0;
        CUM0 = 6.0;
        CUM1 = 10.0;
        Topt = 18.0;
        Tmin = 3.0;
        FACT = 2000.0;
        SSA = 1. / 17700;

        _total_ready_spore_number_t = 0.0;
        _total_ready_spore_number_t2 = 0.0;
        _total_ready_spore_number_t3 = 0.0;

        _zoospore_number = 0.0;
    }

    double get_ready_spore_number() const
    { return TotalSporeReady; }

    virtual void operator()(typename common::DoubleTime::type t)
    {
        _total_ready_spore_number_t = 0.0;

        _climate.get_temperatures(t, _temperatures);
        _climate.get_humidities(t, _humidities);

        createCohorte();
        for (unsigned int i = 0; i < _cohortes.size(); ++i) {
            for (unsigned int j = 0; j < 24 ; ++j) {
                _cohortes[i].compute(_temperatures[j], _humidities[j]);
                _total_ready_spore_number_t += _cohortes[i].sporul();
                if (condEndCohorte(i)) {
                    _cohortes.erase(_cohortes.begin() + i);
                    i--;
                    break;
                }
            }
        }
        _total_ready_spore_number_t2 = _total_ready_spore_number_t -
            _total_ready_spore_number_t3;
        _total_ready_spore_number_t3 = _total_ready_spore_number_t;
        TotalSporeReady = _total_ready_spore_number_t2;
    }

    void add_zoospore_number(double n)
    { _zoospore_number += n; }

    double get_zoospore_number() const
    { return _zoospore_number; }

    void set_inoculum_primaire_number(double n)
    { _zoospore_number = n; }

private:
    bool condEndCohorte(int k)
    { return (_cohortes[k].age() > p9); }

    bool condBeginCohorte()
    { return _zoospore_number > 0.0; }

    void createCohorte()
    {
        if (condBeginCohorte()) {
            ++count;
            _cohortes.push_back(
                Cohorte(count, _zoospore_number, p1, p2, p3, p4, p5,
                        p6, p7,
                        p8, p9, p10, p11, p12, p13, p14, D0, D1, Dc, CUM0,
                        CUM1, Topt, Tmin, FACT, SSA));
        }
    }

    double p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
    double D0, D1, Dc;
    double CUM0, CUM1;
    double Topt, Tmin;
    double FACT, SSA;

    std::vector < Cohorte > _cohortes;
    std::vector < double > _temperatures;
    std::vector < double > _humidities;
    int count;
    double TotalSporeReady;
    double _total_ready_spore_number_t;
    double _total_ready_spore_number_t2;
    double _total_ready_spore_number_t3;
    double _zoospore_number;

    Climate _climate;
};

} } } // namespace paradevs tests plot

#endif
