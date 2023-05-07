#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"


int main() {

    Xion::System s;

    auto ptype = Xion::ParticleType{"H", 1.0, 1.0, Xion::Charge::zero};
    s.PTypes["H"] = ptype;

    for (size_t i = 0; i < 3; ++i) {
        s.addParticle("H");
    }
    for (size_t i = 0; i < 3; ++i) {
        std::cout << "particle: " << i << std::endl;
        s.getParticleByID(i)->printPosition();
        std::cout << "interactions: ";
        for (auto &&i: s.getParticleByID(i)->interactions) {
            std::cout << i->id << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "deleting particle 1." << std::endl;
    s.deleteParticle(1, "H");

    for (size_t i = 0; i < 3; ++i) {
        std::cout << "particle: " << i << std::endl;
        if (s.getParticleByID(i) != nullptr) {
            s.getParticleByID(i)->printPosition();
            std::cout << "interactions: ";
            for (auto &&i: s.getParticleByID(i)->interactions) {
                std::cout << i->id << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "particle with id " << i << " does not exist!" << std::endl;
        }
    }


    return 0;
}
