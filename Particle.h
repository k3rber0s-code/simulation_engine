//
// Created by ema on 14.4.23.
//
#include <memory>
#include <vector>
#ifndef ENGINE_PARTICLE_H
#define ENGINE_PARTICLE_H

namespace Xion {
    class Interaction;
    enum class InteractionType {
        lennard_jones,
        electrostatic
    };
    struct Coordinates {
        double x;
        double y;
        double z;
        Coordinates();
    };

    typedef std::vector<std::shared_ptr<Interaction>> Interactions;

    class Particle {
    public:
        Coordinates coordinates;
        void addInteraction(std::shared_ptr<Interaction> i);
        Particle();
    protected:
        Interactions interactions;
        friend class System;
    };
    class ChainedParticle : Particle {
    public:
        ChainedParticle();
        using Particle::addInteraction;//(std::shared_ptr<Interaction> i);
    private:
        Particle* previous;
        Particle* next;
    };

} // Xion

#endif //ENGINE_PARTICLE_H
