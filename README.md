[![Build Status](https://travis-ci.com/maracatronics/Armorial-Suassuna.svg?branch=dev)](https://travis-ci.com/maracatronics/Armorial-Suassuna)

# Armorial-Suassuna

This repository contains the central AI controller software framework used by Maracatronics Robotics Team and Research Lab at Small Size League


## Release 0.5
This release contains our central AI controller software framework that is currently being developed. It catches information in the network (using the GEARSystem structure) that is sent by Armorial-Carrero and use this to decide how the robots should act in the field.


Currently, our AI framework is divided in 6 steps:

* Coach
	* The coach is the brain of the system, he reunites and calculates some data in the field and decide what strategy our team should use.
* Strategy
	* The strategy is the layer that defines how our players should be distributed.
* Playbook
	* The playbook allows to gather a set of players (defined by strategy) and give them a role itself.
* Role
	* The function of the robot itself. This layer allows a set of behaviours to be 'unified' in a soccer behaviour itself.
* Behaviour
	* More complex actions of the robots, which mix a set of skills to form a behaviour itself.
* Skill
	* The basic actions of the robots, like go to a point, rotate, etc.


## Armorial-Project


## Dependencies:

### Ubuntu Dependencies (You can install using apt)
* Google Protobuf
* qt5-default
* libqt5opengl5-dev
* libprotobuf-dev 
* protobuf-compiler
* build-essential 
* libsfml-dev


### Instaling GearSystem
This Lib was created By Warthog Robotics and was modificated by Maracatronics to attend our necessity.

```bash
git clone https://github.com/maracatronics/GEARSystem.git
cd GEARSystem && cd trunk
sh build/corba_skeletons.sh
qmake install_path=/usr/lib/
make -j4
sudo sh install.sh
```


### Instaling WRBackbone
This Lib was created By Warthog Robotics and was modificated by Maracatronics to attend our necessity. It allows communication between all modules of Armorial-Project. 

```bash
git clone https://github.com/maracatronics/GEARSystem.git
cd GEARSystem && cd trunk
sh build/corba_skeletons.sh
qmake install_path=/usr/lib/
make -j4
sudo sh install.sh
```


## Compiling Armorial-Suassuna

- Step 1: Install all dependencies

- Step 2: Compiling 
```bash
git clone https://github.com/maracatronics/Armorial-Suassuna.git
sudo sh protobuf.sh

mkdir build
cd build
qmake ..
make
```
- Step 3: Run WRBackbone

- Step 4: Run Armorial-Suassuna

```bash
./bin/Armorial-Suassuna
```
