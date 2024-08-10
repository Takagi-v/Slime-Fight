#include "battlefield.h"
#include "define.h"
#include "helper.h"
#include "graph.h"
#include <easy2d/easy2d.h>
#include <string>
#include <vector>

BattleField::BattleField(int maxRounds) : maxRounds_(maxRounds) {}

void BattleField::Reset()
{
  this->rounds_ = 0;
  this->actors_[0] = this->actors_[1] = nullptr;
  this->attr.clear();
}

void BattleField::Run()
{
  this->BeforeGame_(); // 已打印game starts，此时应出现图像

  if (easy2d::Game::init())
  {
    GraphSlime graph(this->actors_[0], this->actors_[1]);
    graph.Update(this->actors_[0], this->actors_[1]);

    bool ending = false;
    while (!ending)
    {
      LOG("--------------------------------------------------\n");
      this->StateCheck_();
      LOG("Round", ++this->rounds_);
      LOG('\n');
      std::vector<ActionInfo> actions = this->ChooseActionPhase_();

      while (true)
      {
        if (actions[0].u.pet != Pet_T::Back && actions[0].u.skill != Skill_T::Back)
          break;
        actions = this->ChooseActionPhase_();
      }
      if (actions[0].action == Action_T::Escape)
      {
        std::cout << "You escape";
        return;
      }

      this->PerformActionPhase_(actions);
      graph.Update(this->actors_[0], this->actors_[1]);
      this->HandleBeatenPetsPhase_();
      ending = this->ShowInformationPhase_();
      graph.Update(this->actors_[0], this->actors_[1]);
    }
  }
  easy2d::Game::destroy();

  /*bool ending = false;
  while (!ending)
  {
    LOG("--------------------------------------------------\n");
    this->StateCheck_();
    LOG("Round", ++this->rounds_); LOG('\n');
    std::vector<ActionInfo> actions = this->ChooseActionPhase_();
    while (true)
    {
      if (actions[0].u.pet != Pet_T::Back && actions[0].u.skill != Skill_T::Back)
        break;
      actions = this->ChooseActionPhase_();
    }
    if (actions[0].action == Action_T::Escape)
    {
      std::cout << "You escape";
      return;
    }
    this->PerformActionPhase_(actions);
    this->HandleBeatenPetsPhase_();
    ending = this->ShowInformationPhase_();
  }*/
}

void BattleField::SetActor(Actor *a, Actor *b)
{
  this->actors_[0] = a;
  this->actors_[1] = b;
  a->opp = b;
  b->opp = a;
  a->field = b->field = this;
}

void BattleField::BeforeGame_()
{
  // welcome messages
  LOG("Welcome to Battle of Slimes 2!\n");

  // 对手指针初始化
  this->actors_[1]->petOrder = this->actors_[1]->ChooseStartingPet();
  this->actors_[1]->petOnCourt = allPets.at(this->actors_[1]->petOrder[0]);
  for (int i = 1; i < this->actors_[1]->petOrder.size(); i++)
    this->actors_[1]->availPets[this->actors_[1]->petOrder[i]] = allPets.at(this->actors_[1]->petOrder[i]);
  this->actors_[1]->opp = this->actors_[0];
  std::cout << "Enemy has " << allPets.at(this->actors_[1]->petOrder[0]).petName << ", " << allPets.at(this->actors_[1]->petOrder[1]).petName << " and " << allPets.at(this->actors_[1]->petOrder[2]).petName << ", starting with " << this->actors_[1]->petOnCourt.petName << std::endl;

  // 玩家指针初始化
  this->actors_[0]->petOrder = this->actors_[0]->ChooseStartingPet();
  this->actors_[0]->petOnCourt = allPets.at(this->actors_[0]->petOrder[0]);
  for (int i = 1; i < this->actors_[0]->petOrder.size(); i++)
    this->actors_[0]->availPets[this->actors_[0]->petOrder[i]] = allPets.at(this->actors_[0]->petOrder[i]);
  this->actors_[0]->opp = this->actors_[1];
  std::cout << "You have " << allPets.at(this->actors_[0]->petOrder[0]).petName << ", " << allPets.at(this->actors_[0]->petOrder[1]).petName << " and " << allPets.at(this->actors_[0]->petOrder[2]).petName << ", starting with " << this->actors_[0]->petOnCourt.petName << std::endl;

  std::cout << "Your " << this->actors_[0]->petOnCourt.petName << ": HP " << this->actors_[0]->petOnCourt.health << " || Enemy's " << this->actors_[1]->petOnCourt.petName << ": HP " << this->actors_[1]->petOnCourt.health << "\n";

  this->attr["weather"] = Weather::Normal;
  this->attr["weatherDuration"] = 0;
  // game start messages
  LOG("Battle starts!\n");
  return;
}

