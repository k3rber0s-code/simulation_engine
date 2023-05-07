//
// Created by ema on 14.4.23.
//
#include <memory>
#include <vector>
#ifndef ENGINE_PARTICLE_H
#define ENGINE_PARTICLE_H

namespace Xion {
    class Interaction;
    struct Coordinates {
        double x;
        double y;
        double z;
        Coordinates();
    };

    enum class Charge {
        negative,
        zero,
        positive
    };

    struct ParticleType {
        std::string name;
        double sigma;
        double epsilon;
        Charge charge;
    };

    typedef std::vector<std::shared_ptr<Interaction>> Interactions;

    class Particle {
    public:
        int id;
        Coordinates coordinates;
        void printPosition();
        void addInteraction(std::shared_ptr<Interaction> i);
        Particle();
        explicit Particle(int _id);
        Interactions interactions;
    protected:
        friend class System;
    };
    class ChainedParticle : Particle {
    public:
        ChainedParticle();
        using Particle::addInteraction;
    private:
        Particle* previous;
        Particle* next;
    };

} // Xion

#endif //ENGINE_PARTICLE_H
