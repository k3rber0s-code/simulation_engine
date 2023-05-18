//
// Created by ema on 13.5.23.
//

#include <iostream>
#include <sstream>
#include "Reader.h"

namespace Xion {


    /// Hashing function as a helper for switch statement for a string
    constexpr unsigned int hash(std::string s, int off = 0) {
        return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
    }

    /// String split util
    std::vector<std::string> tokenizer(const std::string &s, char del) {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string word;
        while (!ss.eof()) {
            getline(ss, word, del);
            tokens.push_back(std::string(word));
        }
        return tokens;
    }

    /// Reads all the data from an input file
    /// \param path Path of the input file
    bool Reader::readFile(const std::string& path) {
        input.open(path);
        std::string line;


        if (input.is_open()) {

            while (input) {
                std::getline(input, line);
                if (line.empty()) continue;

                if (std::find(headings.begin(), headings.end(), std::string(line)) != headings.end()) {
                    save_data();
                    active_flag = line;
                } else {
                    std::vector<std::string> tokens = tokenizer(line, '=');
                    auto key = tokens[0];
                    auto value = tokens[1];
                    curr[tokens[0]] = tokens[1];
                }
            }

            save_data();
            return true;
        }
        else {
            std::cout << "Couldn't open file\n";
            return false;
        }
    }

    /// Save map 'curr' as a different variable based on current flag (loaded data type)
    void Reader::save_data() {
        if (!curr.empty()) {
            switch (hash(active_flag)) {
                case hash("# SYSTEM"): {
                    simulationParameters.par_system = curr;
                    break;
                }
                case hash("## PT"):
                    simulationParameters.par_particle_types.push_back(curr);
                    break;

                case hash("## NUMBERS"):
                    simulationParameters.par_particle_counts = curr;
                    break;

                case hash("# SIMULATION"):
                    simulationParameters.par_simulation = curr;
                    break;

                case hash("## R"):
                    simulationParameters.par_reactions.push_back(curr);
                    break;
                default:
                    std::cout << "Error: unknown heading" << std::endl;
                    break;
            }
            curr.clear();
        }
    }

    SimulationParameters &Reader::getParameters() {
        return simulationParameters;
    }


} // Xion