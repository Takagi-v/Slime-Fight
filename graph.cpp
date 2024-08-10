#include "graph.h"
#include "define.h"
#include <easy2d/easy2d.h>
#include <string>

GraphSlime::GraphSlime(Actor *left, Actor *right)
{
  // 初始化图像画面
  auto battle_scene = easy2d::gcnew easy2d::Scene();
  easy2d::Window::setTitle("Slime Fight");
  easy2d::Window::setSize(1375, 792);
  easy2d::SceneManager::enter(battle_scene);

  auto leftSlime = SetSlimeOnPitch(true, 180, 500, left);    // 玩家史莱姆
  auto rightSlime = SetSlimeOnPitch(false, 970, 500, right); // 敌方史莱姆
  auto bg = easy2d::gcnew easy2d::Sprite("Res\\Background.png");
  auto hp_photo = easy2d::gcnew easy2d::Sprite("Res\\Menu.png");
  auto left_hp_bar = SetHPBar(180 + 80, 500 + 200);
  auto right_hp_bar = SetHPBar(970 + 75, 500 + 200);
  auto left_hp_text = SetHP(180 + 80 + 20, 500 + 200 + 10, left);
  auto right_hp_text = SetHP(970 + 75 + 20, 500 + 200 + 10, right);
  battle_scene->addChild(bg);
  battle_scene->addChild(leftSlime);
  battle_scene->addChild(rightSlime);
  battle_scene->addChild(left_hp_bar);
  battle_scene->addChild(right_hp_bar);
  battle_scene->addChild(left_hp_text);
  battle_scene->addChild(right_hp_text);
}

void GraphSlime::Update(Actor *left, Actor *right)
{
  // 更新图像画面
  auto battle_scene = easy2d::gcnew easy2d::Scene();
  auto leftSlime = SetSlimeOnPitch(true, 180, 500, left);    // 玩家史莱姆
  auto rightSlime = SetSlimeOnPitch(false, 970, 500, right); // 敌方史莱姆
  auto bg = easy2d::gcnew easy2d::Sprite("Res\\Background.png");
  auto hp_photo = easy2d::gcnew easy2d::Sprite("Res\\Menu.png");
  auto left_hp_bar = SetHPBar(180 + 80, 500 + 200);
  auto right_hp_bar = SetHPBar(970 + 75, 500 + 200);
  auto left_hp_text = SetHP(180 + 80 + 20, 500 + 200 + 10, left);
  auto right_hp_text = SetHP(970 + 75 + 20, 500 + 200 + 10, right);
  battle_scene->addChild(bg);
  battle_scene->addChild(leftSlime);
  battle_scene->addChild(rightSlime);
  battle_scene->addChild(left_hp_bar);
  battle_scene->addChild(right_hp_bar);
  battle_scene->addChild(left_hp_text);
  battle_scene->addChild(right_hp_text);

  if (left->petOnCourt.attr["sleep"] > 0)
  {
    auto sleep = easy2d::gcnew easy2d::Sprite("Res\\IconSleep.png");
    sleep->setPos(180 + 10, 500 + 90);
    battle_scene->addChild(sleep);
  }
  if (left->petOnCourt.attr["paint"] > 0)
  {
    auto paint = easy2d::gcnew easy2d::Sprite("Res\\IconPaint.png");
    paint->setPos(180 + 10, 500 + 90 + 40);
    battle_scene->addChild(paint);
  }
  if (right->petOnCourt.attr["sleep"] > 0)
  {
    auto sleep = easy2d::gcnew easy2d::Sprite("Res\\IconSleep.png");
    sleep->setPos(970 + 10, 500 + 90);
    battle_scene->addChild(sleep);
  }
  if (right->petOnCourt.attr["paint"] > 0)
  {
    auto paint = easy2d::gcnew easy2d::Sprite("Res\\IconPaint.png");
    paint->setPos(970 + 10, 500 + 90 + 40);
    battle_scene->addChild(paint);
  }

  easy2d::Timer::add([=]()
                     {
                       easy2d::Game::quit(); // 将控制权退出图形窗口，返回控制台（图形窗口仍存在）
                     });

  easy2d::SceneManager::enter(battle_scene);
  easy2d::Game::start();
}

easy2d::Sprite *GraphSlime::SetSlimeOnPitch(bool PorE, float x, float y, Actor *slime)
{
  // 设置史莱姆图片，调用设置HP图片和状态图标的函数
  auto spirte = easy2d::gcnew easy2d::Sprite(GetSlimeImg(&slime->petOnCourt, PorE));
  spirte->setPos(x, y);
  float width = spirte->getWidth();
  float height = spirte->getHeight();
  spirte->setSize(width * 0.3, height * 0.3);
  return spirte;
}

easy2d::Sprite *GraphSlime::SetHPBar(float x, float y)
{
  // 设置HP图片
  auto hp_photo = easy2d::gcnew easy2d::Sprite("Res\\Menu.png");
  hp_photo->setPos(x, y);
  return hp_photo;
}

easy2d::Text *GraphSlime::SetHP(float x, float y, Actor *slime)
{
  int hp = slime->petOnCourt.health;
  int hp_max = allPets.at(slime->petOnCourt.pet).health;
  std::string s = std::to_string(hp) + "/" + std::to_string(hp_max);
  auto hp_text = easy2d::gcnew easy2d::Text(s);
  hp_text->setPos(x, y);
  easy2d::Font font = easy2d::Font();
  font.weight = easy2d::Font::Weight::Bold;
  font.italic = true;
  hp_text->setFont(font);
  return hp_text;
}

std::string GraphSlime::GetSlimeImg(const Pet *slime, bool PorE)
{
  // 取史莱姆图片的文件名
  if (slime->petName == "Red" && PorE)
    return "Res\\SlimeFire.png";
  else if (slime->petName == "Red" && !PorE)
    return "Res\\SlimeFire_r.png";
  else if (slime->petName == "Blue" && PorE)
    return "Res\\SlimeWater.png";
  else if (slime->petName == "Blue" && !PorE)
    return "Res\\SlimeWater_r.png";
  else if (slime->petName == "Green" && PorE)
    return "Res\\SlimeGrass.png";
  else if (slime->petName == "Green" && !PorE)
    return "Res\\SlimeGrass_r.png";
  else if (slime->petName == "Yellow" && PorE)
    return "Res\\SlimeThunder.png";
  else if (slime->petName == "Yellow" && !PorE)
    return "Res\\SlimeThunder_r.png";
  else if (slime->petName == "Pink" && PorE)
    return "Res\\SlimeNormal.png";
  else
    return "Res\\SlimeNormal_r.png";
}

GraphSlime::~GraphSlime()
{
  delete battle_scene;
}
