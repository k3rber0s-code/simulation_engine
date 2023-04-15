//
// Created by ema on 15.4.23.
//

#include "System.h"

namespace Xion {
    System::System() {
        nextID = 0;
    }

    void System::addParticle(std::string ptype_id) {
        int particle_id = generateID();
        Particles[ptype_id].first[particle_id] = Particle(particle_id);
        Particles[ptype_id].second.push_back(particle_id);

        // TODO: add interactions
    }
} // Xion