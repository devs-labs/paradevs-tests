/**
 * @file tests/mpi/main.cpp
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

#include <tests/mpi/graph_manager.hpp>

#include <paradevs/common/RootCoordinator.hpp>
#include <paradevs/kernel/pdevs/mpi/LogicalProcessor.hpp>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

using namespace paradevs::tests::mpi;
using namespace paradevs::common;
using namespace boost::mpi;

int main(int argc, char *argv[])
{
    environment env(argc, argv);
    communicator world;

    if (world.rank() == 0) {
        paradevs::tests::mpi::RootGraphManagerParameters parameters;

        parameters.S1_rank = 1;
        parameters.S2_rank = 2;

        paradevs::common::RootCoordinator <
            DoubleTime,
            paradevs::pdevs::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::RootGraphManager,
                paradevs::common::NoParameters,
                paradevs::tests::mpi::RootGraphManagerParameters >
            > rc(0, 10, "root", paradevs::common::NoParameters(), parameters);

        rc.run();
    } else {
        if (world.rank() == 1) {
            paradevs::pdevs::mpi::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::S1GraphManager > model(
                    "S1", paradevs::common::NoParameters(),
                    paradevs::common::NoParameters());
            paradevs::pdevs::mpi::LogicalProcessor <
                DoubleTime > LP(&model, world.rank(), 0);

            model.set_logical_processor(&LP);
            LP.loop();
        } else if (world.rank() == 2) {
            paradevs::pdevs::mpi::Coordinator <
                DoubleTime,
                paradevs::tests::mpi::S2GraphManager > model(
                    "S2", paradevs::common::NoParameters(),
                    paradevs::common::NoParameters());
            paradevs::pdevs::mpi::LogicalProcessor <
                DoubleTime > LP(&model, world.rank(), 0);

            model.set_logical_processor(&LP);
            LP.loop();
        }
    }

    return 0;
}
