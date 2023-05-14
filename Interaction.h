//
// Created by ema on 15.4.23.
//
#include <vector>
#include <memory>
#include <cmath>
#include "Particle.h"

#ifndef ENGINE_INTERACTION_H
#define ENGINE_INTERACTION_H

namespace Xion {
    enum class InteractionType {
        lennard_jones,
        electrostatic
    };

    class Interaction {
    public:
        int id;

        double getPotential() const { return potential; };
        Interaction(int _id) : id(_id) {};
    protected:
        double potential;
    };

    template<typename... T>
    constexpr auto average(T &&... v) { return ( 0.0 + ... + v ) / sizeof...(T);}

    class LennardJonesPotential : public Interaction {
    public:
        void calculatePotentialEnergy(double sigma, double epsilon, double distance) {
            potential = 4 * epsilon * (pow(sigma / distance, 12) - pow(sigma / distance, 6));
        };

        LennardJonesPotential() = default;
        LennardJonesPotential(int _id, double sigma, double epsilon, double distance) : Interaction(_id) {
            calculatePotentialEnergy(sigma, epsilon, distance);
        };
    };


} // Xion

#endif //ENGINE_INTERACTION_H
