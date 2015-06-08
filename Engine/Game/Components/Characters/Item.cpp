#include "Item.h"
#include "Character.h"

using namespace Game::Components::Characters;


ItemStatBonus::ItemStatBonus(std::string stat, int bonus) : Stat(stat), Bonus(bonus) {}
void ItemStatBonus::ApplyBonus(Character& c, int Bonus)
{
	c.GetStats().GetStat(Stat).Add(Bonus);
}
void ItemStatBonus::OnEquip(Character& c)
{
	ApplyBonus(c, Bonus);
}
void ItemStatBonus::OnUnequip(Character& c)
{
	ApplyBonus(c, -Bonus);
}