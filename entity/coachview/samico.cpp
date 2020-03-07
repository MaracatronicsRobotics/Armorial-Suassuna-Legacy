#include "samico.h"
#include <iostream>

MRCTeam* MyCanvas::_ourTeam = NULL;
MRCTeam* MyCanvas::_theirTeam = NULL;

MyCanvas::MyCanvas(QWidget *parent) :
QSFMLCanvas(parent)
{

}

void MyCanvas::onInit()
{
    mBackground = new sf::RectangleShape(sf::Vector2f((float) soccerfield_width, (float) soccerfield_height));
    mBackground->setFillColor(sf::Color(0, 100, 0, 255));

    // Central circle
    circuloCentral = new sf::CircleShape(centralCirleRadius);
    circuloCentral->setFillColor(sf::Color(0, 0, 0, 0));
    circuloCentral->setOutlineThickness(15.f);
    circuloCentral->setOutlineColor(sf::Color(255, 255, 255, 255));
    circuloCentral->setPointCount(circlePrecision);
    circuloCentral->setPosition((soccerfield_width/2.0) - centralCirleRadius, (soccerfield_height/2.0) - centralCirleRadius);

    // Ball
    ball->setFillColor(sf::Color(255, 69, 0, 255));
    ball->setPointCount(circlePrecision);
    ball->setPosition(soccerfield_width/2.0 - ballRadius, soccerfield_height/2.0 - ballRadius);

    // Framerate settings
    sf::RenderWindow::setFramerateLimit(framerate);

    double widthZoom = soccerfield_width / 15.07;
    this->zoomViewAt(sf::Vector2i(0, 0), soccerfield_width / widthZoom);
}

void MyCanvas::drawBall(){
    if(_ourTeam == NULL) return ;
    if(_ourTeam->ballPosition().isValid()){
        double ballx = abs(((_ourTeam->ballPosition().x()*1000)-(real_width / 2.0))/((real_width / 2.0)/(soccerfield_height/2.0)));
        double bally = abs(((_ourTeam->ballPosition().y()*1000)-(real_height / 2.0))/((real_height / 2.0)/(soccerfield_width/2.0)));

        ball->setPosition(bally - ballRadius, ballx - ballRadius);
        ball->setOutlineColor(sf::Color::Black);
        ball->setOutlineThickness(12.f);

        sf::RenderWindow::draw(*ball);
    }
}

void MyCanvas::drawRobots(){
    if(_ourTeam == NULL || _theirTeam == NULL) return ;
    char direct[30];

    QList<Player*> list = _ourTeam->avPlayers().values();
    QList<Player*> list2 = _theirTeam->avPlayers().values();

    for(int x = 0; x < list2.size(); x++){
        list.push_back(list2.at(x));
    }

    // list now contains every player
    int size = list.size();
    for(int x = 0; x < size; x++){
        double t = list.at(x)->orientation().value(), newx, newy;
        if(list.at(x)->teamColor() == Colors::Color::BLUE){
            // carregando imagem para sprite e setando a textura
            sprintf(direct, "../textures/b%d.png", list.at(x)->playerId());
            if(!texture.loadFromFile(direct)){
                printf("Error loading texture.\n");
                exit(-1);
            }
        }else{
            // carregando imagem para sprite e setando a textura
            sprintf(direct, "../textures/y%d.png", list.at(x)->playerId());
            if(!texture.loadFromFile(direct)){
                printf("Error loading texture.\n");
                exit(-1);
            }
        }

        sprite.setTexture(texture);
        sprite.setScale(0.43,0.43);
        newx = abs(((list.at(x)->position().x()*1000.0)-(real_width / 2.0))/((real_width / 2.0)/(soccerfield_height/2.0)));
        newy = abs(((list.at(x)->position().y()*1000.0)-(real_height / 2.0))/((real_height / 2.0)/(soccerfield_width/2.0)));

        // setando dados para a sprite
        sprite.setPosition(newy, newx);
        sprite.setOrigin(256, 256);
        sprite.setRotation(-0-(t*57.2958));

        if(list.at(x)->position().isValid()){ // se o frame for valido, desenha
            sf::RenderWindow::draw(sprite);
        }
    }
}

void MyCanvas::zoomViewAt(sf::Vector2i pixel, double zoom){
    const sf::Vector2f beforeCoord{ sf::RenderWindow::mapPixelToCoords(pixel) };
    sf::View view{ sf::RenderWindow::getView() };
    view.zoom(zoom);
    sf::RenderWindow::setView(view);
    const sf::Vector2f afterCoord{ sf::RenderWindow::mapPixelToCoords(pixel) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    sf::RenderWindow::setView(view);
}

void MyCanvas::onUpdate()
{
    sf::RenderWindow::clear(sf::Color(0, 0, 0));

    // Draw field background
    sf::RenderWindow::draw(*mBackground);

    // Draw field components (lines, circles, rectangles, etc)
    sf::RenderWindow::draw(linhasExternas, 10, sf::Lines);
    sf::RenderWindow::draw(golEsquerdo, 6, sf::Lines);
    sf::RenderWindow::draw(golDireito, 6, sf::Lines);
    sf::RenderWindow::draw(*circuloCentral);

    // Draw soccer objects (ball and robots)
    drawRobots();
    drawBall();
}
