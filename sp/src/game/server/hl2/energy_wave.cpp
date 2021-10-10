#include "cbase.h"
#include "energy_wave.h"
#include "gamerules.h"

LINK_ENTITY_TO_CLASS(energy_wave, CEnergyWave);

EXTERN_SEND_TABLE(DT_BaseEntity)

//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC(CEnergyWave)

END_DATADESC()

void CEnergyWave::Precache( void )
{
	SetClassname("energy_wave");
}

void CEnergyWave::Spawn(void)
{
	Precache();

	SetSolid(SOLID_BBOX);
	SetMoveType(MOVETYPE_NONE);
}


CEnergyWave* Create(CBaseEntity* pentOwner)
{
	CEnergyWave* pWave = (CEnergyWave*)CreateEntityByName("energy_wave");

	pWave->SetOwnerEntity(pentOwner);
	pWave->SetLocalAngles(pentOwner->GetLocalAngles());
	UTIL_SetOrigin(pWave, pentOwner->GetLocalOrigin());
	pWave->Spawn();

	return pWave;
}