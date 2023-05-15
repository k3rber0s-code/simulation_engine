//
// Created by ema on 15.4.23.
//
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Particle.h"
#include "Interaction.h"
#include "Random.h"

typedef std::map<std::string, std::string> parameters;
typedef std::tuple<std::vector<parameters>, std::vector<parameters>, parameters,parameters,parameters> data_intake;

#ifndef ENGINE_SYSTEM_H
#define ENGINE_SYSTEM_H

namespace Xion {
    /*
     * PTypeID: ID of particle types (e.g. Hydrogen: "H", Polar amino acid: "P", Non-polar amino acid: "NP")
     * PID: ID of specific particle
     * IID: ID of specific interaction
     */
    typedef std::string PTypeID;
    typedef int PID;
    typedef int IID;
    typedef int RID;

    /*
     * ParticleMap: storage of particles accessible by their IDs in O(1)
     * ParticlesByType: storage of particle IDs by particle type - for randomly choosing a particle of certain type in O(1)
     * InteractionMap: for each interaction denoted by its IID, it stores info about participating particles (PIDs)
     */
    typedef std::map<PID, Particle> ParticleMap;
    typedef std::map<PTypeID, std::vector<PID>> ParticlesByType;
    typedef std::map<IID, std::vector<PID>> InteractionMap;

    /*
     * Type definition for fast lookup of information regarding different particle types.
     */
    typedef std::map<PTypeID, Xion::ParticleType> ParticleTypeInfo;

    typedef std::map<PTypeID, int> StoichiometricCoefficients;

    struct Reaction {
        StoichiometricCoefficients stoichiometry;
        int nu;

        Reaction(std::map<PTypeID, int> &_stoichiometry, int _nu) {
            stoichiometry = _stoichiometry;
            nu = _nu;
        }
    };

    class System {
    public:
        // Parsing
        void parseParameters(const data_intake &);

        void addReaction(std::map<PTypeID, int> &, int);

        void addPType(PTypeID, double, double, Charge);

        void addParticle(PTypeID);

        void addParticle(PTypeID, PID);
        void addParticle(PTypeID, PID, double);

        void deleteParticle(PID, PTypeID);

        void doRxMCStep();

        //System() : nextPID(0), nextIID(0) {};
        explicit System(double _box_l) : nextPID(0), nextIID(0), box_l(_box_l), current_state("Initial state")  {};

        friend class Writer;

    private:
        // PROPERTIES
        double box_l = 100.0;
        std::string current_state;
        // REACTIONS
        std::vector<Reaction> Reactions;
        // PARTICLES
        ParticleTypeInfo PTypes;
        ParticleMap Particles;
        ParticlesByType PByType;

        // INTERACTIONS
        InteractionMap Interactions;

        // OBSERVABLES
        double energy = 0;

        // ID HANDLER
        int generatePID() { return nextPID++; }

        int generateIID() { return nextIID++; }

        int nextPID;
        int nextIID;


        // METHODS

        // Particles
        void changePType(PID, PTypeID, PTypeID);

        PID getRandomParticleID(const PTypeID &);

        Particle *getParticleByID(PID);

        // Reactions
        Reaction *getRandomReaction();

        static int getReactionDirection();

        // Interactions
        void addInteraction(PID, PID, PTypeID &, PTypeID &, InteractionType);

        void deleteInteractions(PID p_id);

        void addInteractions(PTypeID &ptype_id, PID p_id, double threshold = 15);

        // Utils
        double getDistance(const PID &id1, const PID &id2, bool root = false);


    };

} // Xion

#endif //ENGINE_SYSTEM_H
