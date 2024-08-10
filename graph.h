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

  easy2d::Sprite *SetSlimeOnPitch(bool PorE, float x, float y, Actor *slime);//ʷ��ķͼƬ����30%������x=180(��)��970(��)��y=500
  easy2d::Sprite *SetHPBar(float x, float y);//HPͼƬ����300%����͸����60%������x+105(��)��x-695(��)��y+200 
  easy2d::Text *SetHP(float x, float y, Actor *slime);//HP����ΪĬ�����壬�����С90���Ӵ֣�ÿ�г���380������
  std::string GetSlimeImg(const Pet *slime, bool PorE);

public:
  GraphSlime(Actor *left, Actor *right);
  ~GraphSlime();
  void Update(Actor *left, Actor *right);
};

#endif // GRAPH_H
