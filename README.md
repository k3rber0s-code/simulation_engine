# 1 Monte Carlo Molecular Simulation Engine

## 1.1 Assignment
Assignment of this software project is to create a tool for simulating protein dynamics over time, focusing specifically on the effect of ionizable side chains (=effect of +/-/0 electrical charge).
## 1.2. Background
Proteins consist of amino acid chains which form secondary (tertiary, etc.) structure. Such structure highly correlates with protein function, therefore it is incredibly useful to determine the structure using a simulation - experimental (=in a chemistry lab) methods may be expensive or cannot be done at all.
## 1.3 Project scope
The goal is to provide a specialized tool for monitoring ionization effects in proteins. Many tools available have more functionalities, but ionization is not implemented well or it is too approximate. Therefore we want to come up with easier way to simulate this type of interaction on protein chains.
*EDIT: right now, no chained molecules are supported, the simulations runs for non-bonded atoms.*
# 2 Description

## 2.1 Overview
The main entities of the tool are:
1. class **Particle**
    - ``Particle.h``
    - simplified depiction of chemical particles
    - it holds the information about its coordinates
2. ***abstract class Interaction***
    - ``Interaction.h``
    - models relationship between particles
    - used for computing overall chemical energy of such relationship
    - stores info about participating Particles
    - class **LennardJones** : Interaction
        - derived class for modeling so called LennardJones potential (type of non-bonded interaction)
        - the only one needed for simulating gases.
3. **struct Reaction**
    - stored in``System.h``
    - stores info about chemical reaction, that is possible in current system
        - reactants, products, coefficients
4. **class System**
    - ``System.h``
    - control centre
    - simulates the behaviour of particles -> reactions, particle movement
    - analyzes overall physical properties of the system (energies, pressure, temperature etc.)
    - stores info about particles themselves, particles in interactions, available reactions and other useful constants


For simulation purposes, we will use Monte Carlo methods to generate next possible state and use *Smith-Triska* (reactions) and *Metropolis* (movement) criterion to accept or reject it.

## 2.2 I/O
### Input
EDIT: Right now, only a simple script in main.cpp is supported, the Reader class is not implemented yet.
The program reads a json file that stores properties about the system, possible reactions, number of particles of certain type (the type needs to be specified as well) and initial interactions. The input file looks like this:

### Output
Three types of output files are generated: **{name}.xyz**, which records the positions of all particles in one step. The format is:
```
%
{particle type} id x y z
...
{particle type} id x y z
%
...
```

The second output file is **{name}.obs**. It contains system properties that are being recorded over the course of the simulation.
*EDIT: right now, it contains the particle counts and total energy of the system*

The third output file is **{name}.log**. This is a basic log file that stores the properties of the system, datetime of the simulation and possible errors.
## 2.2 Model
--- UML DIAGRAM ---

# 3 Requirements

## 3.1 Software
The program will be written in C++.
## 3.2 Libraries
No additional libraries need to be installed.

