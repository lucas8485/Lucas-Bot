/*
 *  This file is part of Lucas' Bot.
 *
 *  Lucas' Bot is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License,
 *  or (at your option) any later version.

    Lucas' Bot is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
    for more details.

    You should have received a copy of the GNU Affero General Public License along
    with Lucas' Bot. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MIRAICPPLUGIN_RANDOM_LIBRARY_H
#define MIRAICPPLUGIN_RANDOM_LIBRARY_H

#include <random>
#include <type_traits>

namespace RandomLibrary {

    std::default_random_engine e;

    void initialize() {
        static bool initialized = false;
        if (initialized)
            return;
        e.seed(std::random_device{}());
        initialized = true;
    }

    enum class RandomNumberType {
        UniformDistribution,
        NormalDistribution
    };

    template <typename TNum>
    TNum getNextRandomNumber(RandomNumberType type, TNum lowerBound, TNum upperBound) {
        static_assert(std::is_arithmetic_v<TNum>, "非算术类型不可用作本函数的参数！");
        initialize();
        if constexpr (std::is_integral_v<TNum>) {
            std::uniform_int_distribution<TNum> u(lowerBound, upperBound);
            return u(e);
        } else {
            if (type == RandomNumberType::UniformDistribution) {
                std::uniform_real_distribution<TNum> u(lowerBound, upperBound);
                return u(e);
            } else {
                std::normal_distribution<TNum> n(lowerBound, upperBound);
                return n(e);
            }
        }
    }

    bool getNextBoolean(double probability) {
        initialize();
        std::bernoulli_distribution b(probability);
        return b(e);
    }
}

#endif //MIRAICPPLUGIN_RANDOM_LIBRARY_H
