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

    for (size_t i = 0; i < 10; ++i) {
        s.addParticle("H");
    }

    s.getParticleByID(1)->printPosition();


    return 0;
}
