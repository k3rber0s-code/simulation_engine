#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"


int main() {

    Xion::System s;

    s.addPType("H", 1.0, 1.0, Xion::Charge::positive);
    s.addPType("A", 2.7, 4.92, Xion::Charge::negative);
    s.addPType("HA", 2.7, 3.0, Xion::Charge::zero);
    for (size_t i = 0; i < 3; ++i) {
        s.addParticle("H");
    }
    //auto particle = Xion::Particle(s.getParticleByID(1));


    for (size_t i = 0; i < 3; ++i) {
        std::cout << "particle: " << i << std::endl;
        s.getParticleByID(i)->printPosition();
        std::cout << "interactions: ";
        for (auto &&interaction: s.getParticleByID(i)->interactions) {
            std::cout << interaction->id << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "changing particle 1 from H to A." << std::endl;
    s.changePType(1, "H", "A");

    for (size_t i = 0; i < 5; ++i) {
        std::cout << "particle: " << i << std::endl;
        if (s.getParticleByID(i) != nullptr) {
            s.getParticleByID(i)->printPosition();
            std::cout << "interactions: ";
            for (auto && interaction: s.getParticleByID(i)->interactions) {
                std::cout << interaction->id << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "particle with id " << i << " does not exist!" << std::endl;
        }
    }


    return 0;
}
