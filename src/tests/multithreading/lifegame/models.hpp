/**
 * @file tests/multithreading/lifegame/models.hpp
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

#ifndef TESTS_MULTITHREADING_LIFEGAME_MODELS_HPP
#define TESTS_MULTITHREADING_LIFEGAME_MODELS_HPP 1

#include <paradevs/common/time/DoubleTime.hpp>

#include <paradevs/kernel/pdevs/Dynamics.hpp>

namespace paradevs { namespace tests {
        namespace multithreading { namespace lifegame {


template < class SchedulerHandle>
class Cell :
        public paradevs::pdevs::Dynamics < common::DoubleTime, SchedulerHandle >
{
public:
    Cell(const std::string& name, const common::NoParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime,
                                    SchedulerHandle >(name, parameters)
    { }
    virtual ~Cell()
    { }

    void dint(typename common::DoubleTime::type t)
    {
    }

    void dext(typename common::DoubleTime::type t,
              typename common::DoubleTime::type /* e */,
              const common::Bag < common::DoubleTime, SchedulerHandle >& msgs)
    {
    }

    void dconf(typename common::DoubleTime::type t,
               typename common::DoubleTime::type /* e */,
               const common::Bag < common::DoubleTime, SchedulerHandle >& msgs)
    {
    }

    typename common::DoubleTime::type start(typename common::DoubleTime::type t)
    {
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type t) const
    {
    }

    common::Bag < common::DoubleTime, SchedulerHandle > lambda(
        typename common::DoubleTime::type t) const
    {
        common::Bag < common::DoubleTime, SchedulerHandle > msgs;

        return msgs;
    }

    void observation(std::ostream& /* file */) const
    { }

private:
    enum Phase { INIT, IDLE, NEWSTATE };

    Phase _phase;
};

} } } } // namespace paradevs tests multithreading lifegame

#endif
