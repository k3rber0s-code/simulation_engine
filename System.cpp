//
// Created by ema on 15.4.23.
//

#include "System.h"

namespace Xion {

    void System::addParticle(PTypeID ptype_id) {
        double threshold = 2.5; // TODO: load thresholds for different types of interactions;
        // Generate id and whole new particle
        int p_id = generatePID();
        Particles[p_id] = Particle(p_id);
        PByType[ptype_id].push_back(p_id);

        // Add interactions
        for (const auto &[ptype_id2, p_ids]: PByType) {
            for (auto &&p_id2: p_ids) {
                if (p_id > p_id2) { // Assures that each interaction will be counted only once
                    auto d = getDistance(p_id, p_id2);
                    if (d < pow(threshold, 2)) {
                        addInteraction(p_id, p_id2, ptype_id, const_cast<PTypeID &>(ptype_id2), InteractionType::lennard_jones); //TODO: cast
                        std::cout << "distance: " << d << ": adding interaction between " << p_id << " " << p_id2 << std::endl;
                    } else {
                        std::cout << "distance: " << d << " too far." <<std::endl;
                    }
                }
            }
        }
    }

    Particle *System::getParticleByID(int p_id) {
        auto it = Particles.find(p_id);
        if (it != Particles.end()) {
            return &it->second;
        } else return nullptr;
    }

    double System::getDistance(const PID &id1, const PID &id2, bool root) {
        auto &p1 = Particles[id1].coordinates;
        auto &p2 = Particles[id2].coordinates;
        double distance = pow((p1.x - p2.x), 2.0) + pow((p1.y - p2.y), 2.0) + pow((p1.z - p2.z), 2);

        if (!root) return distance;
        else return sqrt(distance);

    }

    void System::addInteraction(PID p1, PID p2, PTypeID &ptype1, PTypeID &ptype2, InteractionType itype) {
        if (itype == InteractionType::lennard_jones) {
            std::cout << "adding lennard jones interaction. id: ";
            // Compute pairwise parameters
            auto sigma = pow(PTypes[ptype1].sigma * PTypes[ptype2].sigma, 0.5);
            auto epsilon = average(PTypes[ptype1].epsilon, PTypes[ptype2].epsilon);
            auto distance = getDistance(p1, p2);

            // Create the interaction and add it to particles
            IID i = generateIID();
            std::cout << i  << std::endl;
            auto i_shared = std::make_shared<LennardJonesPotential>(i, sigma, epsilon, sqrt(distance));
            Particles[p1].addInteraction(i_shared);
            Particles[p2].addInteraction(i_shared);

            // Create info about the interaction itself
            Interactions[i] = {p1, p2};

            // Update the system energy
            std::cout << "energy before: " << energy << " ";
            energy += i_shared->getPotential();
            std::cout << "energy after: " << energy << std::endl;
        }

    }


} // Xion