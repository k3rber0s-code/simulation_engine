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
        explicit Coordinates(double ceil);
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

        Particle();
        Particle(Particle&, int);
        explicit Particle(int _id);
        explicit Particle(int _id, double ceil);
    protected:
        Interactions interactions;
        bool masked;

        void printPosition();
        void addInteraction(std::shared_ptr<Interaction> i);
        friend class System;
    };

} // Xion

#endif //ENGINE_PARTICLE_H
