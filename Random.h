//
// Created by ema on 14.4.23.
//

#ifndef ENGINE_RANDOM_H
#define ENGINE_RANDOM_H

#include <cstdlib>
#include <stdlib.h>
#include <chrono>
#include <random>
#include <iterator>

namespace Xion {

    /*
     * This is a helper class containing a method to generate random value of multiple numeric types, such as int, double etc.
     */
    class Random {
    public:
        template<typename Numeric, typename Generator = std::mt19937>
        static Numeric getRandomNumber(Numeric from, Numeric to) {
            thread_local static Generator gen(std::random_device{}());

            using dist_type = typename std::conditional
                    <
                            std::is_integral<Numeric>::value, std::uniform_int_distribution<Numeric>, std::uniform_real_distribution<Numeric>
                    >::type;

            thread_local static dist_type dist;

            return dist(gen, typename dist_type::param_type{from, to});
        }
    };
}
#endif //ENGINE_RANDOM_H
