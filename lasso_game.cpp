#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <composite.h>
#include <sprite.h>
#include <random>
#include <time.h> 

using namespace simplecpp;
// properties of moving object
class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);   //move the obejt
  }
  vx += ax*t;
  vy += ay*t; //change the velocity
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1; //maybe to like getClick to get position
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}
//above they defined the properties of MO
//coin.h
#ifndef __COIN_H__
#define __COIN_H__




#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 300
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 12
#define LASSO_RADIUS 50
#define COIN_SIZE 6



#endif

//coin.h
class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  public:
  // Moving parts
  Circle coin_circle; //here only 1 circle ... defining shape of the moving object
  bool is_bomb=false;
  bool is_magnet=false;
  int coin_point;
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void resetCoin();
  void bomb_switch(); // declared functions to convert coin to bomb
  void coin_switch();
  void magnet_switch();
}; // End class Coin

//basically to start the coin movement
void Coin::initCoin() {
  coin_start_x = 400 + ((rand()) / ((RAND_MAX)))*(PLAY_X_START+WINDOW_X)/2 ; //im changing it to rand
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(255,215,0));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

// reset coin when it reacehs x axis of after yank
void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;   //changing defalut coin to parabola
  coin_ay = COIN_G; //gravity on coin
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::bomb_switch(){
    coin_circle.setColor(COLOR("black"));
    coin_point=-2;
    is_bomb=true;
    is_magnet=false;
}

void Coin::coin_switch(){
    coin_circle.setColor(COLOR(255,215,0));
    coin_point=1;
    is_bomb=false;
    is_magnet=false;
}

void Coin::magnet_switch(){
    coin_circle.setColor(COLOR(192,192,192));
    coin_point=1;
    is_bomb=false;
    is_magnet=true;
}


//lasso.cpp
class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin; // pointer to coin which is catched

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }
    int num_coins;  // number of coins
    bool caught_magnet=false; // check for magnet

  void draw_lasso_band();
  void yank();
  void yank_2();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_bomb(Coin *coin);
  int getNumCoins() { return num_coins; }

}; // End class Lasso

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

//to start the lasso
void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(255,140,0));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("blue"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("blue"));

  lasso_band.setColor(COLOR(138,43,226));
  draw_lasso_band();

} // End Lasso::initLasso()

//check coin
void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT,COIN_SPEED,COIN_ANGLE_DEG, 0, COIN_G, paused, rtheta);
    the_coin=NULL;
  }
} // End Lasso::yank()
// type to yank
void Lasso::yank_2() {
    // once thrown can not be reserved without catching the coin; necceracry for bomb
  if(getYPos()>PLAY_Y_HEIGHT || the_coin != NULL){
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;

  if(the_coin != NULL) {
    num_coins+=the_coin->coin_point;
    the_coin->reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SPEED, COIN_ANGLE_DEG,0, COIN_G, paused, rtheta);
    if(the_coin->is_magnet){
        caught_magnet=true;
    }
    the_coin=NULL; // the_coin reset
  }
  }
} // End Lasso::yank_2()

//the catching operation
// the cirlce which appear while catching
void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped then do nothing
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

// operation on lasso to change the throwing angle
void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

//change thrwo speed
void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

// basically for contineous motion of lasso
void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

//
void Lasso::check_for_bomb(Coin *coinPtr) {
    if(coinPtr->is_bomb){
      double lasso_x = getXPos();
      double lasso_y = getYPos();
      double coin_x = coinPtr->getXPos();
      double coin_y = coinPtr->getYPos();
      double xdiff = (lasso_x - coin_x);
      double ydiff = (lasso_y - coin_y);
      double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
      if(distance <= LASSO_RADIUS/2.5){
        num_coins+=coinPtr->coin_point;
        coinPtr->resetCoin();
    }
  }
}
void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }

}
// End Lasso::check_for_coin()


