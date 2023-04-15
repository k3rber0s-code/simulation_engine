//
// Created by ema on 15.4.23.
//
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"

#ifndef ENGINE_SYSTEM_H
#define ENGINE_SYSTEM_H

namespace Xion {
    /* Type definition for System.Particles:
     * (1)map<(2)"particle_type_id", (3)pair<(4)map<"particle_id", particle>, (5)vector<"particle_ids">>>
     *
     * Explanation:
     * We need to do following things:
     *  - access a particle of certain type in O(1) -> (1) and (2) for looking up all particles of certain type
     *  - access a RANDOM particles -> that's why we store (5), all particle ids of certain type. we choose random element
     *    from this vector in O(1) and use it to lookup the particle in (4) in O(1)
     *  - access a particle with SPECIFIC PARTICLE ID -> this is needed when we need to manipulate an interaction and we
     *    have a list od all particle ids involved in it.
     *  - (3) is just for storing (4) and (5) together
     */
    typedef std::map<std::string, std::pair<std::map<int, Particle>, std::vector<int>>> ParticleMap;

    /*
     * Type definition for storing Interaction ids and Particle ids of Particles involved in the interaction.
     */
    typedef std::map<int, std::vector<int>> InteractionMap;

    /*
     * Type definition for fast lookup of information regarding different particle types.
     */
    typedef std::map<std::string, Xion::ParticleType> ParticleTypeInfo;

    class System {
    public:
        int generateID() { return nextID++;}
        void addParticle(std::string ptype_id);
        Particle* getParticleByID(int p_id);
        System();
    private:
        ParticleTypeInfo PTypeInfo;
        ParticleMap Particles;
        InteractionMap Interactions;

        double energy = 0;
        int nextID;
    };

} // Xion

#endif //ENGINE_SYSTEM_H
