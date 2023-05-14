//
// Created by ema on 13.5.23.
//

#include <iostream>
#include <sstream>
#include "Reader.h"

namespace Xion {


    constexpr unsigned int hash(std::string s, int off = 0) {
        return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
    }

    std::vector<std::string> tokenizer(std::string &s, char del) {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string word;
        while (!ss.eof()) {
            getline(ss, word, del);
            tokens.push_back(std::string(word));
        }
        return tokens;
    }

    void Reader::readFile(std::string path) {

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
        }
        else {
            std::cout << "Couldn't open file\n";
        }
    }

    void Reader::save_data() {
        if (!curr.empty()) {
            switch (hash(active_flag)) {
                case hash("# SYSTEM"): {
                    par_system = curr;
                    break;
                }
                case hash("## PT"):
                    par_particle_types.push_back(curr);
                    break;

                case hash("## NUMBERS"):
                    par_particle_counts = curr;
                    break;

                case hash("# SIMULATION"):
                    par_simulation = curr;
                    break;

                case hash("## R"):
                    par_reactions.push_back(curr);
                    break;
                default:
                    std::cout << "Error: unknown heading" << std::endl;
                    break;
            }
            curr.clear();
        }
    }

    data_dump Reader::dumpData() {
        return {par_reactions, par_particle_types, par_particle_counts, par_system, par_simulation};
    }


} // Xion