main_program {

    srand (time(NULL));
    initCanvas("Lasso", WINDOW_X, WINDOW_Y);

    {
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"PRESS");
        Text Instruction1(WINDOW_X/2, WINDOW_Y/2 - 75,": Shoot");
        Text Instruction2(WINDOW_X/2, WINDOW_Y/2 - 50,"y: Yank");
        Text Instruction3(WINDOW_X/2, WINDOW_Y/2 - 25,"l: Catch");
        Text Instruction4(WINDOW_X/2, WINDOW_Y/2 -  0,"[: decrease elevation");
        Text Instruction5(WINDOW_X/2, WINDOW_Y/2 + 25,"]: increase elevation");
        Text Instruction6(WINDOW_X/2, WINDOW_Y/2 + 50,"-: decrease lasso speed");
        Text Instruction7(WINDOW_X/2, WINDOW_Y/2 + 75,"=: increase lasso speed");
        Text Instruction8(WINDOW_X/2, WINDOW_Y/2 +100,"q: quit game");
        Text Instruction9(WINDOW_X/2, WINDOW_Y/2 +200,"you have 3 lives");
        Text Instruction10(WINDOW_X/2, WINDOW_Y/2 +250,"click to continue");

        getClick();
    }


  //
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = 45; // sec; -ve means infinite
  float currTime = 0;
  int lives=3;
  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;

  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);


  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;  //here to intro parabola
  double coin_ay = COIN_G;


  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  //level 1
  int min_coins=2;//cha
  {
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"Round 1");
        Text Instruction1(WINDOW_X/2, WINDOW_Y/2 - 75,"minimun coins to pass level");
        Text Instruction2(WINDOW_X/2, WINDOW_Y/2 - 50, min_coins);
        Text Instruction3(WINDOW_X/2, WINDOW_Y/2 - 20," catch gold coins");
        Circle coin_circle(WINDOW_X/2, WINDOW_Y/2 +20, 20);
        coin_circle.setColor(COLOR(255,215,0));
        coin_circle.setFill(true);

        Text Instruction10(WINDOW_X/2, WINDOW_Y/2 +250,"click to continue");

        getClick();
  }

  runTime = 45;
  while(lives>0){
    Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta); //only 1 coin declared
    coin.hide();
  // After every COIN_GAP sec, make the coin jump
    double last_coin_jump_end = 0;
    currTime=0; //reset time
    for(;;) {
        if((runTime > 0) && (currTime > runTime)) {
          lives-=1;// life lost when timer reaches zero
          Text lostLives(WINDOW_X/2, WINDOW_Y/2, " YOU LOST A LIFE" );
          wait(2);
          break;
        }
        if(lives==0 || lasso.getNumCoins()>= min_coins){
            break;
        }
        // added timer
        char timer[256];
        sprintf(timer, "remaing time: %.2f", runTime-currTime);
        Text Timer(450,50, timer);
        if(currTime>5){
            coin.show();
        }

        XEvent e; //taking input from keyboard
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
          char c = charFromEvent(e); //in cmd
          msg[msg.length()-1] = c;
          charPressed.setMessage(msg);
          switch(c) {
          case 't':
        lasso.unpause();
        break;
          case 'y':
        lasso.yank(); //code for yank without coin
        break;
          case 'l':
        lasso.loopit();
        lasso.check_for_coin(&coin);
        wait(STEP_TIME*5); // to slow down the lasso
        break;
          case '[':
        if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
        break;
          case ']':
        if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
        break;
          case '-':
        if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
        break;
          case '=':
        if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
        break;
          case 'q':
        exit(0);   //initCnavas function
          default:
        break; // to break the switch .... for loop continues
          }
        }

        lasso.nextStep(stepTime);

        coin.nextStep(stepTime);
        if(coin.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin.unpause();
          }
        }

        if(coin.getYPos() > PLAY_Y_HEIGHT) {
          coin.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
          last_coin_jump_end = currTime;
        }

        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
      } // End for(;;)

    if(lives==0){
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"YOU DEAD BRO");
        wait(3);
        exit(0);
    }
    if(lasso.getNumCoins()>= min_coins){
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"congo....here comes the next!");
        break;
    }
}
    lasso.num_coins=0;

