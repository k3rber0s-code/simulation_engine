#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"




int main() {

    auto p1 = Xion::ChainedParticle();
    auto p2 = Xion::ChainedParticle();

    auto i1 = std::make_shared<Xion::LennardJonesPotential>();

    p1.addInteraction(i1);
    p2.addInteraction(i1);
    std::cout << i1.use_count();

    Xion::System s;
    std::cout << s.generateID() << " " << s.generateID();

    return 0;
}
