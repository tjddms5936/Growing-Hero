#include "pch.h"
#include "VilageRoom.h"

shared_ptr<UnitBase> VilageRoom::GetUnit(uint64 UnitSessionID)
{
    return m_mapUnits[UnitSessionID];
}
