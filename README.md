# Naval Battle Simulator

## Project Overview
The **Naval Battle Simulator** is a training simulation tool aimed at replicating naval combat scenarios. The simulator allows users to control a stationary battleship and engage with multiple enemy escort ships, simulation projectile motion and attck impacts.

This repository includes broken into three parts (A, B, and C) of the project, focusing on the initial development stages:
- **PART A** Setting up the battlefield, placing ships, and engaging in basic battles.
- **PART B** Simulating movement paths for the battleship and sequential engagement with enemy ships.
- **PART C** Enhancing realism by allowing the battleship to withstand multiple hits from enemy attacks based on impact power.

## Features
- **Battlefield Setup** A square battlefield is populated with a single battleship and randomly placed enemy escort ships.
- **Randomized Parameters** The intial positions, attack angles, and shell velocities of each ship are randomly generated.
- **Projectile Motion Simulation** Simulates the parabolic path of shells fired by the battleship and escort ships.
- **Health & Impact Calculations** Each escort ship attack reduces the battleships's heath by a percentage based on its impact power, allowing for multiple engagements.

## Requirements
- C Compiler (GCC recommended)

## Running the Simulator
1. Clone this repository.
   ```bash
   git clone https://github.com/san-rat/Naval_Battle_Simulator.git
