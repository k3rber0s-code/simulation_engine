//
// Created by ema on 15.4.23.
//

#include "System.h"
#include <cassert>

namespace Xion {

    /// Util method for fast unstable erase from vector. The deleted element is switched with the last element and the vector is then resized.
    /// \tparam T
    /// \param container
    /// \param it
    /// \return
    template<class T>
    void unstable_erase(
            typename std::vector<T> &container,
            typename std::vector<T>::iterator it
    ) {
        auto lastElement = container.end() - 1;
        if (it != lastElement) {
            *it = std::move(*lastElement);
        }
        container.pop_back();
    }

    /// Adds a particle of certain type to the system.
    /// \param ptype_id type of the particle
    void System::addParticle(PTypeID ptype_id) {
        // Generate id and whole new particle
        int p_id = generatePID();
        Particles[p_id] = Particle(p_id);
        PByType[ptype_id].push_back(p_id);

        // Add interactions
        addInteractions(ptype_id, p_id);
    }

    /// Adds a particle with specified ptype, pid
    void System::addParticle(PTypeID ptype_id, PID pid) {
        // Generate id and whole new particle
        Particles[pid] = Particle(pid);
        PByType[ptype_id].push_back(pid);

        // Add interactions
        addInteractions(ptype_id, pid);
    }

    /// Adds a particle with specified ptype, pid and ceil for coordinates
    void System::addParticle(PTypeID ptype_id, PID pid, double ceil) {

        // Generate id and whole new particle
        Particles[pid] = Particle(pid, ceil);
        PByType[ptype_id].push_back(pid);

        // Add interactions
        addInteractions(ptype_id, pid);

    }

    /// Adds all possible interactions for a particle
    /// \param ptype_id Type of the particle
    /// \param p_id ID of the particle
    /// \param threshold cutoff threshold for where potentials are no longer computed
    void System::addInteractions(const PTypeID &ptype_id, const PID &p_id, double threshold) {
        for (const auto &[ptype_id2, p_ids]: PByType) {
            for (auto &&p_id2: p_ids) {
                if (p_id > p_id2 &&
                    !Particles[p_id2].masked) { // Assures that each interaction will be counted only once
                    auto d = getDistance(p_id, p_id2);
                    if (d < pow(threshold, 2)) {
                        addInteraction(p_id, p_id2, ptype_id, const_cast<PTypeID &>(ptype_id2),
                                       InteractionType::lennard_jones);
                        //std::cout << "distance: " << d << ": adding interaction between " << p_id << " " << p_id2
                        // << std::endl;
                    } else {
                        // std::cout << "distance: " << d << " too far." << std::endl;
                    }
                }
            }
        }
    }

    /// Fetches a particle by id.
    /// \param p_id ID of the particle
    /// \return pointer on the particle or nullptr of not found
    Particle *System::getParticleByID(const PID &p_id) {
        auto it = Particles.find(p_id);
        if (it != Particles.end()) {
            return &it->second;
        } else return nullptr;
    }

    /// Computes the distance between two particles
    /// \param id1 particle 1
    /// \param id2 particle 2
    /// \param root flag for returning sqrt of the value
    /// \return distance between particle 1 and particle 2
    double System::getDistance(const PID &id1, const PID &id2, bool root) {
        auto &p1 = Particles[id1].coordinates;
        auto &p2 = Particles[id2].coordinates;
        double distance = pow((p1.x - p2.x), 2.0) + pow((p1.y - p2.y), 2.0) + pow((p1.z - p2.z), 2);

        if (!root) return distance;
        else return sqrt(distance);

    }

    /// Creates a pairwise potential of certain type.
    /// \param p1 PID of first particle
    /// \param p2 PID of second particle
    /// \param ptype1 type of first particle
    /// \param ptype2 type of second particle
    /// \param itype type of interaction to be created
    void System::addInteraction(const PID &p1, const PID &p2, const PTypeID &ptype1, const PTypeID &ptype2,
                                InteractionType itype) {
        if (itype == InteractionType::lennard_jones) {
            //std::cout << "adding lennard jones interaction. id: ";
            // Compute pairwise parameters
            auto sigma = pow(PTypes[ptype1].sigma * PTypes[ptype2].sigma, 0.5);
            auto epsilon = average(PTypes[ptype1].epsilon, PTypes[ptype2].epsilon);
            auto distance = getDistance(p1, p2);

            // Create the interaction and add it to particles
            IID i = generateIID();
            //std::cout << i << std::endl;
            auto i_shared = std::make_shared<LennardJonesPotential>(i, sigma, epsilon, sqrt(distance));
            Particles[p1].addInteraction(i_shared);
            Particles[p2].addInteraction(i_shared);

            // Create info about the interaction itself
            Interactions[i] = {p1, p2};

            // Update the system energy
            // std::cout << "energy before: " << energy << " ";
            energy += i_shared->getPotential();
            //std::cout << "energy after: " << energy << std::endl;
        }

    }