// level 3 or bonus level
    runTime=30;

    {
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"Bonus roung");
        Text Instruction1(WINDOW_X/2, WINDOW_Y/2 - 75,"catch as many coins as possible");
        Text Instruction2(WINDOW_X/2, WINDOW_Y/2 - 50," these will be carried on to next round");

        Text Instruction10(WINDOW_X/2, WINDOW_Y/2 +250,"click to continue");
        getClick();
  }
  {

      Coin coin(coin_speed, coin_angle_deg, coin_ax + (rand()%30), coin_ay +(rand()%15), paused, rtheta);//only 1 coin declared
      coin.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);
      Coin coin1(coin_speed, coin_angle_deg, coin_ax + (rand()%30), coin_ay+(rand()%15), paused, rtheta);
      coin1.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);
      Coin coin2(coin_speed, coin_angle_deg, coin_ax + (rand()%30), coin_ay+(rand()%15), paused, rtheta);
      coin2.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);


  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

    currTime=0; //reset time
    for(;;) {
        if((runTime > 0) && (currTime > runTime)) {
          break;
        }

        char timer[256];
        sprintf(timer, "remaing time: %.2f", runTime-currTime);
        Text Timer(450,50, timer);

        XEvent e; //taking input from keyboard
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
          char c = charFromEvent(e); //in cmd
          msg[msg.length()-1] = c;
          charPressed.setMessage(msg);
          switch(c) {
          case 't':
        lasso.unpause();
        break;
          case 'y':
        lasso.yank(); //code for yank without coin
        break;
          case 'l':
        lasso.loopit();
        lasso.check_for_coin(&coin);
        lasso.check_for_coin(&coin1);
        lasso.check_for_coin(&coin2);
        wait(STEP_TIME*5); // to slow down the lasso
        break;
          case '[':
        if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
        break;
          case ']':
        if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
        break;
          case '-':
        if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
        break;
          case '=':
        if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
        break;
          case 'q':
        exit(0);   //initCnavas function
          default:
        break; // to break the switch .... for loop continues
          }
        }

        lasso.nextStep(stepTime);

        coin.nextStep(stepTime);
        if(coin.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin.unpause();
          }
        }

        if(coin.getYPos() > PLAY_Y_HEIGHT) {
          coin.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg + (rand()%45)-rand()%45 , coin_ax , coin_ay + (rand()%15)-rand()%7, paused, rtheta);

          last_coin_jump_end = currTime;
        }

        coin1.nextStep(stepTime);
        if(coin1.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin1.unpause();
          }
        }

        if(coin1.getYPos() > PLAY_Y_HEIGHT || coin1.getXPos()<PLAY_X_START) {
          coin1.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg + (rand()%45)-rand()%45, coin_ax , coin_ay + (rand()%15)-rand()%7, paused, rtheta);
          last_coin_jump_end = currTime;
        }
        coin2.nextStep(stepTime);
        if(coin2.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin2.unpause();
          }
        }

        if(coin2.getYPos() > PLAY_Y_HEIGHT) {
          coin2.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg + (rand()%45)-rand()%45, coin_ax , coin_ay + (rand()%15)-rand()%7, paused, rtheta);

          last_coin_jump_end = currTime;
        }

        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
      } // End for(;;)

  }
  wait(3);

    //level 2
    runTime = 60;
  {
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"Round 2");
        Text Instruction1(WINDOW_X/2, WINDOW_Y/2 - 75,"minimun coins to pass level");
        Text Instruction2(WINDOW_X/2, WINDOW_Y/2 - 50, min_coins);
        Text Instruction3(WINDOW_X/2, WINDOW_Y/2 - 20," beware of bombs...they give -2 point T_T");
        Circle coin_bomb(WINDOW_X/2, WINDOW_Y/2 +20, 20);
        coin_bomb.setColor(COLOR("black"));
        coin_bomb.setFill(true);

        Text Instruction10(WINDOW_X/2, WINDOW_Y/2 +250,"click to continue");
        getClick();
  }
  while(lives>0){
    Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);//only 1 coin declared
    coin.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);
    Coin coin1(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    coin1.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);

    //if(currTime>rand()%15){
    //      }
    currTime=0;
    min_coins=9; //min coins required to pass the level
    // After every COIN_GAP sec, make the coin jump
    double last_coin_jump_end = 0;

    // When t is pressed, throw lasso
    // If lasso within range, make coin stick
    // When y is pressed, yank lasso
    // When l is pressed, loop lasso
    // When q is pressed, quit


    for(;;) {
        if((runTime > 0) && (currTime > runTime) && lasso.getNumCoins()< min_coins) {
          lives-=1;// life lost when timer reaches zero
          Text lostLives(WINDOW_X/2, WINDOW_Y/2, " YOU LOST A LIFE" );
          currTime=0; //reset time
          wait(2);
          break;
        }
        if(lives==0 || lasso.getNumCoins()>= min_coins){
            break;
        }
        // added timer
        char timer[256];
        sprintf(timer, "remaing time: %.2f", runTime-currTime);
        Text Timer(450,50, timer);


        XEvent e; //taking input from keyboard
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
          char c = charFromEvent(e); //in cmd
          msg[msg.length()-1] = c;
          charPressed.setMessage(msg);
          switch(c) {
          case 't':
        lasso.unpause();
        break;
          case 'y':
        lasso.yank(); //code for yank without coin
        break;
          case 'l':
        lasso.loopit();
        lasso.check_for_coin(&coin);
        lasso.check_for_coin(&coin1);
        wait(STEP_TIME*5); // to slow down the lasso
        break;
          case '[':
        if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
        break;
          case ']':
        if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
        break;
          case '-':
        if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
        break;
          case '=':
        if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
        break;
          case 'q':
        exit(0);   //initCnavas function terminate the program
          default:
        break; // to break the switch .... for loop continues
          }
        }

        lasso.nextStep(stepTime);

        coin.nextStep(stepTime);
        if(coin.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin.unpause();
          }
        }


        if(coin.getYPos() > PLAY_Y_HEIGHT) {
          coin.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
          if(rand()%10 >4){
            coin.bomb_switch();
          }
          else{
            coin.coin_switch();
          }
          last_coin_jump_end = currTime;
        }

        //same thing for coin 2
        coin1.nextStep(stepTime);
        if(coin1.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
            coin1.unpause();
          }
        }

        if(coin1.getYPos() > PLAY_Y_HEIGHT) {
          coin1.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
          if(rand()%10 >6){
            coin1.bomb_switch();
          }
          else{
            coin1.coin_switch();
          }
          last_coin_jump_end = currTime;
        }


        lasso.check_for_bomb(&coin);
        lasso.check_for_bomb(&coin1);

        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
      } // End for(;;)

    if(lives==0){
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"YOU DEAD BRO");
        wait(3);
        exit(0);
    }
    if(lasso.getNumCoins()>= min_coins){
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"congo....here comes the next!");
        lasso.num_coins=0;
        break;
        wait(3);
    }

  }
  wait(3);

  // level 3
  while(lives>0){

    Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);//only 1 coin declared
    coin.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);
    Coin coin1(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    coin1.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);
    Coin coin2(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    coin2.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);
    Coin coin3(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    coin3.coin_circle.reset(WINDOW_X/2 +WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, COIN_SIZE);

    //if(currTime>rand()%15){
    //      }

    min_coins=5; //min coins required to pass the level
    // After every COIN_GAP sec, make the coin jump
    double last_coin_jump_end = 0;
    currTime=0;


    for(;;) {
        if((runTime > 0) && (currTime > runTime) && lasso.getNumCoins()< min_coins) {
          lives-=1;// life lost when timer reaches zero
          Text lostLives(WINDOW_X/2, WINDOW_Y/2, " YOU LOST A LIFE" );
          currTime=0; //reset time
          wait(2);
          break;
        }
        if(lives==0 || lasso.getNumCoins()>= min_coins){
            break;
        }
        // added timer
        char timer[256];
        sprintf(timer, "remaing time: %.2f", runTime-currTime);
        Text Timer(450,50, timer);


        XEvent e; //taking input from keyboard
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
          char c = charFromEvent(e); //in cmd
          msg[msg.length()-1] = c;
          charPressed.setMessage(msg);
          switch(c) {
          case 't':
        lasso.unpause();
        break;
          case 'y':
        lasso.yank_2(); //code for yank without coin
        break;
          case 'l':
        lasso.loopit();
        lasso.check_for_coin(&coin);
        lasso.check_for_coin(&coin1);
        wait(STEP_TIME*5); // to slow down the lasso
        break;
          case '[':
        if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
        break;
          case ']':
        if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
        break;
          case '-':
        if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
        break;
          case '=':
        if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
        break;
          case 'q':
        exit(0);   //initCnavas function terminate the program
          default:
        break; // to break the switch .... for loop continues
          }
        }

        lasso.nextStep(stepTime);

        coin.nextStep(stepTime);
        if(coin.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin.unpause();
          }
        }


        if(coin.getYPos() > PLAY_Y_HEIGHT || coin.getXPos()<PLAY_X_START) {
          coin.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax + (rand()%30)-18, coin_ay + (rand()%15)-7, paused, rtheta);
          int coin_bomb_magnet=rand()%10;
          if(coin_bomb_magnet>5 && coin_bomb_magnet<9){
            coin.bomb_switch();
          }
          else if(coin_bomb_magnet==9){
            coin.magnet_switch();
          }
          else{
            coin.coin_switch();
          }
          last_coin_jump_end = currTime;
        }

        coin1.nextStep(stepTime);
        if(coin1.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin1.unpause();
          }
        }

        if(coin1.getYPos() > PLAY_Y_HEIGHT || coin1.getXPos()<PLAY_X_START) {
          coin1.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax + (rand()%30)-18, coin_ay + (rand()%15)-7, paused, rtheta);
          int coin_bomb_magnet=rand()%10;
          if(coin_bomb_magnet>5 && coin_bomb_magnet<9){
            coin1.bomb_switch();
          }
          else if(coin_bomb_magnet==9){
            coin1.magnet_switch();
          }
          else{
            coin1.coin_switch();
          }
          last_coin_jump_end = currTime;
        }

        coin2.nextStep(stepTime);
        if(coin2.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin2.unpause();
          }
        }


        if(coin2.getYPos() > PLAY_Y_HEIGHT || coin2.getXPos()<PLAY_X_START) {
          coin2.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax + (rand()%30)-18, coin_ay + (rand()%15)-7, paused, rtheta);
          int coin_bomb_magnet=rand()%10;
          if(coin_bomb_magnet>5 && coin_bomb_magnet<9){
            coin2.bomb_switch();
          }
          else if(coin_bomb_magnet==9){
            coin2.magnet_switch();
          }
          else{
            coin2.coin_switch();
          }
          last_coin_jump_end = currTime;
        }

        coin3.nextStep(stepTime);
        if(coin.isPaused()) {
          if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin3.unpause();
          }
        }


        if(coin3.getYPos() > PLAY_Y_HEIGHT || coin3.getXPos()<PLAY_X_START) {
          coin3.reset_all(PLAY_X_START*4 + WINDOW_X/2*(rand()%100)/100.0, PLAY_Y_HEIGHT, coin_speed, coin_angle_deg, coin_ax + (rand()%30)-18, coin_ay + (rand()%15)-7, paused, rtheta);
          int coin_bomb_magnet=rand()%10;
          if(coin_bomb_magnet>5 && coin_bomb_magnet<9){
            coin3.bomb_switch();
          }
          else if(coin_bomb_magnet==9){
            coin3.magnet_switch();
          }
          else{
            coin3.coin_switch();
          }
          last_coin_jump_end = currTime;
        }

        lasso.check_for_bomb(&coin);
        lasso.check_for_bomb(&coin1);
        lasso.check_for_bomb(&coin2);
        lasso.check_for_bomb(&coin3);


        if(lasso.caught_magnet){
            lasso.caught_magnet=false;
            coin.reset_all(coin.getXPos(), coin.getYPos(), coin_speed + 20 , 180 + atan2(coin.getYPos()-(PLAY_Y_HEIGHT-LASSO_Y_HEIGHT),coin.getXPos()-(PLAY_X_START+LASSO_X_OFFSET)), coin_ax , coin_ay + (rand()%15)-7, paused, rtheta);
            coin1.reset_all(coin1.getXPos(), coin1.getYPos(), coin_speed + 20 , 180 + atan2(coin1.getYPos()-(PLAY_Y_HEIGHT-LASSO_Y_HEIGHT),coin1.getXPos()-(PLAY_X_START+LASSO_X_OFFSET)), coin_ax , coin_ay + (rand()%15)-7, paused, rtheta);
            coin2.reset_all(coin2.getXPos(), coin2.getYPos(), coin_speed + 20 , 180 + atan2(coin2.getYPos()-(PLAY_Y_HEIGHT-LASSO_Y_HEIGHT),coin2.getXPos()-(PLAY_X_START+LASSO_X_OFFSET)), coin_ax , coin_ay + (rand()%15)-7, paused, rtheta);
            coin3.reset_all(coin3.getXPos(), coin3.getYPos(), coin_speed + 20 , 180 + atan2(coin3.getYPos()-(PLAY_Y_HEIGHT-LASSO_Y_HEIGHT),coin3.getXPos()-(PLAY_X_START+LASSO_X_OFFSET)), coin_ax , coin_ay + (rand()%15)-7, paused, rtheta);
        }

        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
      } // End for(;;)

    if(lives==0){
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"YOU DEAD BRO");
        wait(3);
        exit(0);
    }
    if(lasso.getNumCoins()>= min_coins){
        Text Instruction0(WINDOW_X/2, WINDOW_Y/2 -100,"congo...YOU WON");
        lasso.num_coins=0;
        wait(3);
        exit(0);
    }

  }
  wait(3);



} // End main_program

