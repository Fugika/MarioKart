// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/KartItemData_Boost.h"

#include "Pawny.h"

bool UKartItemData_Boost::Activate(APawny* Kart) const
{
	if (Kart == nullptr)
	{
		return false;
	}
	
	Kart->ApplyBoost(BoostDuration);
	return true;
}
