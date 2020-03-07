#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "qsfmlwidget.h"
#include <QObject>
#include <entity/contromodule/mrcteam.h>

// widget config
#define framerate 60

// field config
#define soccerfield_width 7400
#define soccerfield_height 10400
#define borderOffset 300.f
#define centralCirleRadius 500.f

#define real_width 10400
#define real_height 7400

// objects config
#define circlePrecision 100.0f

// members config
#define maxRobots 6
#define ballRadius 50.f
#define robotRadius 110.f
#define robotRealRadius 180.f
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

  // center circle
  sf::CircleShape *circuloCentral;

  // members (ball and robots)
  sf::Texture texture;
  sf::Sprite sprite;
  sf::CircleShape *ball = new sf::CircleShape(ballRadius);
  static MRCTeam *_ourTeam;
  static MRCTeam *_theirTeam;

  public:
  explicit MyCanvas(QWidget *parent);
  static void setTeams(MRCTeam *our, MRCTeam *their){_ourTeam = our; _theirTeam = their;}

  signals:

  public slots:
};

#endif // MYCANVAS_H
