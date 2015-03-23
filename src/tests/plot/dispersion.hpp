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

namespace paradevs { namespace tests { namespace plot {

#define SCALE 4
#define M_PI 3.14159265358979323846 

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
		paradevs::tests::boost_graph::Point destination1,destination2;
		
		//Modification à apporter pour prendre en compte la vitesse du vent
		//Valeurs arbitraire à modifier en fonction du parcellaire étudié
		double d = 400; //distance entre la source et le point d'infection maximum
		double sigma_x = 500; //dispertion sur l'axe x
		double moy_x   = d/std::cos(wind_direction);
		
		destination1  = destination;
		
		//Changement de repère centré en (source._x, source._y)
        destination1._x -= source._x;
        destination1._y -= source._y;
        
        //Rotation des axes d'angle wind_direction
        destination2._x = destination1._x*std::cos(wind_direction) + 
						  destination1._y*std::sin(wind_direction);
		destination2._y = -destination1._x*std::sin(wind_direction) + 
						  destination1._y*std::cos(wind_direction);
        
        double distance = std::sqrt(
            (destination2._x) * (destination2._x) +
            (destination2._y) * (destination2._y));
        
        std::pair<double,double> param_y;
        int caseSwitch = 1;
		switch (caseSwitch)
		{
		    case 1:
		        param_y.first  = 0.495;
		        param_y.second = 0.873;
		        break;
		    case 2:
		        param_y.first  = 0.310;
		        param_y.second = 0.897;
		        break;
		    case 3:
		        param_y.first  = 0.197;
		        param_y.second = 0.908;
		        break;
		    case 4:
		        param_y.first  = 0.122;
		        param_y.second = 0.916;
		        break;
		    case 5:
		        param_y.first  = 0.0934;
		        param_y.second = 0.912;
		        break;
		    case 6:
		        param_y.first  = 0.0625;
		        param_y.second = 0.911;
		        break;
		    default:
		        std::cout << "Problem ! Choose a good case !" << std::endl;
		        break;
		}
         
        double sigma_y = param_y.first * std::pow(distance, param_y.second);
        //Problème si calcul de dispertion intra parcelle -> voir modification
        
        double disp = 1/(M_PI*sigma_x*sigma_y) * 
					  std::exp(-0.5*std::pow((destination2._x-moy_x)/sigma_x,2)) *
					  std::exp(-0.5*std::pow(destination2._y/sigma_y,2));
					  
		/* Formule extraite du papier sur plume / par la vitesse du vent 
		 * (mais je ne vois pas ce que ça apporte)
		 * double disp = std::pow(10,3)/(M_PI*sigma_x*sigma_y*wind_speed) * 
					  std::exp(-0.5*std::pow((destination2._x-moy_x)/sigma_x,2)) *
					  std::exp(-0.5*std::pow(destination2._y/sigma_y,2));
		 */
		
		//Problème avec la normalisation de disp, manque d'information sur les voisins ...
		
		/** ATTENTION **/ 
		//Algorithme non validé et non testé, juste compilé.
       
        return disp;
    }
};

} } } // namespace paradevs tests plot

#endif
