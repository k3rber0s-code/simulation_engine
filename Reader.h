//
// Created by ema on 13.5.23.
//

#ifndef ENGINE_READER_H
#define ENGINE_READER_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

typedef std::map<std::string, std::string> parameters;

namespace Xion {

    struct SimulationParameters {
        std::vector<parameters> par_reactions;
        std::vector<parameters> par_particle_types;
        parameters par_particle_counts;
        parameters par_system;
        parameters par_simulation;
    };


    class Reader {
    public:
        bool readFile(const std::string& path);
        SimulationParameters& getParameters();
    private:
        // INPUT STREAM
        std::ifstream input;

        // STORAGE OF PARAMETERS
        SimulationParameters simulationParameters;
//        std::vector<parameters> par_reactions;
//        std::vector<parameters> par_particle_types;
//        parameters par_particle_counts;
//        parameters par_system;
//        parameters par_simulation;

        // TEMPORARY FLAG AND MAP FOR EXTRACTING PARAMETERS
        parameters curr = {};
        std::string active_flag = "default";
        const std::vector<std::string> headings = {"# SYSTEM", "# PARTICLES", "## PT", "## NUMBERS", "# REACTIONS", "## R", "# SIMULATION"};

        // Utils for data extractions
        void save_data();
    };

} // Xion

#endif //ENGINE_READER_H
