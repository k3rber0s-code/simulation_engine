//
// Created by ema on 14.4.23.
//

#include <iostream>
#include "Particle.h"
#include "Random.h"
#include "Interaction.h"

namespace Xion {
    /// Coordinates constructors with or without specified ceiling value for the axes
    Coordinates::Coordinates() {
        x = Random::getRandomNumber(0.0, 100.0);
        y = Random::getRandomNumber(0.0, 100.0);
        z = Random::getRandomNumber(0.0, 100.0);
    }
    Coordinates::Coordinates(double ceil) {
        x = Random::getRandomNumber(0.0, ceil);
        y = Random::getRandomNumber(0.0, ceil);
        z = Random::getRandomNumber(0.0, ceil);
    }

    /// Particles with no type and idF
    Particle::Particle() {
        coordinates = Coordinates();
        masked = false;
    }

    /// Particles with no type
    Particle::Particle(int _id) {
        coordinates = Coordinates();
        id = _id;
        masked = false;
    }

    /// Particles with no specified ceilling for coordinates
    Particle::Particle(Particle &p, int _id) {
        id = _id;
        coordinates = p.coordinates;
        interactions = p.interactions;
        masked = p.masked;
    }

    /// Particles with all properties set
    Particle::Particle(int _id, double ceil) {
        coordinates = Coordinates(ceil);
        id = _id;
        masked = false;
    }

    /// Adds interaction to particle interactions container
    /// \param i
    void Particle::addInteraction(std::shared_ptr<Interaction> i) {
        interactions.push_back(i);
    }

    /// Deprecated, prints the particle position to cout
    void Particle::printPosition() {
        std::cout << coordinates.x
        << " " << coordinates.y
        << " " << coordinates.z
        << std::endl;
    }

} // Xion