/**
 * @file tests/dtss/tests.cpp
 * @author The PARADEVS Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * PARADEVS - the multimodeling and simulation environment
 * This file is a part of the PARADEVS environment
 *
 * Copyright (C) 2013 ULCO http://www.univ-litoral.fr
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

#include <paradevs/common/scheduler/SchedulerHandle.hpp>

#include <tests/dtss/graph_manager.hpp>
#include <tests/dtss/models.hpp>

#include <paradevs/common/RootCoordinator.hpp>

#define CATCH_CONFIG_MAIN
#include <tests/catch.hpp>

using namespace paradevs::tests::dtss;
using namespace paradevs::common;

TEST_CASE("dtss/only_one", "run")
{

    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::dtss::Coordinator <
            DoubleTime,
            Policy < paradevs::common::scheduler::NoSchedulerHandle >,
            OnlyOneGraphManager <
                paradevs::common::scheduler::NoSchedulerHandle >,
            paradevs::common::scheduler::NoSchedulerHandle,
            paradevs::dtss::Parameters < DoubleTime > >
        > rc(0, 10, "root",
             paradevs::dtss::Parameters < DoubleTime >(1),
             paradevs::common::NoParameters());

    paradevs::common::Trace < DoubleTime >::trace().clear();
    rc.run();

    REQUIRE(paradevs::common::Trace <
                DoubleTime >::trace().elements().
            filter_model_name("a").
            filter_type(paradevs::common::START).size() == 1);
    for (double t = 0; t <= 10; ++t) {
        REQUIRE(paradevs::common::Trace <
                    DoubleTime >::trace().elements().
                filter_model_name("a").filter_time(t).
                filter_type(paradevs::common::DELTA_INT).size() == 1);
        REQUIRE(paradevs::common::Trace <
                    DoubleTime >::trace().elements().
                filter_model_name("a").filter_time(t).
                filter_type(paradevs::common::LAMBDA).size() == 1);
    }
}

TEST_CASE("dtss/two", "run")
{
    paradevs::common::RootCoordinator <
        DoubleTime, paradevs::dtss::Coordinator <
            DoubleTime,
            Policy < paradevs::common::scheduler::NoSchedulerHandle >,
            TwoGraphManager <
                paradevs::common::scheduler::NoSchedulerHandle >,
            paradevs::common::scheduler::NoSchedulerHandle,
            paradevs::dtss::Parameters < DoubleTime > >
        > rc(0, 10, "root",
             paradevs::dtss::Parameters < DoubleTime >(1),
             paradevs::common::NoParameters());

    paradevs::common::Trace < DoubleTime >::trace().clear();
    rc.run();

    REQUIRE(paradevs::common::Trace <
                DoubleTime >::trace().elements().
            filter_model_name("a").
            filter_type(paradevs::common::START).size() == 1);
    for (unsigned int t = 0; t <= 10; ++t) {
        REQUIRE(paradevs::common::Trace <
                    DoubleTime >::trace().elements().
                filter_model_name("a").filter_time(t).
                filter_type(paradevs::common::DELTA_INT).size() == 1);
        REQUIRE(paradevs::common::Trace <
                    DoubleTime >::trace().elements().
                filter_model_name("a").filter_time(t).
                filter_type(paradevs::common::LAMBDA).size() == 1);
    }

    REQUIRE(paradevs::common::Trace <
                DoubleTime >::trace().elements().
            filter_model_name("b").
            filter_type(paradevs::common::START).size() == 1);
    for (unsigned int t = 0; t <= 10; ++t) {
        REQUIRE(paradevs::common::Trace <
                    DoubleTime >::trace().elements().
                filter_model_name("b").filter_time(t).
                filter_type(paradevs::common::DELTA_INT).size() == 1);
        REQUIRE(paradevs::common::Trace <
                    DoubleTime >::trace().elements().
                filter_model_name("b").filter_time(t).
                filter_type(paradevs::common::LAMBDA).size() == 1);
    }
}
