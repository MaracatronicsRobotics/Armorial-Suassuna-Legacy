

void Player::goToLookTo(double robot_x, double robot_y, double point_x, double point_y, double aim_x, double aim_y, double angleOrigin2Robot, double offset){
    // Configura o robô para ir até a bola e olhar para um alvo
    std::pair<float, float> a;

void Player::goToLookTo(double robot_x, double robot_y, double point_x, double point_y, double aim_x, double aim_y, double angleOrigin2Robot, double offset){
    // Configura o robô para ir até a bola e olhar para um alvo
    std::pair<float, float> a;
    a = GoTo(robot_x, robot_y, point_x, point_y, angleOrigin2Robot, _distBall);
    float theta = RotateTo(robot_x, robot_y, point_x, point_y, angleOrigin2Robot);

    setSpeed(a.first, a.second, w.second);
}

void Player::AroundTheBall(double robot_x, double robot_y, double point_x, double point_y, double robotAngle, double offset){
    // Configura o robô para ir até a bola e girar em torno dela
    std::pair<float, float> a;
    long double moduloDistancia = sqrt(pow((point_x - robot_x),2)+pow((point_y - robot_y),2));
    a = GoTo(robot_x, robot_y, point_x, point_y, robotAngle, offset);
    float theta = RotateTo(robot_x, robot_y, point_x, point_y, robotAngle);

    if (moduloDistancia < offset) setSpeed(0, 0.2, theta); //3% de diferença nas velocidades
    else setSpeed(a.first, a.second, theta);
}

void Player::Dribble(){}


void Player::goToLookTo(double robot_x, double robot_y, double point_x, double point_y, double aim_x, double aim_y, double angleOrigin2Robot, double offset){
    // Configura o robô para ir até a bola e olhar para um alvo
    std::pair<float, float> a;
    double minValue = 1.5;
    double maxValue = 3.0;

void Player::goToLookTo(double robot_x, double robot_y, double point_x, double point_y, double aim_x, double aim_y, double angleOrigin2Robot, double offset){
    // Configura o robô para ir até a bola e olhar para um alvo
    std::pair<float, float> a;
    a = GoTo(robot_x, robot_y, point_x, point_y, angleOrigin2Robot, _distBall);
    float theta = RotateTo(robot_x, robot_y, point_x, point_y, angleOrigin2Robot);
    a = GoTo(robot_x, robot_y, point_x, point_y, angleOrigin2Robot, _distBall);
    std::pair<double, double> w;
    w = RotateTo(robot_x, robot_y, point_x, point_y, angleOrigin2Robot);

    setSpeed(a.first, a.second, w.second);
}

void Player::AroundTheBall(double robot_x, double robot_y, double point_x, double point_y, double robotAngle, double offset){
    // Configura o robô para ir até a bola e girar em torno dela
    std::pair<float, float> a, b;
    long double moduloDistancia = sqrt(pow((point_x - robot_x),2)+pow((point_y - robot_y),2));
    a = GoTo(robot_x, robot_y, point_x, point_y, robotAngle, offset);
    b = RotateTo(robot_x, robot_y, point_x, point_y, robotAngle);

    if (moduloDistancia < offset) setSpeed(0, 0.4, b.second); //3% de diferença nas velocidades
    else setSpeed(a.first, a.second, b.second);
}
