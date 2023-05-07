//
// Created by ema on 14.4.23.
//

#include <iostream>
#include "Particle.h"
#include "Random.h"
#include "Interaction.h"

namespace Xion {
    Coordinates::Coordinates() {
        x = Random::getRandomNumber(0.0, 10.0);
        y = Random::getRandomNumber(0.0, 10.0);
        z = Random::getRandomNumber(0.0, 10.0);
    }

    Particle::Particle() {
        coordinates = Coordinates();
    }

    Particle::Particle(int _id) {
        coordinates = Coordinates();
        id = _id;
    }

    ChainedParticle::ChainedParticle() {
        previous = nullptr;
        next = nullptr;
    }

    void Particle::addInteraction(std::shared_ptr<Interaction> i) {
        interactions.push_back(i);
    }

    void Particle::printPosition() {
        std::cout << coordinates.x
        << " " << coordinates.y
        << " " << coordinates.z
        << std::endl;
    }

    Particle::Particle(Particle &p, int _id) {
        id = _id;
        coordinates = p.coordinates;
        interactions = p.interactions;
    }
} // Xion