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


## Installing

Please, refer to [Armorial-WS](https://github.com/maracatronics/Armorial-WS) and install the entire Workspace.

## Runnning

To run the Armorial-Carrero please follow the steps listed in [Armorial-WS](https://github.com/maracatronics/Armorial-WS).