    /// Function for sampling a random particle of certain type.
    /// \param ptype_id ID of the type of desired particle
    /// \return ID of the particle in ParticlesByType[ptype_id] vector
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
        auto erased_id = Particles.find(p_id);
        assert(erased_id != Particles.end());
        Particles.erase(erased_id);

    }

    /// Deletes all interactions regarding a particle denoted by PID. Used when deleting a particle or changing its type
    /// \param p_id PID of the particle
    void System::deleteInteractions(const PID &p_id) {
        auto &i_tbd = Particles[p_id].interactions;
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
                        if (it != Particles[p_id2].interactions.end()) {
                            if (it->use_count() == 1) {
                                Particles[p_id2].interactions.erase(it);
                            } else {
                                it->reset();
                            }
                            unstable_erase(Particles[p_id2].interactions, it);
                        }
                    }
                }
                // Delete info from interaction map
                auto it_interaction = Interactions.find(i->id);
                // Safeguard if deleting multiple particles and this is an interaction regarding both of them
                // - one of them could have already deleted this info in the map
                if (it_interaction != Interactions.end()) {
                    Interactions.erase(it_interaction);
                }
            }
        }
    }

    /// Stores information about a particle type in the system.
    /// \param typeId string ID of the type
    /// \param sigma radius
    /// \param epsilon depth of potential well
    /// \param charge charge
    void System::addPType(PTypeID typeId, double sigma, double epsilon, Charge charge) {
        auto ptype = Xion::ParticleType{typeId, sigma, epsilon, charge};
        PTypes[typeId] = ptype;
    }

    /// Changes the type of the particle and generates new id
    /// \param pid
    /// \param typeId_old  old type
    /// \param typeId_new  new type
    void System::changePType(const PID &pid, const PTypeID &typeId_old, const PTypeID &typeId_new) {
        // Generate new ID
        PID new_pid = generatePID();
        // Remove old interactions
        deleteInteractions(pid);
        // Reassign it in the particle
        Particles[pid].id = new_pid;
        // Change Particles key
        auto particleHandler = Particles.extract(pid);
        particleHandler.key() = new_pid;
        Particles.insert(std::move(particleHandler));
        // Change it in PByType
        unstable_erase(PByType[typeId_old], std::find(PByType[typeId_old].begin(), PByType[typeId_old].end(), pid));
        PByType[typeId_new].push_back(new_pid);
        // Generate new interactions
        addInteractions(typeId_new, new_pid);

    }

    /// Adds reaction to the system
    /// \param _stoichiometry stoichiometric coefficients
    /// \param _nu nu of the reaction
    void System::addReaction(std::map<PTypeID, int> &_stoichiometry, int _nu) {
        Reactions.push_back(Reaction(_stoichiometry, _nu));
    }


    /// Executes one RxMC step
    void System::doRxMCStep() {
        std::cout << "beginning RxMC step..." << std::endl;
        // Choose random reaction and its direction
        auto reaction = getRandomReaction();
        int reactionDirection = getReactionDirection();
        // Set reactants and products
        // Positive coefficient (multiplied by reactionDirection) denotes a product, negative denotes a reactant

        std::vector<std::string> reactants;
        std::vector<std::string> products;
        {
            std::for_each(reaction->stoichiometry.begin(), reaction->stoichiometry.end(),
                          [&reactants, &products, reactionDirection](const std::pair<PTypeID, int> &it) mutable {
                              if (it.second * reactionDirection < 0) {
                                  reactants.push_back(it.first);
                              } else {
                                  products.push_back(it.first);
                              }
                          });
        }
        // Create a mask to save changes proposed by next step
        std::vector<std::pair<PTypeID, PID>> deleted_particles;

        // Choose specific particles from reactant types
        for (auto &&r: reactants) {
            std::vector<PID> random_pids = {};
            // Check if we have enough particles of this type in the system
            if (PByType[r].size() < abs(reaction->stoichiometry[r])) {
                current_state = "Reactant missing: " + r + ". Reaction cannot proceed.";
                std::cout << "Reactant missing: " << r << ". Reaction cannot proceed." << std::endl;
                return;
            } else {
                while (random_pids.size() != abs(reaction->stoichiometry[r])) {
                    PID id = getRandomParticleID(r);
                    if (std::find(random_pids.begin(), random_pids.end(), id) == random_pids.end()) {
                        random_pids.push_back(id);
                    }
                }
            }
            for (auto &&pid: random_pids) {
                auto p = getParticleByID(pid);
                if (p != nullptr) {
                    deleted_particles.push_back({r, pid});
                    // Mask the particles to be deleted so when we add new, they disregard any possible interactions with them
                    Particles[pid].masked = true;
                }
                    // Reactants missing from system
                else {
                    current_state = "Reactant missing: " + r + ". Reaction cannot proceed.";
                    std::cout << "Reactant missing: " << r << ". Reaction cannot proceed." << std::endl;
                    return;
                }
            }

        }
//        std::cout << "deleted particles: ";
//        for (auto &&p: deleted_particles) {
//            std::cout << p.second << " ";
//        }
//        std::cout << std::endl;

        //// PROPOSE NEW STATE

        // Energy delta that will be subtracted when deleting reactants and their interactions from the system
        double energy_delta_del = 0;

        for (auto &&dp: deleted_particles) {
            for (auto &&i: Particles[dp.second].interactions) {
                energy_delta_del += i->getPotential();
            }
        }


        // Energy after delta del
        double new_energy = energy - energy_delta_del;



        // Create new particles de novo or from particle buffer
        std::vector<std::pair<PTypeID, PID>> added_particles;
        // Create new particle
        for (auto &&p: products) {
            for (int i = 0; i < abs(reaction->stoichiometry[p]); ++i) {
                PID id = generatePID();
                added_particles.push_back({p, id});
                addParticle(p, id, box_l);
            }
        }

        // Energy delta that will be added when adding products and their interactions to the system
        double energy_delta_add = 0;
        for (auto &&ap: added_particles) {
            for (auto &&i: Particles[ap.second].interactions) {
                energy_delta_add += i->getPotential();
            }
        }

        // Energy after delta add
        new_energy += energy_delta_add;

        // Nu is the sum of stoichiometric coefficients, needed in computation of acceptance probability
        double nu = 0;
        for (auto &&ptype: reactants) {
            nu += reaction->stoichiometry[ptype] * reactionDirection;
        }
        for (auto &&ptype: products) {
            nu += reaction->stoichiometry[ptype] * reactionDirection;
        }

        //// ACCEPTANCE OF NEW STATE
        double xi = 1.0; // extent of reaction, for us, it will be always 1
        if (new_energy > energy) {
            // beta = 1 / k_B * T
            double beta = 1 / (temperature / 300);
            double gamma = pow(10, -pK_A + (A_3_to_dm_3 - N_A_exp)) * 1/N_A;
            // Computed acceptance probability
            auto volume_change = pow(pow(box_l, 3), nu * xi);
            auto boltzmann = exp(-beta * (new_energy - energy));
            auto p_accept = volume_change
                                     * pow(gamma, xi)
                                     * boltzmann;
            p_accept = std::min(1.0, p_accept);
            // Random probability taken uniformly from distribution
            double p_rand = Random::getRandomNumber(0.0, 1.0);

            // Smith-Triska criterion
            if (p_accept > p_rand) {
                current_state =
                        "step accepted: p acc: " + std::to_string(p_accept) + " p rand: " + std::to_string(p_rand);
                std::cout << "step accepted: " << "p acc: " << p_accept << " " << "p rand: " << p_rand << std::endl;
            } else {
                current_state =
                        "step declined: p acc: " + std::to_string(p_accept) + " p rand: " + std::to_string(p_rand);
                std::cout << "step declined: " << "p acc: " << p_accept << " " << "p rand: " << p_rand << std::endl;
                for (auto &&p: added_particles) {
                    deleteParticle(p.second, p.first);
                }
                for (auto &&p: deleted_particles) {
                    Particles[p.second].masked = false;
                }
                return;
            }
        }
            // New energy is lower that old energy, state is automatically accepted
        else {
            current_state = "step automatically accepted";
            std::cout << "step automatically accepted" << std::endl;
        }

        //// DELETE OLD STATE

        // PARTICLES FROM SYSTEM
        for (auto &&p: deleted_particles) {
            deleteParticle(p.second, p.first);
        }

        energy = new_energy;
    }

    /// Returns a pointer to a random available reaction
    /// \return
    Reaction *System::getRandomReaction() {
        if (!Reactions.empty()) {
            int idx = Random::getRandomNumber<int>(0, Reactions.size() - 1);
            return &Reactions[idx];
        } else return nullptr;
    }

    /// Returns random number (reaction direction) from {-1, 1}
    int System::getReactionDirection() {
        auto p = Random::getRandomNumber(0.0, 1.0);
        if (p >= 0.5) return 1; else return -1;
    }

    /// Parses parameters from data dump of the reader
    /// \param data
    void System::parseParameters(const SimulationParameters &sp) {
        // Parse system variables
        box_l = std::stod(sp.par_system.at("box_l"));
        // Parse reactions
        for (auto &&r: sp.par_reactions) {
            std::map<Xion::PTypeID, int> reaction;
            for (const auto &[k, v]: r) {
                reaction[k] = std::stoi(v);
            }
            addReaction(reaction, 1);
        }
        // Parse particle types
        for (auto &&pt: sp.par_particle_types) {
            auto charge_str = pt.at("charge");
            Charge c;
            if (charge_str == "negative") c = Charge::negative;
            else if (charge_str == "positive") c = Charge::positive;
            else c = Charge::zero;
            addPType(pt.at("name"), std::stod(pt.at("sigma")), std::stod(pt.at("epsilon")), c);
        }
        // Parse particle numbers
        for (const auto &[ptype, n]: sp.par_particle_counts) {
            auto threshold = std::stoi(n);
            for (int i = 0; i < threshold; ++i) {
                addParticle(ptype, generatePID(), box_l);
            }
        }


    }


} // Xion