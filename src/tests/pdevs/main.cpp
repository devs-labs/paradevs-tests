/**
 * @file pdevs/tests.cpp
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

#include <tests/pdevs/graph_manager.hpp>
#include <tests/pdevs/models.hpp>

#include <paradevs/common/RootCoordinator.hpp>

using namespace paradevs::tests::pdevs;
using namespace paradevs::common;

int main()
{
    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::pdevs::Coordinator <
            DoubleTime,
            OnlyOneGraphManager < ThreeStateModel > >
        > rc(0, 10000, "root", paradevs::common::NoParameters(),
             paradevs::common::NoParameters());

    rc.run();

    return 0;

}
