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

        //virtual void calculatePotentialEnergy(Particle &p1, Particle &p2) = 0;
    };

    template<typename... T>
    constexpr auto average(T &&... v) { return ( 0.0 + ... + v ) / sizeof...(T);}

    class LennardJonesPotential : public Interaction {
    public:
        //double cutoff = 2.5;
        void calculatePotentialEnergy(double sigma, double epsilon, double distance) {
//            auto epsilon = 1.0;  //average(particles[0]->type->epsilon, particles[1].get()->type->epsilon);
//            auto sigma = 1.0; //pow(particles[0].get()->type->sigma*particles[1].get()->type->sigma, 0.5);
//            auto distance = 1.0;
            potential = 4 * epsilon * (pow(sigma / distance, 12) - pow(sigma / distance, 6));
        };

        LennardJonesPotential() = default;
        LennardJonesPotential(int _id, double sigma, double epsilon, double distance) : Interaction(_id) {
            calculatePotentialEnergy(sigma, epsilon, distance);
        };
    };


} // Xion

#endif //ENGINE_INTERACTION_H
