//
// Created by ema on 14.4.23.
//

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

    void Particle::addInteraction(std::shared_ptr<Interaction> i) {
        interactions.push_back(i);
//        switch (i_type) {
//            case InteractionType::lennard_jones:
//                break;
//            case InteractionType::electrostatic:
//                break;
//            default:
//                break;
//
//        }
    }

    ChainedParticle::ChainedParticle() {
        previous = nullptr;
        next = nullptr;
    }
} // Xion