void BattleField::StateCheck_()
{
  // 每回合开始前，检查天气、昏睡回合数等
  if (this->attr["weatherDuration"] > 0)
  {
    this->attr["weatherDuration"]--;
    if (this->attr["weatherDuration"] == 0)
    {
      std::cout << "Weather becomes normal" << std::endl;
      this->attr["weather"] = Weather::Normal;
    }
  }
  else
  {
    this->attr["weather"] = Weather::Normal;
  }

  // 检查宠物昏睡状态
  for (auto &actor : this->actors_)
  {
    Pet &pet = actor->petOnCourt;
    if (pet.attr["sleep"] > 0)
    {
      pet.attr["sleepCnt"]--;
      if (pet.attr["sleepCnt"] <= 0)
      {
        std::cout << Whose(actor->GetName()) << pet.petName << " wakes up\n";
        pet.attr["sleep"] = 0;
      }
    }
  }

  // 重置技能成功标志
  actors_[0]->skill_success = false;
  actors_[1]->skill_success = false;
  return;
}

std::vector<ActionInfo> BattleField::ChooseActionPhase_()
{
  // 向各Actor轮询采用的动作并返回
  std::vector<ActionInfo> actions;
  Action_T actionA = this->actors_[0]->ChooseAction();
  Action_T actionB = this->actors_[1]->ChooseAction();
  int priorityA = allActions.at(actionA).priority;
  int priorityB = allActions.at(actionB).priority;
  ActionInfo infoA = {actionA, this->actors_[0], priorityA};
  ActionInfo infoB = {actionB, this->actors_[1], priorityB};

  switch (actionA)
  {
  case Action_T::Escape:
    break;
  case Action_T::Change:
    infoA.u.pet = this->actors_[0]->ChoosePet(true);
    break;
  case Action_T::Skill:
    infoA.u.skill = this->actors_[0]->ChooseSkill();
    break;
  }
  switch (actionB)
  {
  case Action_T::Change:
    infoB.u.pet = this->actors_[1]->ChoosePet(true);
    break;
  case Action_T::Skill:
    infoB.u.skill = this->actors_[1]->ChooseSkill();
    break;
  case Action_T::Potion:
    infoB.u.potion = this->actors_[1]->ChoosePotion();
    break;
  }
  actions.push_back(infoA);
  actions.push_back(infoB);

  return actions;
}

