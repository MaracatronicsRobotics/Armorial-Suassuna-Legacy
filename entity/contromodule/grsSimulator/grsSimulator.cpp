/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "grsSimulator.h"
#include <iostream>

QString grsSimulator::name(){
    return "GRSim Simulator";
}

grsSimulator::grsSimulator() : Entity(ENT_GRSIMULATOR)
{
    if(_socket.isOpen())
        _socket.close();

    _socket.connectToHost("127.0.0.1", 20011, QIODevice::WriteOnly, QAbstractSocket::IPv4Protocol);

    // reseting
    for(int x = 0; x < MAX_TEAMS; x++){
        for(int y = 0; y < MAX_ROBOTS; y++){
            packets[x][y].id = y;
            packets[x][y].isYellow = x ? false : true;
            packets[x][y].vx = 0.0;
            packets[x][y].vy = 0.0;
            packets[x][y].angle = 0.0;
            packets[x][y].spinner = false;
            packets[x][y].kickspeedx = 0.0;
            packets[x][y].kickspeedz = 0.0;
        }
    }
}

void grsSimulator::sendPacket(grs_robot robot){
    grSim_Packet packet;
    packet.mutable_commands()->set_isteamyellow(robot.isYellow);
    packet.mutable_commands()->set_timestamp(0.0);

    grSim_Robot_Command *command = packet.mutable_commands()->add_robot_commands();

    command->set_id(robot.id);

    command->set_wheelsspeed(false);
    command->set_wheel1(robot.v1);
    command->set_wheel2(robot.v2);
    command->set_wheel3(robot.v3);
    command->set_wheel4(robot.v4);

    command->set_veltangent(robot.vx);
    command->set_velnormal(robot.vy);
    command->set_velangular(robot.angle);

    command->set_kickspeedx(robot.kickspeedx);
    command->set_kickspeedz(robot.kickspeedz);
    command->set_spinner(robot.spinner);

    std::string s;
    packet.SerializeToString(&s);
    if(_socket.write(s.c_str(), s.length()) == -1)
        std::cout << "[GRSIM ACTUATOR] Failed to write to socket: " << _socket.errorString().toStdString() << std::endl;
}

void grsSimulator::setSpeed(quint8 teamId, quint8 robotId, float vx, float vy, float theta){
    if(!(teamId < MAX_TEAMS)){
        std::cout << "[GRSIM ACTUATOR] Invalid team id." << std::endl;
        return;
    }
    if(!(robotId < MAX_ROBOTS)){
        std::cout << "[GRSIM ACTUATOR] Invalid robot id." << std::endl;
        return ;
    }

    _writeMutex.lock();
    packets[teamId][robotId].id = robotId;
    packets[teamId][robotId].isYellow = teamId ? false : true;
    packets[teamId][robotId].vx = vx;
    packets[teamId][robotId].vy = vy;
    packets[teamId][robotId].angle = theta;
    _writeMutex.unlock();

}

void grsSimulator::setKickSpeed(quint8 teamId, quint8 robotId, float kickx, float kickz){
    if(!(teamId < MAX_TEAMS)){
        std::cout << "[GRSIM ACTUATOR] Invalid team id." << std::endl;
        return;
    }
    if(!(robotId < MAX_ROBOTS)){
        std::cout << "[GRSIM ACTUATOR] Invalid robot id." << std::endl;
        return ;
    }

    _writeMutex.lock();
    packets[teamId][robotId].kickspeedx = kickx;
    packets[teamId][robotId].kickspeedz = kickz;
    _writeMutex.unlock();
}

void grsSimulator::setDribble(quint8 teamId, quint8 robotId, bool active){
    if(!(teamId < MAX_TEAMS)){
        std::cout << "[GRSIM ACTUATOR] Invalid team id." << std::endl;
        return;
    }
    if(!(robotId < MAX_ROBOTS)){
        std::cout << "[GRSIM ACTUATOR] Invalid robot id." << std::endl;
        return ;
    }

    _writeMutex.lock();
    packets[teamId][robotId].spinner = active;
    _writeMutex.unlock();
}

void grsSimulator::initialization(){
    std::cout << "[GRSIM ACTUATOR] Thread started." << std::endl;
}

void grsSimulator::loop(){
    for(int x = 0; x < MAX_TEAMS; x++){
        for(int y = 0; y < MAX_ROBOTS; y++){
            sendPacket(packets[x][y]);
        }
    }
}

void grsSimulator::finalization(){
    _socket.disconnectFromHost();
    _socket.close();
    std::cout << "[GRSIM ACTUATOR] Thread ended." << std::endl;
}
