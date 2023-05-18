#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <stdio.h>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"
#include "Writer.h"
#include "Reader.h"

int main(int argc, char *argv[]) {
    std::string input_path;
    if (argc != 2) {
        std::cout << "Invalid number of arguments, input file path missing.";
        return 0;
    }
    else {
        input_path = argv[1];
    }
    // Setting up the reader and reading the input file
    Xion::Reader r;
    bool file_good = r.readFile(input_path);
    if (!file_good) return 0;

    auto data = r.getParameters();

    // Setting up simulation variables
    int n_steps = std::stoi(data.par_simulation["n_steps"]);
    bool xyz = (data.par_simulation["xyz"] == "true");
    bool obs = (data.par_simulation["obs"] == "true");
    bool log = (data.par_simulation["log"] == "true");

    // Creating system of particles
    double box_l = std::stod(data.par_system["box_l"]);
    Xion::System s(box_l);
    s.parseParameters(data);
    std::cout << std::endl;

    // Setting up the writer
    Xion::Writer w;
    // Writing simulation info in the log
    if (log) {
        w.writeDateTime();
        w.writeLog("This is a RxMC Monte Carlo simulation.");
        w.writeLog("System parameters: ");
        w.writeSimulationParameters(data);
    }


    // Simulate for n_steps
    w.writeParticlePositions(s);
    for (int i = 0; i < n_steps; ++i) {
        s.doRxMCStep();
        if (log) {
            w.writeLog("Step " + std::to_string(i));
            w.writeSystemState(s);
        }
        if (obs) w.writeObs(s);
        if (xyz) w.writeParticlePositions(s, true);
    }
    return 0;
}
