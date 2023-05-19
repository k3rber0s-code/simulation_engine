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
#include "Reader.h"

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
    typedef char PTypeID;
    typedef int PID;
    typedef int IID;

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
        void parseParameters(const SimulationParameters &);
        void doRxMCStep();

        //System() : nextPID(0), nextIID(0) {};
        explicit System(double _box_l) : nextPID(0), nextIID(0), box_l(_box_l), current_state("Initial state")  {};

        friend class Writer;

    private:
        // PROPERTIES
        double box_l = 100.0;
        double temperature = 300.0; // K
        double pK_A = 7.04; // dis constant

        // Constants
        constexpr const static int A_3_to_dm_3 = 27;
        constexpr const static int N_A_exp = 23;
        constexpr const static double N_A = 6.02214076;
        constexpr const static double lj_threshold = 45;

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
        void addPType(PTypeID, double, double, Charge);

        void addParticle(PTypeID);
        void addParticle(PTypeID, PID);
        void addParticle(PTypeID, PID, double);

        void deleteParticle(PID, PTypeID);

        void changePType(const PID&, const PTypeID&, const PTypeID&);

        PID getRandomParticleID(const PTypeID &);

        Particle *getParticleByID(const PID&);

        // Reactions
        void addReaction(std::map<PTypeID, int> &, int);

        Reaction *getRandomReaction();
        static int getReactionDirection();

        // Interactions
        void addInteractions(const PTypeID &ptype_id, const PID &p_id, double threshold = lj_threshold);
        void addInteraction(const PID&, const PID&, const PTypeID &, const PTypeID &, InteractionType);

        void deleteInteractions(const PID& p_id);

        // Utils
        double getDistance(const PID &id1, const PID &id2, bool root = false);


    };

} // Xion

#endif //ENGINE_SYSTEM_H
