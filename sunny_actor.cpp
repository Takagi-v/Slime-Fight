#include "actor.h"
#include "battlefield.h"

std::vector<Pet_T> SunnyActor::ChooseStartingPet()
{
  std::vector<Pet_T> startingPets = {Pet_T::P, Pet_T::G, Pet_T::R};
  return startingPets;
}

Action_T SunnyActor::ChooseAction()
{
  // 返回选取的动作
  if (this->petOnCourt.pet == Pet_T::G && this->petOnCourt.health < allPets.at(Pet_T::G).health / 2 && this->availPets.count(Pet_T::R) > 0)
    return Action_T::Change;
  return Action_T::Skill;
}

Pet_T SunnyActor::ChoosePet(bool active)
{
  // 返回即将交换上场的宠物
  if (active)
    return Pet_T::R;
  else
  {
    if (this->availPets.count(Pet_T::G) > 0)
      return Pet_T::G;
    else
      return Pet_T::R;
  }
}

Skill_T SunnyActor::ChooseSkill()
{
  Pet *player = &this->opp->petOnCourt;
  // 选择场上宠物使用的技能
  if (this->petOnCourt.pet == Pet_T::P)
  {
    if (player->health < allPets.at(player->pet).health / 3 && (player->attr["sleep"] > 0 || player->attr["paint"] > 0))
      return Skill_T::Slap;
    if (player->attr["sleep"] == 0)
      return Skill_T::Sing;
    if (player->attr["paint"] == 0 && player->attr["sleep"] > 0)
      return Skill_T::Paint;
    if (player->attr["paint"] > 0 && player->attr["sleep"] > 0)
      return Skill_T::Slap;
  }
  if (this->petOnCourt.pet == Pet_T::R)
  {
    if (player->health < allPets.at(player->pet).health / 5 && (allPets.at(player->pet).type == Type::Water || allPets.at(player->pet).type == Type::Fire))
      return Skill_T::Claw;
    else
    {
      if (this->field->attr["weather"] == Weather::Sunny)
      {
        return Skill_T::Flame;
      }
      else
      {
        return Skill_T::Sunny;
      }
    }
  }
  if (this->petOnCourt.pet == Pet_T::G)
  {
    if (this->petOnCourt.health > allPets.at(this->petOnCourt.pet).health / 2 || (player->health < this->petOnCourt.health && this->availPets.empty() == true))
    {
      if (player->type == Type::Water)
        return Skill_T::Leaf;
      else
        return Skill_T::Tackle;
    }
    else
      return Skill_T::Synthesis;
  }
}

std::string SunnyActor::GetName()
{
  return "Enemy"; //"Sunny Team";
}
