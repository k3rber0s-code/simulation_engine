//
// Created by ema on 15.4.23.
//

#include "System.h"

namespace Xion {

    template<class T>
    bool unstable_erase(
            typename std::vector<T> &container,
            typename std::vector<T>::iterator it
    ) {
        auto lastEl = container.end() - 1;
        if (it != lastEl) {
            *it = std::move(*lastEl);
        }
        container.pop_back();
    }

    void System::addParticle(PTypeID ptype_id) {
        double threshold = 15; // TODO: load thresholds for different types of interactions;
        // Generate id and whole new particle
        int p_id = generatePID();
        Particles[p_id] = Particle(p_id);
        PByType[ptype_id].push_back(p_id);

        // Add interactions
        addInteractions(ptype_id, p_id);
    }

    void System::addInteractions(PTypeID &ptype_id, int p_id, double threshold) {
        for (const auto &[ptype_id2, p_ids]: PByType) {
            for (auto &&p_id2: p_ids) {
                if (p_id > p_id2) { // Assures that each interaction will be counted only once
                    auto d = getDistance(p_id, p_id2);
                    if (d < pow(threshold, 2)) {
                        addInteraction(p_id, p_id2, ptype_id, const_cast<PTypeID &>(ptype_id2),
                                       InteractionType::lennard_jones); //TODO: cast
                        std::cout << "distance: " << d << ": adding interaction between " << p_id << " " << p_id2
                                  << std::endl;
                    } else {
                        std::cout << "distance: " << d << " too far." << std::endl;
                    }
                }
            }
        }
    }

    Particle *System::getParticleByID(PID p_id) {
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
            std::cout << i << std::endl;
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

    PID System::getRandomParticleID(const PTypeID &ptype_id) {
        if (!PByType[ptype_id].empty()) {
            int idx = Random::getRandomNumber<int>(0, PByType[ptype_id].size() - 1);
            return PByType[ptype_id][idx];
        } else return -1;
    }

    void System::deleteParticle(PID p_id, PTypeID p_type) {
        // DELETE ALL INTERACTIONS
        deleteInteractions(p_id);
        // ERASE THE PARTICLE ITSELF
        PByType[p_type].erase(std::find(PByType[p_type].begin(), PByType[p_type].end(), p_id));
        Particles.erase(Particles.find(p_id));

    }

    void System::deleteInteractions(PID p_id) {// ERASE INTERACTIONS
        auto& i_tbd = Particles[p_id].interactions;
        if (!i_tbd.empty()) {
            for (auto &i: i_tbd) {
                energy -= i->getPotential();
                // Get all other particles that interact with Particle <p_id> from interaction map
                for (auto &&p_id2: Interactions[i->id]) {
                    if (p_id != p_id2) {
                        // Delete shared_ptr on this interaction
                        auto it = std::find_if(Particles[p_id2].interactions.begin(),
                                               Particles[p_id2].interactions.end(),
                                               [&i](const std::shared_ptr<Interaction> &obj) {
                                                   return obj->id == i->id;
                                               });
                        Particles[p_id2].interactions.erase(it);
                    }
                }
                // Delete info from interaction map
                Interactions.erase(Interactions.find(i->id));
            }
        }
    }

    void System::addPType(PTypeID typeId, double sigma, double epsilon, Charge charge) {
        auto ptype = Xion::ParticleType{typeId, sigma, epsilon, charge};
        PTypes[typeId] = ptype;
    }

    void System::changePType(PID pid, PTypeID typeId_old, PTypeID typeId_new) {
        // Generate new ID
        PID new_pid = generatePID();
        // Reassign it in the particle
        Particles[pid].id = new_pid;
        // Change Particles key
        auto particleHandler = Particles.extract(pid);
        particleHandler.key() = new_pid;
        Particles.insert(std::move(particleHandler));
        // Change it in PByType
        unstable_erase(PByType[typeId_old], std::find(PByType[typeId_old].begin(), PByType[typeId_old].end(), pid));
        PByType[typeId_new].push_back(new_pid);
        // Change it in Interactions
        for (auto && interaction : Particles[new_pid].interactions) {
            //unstable_erase(Interactions[interaction->id])
        }
    }


} // Xion