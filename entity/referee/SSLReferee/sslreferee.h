/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
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

#ifndef SSLREFEREE_HH
#define SSLREFEREE_HH

#include <QUdpSocket>
#include <QNetworkInterface>
#include <include/3rd_party/referee.pb.h>
#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <entity/referee/referee.h>
#include <utils/basics/color.hh>

class SSLReferee : public Referee {
public:
    SSLReferee(QString ipAddress = "224.5.23.2", int port = 10003);
    ~SSLReferee();
    QString name();

    // Add and get game info
    SSLGameInfo* getGameInfo(Colors::Color color);
    void addGameInfo(Colors::Color color);

    // Set ball kicked
    void setBallKicked();
private:
    // Entity inherit virtual methods
    void initialization();
    void loop();
    void finalization();

    // Network configuration
    QHostAddress _ipAddress;
    int _port;

    // UDP socket
    QUdpSocket *_socket;

    // Game infos (two team colors)
    QMap<Colors::Color, SSLGameInfo*> _gameInfo;

};

#endif // SSLREFEREE_HH
