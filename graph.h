#ifndef GRAPH_H
#define GRAPH_H

#include "battlefield.h"
#include <easy2d/easy2d.h>

#define WINDOW_WIDTH 1375
#define WINDOW_HEIGHT 792
#define SLIME_X_PLAYER 180
#define SLIME_X_ENEMY 970
#define SLIME_Y 500

class GraphSlime
{
private:
  easy2d::Scene *battle_scene;

  Actor *player;
  Actor *enemy;

  easy2d::Sprite *SetSlimeOnPitch(bool PorE, float x, float y, Actor *slime);//史莱姆图片缩放30%，坐标x=180(左)或970(右)，y=500
  easy2d::Sprite *SetHPBar(float x, float y);//HP图片缩放300%，不透明度60%，坐标x+105(左)或x-695(右)，y+200 
  easy2d::Text *SetHP(float x, float y, Actor *slime);//HP文字为默认字体，字体大小90，加粗，每行长度380，居中
  std::string GetSlimeImg(const Pet *slime, bool PorE);

public:
  GraphSlime(Actor *left, Actor *right);
  ~GraphSlime();
  void Update(Actor *left, Actor *right);
};

#endif // GRAPH_H
