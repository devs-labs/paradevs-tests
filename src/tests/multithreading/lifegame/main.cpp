/**
 * @file tests/multithreading/lifegame/main.cpp
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

#include <paradevs/common/RootCoordinator.hpp>

#include <tests/multithreading/lifegame/graph_manager.hpp>
#include <tests/multithreading/lifegame/models.hpp>

#include <chrono>

using namespace paradevs::common;
using namespace std::chrono;
using namespace paradevs::tests::multithreading::lifegame;

double lifegame_monothreading()
{
    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::Coordinator <
            DoubleTime,
            HierarchicalGraphManager,
            paradevs::common::NoParameters,
            GraphManagerParameters >
        > rc(0, 10, "root", paradevs::common::NoParameters(),
             GraphManagerParameters(4));

    steady_clock::time_point t1 = steady_clock::now();

    rc.run();

    steady_clock::time_point t2 = steady_clock::now();

    duration < double > time_span = duration_cast <
        duration < double > >(t2 - t1);

    return time_span.count();
}

double lifegame_multithreading(int cluster_number)
{
    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::multithreading::Coordinator <
            DoubleTime,
            ParallelHierarchicalGraphManager,
            paradevs::common::NoParameters,
            GraphManagerParameters >
        > rc(0, 10, "root", paradevs::common::NoParameters(),
             GraphManagerParameters(cluster_number));

    steady_clock::time_point t1 = steady_clock::now();

    rc.run();

    steady_clock::time_point t2 = steady_clock::now();

    duration < double > time_span = duration_cast <
        duration < double > >(t2 - t1);

    return time_span.count();
}

void lifegame(int n)
{
    if (n == 1) {
        std::cout << lifegame_monothreading() << std::endl;
    } else {
        std::cout << lifegame_multithreading(n) << std::endl;
        // lifegame_multithreading(n);
    }
}

int main(int argc, char** argv)
{
    if (argc > 1) {
        lifegame(atoi(argv[1]));
    }
    return 0;
}
