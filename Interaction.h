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
        lennard_jones
    };

    /*
     * Interaction class serves as a model of relationship between particles. Different types of interactions are able
     * to compute its exact potential and store the value.
     */
    class Interaction {
    public:
        int id;

        double getPotential() const { return potential; };
        Interaction(int _id) : id(_id) {};
        Interaction(){};
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
