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

#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "qsfmlwidget.h"
#include <QObject>
#include <entity/contromodule/mrcteam.h>
#include <entity/player/player.h>

// widget config
#define framerate 60

// field config
#define soccerfield_width 7400
#define soccerfield_height 10400
#define widget_height 740.f
#define borderOffset 700.f
#define centralCirleRadius 500.f
#define goalDistToMargin 500.f

#define real_width 10400
#define real_height 7400

// objects config
#define circlePrecision 100.0f

// members config
#define maxRobots 6
#define ballRadius 50.f
#define robotRadius 110.f
#define robotRealRadius 1860.f
#define border 1400

class MyCanvas : public QSFMLCanvas
{
  Q_OBJECT
  private:
  void onInit();
  void onUpdate();

  // functions
  void zoomViewAt(sf::Vector2i pixel, double zoom);
  void drawBall();
  void drawRobots();

  // background
  sf::RectangleShape *mBackground;

  // field lines
  sf::Vertex linhasExternas[10] =
  {
      sf::Vertex(sf::Vector2f(borderOffset, borderOffset)),
      sf::Vertex(sf::Vector2f(soccerfield_width - borderOffset, borderOffset)),
      sf::Vertex(sf::Vector2f(soccerfield_width - borderOffset, borderOffset)),
      sf::Vertex(sf::Vector2f(soccerfield_width - borderOffset, soccerfield_height - borderOffset)),
      sf::Vertex(sf::Vector2f(soccerfield_width - borderOffset, soccerfield_height - borderOffset)),
      sf::Vertex(sf::Vector2f(borderOffset, soccerfield_height - borderOffset)),
      sf::Vertex(sf::Vector2f(borderOffset, soccerfield_height - borderOffset)),
      sf::Vertex(sf::Vector2f(borderOffset, borderOffset)),
      sf::Vertex(sf::Vector2f(borderOffset, soccerfield_height/2.0)),
      sf::Vertex(sf::Vector2f(soccerfield_width - borderOffset, soccerfield_height/2.0))
  };

  sf::Color *colorDeepGoal = new sf::Color(255, 26, 26);

  // vetor do gol (literalmente) esquerdo (baixo)
  sf::Vertex golFundoEsquerdo[6] =
  {
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 500.f, goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 500.f, 200.f + goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 500.f, goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 500.f, 200.f + goalDistToMargin),(*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 500.f, goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 500.f, goalDistToMargin), (*colorDeepGoal))
  };

  // vetor do gol esquerdo (baixo)
  sf::Vertex golEsquerdo[6] =
  {
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 1000.f, borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 1000.f, 1000.f + borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 1000.f, borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 1000.f, 1000.f + borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 1000.f, 1000.f + borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 1000.f, 1000.f + borderOffset))
  };

  // vetor do gol (literalmente) direto (cima)
  sf::Vertex golFundoDireito[6] =
  {
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 500.f, soccerfield_height - goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 500.f, soccerfield_height - (200.f + goalDistToMargin)), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 500.f, soccerfield_height - goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 500.f, soccerfield_height - (200.f + goalDistToMargin)), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 500.f, soccerfield_height - goalDistToMargin), (*colorDeepGoal)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 500.f, soccerfield_height - goalDistToMargin), (*colorDeepGoal))
  };

  // vetor do gol direito (cima)
  sf::Vertex golDireito[6] =
  {
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 1000.f, soccerfield_height - borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 1000.f, soccerfield_height - (1000.f + borderOffset))),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 1000.f, soccerfield_height - borderOffset)),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 1000.f, soccerfield_height - (1000.f + borderOffset))),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) - 1000.f, soccerfield_height - (1000.f + borderOffset))),
      sf::Vertex(sf::Vector2f((soccerfield_width/2.0) + 1000.f, soccerfield_height - (1000.f + borderOffset)))
  };

  sf::Vertex linhaVertical[2] =
  {
      sf::Vertex(sf::Vector2f(soccerfield_width/2.0, borderOffset)),
      sf::Vertex(sf::Vector2f(soccerfield_width/2.0, soccerfield_height - borderOffset))
  };

  // center circle
  sf::CircleShape *circuloCentral;

  // members (ball and robots)
  sf::Texture texture;
  sf::Sprite sprite;
  sf::CircleShape *ball = new sf::CircleShape(ballRadius);
  static MRCTeam *_ourTeam;
  static MRCTeam *_theirTeam;
  void drawPathLines(quint8 playerId);
  void drawKalmanPredict(quint8 playerId);

  public:
  explicit MyCanvas(QWidget *parent);
  static void setTeams(MRCTeam *our, MRCTeam *their){_ourTeam = our; _theirTeam = their;}

  signals:

  public slots:
};

#endif // MYCANVAS_H
