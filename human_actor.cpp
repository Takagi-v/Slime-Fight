#include "actor.h"

#include <sstream>
#include <unordered_set>

HumanActor::HumanActor()
{
  this->GetName();
}

std::vector<Pet_T> HumanActor::ChooseStartingPet()
{
  // 从用户输入中得到上场的宠物
  std::vector<Pet_T> startingPets;
  Pet_T pet;
  int input;
  std::cout << "You can select three from five (1 for Green, 2 for Red, 3 for Blue, 4 for Yellow, 5 for Pink)." << std::endl;
  int count = 1;
  while (true)
  {
    if (count > 3)
      break;
    std::cout << "Select your ";
    if (count == 1)
      std::cout << "1st Slime: ";
    else if (count == 2)
      std::cout << "2nd Slime: ";
    else if (count == 3)
      std::cout << "3rd Slime: ";
    std::cin >> input;
    if (input < 1 || input > 5)
      continue;
    else if (input == 1)
      pet = Pet_T::G;
    else if (input == 2)
      pet = Pet_T::R;
    else if (input == 3)
      pet = Pet_T::B;
    else if (input == 4)
      pet = Pet_T::Y;
    else if (input == 5)
      pet = Pet_T::P;
    if (std::find(startingPets.begin(), startingPets.end(), pet) == startingPets.end())
    {
      startingPets.push_back(pet);
      count++;
    }
    else
      continue;
  }
  return startingPets;
}

Action_T HumanActor::ChooseAction()
{
  // 返回选取的动作
  int action;
  while (true)
  {
    std::cout << "Select your action (0 for escape, 1 for skill";
    if (this->availPets.size() >= 1)
      std::cout << ", 2 for change";
    std::cout << "):";
    std::cin >> action;
    if (action == 0 || action == 1 || (action == 2 && this->availPets.size() >= 1))
      break;
  }
  if (action == 0)
    return Action_T::Escape;
  if (action == 1)
    return Action_T::Skill;
  if (action == 2)
    return Action_T::Change;
}

Pet_T HumanActor::ChoosePet(bool active)
{
  // 返回即将交换上场的宠物
  // active 代表<主动交换宠物>or<场上宠物阵亡后被动交换宠物>
  int petName;
  while (true)
  {
    if (active)
      std::cout << "Select your next slime(0 for back, ";
    else
      std::cout << "Select your next slime( ";
    if (this->availPets.count(this->petOrder[0]) > 0)
    {
      std::cout << "1 for " << this->availPets[this->petOrder[0]].petName << "-HP" << this->availPets[this->petOrder[0]].health;
      if (this->availPets.count(this->petOrder[1]) > 0 || this->availPets.count(this->petOrder[2]) > 0)
        std::cout << ", ";
    }
    if (this->availPets.count(this->petOrder[1]) > 0)
    {
      std::cout << "2 for " << this->availPets[this->petOrder[1]].petName << "-HP" << this->availPets[this->petOrder[1]].health;
      if (this->availPets.count(this->petOrder[2]) > 0)
        std::cout << ", ";
    }
    if (this->availPets.count(this->petOrder[2]) > 0)
      std::cout << "3 for " << this->availPets[this->petOrder[2]].petName << "-HP" << this->availPets[this->petOrder[2]].health;
    std::cout << ")";
    std::cin >> petName;
    if (petName == 0 && active)
      return Pet_T::Back;
    if (petName == 1 && this->availPets.count(petOrder[0]) > 0)
      return petOrder[0];
    if (petName == 2 && this->availPets.count(petOrder[1]) > 0)
      return petOrder[1];
    if (petName == 3 && this->availPets.count(petOrder[2]) > 0)
      return petOrder[2];
  }
}

Skill_T HumanActor::ChooseSkill()
{
  // 选择场上宠物使用的技能
  int skill;
  while (true)
  {
    std::cout << "Select the skill (0 for back, ";
    std::cout << "1 for " << allSkills.at(this->petOnCourt.skills[0]).skillName << ", 2 for " << allSkills.at(this->petOnCourt.skills[1]).skillName << ", 3 for " << allSkills.at(this->petOnCourt.skills[2]).skillName << "):";
    std::cin >> skill;
    if (skill == 0 || skill == 1 || skill == 2 || skill == 3)
      break;
  }
  if (skill == 0)
    return Skill_T::Back;
  if (skill == 1)
    return this->petOnCourt.skills[0];
  if (skill == 2)
    return this->petOnCourt.skills[1];
  if (skill == 3)
    return this->petOnCourt.skills[2];
}

std::string HumanActor::GetName()
{
  return "You";
}
