#include "actor.h"
#include "battlefield.h"

int main()
{
  HumanActor a;
  SunnyActor b; // 晴天队AI
  BattleField field(100);
  field.Reset();
  field.SetActor(&a, &b);
  field.Run();
  std::cin.get();
  std::cin.get();
  return 0;
}