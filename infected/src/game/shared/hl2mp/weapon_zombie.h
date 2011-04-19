//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2MP_WEAPON_ZOMBIE_H
#define HL2MP_WEAPON_ZOMBIE_H
#pragma once


#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "weapon_hl2mpbasebasebludgeon.h"


#ifdef CLIENT_DLL
#define CWeaponZombie C_WeaponZombie
#endif

//-----------------------------------------------------------------------------
// CWeaponZombie
//-----------------------------------------------------------------------------

class CWeaponZombie : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponZombie, CBaseHL2MPBludgeonWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponZombie();

	float		GetRange( void );
	float		GetFireRate( void );

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );
	void		SecondaryAttack( void )	{	return;	}

	void		Drop( const Vector &vecVelocity );


	// Animation event
#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	int WeaponMeleeAttack1Condition( float flDot, float flDist );
#endif

	CWeaponZombie( const CWeaponZombie & );

private:
		
};


#endif // HL2MP_WEAPON_ZOMBIE_H

