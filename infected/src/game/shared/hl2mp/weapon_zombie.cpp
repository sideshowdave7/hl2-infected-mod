//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		ZOMBIE - an old favorite
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hl2mp/weapon_zombie.h"
#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "vstdlib/random.h"
#include "npcevent.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "ai_basenpc.h"
#endif


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	ZOMBIE_RANGE	80.0f
#define	ZOMBIE_REFIRE	0.5f


//-----------------------------------------------------------------------------
// CWeaponZombie
//-----------------------------------------------------------------------------

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponZombie, DT_WeaponZombie )

BEGIN_NETWORK_TABLE( CWeaponZombie, DT_WeaponZombie )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponZombie )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_zombie, CWeaponZombie );
PRECACHE_WEAPON_REGISTER( weapon_zombie );

acttable_t	CWeaponZombie::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_MELEE,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_MELEE,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_MELEE,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_MELEE,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_MELEE,					false },
};

IMPLEMENT_ACTTABLE(CWeaponZombie);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponZombie::CWeaponZombie( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponZombie::GetDamageForActivity( Activity hitActivity )
{
	return 50.0f;
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponZombie::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = SharedRandomFloat( "zombiepax", 1.0f, 2.0f );
	punchAng.y = SharedRandomFloat( "zombiepay", -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}


#ifndef CLIENT_DLL
//-----------------------------------------------------------------------------
// Animation event handlers
//-----------------------------------------------------------------------------
void CWeaponZombie::HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	// Trace up or down based on where the enemy is...
	// But only if we're basically facing that direction
	Vector vecDirection;
	AngleVectors( GetAbsAngles(), &vecDirection );

	Vector vecEnd;
	VectorMA( pOperator->Weapon_ShootPosition(), 50, vecDirection, vecEnd );
	CBaseEntity *pHurt = pOperator->CheckTraceHullAttack( pOperator->Weapon_ShootPosition(), vecEnd, 
		Vector(-16,-16,-16), Vector(36,36,36), GetDamageForActivity( GetActivity() ), DMG_CLUB, 0.75 );
	
	// did I hit someone?
	if ( pHurt )
	{
		// play sound
		WeaponSound( MELEE_HIT );

		// Fake a trace impact, so the effects work out like a player's crowbaw
		trace_t traceHit;
		UTIL_TraceLine( pOperator->Weapon_ShootPosition(), pHurt->GetAbsOrigin(), MASK_SHOT_HULL, pOperator, COLLISION_GROUP_NONE, &traceHit );
		ImpactEffect( traceHit );
	}
	else
	{
		WeaponSound( MELEE_MISS );
	}
}


//-----------------------------------------------------------------------------
// Animation event
//-----------------------------------------------------------------------------
void CWeaponZombie::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( pEvent, pOperator );
		break;

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}

//-----------------------------------------------------------------------------
// Attempt to lead the target (needed because citizens can't hit manhacks with the zombie!)
//-----------------------------------------------------------------------------
ConVar sk_zombie_lead_time( "sk_zombie_lead_time", "0.9" );

int CWeaponZombie::WeaponMeleeAttack1Condition( float flDot, float flDist )
{
	// Attempt to lead the target (needed because citizens can't hit manhacks with the Zombie!)
	CAI_BaseNPC *pNPC	= GetOwner()->MyNPCPointer();
	CBaseEntity *pEnemy = pNPC->GetEnemy();
	if (!pEnemy)
		return COND_NONE;

	Vector vecVelocity;
	vecVelocity = pEnemy->GetSmoothedVelocity( );

	// Project where the enemy will be in a little while
	float dt = sk_zombie_lead_time.GetFloat();
	dt += SharedRandomFloat( "zombiemelee1", -0.3f, 0.2f );
	if ( dt < 0.0f )
		dt = 0.0f;

	Vector vecExtrapolatedPos;
	VectorMA( pEnemy->WorldSpaceCenter(), dt, vecVelocity, vecExtrapolatedPos );

	Vector vecDelta;
	VectorSubtract( vecExtrapolatedPos, pNPC->WorldSpaceCenter(), vecDelta );

	if ( fabs( vecDelta.z ) > 70 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	Vector vecForward = pNPC->BodyDirection2D( );
	vecDelta.z = 0.0f;
	float flExtrapolatedDist = Vector2DNormalize( vecDelta.AsVector2D() );
	if ((flDist > 64) && (flExtrapolatedDist > 64))
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	float flExtrapolatedDot = DotProduct2D( vecDelta.AsVector2D(), vecForward.AsVector2D() );
	if ((flDot < 0.7) && (flExtrapolatedDot < 0.7))
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_MELEE_ATTACK1;
}

#endif



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponZombie::Drop( const Vector &vecVelocity )
{
#ifndef CLIENT_DLL
	UTIL_Remove( this );
#endif
}

float CWeaponZombie::GetRange( void )
{
	return	ZOMBIE_RANGE;	
}

float CWeaponZombie::GetFireRate( void )
{
	return	ZOMBIE_REFIRE;	
}


