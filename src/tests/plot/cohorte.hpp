/**
 * @file tests/plot/cohorte.hpp
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

#ifndef TESTS_PLOT_COHORTE_HPP
#define TESTS_PLOT_COHORTE_HPP 1

namespace paradevs { namespace tests { namespace plot {

class Cohorte
{
public:
    Cohorte(int id, double InocPrim, double P1, double P2, double P3,
            double P4, double P5, double P6, double P7, double P8,
            double P9, double P10, double P11, double P12, double P13,
            double P14, double d0, double d1, double dc, double cum0,
            double cum1, double topt, double tmin, double fact, double ssa)
    {
        ID = id;
        count_AFFAIB = 0;

        TUDESPO = 0.0;
        CUMDDS = 0.0;
        SURVIE = 1.0;
        SPORES = InocPrim;
        GRAVI = 0.0;
        POIDS = 0.0;
        RET = 0.0;
        TINCUB = 0.0;
        AGE = 0.0;
        KASPOTHEO = 0.0;
        TRED = 0.0;
        AFFAIB = 0.0;
        KASPOREELLE = 0.0;
        POSPO = 0.0;
        SPOSPO = 0.0;
        TUSPORU = 0.0;
        CUMSPO = 0.0;
        ACTISPO = 0.0;
        SPORUL = 0.0;
        SURFMIL = 0.0;

        p1 = P1;
        p2 = P2;
        p3 = P3;
        p4 = P4;
        p5 = P5;
        p6 = P6;
        p7 = P7;
        p8 = P8;
        p9 = P9;
        p10 = P10;
        p11 = P11;
        p12 = P12;
        p13 = P13;
        p14 = P14;
        D0 = d0;
        D1 = d1;
        Dc = dc;
        CUM0 = cum0;
        CUM1 = cum1;
        Topt = topt;
        Tmin = tmin;
        FACT = fact;
        SSA = ssa;
    }

    virtual ~Cohorte()
    { };

    void compute(double Tmoy, double HR)
    {
        if (CUMDDS < D1) { //Compartiment Survie et Contamination
            //TUDESPO
            if (Tmoy < 18.0) {
                TUDESPO = p1 * Tmoy + p2;
            } else {
                TUDESPO = p3;
            }

            //CUMDDS
            CUMDDS += TUDESPO;

            //SURVIE
            if (HR > 90.0) {
                SURVIE = 1.0;
            } else if (CUMDDS <= Dc) {
                SURVIE = 1 - CUMDDS / Dc;
            } else {
                SURVIE = 0.0;
            }

            //SPORES
            SPORES *= SURVIE;

            //GRAVI
            if (HR < 90.0) {
                GRAVI = 0.0;
            } else {
                GRAVI = (CUMDDS - D0) / (D1 - D0);
            }

            //POIDS
            POIDS = SPORES * GRAVI;
        } else { //Compartiment Incubation et Sporulation
            //potentielle & réelle
            //RET
            if (Tmoy <= Topt) {
                RET = 0.0;
            } else {
                RET = p5 * std::pow((Tmoy - Topt), p6);
            }

            //TINCUB
            if (Tmoy <= 18.0) {
                TINCUB = p4 * Tmoy;
            } else {
                TINCUB = p4 * Tmoy - RET;
            }

            //AGE
            AGE += TINCUB;

            if (AGE > p7)
            {
                //KASPOTHEO
                if (AGE < p8) {
                    KASPOTHEO = FACT * (AGE - p7) / p7;
                } else if (AGE < p9) {
                    KASPOTHEO = FACT * (p9 - AGE) / p9;
                } else {
                    KASPOTHEO = 0.0;
                }

                //TRED
                if (Tmoy <= 18.0) {
                    TRED = 0.0;
                } else {
                    TRED = FACT * p10 * std::pow(Tmoy - Topt, p6);
                }

                //AFFAIB
                count_AFFAIB++;
                if (count_AFFAIB % 12 != 1) {
                    AFFAIB += TRED;
                } else {
                    AFFAIB = TRED; // Remise a 0 de AFFAIB toutes
                    // les 12 heures
                }
                //KASPOREELLE
                KASPOREELLE = KASPOTHEO - AFFAIB;

                //POSPO
                POSPO = POIDS * KASPOREELLE;

                //SPOSPO
                SPOSPO += POSPO;

                //TUSPORU
                if (HR <= 90.0) {
                    TUSPORU = 0.0;
                } else {
                    TUSPORU = p11 * std::pow(Tmoy - Tmin, 2) *
                        (p13 - p14 * (Tmoy - Tmin));
                }

                //CUMSPO
                CUMSPO += TUSPORU;

                //ACTISPO
                if (CUMSPO < CUM0) {
                    ACTISPO = 0.0;
                } else if (CUMSPO < CUM1) {
                    ACTISPO = (CUMSPO - CUM0) / (CUM1 - CUM0);
                } else {
                    ACTISPO = 1.0;
                }

                //SPORUL
                SPORUL = ACTISPO * SPOSPO;

                //SURFMIL
                SURFMIL = SPORUL * SSA;
            }
        }
    }

    double age() const
    { return AGE; }

    double sporul() const
    { return SPORUL; }

private:
    int ID;
    unsigned int count_AFFAIB;

    //paramètres du modèle de cohorte
    double p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
    double D0, D1, Dc;
    double CUM0, CUM1;
    double Topt, Tmin;
    double FACT, SSA;

    //variables d'état du modèle de cohorte
    double TUDESPO; // Unité de développement des spores en 1
                    // heure en fonction de la température horaire
    double CUMDDS;  // Evolution du développement des spores depuis
                    // l'instant initiale (arrivée de l'inoculum
                    // primaire). Il faut 100 unités pour que
                    // commence la germinantion des spores. A 150
                    // unités, toutes les spores ont germé.
    double SURVIE;  // Calcul de la survie des spores en fonction
                    // de l'HR et de CUMDDS. Si HR > 90, toutes
                    // les spores survivent ; si HR<90, la
                    // mortalité des spores augmente au fur et à
                    // mesure que  CUMDDS apporche de 100. Les
                    // spores qui ont commencé à émettre un tube
                    // germinantif meurent en 1 heure si HR < 90.
    double SPORES;  // C'est le nombre de spores au cours du temps
                    // en fonction de la survie. Cette variable
                    // donne à la fin (CUMDDS = 150) le nombre de
                    // spores réellement prêtes pour germer après
                    // la mortatilté due aus conditions climatiques.
    double GRAVI;   // Proportion de spores (parmi celles qui ont
                    // survecu = SPORES) ayant germé en fonction de HR
    double POIDS;   // Cette variable donne le nombre de spores
                    // contaminatrices, càd celles qui entrent en
                    // période d'incubation et qui sont
                    // susceptibles de sporuler en fonction des
                    // conditions de développement et climatiques
    double RET;     // Les températures > 18 °C diminuent la
                    // valeur de TINCUB. Cette variable permet de
                    // calculer cette diminution.
    double TINCUB;  // Unité de développement horaire des
                    // mycéliums pour provoquer les lésions (en
                    // fonction de  la témpérature horaire)
    double AGE;     // C'est la valeur au cours du temps du
                    // développement des mycéliums. 75 = nombre
                    // d'unités nécessaire pour la période
                    // d'incubation, ce qui correspond au début
                    // sporulation si condition favorable 150 =
                    // âge maximale de la lésion correspondant au
                    // maximum de sporulation 225 = toute la
                    // lésion est nécrosée, ce qui correspond à la
                    // fin de la sporulation si condition favorable
    double KASPOTHEO; // C'est la capacité de sporulation
                      // théorique d'une spore qui commence
                      // uniquement après qu'on ait atteint AGE =
                      // 75. Il augmente et atteint son maximum à
                      // AGE =150 et diminue jusqu'à 0 vers AGE = 225.
    double TRED;    // Quand T > 18 °C, il y a réduction de KASPO,
                    // cette variable quantifie cette diminution
                    // en fonction de la température horaire.
    double AFFAIB;  // C'est la somme de TRED qui est calculée par
                    // période de 12 heures.
    double KASPOREELLE; // C'est la capacité de sporulation réelle
                        // après enlèvement des effets dus à AFFAIB
    double POSPO;   // C'est la capacité de sporulation de
                    // l'ensemble des spores contaminatrices (POIDS)
    double SPOSPO;  // Potentiel de sporulation des lésions
                    // (causées par toutes les spores
                    // contaminatrices, tant que les conditions
                    // sont favorables) jusqu'à ce qu'elles ne
                    // sont plus en mesure de sporuler (necrose
                    // des lésions).
    double TUSPORU; // Unité de développement de la sporulation en
                    // fonction de la température horaire
    double CUMSPO;  // Degré de développement de la sporulation au
                    // cours du temps.
    double ACTISPO; // Proportion de spores effectivement
                    // produites par rapport à sporulation
                    // potentielle. Il faut CUMSPO = 6 pour
                    // débuter la sporulation réelle CUMSPO = 10,
                    // le potentiel est atteint
    double SPORUL;  // C'est le nombre de spores réellement
                    // produites et prêtes à être dispersées.
    double SURFMIL; // Conversion du nombre de spores en une
                    // taille de lésion. Elle correspond à la
                    // surface mildiousée.
};

} } } // namespace paradevs tests plot

#endif
