#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"
#include "Writer.h"
#include "Reader.h"

typedef std::map<std::string, std::string> parameters;

constexpr unsigned int hash(std::string s, int off = 0) {
    return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
}

std::vector<std::string> tokenizer(std::string &s, char del) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string word;
    while (!ss.eof()) {
        getline(ss, word, del);
        tokens.push_back(word);
    }
    return tokens;
}

int main() {

    Xion::Reader r;
    r.readFile("/home/ema/software_projects/engine/input.txt");

    auto data = r.dumpData();

    // Setting up simulation variables
    int n_steps = std::stoi(std::get<4>(data).at("n_steps"));
    bool xyz = (std::get<4>(data).at("xyz") == "true") ? true : false;
    bool obs = (std::get<4>(data).at("obs") == "true") ? true : false;
    bool log = (std::get<4>(data).at("log") == "true") ? true : false;
    Xion::System s;
    s.parseParameters(data);
    std::cout << std::endl;
//
//    s.addPType("H", 1.0, 1.0, Xion::Charge::positive);
//    s.addPType("A", 2.7, 4.92, Xion::Charge::negative);
//    s.addPType("HA", 2.7, 3.0, Xion::Charge::zero);
//
//    std::map<Xion::PTypeID, int> reaction1;
//    reaction1["HA"] = -1;
//    //reaction1["H"] = 1;
//    reaction1["A"] = 1;
//    s.addReaction(reaction1, 1);
//
//    std::map<Xion::PTypeID, int> reaction2;
//    reaction2["H"] = -1;
//    reaction2["A"] = 1;
//    s.addReaction(reaction2, 1);
//
//    for (size_t i = 0; i < 15; ++i) {
//        s.addParticle("H");
//        s.addParticle("HA");
//        s.addParticle("O");
//        s.addParticle("A");
//    }
//
    Xion::Writer w;
    w.writeParticlePositions(s);
    for (int i = 0; i < n_steps; ++i) {
        s.doRxMCStep();
        if (obs) w.writeObs(s);
        if (xyz) w.writeParticlePositions(s, true);
    }
    return 0;
}
