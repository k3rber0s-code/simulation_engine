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

    class Interaction {
    public:
        double getPotential() const { return potential; };
    protected:
        double potential;
        virtual void calculatePotentialEnergy(Particle& p1, Particle& p2) = 0;
    };

    template < typename... T >
    constexpr auto average( T&&... v ) { return ( 0.0 + ... + v ) / sizeof...(T) ; }

    class LennardJonesPotential : public Interaction {
    public:
        //double cutoff = 2.5;
        void calculatePotentialEnergy(Particle& p1, Particle& p2) override {
            auto epsilon = 1.0;  //average(particles[0]->type->epsilon, particles[1].get()->type->epsilon);
            auto sigma = 1.0; //pow(particles[0].get()->type->sigma*particles[1].get()->type->sigma, 0.5);
            auto distance = 1.0;

            potential = 4 * epsilon * (pow(sigma / distance, 12) - pow(sigma / distance, 6));
        };

    };

} // Xion

#endif //ENGINE_INTERACTION_H
