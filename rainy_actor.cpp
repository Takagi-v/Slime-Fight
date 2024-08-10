#include "actor.h"
#include "battlefield.h"
#include "helper.h"

RainyActor::RainyActor()
{
  this->attr["revivalPotion"] = 1;
}

std::vector<Pet_T> RainyActor::ChooseStartingPet()
{
  std::vector<Pet_T> startingPets = {Pet_T::B, Pet_T::G, Pet_T::Y};
  ShuffleVector(startingPets);
  return startingPets;
}

Action_T RainyActor::ChooseAction()
{
  // 返回选取的动作
  if ((this->availPets.count(Pet_T::B) == 0 && this->petOnCourt.pet != Pet_T::B) && this->attr["revivalPotion"] > 0)
  {
    return Action_T::Potion;
  }
  if (this->petOnCourt.pet == Pet_T::G)
  {
    if (this->opp->petOnCourt.pet == Pet_T::R)
    {
      if (this->availPets.count(Pet_T::B) > 0)
        return Action_T::Change;
      else
        return Action_T::Skill;
    }
  }
  if (this->petOnCourt.pet == Pet_T::B)
  {
    if (this->field->attr["weather"] == Weather::Sunny)
      return Action_T::Skill;
    if ((this->field->attr["weather"] != Weather::Sunny && this->petOnCourt.pet == Pet_T::Y) || (this->field->attr["weather"] == Weather::Rainy && this->opp->petOnCourt.pet == Pet_T::G))
    {
      if (this->availPets.count(Pet_T::G) > 0)
        return Action_T::Change;
      else if (this->availPets.count(Pet_T::Y) > 0 && this->field->attr["weather"] == Weather::Rainy)
        return Action_T::Change;
      else
        return Action_T::Skill;
    }
    else
    {
      return Action_T::Skill;
    }
  }
  if (this->petOnCourt.pet == Pet_T::Y)
  {
    if (this->opp->petOnCourt.health < allPets.at(this->opp->petOnCourt.pet).health / 5)
      return Action_T::Skill;
    else
    {
      if (this->opp->petOnCourt.pet == Pet_T::R)
      {
        if (this->availPets.count(Pet_T::B) > 0)
          return Action_T::Change;
        else
          return Action_T::Skill;
      }
      else
      {
        if (this->field->attr["weather"] != Weather::Rainy && this->availPets.count(Pet_T::G) > 0)
          return Action_T::Change;
        else
          return Action_T::Skill;
      }
    }
  }
}

Pet_T RainyActor::ChoosePet(bool active)
{
  // 返回即将交换上场的宠物
  if (this->petOnCourt.pet == Pet_T::G)
  {
    if (!active)
    {
      if (this->availPets.count(Pet_T::B) > 0)
      {
        return Pet_T::B;
      }
      else
      {
        return Pet_T::Y;
      }
    }
    if (this->opp->petOnCourt.pet == Pet_T::R && this->availPets.count(Pet_T::B) > 0)
      return Pet_T::B;
  }
  if (this->petOnCourt.pet == Pet_T::B)
  {
    if (!active)
    {
      if (this->availPets.count(Pet_T::G) > 0)
      {
        return Pet_T::G;
      }
      else
      {
        return Pet_T::Y;
      }
    }
    if ((this->field->attr["weather"] != Weather::Sunny && this->petOnCourt.pet == Pet_T::Y) || (this->field->attr["weather"] == Weather::Rainy && this->opp->petOnCourt.pet == Pet_T::G))
    {
      if (this->availPets.count(Pet_T::G) > 0)
        return Pet_T::G;
      else if (this->availPets.count(Pet_T::Y) > 0 && this->field->attr["weather"] == Weather::Rainy)
        return Pet_T::Y;
    }
  }
  if (this->petOnCourt.pet == Pet_T::Y)
  {
    if (!active)
    {
      if (this->availPets.count(Pet_T::B) > 0)
      {
        return Pet_T::B;
      }
      else
      {
        return Pet_T::Y;
      }
    }
    if (this->opp->petOnCourt.pet == Pet_T::R && this->availPets.count(Pet_T::B) > 0)
    {
      return Pet_T::B;
    }
    else
    {
      if (this->field->attr["weather"] != Weather::Rainy && this->availPets.count(Pet_T::G) > 0)
        return Pet_T::G;
    }
  }
}

Skill_T RainyActor::ChooseSkill()
{
  // 选择场上宠物使用的技能
  if (this->petOnCourt.pet == Pet_T::G)
  {
    if (this->opp->petOnCourt.pet == Pet_T::R)
    {
      if (this->petOnCourt.health >= allPets.at(this->petOnCourt.pet).health * 2 / 3)
        return Skill_T::Tackle;
      else
        return Skill_T::Synthesis;
    }
    if (this->petOnCourt.pet == Pet_T::G)
    {
      if (this->petOnCourt.health >= allPets.at(this->petOnCourt.pet).health / 2)
        return Skill_T::Tackle;
      else
        return Skill_T::Synthesis;
    }
    else
    {
      if (this->petOnCourt.health >= allPets.at(this->petOnCourt.pet).health / 2 || this->opp->petOnCourt.health < allPets.at(this->opp->petOnCourt.pet).health * 4 / 5)
        return Skill_T::Leaf;
      else
        return Skill_T::Synthesis;
    }
  }
  if (this->petOnCourt.pet == Pet_T::B)
  {
    if (this->field->attr["weather"] == Weather::Sunny)
      return Skill_T::Rainy;
    if ((this->field->attr["weather"] != Weather::Sunny && this->petOnCourt.pet == Pet_T::Y) || (this->field->attr["weather"] == Weather::Rainy && this->opp->petOnCourt.pet == Pet_T::G))
    {
      if (this->availPets.count(Pet_T::G) == 0)
      {
        return Skill_T::Stream;
      }
    }
    if (this->field->attr["weather"] != Weather::Sunny && this->petOnCourt.pet == Pet_T::B)
      return Skill_T::Tackle;
    else
    {
      if (this->field->attr["weather"] == Weather::Rainy)
        return Skill_T::Stream;
      else
        return Skill_T::Rainy;
    }
  }
  if (this->petOnCourt.pet == Pet_T::Y)
  {
    if (this->opp->petOnCourt.health < allPets.at(this->opp->petOnCourt.pet).health / 5)
      return Skill_T::Quick;
    else
    {
      if (this->opp->petOnCourt.pet == Pet_T::R)
      {
        return Skill_T::Volt;
      }
      else
      {
        if (this->field->attr["weather"] == Weather::Rainy)
          return Skill_T::Thunder;
        else
        {
          if (this->opp->petOnCourt.pet == Pet_T::G || this->opp->petOnCourt.pet == Pet_T::Y)
            return Skill_T::Quick;
          else
            return Skill_T::Volt;
        }
      }
    }
  }
}

PotionInfo RainyActor::ChoosePotion()
{
  PotionInfo potionInfo;
  potionInfo.potionT = Potion_T::Revival;
  potionInfo.petT = Pet_T::B;
  return potionInfo;
}

std::string RainyActor::GetName()
{
  return "Enemy"; //"Rainy Team";
}
