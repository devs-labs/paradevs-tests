/**
 * @file tests/multithreading/lifegame/models.hpp
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

#ifndef TESTS_MULTITHREADING_LIFEGAME_MODELS_HPP
#define TESTS_MULTITHREADING_LIFEGAME_MODELS_HPP 1

#include <paradevs/common/time/DoubleTime.hpp>

#include <paradevs/kernel/pdevs/Dynamics.hpp>

namespace paradevs { namespace tests {
        namespace multithreading { namespace lifegame {

struct CellParameters
{
    CellParameters(int n) : neighbour_number(n)
    { }

    int neighbour_number;
};

class Cell : public paradevs::pdevs::Dynamics < common::DoubleTime,
                                                CellParameters >
{
public:
    Cell(const std::string& name, const CellParameters& parameters) :
        paradevs::pdevs::Dynamics < common::DoubleTime, CellParameters >(
            name, parameters),
        _neighbour_number(parameters.neighbour_number)
    { }
    virtual ~Cell()
    { }

    void dint(typename common::DoubleTime::type t)
    {

        // std::cout << t << " [" << get_name() << "] -> dint: "
        //           << _phase << " => ";

        if (_phase == SEND) {
            _phase = WAIT;
            _sigma = common::DoubleTime::infinity;
        } else if (_phase == NEWSTATE) {
            if (_state && (_true_neighbour_number < 2 ||
                           _true_neighbour_number > 3)) {
                _state = false;
            } else if (not _state && (_true_neighbour_number == 3)) {
                _state = true;
            }
            _phase  = SEND;
            _sigma = 1;
            _true_neighbour_number = 0;
            _received = 0;
        }

        // std::cout << _phase << std::endl;

    }

    void dext(typename common::DoubleTime::type t,
              typename common::DoubleTime::type /* e */,
              const common::Bag < common::DoubleTime >& bag)
    {

        // std::cout << t << " [" << get_name() << "] -> dext => "
        //           << _received << " " << _phase << std::endl;

        for (common::Bag < common::DoubleTime >::const_iterator it =
                 bag.begin(); it != bag.end(); ++it) {
            if (*(bool*)(it->get_content())) {
                ++_true_neighbour_number;
            }
            ++_received;
        }

        if (_received == _neighbour_number) {
            _phase = NEWSTATE;
            _sigma = 0;
        } else {
            _phase = WAIT;
            _sigma = common::DoubleTime::infinity;
        }

        // std::cout << t << " [" << get_name() << "] -> dext (AFTER) => "
        //           << _received << " " << _phase << std::endl;

    }

    void dconf(typename common::DoubleTime::type t,
               typename common::DoubleTime::type e,
               const common::Bag < common::DoubleTime >& bag)
    {

        // std::cout << t << " [" << get_name() << "] -> dconf" << std::endl;

        dext(t, e, bag);
    }

    typename common::DoubleTime::type start(typename common::DoubleTime::type t)
    {

        // std::cout << t << " [" << get_name() << "] -> START" << std::endl;

        _phase = SEND;
        _sigma = 0;
        _state = false;
        _true_neighbour_number = 0;
        _received = 0;
        return 0;
    }

    typename common::DoubleTime::type ta(
        typename common::DoubleTime::type t) const
    { return _sigma; }

    common::Bag < common::DoubleTime > lambda(
        typename common::DoubleTime::type t) const
    {
        common::Bag < common::DoubleTime > bag;

        if (_phase == SEND) {

            // std::cout << t << " [" << get_name() << "] -> lambda"
            //           << std::endl;

            bag.push_back(common::ExternalEvent < common::DoubleTime >(
                              "out", (void*)(&_state)));
        }
        return bag;
    }

    void observation(std::ostream& /* file */) const
    { }

private:
    enum Phase { SEND, WAIT, NEWSTATE };

    unsigned int _neighbour_number;

    Phase _phase;
    common::DoubleTime::type _sigma;
    bool _state;
    unsigned int _received;
    unsigned int _true_neighbour_number;
};

} } } } // namespace paradevs tests multithreading lifegame

#endif
