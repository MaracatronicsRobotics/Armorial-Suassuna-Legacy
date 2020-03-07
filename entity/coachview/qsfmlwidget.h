/* Header file */
#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include <QObject>

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
  Q_OBJECT
  private:
  virtual void onInit() = 0;
  virtual void onUpdate() = 0;

  virtual QPaintEngine* paintEngine() const;

  virtual void showEvent(QShowEvent* e);

  virtual void paintEvent(QPaintEvent* e);

  QTimer mTimer;
  bool mInitialized;

  public:
  explicit QSFMLCanvas(QWidget *parent, unsigned int frametime = 0);

  virtual ~QSFMLCanvas();

  signals:

  public slots:

};

#endif // QSFMLCANVAS_H