void BattleField::PerformActionPhase_(std::vector<ActionInfo> &actions)
{
  // 执行Actor选取的动作（先执行较快的动作）
  if (actions[0].priority > actions[1].priority)
  {
    ExecuteAction(actions[0]);
    ExecuteAction(actions[1]);
  }
  else if (actions[0].priority == actions[1].priority)
  {
    if (actions[0].actor->petOnCourt.speed > actions[1].actor->petOnCourt.speed)
    {
      ExecuteAction(actions[0]);
      ExecuteAction(actions[1]);
    }
    else
    {
      ExecuteAction(actions[1]);
      ExecuteAction(actions[0]);
    }
  }
  else
  {
    ExecuteAction(actions[1]);
    ExecuteAction(actions[0]);
  }
}
void BattleField::ExecuteAction(const ActionInfo &actionInfo)
{
  if (actionInfo.actor->petOnCourt.health <= 0)
    return;
  switch (actionInfo.action)
  {
  case Action_T::Change:
  {
    Pet_T newPet = actionInfo.u.pet;
    Pet_T pastPet = actionInfo.actor->petOnCourt.pet;
    actionInfo.actor->availPets[pastPet] = actionInfo.actor->petOnCourt;
    actionInfo.actor->petOnCourt = actionInfo.actor->availPets.at(newPet);
    actionInfo.actor->availPets.erase(newPet);
    if (actionInfo.actor->GetName() == "You")
    {
      std::cout << actionInfo.actor->GetName() << " send " << actionInfo.actor->petOnCourt.petName << std::endl;
    }
    else
    {
      std::cout << actionInfo.actor->GetName() << " sends " << actionInfo.actor->petOnCourt.petName << std::endl;
    }
    break;
  }
  case Action_T::Skill:
  {
    if (actionInfo.actor->petOnCourt.attr["sleep"] > 0)
    {
      std::cout << Whose(actionInfo.actor->GetName()) << actionInfo.actor->petOnCourt.petName << " is sleeping!\n";
      break;
    }

    Skill skill = allSkills.at(actionInfo.u.skill);
    SkillCbReturn result = skill.cb(this, actionInfo.actor, &skill);
    actionInfo.actor->skill_success = result.success;
    std::cout << Whose(actionInfo.actor->GetName()) << actionInfo.actor->petOnCourt.petName << " uses " << skill.skillName << "! ";
    std::cout << result.prompt << std::endl;
    if (result.success)
      actionInfo.actor->opp->petOnCourt.health -= result.damage;
    if (actionInfo.actor->opp->petOnCourt.health < 0)
      actionInfo.actor->opp->petOnCourt.health = 0;
    if (result.damage > 0 && actionInfo.actor->opp->petOnCourt.attr["sleep"] > 0 && actionInfo.actor->opp->petOnCourt.health > 0)
    {
      std::cout << Whose(actionInfo.actor->opp->GetName()) << actionInfo.actor->opp->petOnCourt.petName << " wakes up\n";
      actionInfo.actor->opp->petOnCourt.attr["sleep"] = actionInfo.actor->opp->petOnCourt.attr["sleepCnt"] = 0;
    }
    break;
  }
    /*case Action_T::Potion:
    {
      Potion_T potionT = actionInfo.u.potion.potionT;
      Pet_T petT = actionInfo.u.potion.petT;
      Potion potion = allPotions.at(potionT);
      if (potionT == Potion_T::Revival)
      {
        actionInfo.actor->availPets[Pet_T::B] = allPets.at(Pet_T::B);
        actionInfo.actor->availPets[Pet_T::B].health = actionInfo.actor->availPets[Pet_T::B].health / 2;
        std::cout << actionInfo.actor->GetName() << " uses " << potion.potionName << " on " << "blue" << std::endl;
        actionInfo.actor->attr["revivalPotion"]--;
      }
      break;
    }*/
  }
}
void BattleField::HandleBeatenPetsPhase_()
{
  // 检查并处理本轮中被击败的宠物
  for (auto *actor : this->actors_)
  {
    if (actor->petOnCourt.health <= 0)
    {
      std::cout << Whose(actor->GetName()) << actor->petOnCourt.petName << " is beaten! \n";
      if (!actor->availPets.empty())
      {
        Pet_T newPet = actor->ChoosePet(false);
        actor->petOnCourt = actor->availPets.at(newPet);
        actor->availPets.erase(newPet);
        actor->petOnCourt.attr["sleep"] = actor->petOnCourt.attr["sleepCnt"] = 0;

        if (actor->GetName() == "You")
        {
          std::cout << actor->GetName() << " send " << actor->petOnCourt.petName << std::endl;
        }
        else
        {
          std::cout << actor->GetName() << " sends " << actor->petOnCourt.petName << std::endl;
        }
      }
      else
      {
        return;
      }
    }
  }
}

bool BattleField::ShowInformationPhase_()
{
  // 检查是否有一方没有可战斗的宠物
  for (auto *actor : this->actors_)
  {
    if (actor->availPets.empty() && actor->petOnCourt.health <= 0)
    {
      if (actor->GetName() == "You")
      {
        std::cout << "You lose!\n";
      }
      else
      {
        std::cout << "You win!\n";
      }
      return true; // 游戏分出胜负
    }
  }
  // 显示剩余HP等信息
  std::cout << "Your " << this->actors_[0]->petOnCourt.petName << ": HP " << this->actors_[0]->petOnCourt.health
            << " || Enemy's " << this->actors_[1]->petOnCourt.petName << ": HP " << this->actors_[1]->petOnCourt.health << "\n";

  return false; // 游戏未分出胜负
}

std::string BattleField::Whose(std::string name)
{
  return (name == "You" ? "Your " : name + "'s ");
}
