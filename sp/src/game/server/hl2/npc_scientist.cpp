//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements the hl1 scientist
//
// $NoKeywords: $
// Recreated by anthony python
//=============================================================================//

#include "cbase.h"
#include "game.h"
#include "AI_Default.h"
#include "AI_Schedule.h"
#include "AI_Hull.h"
#include "AI_Navigator.h"
#include "AI_Motor.h"
#include "ai_squad.h"
#include "ai_route.h"
#include "ai_pathfinder.h"
#include "npc_scientist.h"
#include "npcevent.h"
#include "soundent.h"
#include "activitylist.h"
#include "npc_headcrab.h"
#include "player.h"
#include "gamerules.h"		// For g_pGameRules
#include "ammodef.h"
#include "entitylist.h"
#include "shake.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "movevars_shared.h"
#include "hl2_shareddefs.h"
#include "soundemittersystem/isoundemittersystembase.h"
#include "AI_Hint.h"
#include "AI_Senses.h"
#include "ai_interactions.h"


ConVar sk_scientist_health("sk_scientist_health", "20");
ConVar sk_scientist_heal("sk_scientist_heal", "25");

#define		NUM_SCIENTIST_HEADS		4 // four heads available for scientist model
enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_LUTHER = 2, HEAD_SLICK = 3 };

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		SCIENTIST_AE_HEAL		( 1 )
#define		SCIENTIST_AE_NEEDLEON	( 2 )
#define		SCIENTIST_AE_NEEDLEOFF	( 3 )

//=======================================================
// Scientist
//=======================================================

#ifdef HL1_NPC
LINK_ENTITY_TO_CLASS(monster_scientist, CNPC_Scientist);
#endif
int ACT_EXCITED;



//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		SCIENTIST_AE_HEAL		( 1 )
#define		SCIENTIST_AE_NEEDLEON	( 2 )
#define		SCIENTIST_AE_NEEDLEOFF	( 3 )


BEGIN_DATADESC(CNPC_Scientist)

	DEFINE_FIELD( m_painTime, FIELD_TIME),
	DEFINE_FIELD( m_healTime, FIELD_TIME),
	DEFINE_FIELD( m_fearTime, FIELD_TIME),
END_DATADESC()


void CNPC_Scientist::Precache(void)
{
	PrecacheModel("models/scientist.mdl");
	PrecacheScriptSound("npc_Scientist.Pain");


	TalkInit();
	BaseClass::Precache();
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================
/*Task_t	tlFollow[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_SCIENTIST_CANT_FOLLOW },	// If you fail, bail out of follow
	{ TASK_MOVE_TO_TARGET_RANGE,(float)128		},	// Move within 128 of target ent (client)
//	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
};*/

/*Schedule_t	slFollow[] =
{
	{
		tlFollow,
		ARRAYSIZE(tlFollow),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER,
		"Follow"
	},
};*/

//-----------------------------------------------------------------------------
// Scientist Conditions
//-----------------------------------------------------------------------------
/*enum
{
	COND_NEW_ENEMY = LAST_SHARED_CONDITION + 1,
	COND_LIGHT_DAMAGE,
	COND_HEAVY_DAMAGE,
	COND_HEAR_SOUND,
	SOUND_COMBAT,
	SOUND_DANGER,
};*/

#if 0

/*Task_t	tlFollowScared[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_TARGET_CHASE },// If you fail, follow normally
	{ TASK_MOVE_TO_TARGET_RANGE_SCARED,(float)128		},	// Move within 128 of target ent (client)
//	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE_SCARED },
};

/*Schedule_t	slFollowScared[] =
{
	{
		tlFollowScared,
		ARRAYSIZE(tlFollowScared),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE,
		bits_SOUND_DANGER,
		"FollowScared"
	},
};*/

/*Task_t	tlFaceTargetScared[] =
{
	{ TASK_FACE_TARGET,			(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_CROUCHIDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_SCIENTIST_TARGET_CHASE_SCARED },
};*/

/*Schedule_t	slFaceTargetScared[] =
{
	{
		tlFaceTargetScared,
		ARRAYSIZE(tlFaceTargetScared),
		bits_COND_HEAR_SOUND |
		bits_COND_NEW_ENEMY,
		bits_SOUND_DANGER,
		"FaceTargetScared"
	},
};*/

/*Task_t	tlStopFollowing[] =
{
	{ TASK_CANT_FOLLOW,		(float)0 },
}; */

/*Schedule_t	slStopFollowing[] =
{
	{
		tlStopFollowing,
		ARRAYSIZE(tlStopFollowing),
		0,
		0,
		"StopFollowing"
	},
};*/




/*Schedule_t	slHeal[] =
{
	{
		tlHeal,
		ARRAYSIZE(tlHeal),
		0,	// Don't interrupt or he'll end up running around with a needle all the time
		0,
		"Heal"
	},
};*/


/*Task_t	tlFaceTarget[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_FACE_TARGET,			(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE },
}; */

/*Schedule_t	slFaceTarget[] =
{
	{
		tlFaceTarget,
		ARRAYSIZE(tlFaceTarget),
		bits_COND_CLIENT_PUSH |
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER,
		"FaceTarget"
	},
};*/


/*Task_t	tlSciPanic[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_SCREAM,				(float)0		},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,		(float)ACT_EXCITED	},	// This is really fear-stricken excitement
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE	},
}; */

/*Schedule_t	slSciPanic[] =
{
	{
		tlSciPanic,
		ARRAYSIZE(tlSciPanic),
		0,
		0,
		"SciPanic"
	},
};*/


/*Task_t	tlIdleSciStand[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		}, // repick IDLESTAND every two seconds.
	{ TASK_TLK_HEADRESET,		(float)0		}, // reset head position
}; */

/*Schedule_t	slIdleSciStand[] =
{
	{
		tlIdleSciStand,
		ARRAYSIZE(tlIdleSciStand),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND |
		bits_COND_SMELL |
		bits_COND_CLIENT_PUSH |
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT |// sound flags
		//bits_SOUND_PLAYER		|
		//bits_SOUND_WORLD		|
		bits_SOUND_DANGER |
		bits_SOUND_MEAT |// scents
		bits_SOUND_CARCASS |
		bits_SOUND_GARBAGE,
		"IdleSciStand"

	},
};*/


/*Task_t	tlScientistCover[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_SCIENTIST_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH_SCARED,			(float)0					},
	{ TASK_TURN_LEFT,				(float)179					},
	{ TASK_SET_SCHEDULE,			(float)SCHED_SCIENTIST_HIDE			},
}; */

/*Schedule_t	slScientistCover[] =
{
	{
		tlScientistCover,
		ARRAYSIZE(tlScientistCover),
		bits_COND_NEW_ENEMY,
		0,
		"ScientistCover"
	},
};*/



Task_t	tlScientistHide[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_SCIENTIST_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_PLAY_SEQUENCE,			(float)ACT_CROUCH			},
	{ TASK_SET_ACTIVITY,			(float)ACT_CROUCHIDLE		},	// FIXME: This looks lame
	{ TASK_WAIT_RANDOM,				(float)10.0					},
};

/*Schedule_t	slScientistHide[] =
{
	{
		tlScientistHide,
		ARRAYSIZE(tlScientistHide),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
		bits_COND_SEE_ENEMY |
		bits_COND_SEE_HATE |
		bits_COND_SEE_FEAR |
		bits_COND_SEE_DISLIKE,
		bits_SOUND_DANGER,
		"ScientistHide"
	},
};*/


Task_t	tlScientistStartle[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_SCIENTIST_PANIC },		// If you fail, just panic!
	{ TASK_RANDOM_SCREAM,			(float)0.3 },				// Scream 30% of the time
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,			(float)ACT_CROUCH			},
	{ TASK_RANDOM_SCREAM,			(float)0.1 },				// Scream again 10% of the time
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,			(float)ACT_CROUCHIDLE		},
	{ TASK_WAIT_RANDOM,				(float)1.0					},
};

/*Schedule_t	slScientistStartle[] =
{
	{
		tlScientistStartle,
		ARRAYSIZE(tlScientistStartle),
		bits_COND_NEW_ENEMY |
		bits_COND_SEE_ENEMY |
		bits_COND_SEE_HATE |
		bits_COND_SEE_FEAR |
		bits_COND_SEE_DISLIKE,
		0,
		"ScientistStartle"
	},
};*/



Task_t	tlFear[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_SAY_FEAR,				(float)0					},
	//	{ TASK_PLAY_SEQUENCE,			(float)ACT_FEAR_DISPLAY		},
};

/*Schedule_t	slFear[] =
{
	{
		tlFear,
		ARRAYSIZE(tlFear),
		bits_COND_NEW_ENEMY,
		0,
		"Fear"
	},
};*/


/*DEFINE_CUSTOM_SCHEDULES(CScientist)
{
	slFollow,
		slFaceTarget,
		slIdleSciStand,
		slFear,
		slScientistCover,
		slScientistHide,
		slScientistStartle,
		slHeal,
		slStopFollowing,
		slSciPanic,
		slFollowScared,
		slFaceTargetScared,
};


IMPLEMENT_CUSTOM_SCHEDULES(CScientist, CTalkMonster);
*/
#endif
void CNPC_Scientist::DeclineFollowing(void)
{
	GetExpresser()->BlockSpeechUntil(gpGlobals->curtime + 10);
	//Talk(10);
	SetSpeechTarget(GetEnemy());
	//m_hTalkTarget = m_hEnemy;
	//Speak("SC_POK");
	PlaySentence("SC_POK", 2, VOL_NORM, SNDLVL_60dB);
}


void CNPC_Scientist::Scream(void)
{
	if (IsOkToSpeak())
	{
		GetExpresser()->BlockSpeechUntil(gpGlobals->curtime + 10);
		//Talk(10);
		SetSpeechTarget(GetEnemy());
		//m_hTalkTarget = m_hEnemy;
		//Speak("SC_SCREAM");
		PlaySentence("SC_SCREAM", random->RandomFloat(3, 6), VOL_NORM, SNDLVL_90dB);
	}
}


Activity CNPC_Scientist::GetStoppedActivity(void)
{
	if (GetEnemy() != NULL)
		return (Activity)ACT_EXCITED;
	return BaseClass::GetStoppedActivity();
}


void CNPC_Scientist::StartTask(const Task_t* pTask)
{
	switch (pTask->iTask)
	{
	case TASK_SAY_HEAL:
		//		if ( FOkToSpeak() )
		GetExpresser()->BlockSpeechUntil(gpGlobals->curtime + 2);
		//Talk(2);
		SetSpeechTarget(GetTarget());
		//m_hTalkTarget = m_hTargetEnt;
		//Speak("SC_HEAL");
		
		PlaySentence("SC_HEAL", 2, VOL_NORM, SNDLVL_60dB);

		TaskComplete();
		break;

	case TASK_SCREAM:
		Scream();
		TaskComplete();
		break;

	case TASK_RANDOM_SCREAM:
		if (random->RandomFloat(0, 1) < pTask->flTaskData)
			Scream();
		TaskComplete();
		break;

	case TASK_SAY_FEAR:
		if (IsOkToSpeak())
		{
			GetExpresser()->BlockSpeechUntil(gpGlobals->curtime + 2);
			//Talk(2);
			SetSpeechTarget(GetTarget());
			//m_hTalkTarget = m_hEnemy;

			if (GetEnemy() && GetEnemy()->IsPlayer())
				//Speak("SC_PLFEAR");
				PlaySentence("SC_PLFEAR", 5, VOL_NORM, SNDLVL_90dB);
			else
				//Speak("SC_FEAR");
				PlaySentence("SC_FEAR", 5, VOL_NORM, SNDLVL_90dB);
		}
		TaskComplete();
		break;

	case TASK_HEAL:
		SetIdealActivity(ACT_MELEE_ATTACK1);
		///m_IdealActivity = ACT_MELEE_ATTACK1;
		break;

	case TASK_RUN_PATH_SCARED:
		GetNavigator()->SetMovementActivity(ACT_RUN_SCARED);
		break;

	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
	{
		if (!GetTarget())
		{
			TaskFail(FAIL_NO_TARGET);
		}
		if ((GetTarget()->GetAbsOrigin() - GetAbsOrigin()).Length() < 1)
			TaskComplete();
		else
		{
			GetNavigator()->SetMovementActivity(ACT_WALK_SCARED);	
		}
	}

	case TASK_CANT_FOLLOW:
	{
		if (IsOkToSpeak())
		{
			GetExpresser()->BlockSpeechUntil(gpGlobals->curtime + 2);

			//Speak("SC_CANTFOLLOW");
			PlaySentence("SC_CANTFOLLOW", 5, VOL_NORM, SNDLVL_60dB);
			
		}
		
		break;
	}
	break;

	default:
		BaseClass::StartTask(pTask);
		break;
	}
}

Activity CNPC_Scientist::NPC_TranslateActivity(Activity newActivity)
{
	if (GetFollowTarget())
	{
		if (GetEnemy())
		{
			CBaseEntity* pEnemy = GetEnemy();

			int relationship = D_NU;

			// Nothing scary, just me and the player
			if (pEnemy != NULL)
				relationship = IRelationType(pEnemy);

			

			// UNDONE: Model fear properly, fix R_FR and add multiple levels of fear
			if (relationship == D_FR && relationship == D_HT)
			{
				switch (newActivity)
				{
				case ACT_WALK:
					return ACT_WALK_SCARED;
					break;
				case ACT_RUN:
					return ACT_RUN_SCARED;
					break;
				}
				
			}

		}
	}
	

	return BaseClass::NPC_TranslateActivity(newActivity);
}

void CNPC_Scientist::RunTask(const Task_t* pTask)
{
	switch (pTask->iTask)
	{
	case TASK_RUN_PATH_SCARED:
		if (!IsMoving())
			TaskComplete();
		if (random->RandomInt(0, 31) < 8)
			Scream();
		break;

	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
	{
		if (random->RandomInt(0, 63) < 8)
			Scream();

		if (GetTarget() == NULL)
		{
			TaskFail(FAIL_NO_TARGET);
		}
		else
		{
			float distance;

			distance = (GetNavigator()->GetPath()->ActualGoalPosition() - GetAbsOrigin()).Length2D();
			// Re-evaluate when you think your finished, or the target has moved too far
			if ((distance < pTask->flTaskData) || (GetNavigator()->GetPath()->ActualGoalPosition() - GetTarget()->GetAbsOrigin()).Length() > pTask->flTaskData * 0.5)
			{
			
				distance = (GetTarget()->GetAbsOrigin() - GetAbsOrigin()).Length2D();
				GetNavigator()->SetGoal(GOALTYPE_TARGETENT);
			}

			// Set the appropriate activity based on an overlapping range
			// overlap the range to prevent oscillation
			if (distance < pTask->flTaskData)
			{
				TaskComplete();
				GetNavigator()->GetPath()->Clear();		// Stop moving
			}
			else if (distance < 190 && GetNavigator()->GetMovementActivity() != ACT_WALK_SCARED)
				GetNavigator()->SetMovementActivity(ACT_WALK_SCARED);
			else if (distance >= 270 && GetNavigator()->GetMovementActivity() != ACT_RUN_SCARED)
				GetNavigator()->SetMovementActivity(ACT_RUN_SCARED);
		}
	}
	break;

	case TASK_HEAL:
		float distance;
		if (GetTarget() != NULL)
		{
			distance = (GetTarget()->GetAbsOrigin() - GetAbsOrigin()).Length2D();
			if (IsSequenceFinished())
			{
				TaskComplete();
			}
			else
			{
				if (distance > 90)
					TaskComplete();
				GetMotor()->SetIdealYaw(UTIL_VecToYaw(GetTarget()->GetAbsOrigin() - GetAbsOrigin()));
				//ChangeYaw(pev->yaw_speed);
			}
		}
		
		break;
	default:
		BaseClass::RunTask(pTask);
		break;
	}
}

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
Class_T	CNPC_Scientist::Classify(void)
{
	return	CLASS_PLAYER_ALLY;//CLASS_HUMAN_PASSIVE;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
float CNPC_Scientist::MaxYawSpeed(void)
{
	float flYS = 0;

	switch (GetActivity())
	{
	case	ACT_IDLE:			flYS = 120;	break;
	case	ACT_WALK:			flYS = 180;	break;
	case	ACT_RUN:			flYS = 150;	break;
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		flYS = 120;
		break;

	default:
		flYS = 90;
		break;
	}

	return flYS;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CNPC_Scientist::HandleAnimEvent(animevent_t* pEvent)
{
	switch (pEvent->event)
	{
	case SCIENTIST_AE_HEAL:		// Heal my target (if within range)
		Heal();
		break;
	case SCIENTIST_AE_NEEDLEON:
	{
		int oldBody = m_nBody;
		m_nBody = (oldBody % NUM_SCIENTIST_HEADS) + NUM_SCIENTIST_HEADS * 1;
		SetBodygroup(2, 1);
	}
	break;
	case SCIENTIST_AE_NEEDLEOFF:
	{
		int oldBody = m_nBody;
		m_nBody = (oldBody % NUM_SCIENTIST_HEADS) + NUM_SCIENTIST_HEADS * 0;
		SetBodygroup(2, 0);
	}
	break;

	default:
		BaseClass::HandleAnimEvent(pEvent);
	}
}

/*void CNPC_Scientist::FollowerUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	// Don't allow use during a scripted_sentence
	if (m_useTime > gpGlobals->curtime)
		return;

	if (pCaller != NULL && pCaller->IsPlayer())
	{
		if (!m_FollowBehavior.GetFollowTarget() && IsInterruptable())
		{
#if TOML_TODO
			LimitFollowers(pCaller, 1);
#endif

			if (m_afMemory & bits_MEMORY_PROVOKED)
				Msg("I'm not following you, you evil person!\n");
			else
			{
				
				StartFollowing(pCaller);
				Speak(TLK_STARTFOLLOW);
				SetSpeechTarget(GetTarget());
				ClearCondition(COND_PLAYER_PUSHING);
			}
		}
		else
		{
			StopFollowing();
			Speak(TLK_STOPFOLLOW);
		}
	}
}
*/
//=========================================================
// Spawn
//=========================================================
void CNPC_Scientist::Spawn(void)
{
	Precache();
	m_nBody = -1;
	SetModel("models/scientist.mdl");
	SetHullType(HULL_HUMAN);
	SetHullSizeNormal();
	//UTIL_SetSize(pev, HUMAN_HULL_MIN, HUMAN_HULL_MAX);

	SetSolid(SOLID_BBOX);
	SetMoveType( MOVETYPE_STEP);
	m_bloodColor = BLOOD_COLOR_RED;
	AddSolidFlags(FSOLID_NOT_STANDABLE);

	m_iHealth = sk_scientist_health.GetFloat();

	m_flFieldOfView = VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so scientists will notice player and say hello
	m_NPCState = NPC_STATE_NONE;

	//	m_flDistTooFar		= 256.0;

	CapabilitiesClear();
	CapabilitiesAdd(bits_CAP_MOVE_GROUND | bits_CAP_ANIMATEDFACE | bits_CAP_TURN_HEAD | bits_CAP_OPEN_DOORS | bits_CAP_AUTO_DOORS | bits_CAP_USE
	| bits_CAP_MOVE_CLIMB | bits_CAP_MOVE_JUMP);

	// White hands
	m_nSkin = 0;

	if (m_nBody == -1)
	{// -1 chooses a random head
		m_nBody = random->RandomInt(0, NUM_SCIENTIST_HEADS - 1);// pick a head, any head

		SetBodygroup(1, m_nBody);
	}

	// Luther is black, make his hands black
	if (m_nBody == HEAD_LUTHER)
		m_nSkin = 1;

	NPCInit();
	SetUse(&CNPC_Scientist::FollowerUse);
}

// Init talk data
void CNPC_Scientist::TalkInit()
{

	BaseClass::TalkInit();

	// scientist will try to talk to friends in this order:

	m_szFriends[0] = "monster_scientist";
	m_szFriends[1] = "monster_sitting_scientist";
	m_szFriends[2] = "monster_barney";
	m_szFriends[3] = "npc_scientist";
	m_szFriends[4] = "npc_sitting_scientist";
	m_szFriends[5] = "npc_barney";
	m_szFriends[6] = "npc_citizen";
	m_szFriends[7] = "npc_crow";

	// scientists speach group names (group names are in sentences.txt)
#if 0
	/m_szGrp[TLK_ANSWER] = "SC_ANSWER";
	m_szGrp[TLK_QUESTION] = "SC_QUESTION";
	m_szGrp[TLK_IDLE] = "SC_IDLE";
	m_szGrp[TLK_STARE] = "SC_STARE";
	m_szGrp[TLK_USE] = "SC_OK";
	m_szGrp[TLK_UNUSE] = "SC_WAIT";
	m_szGrp[TLK_STOP] = "SC_STOP";
	m_szGrp[TLK_NOSHOOT] = "SC_SCARED";
	m_szGrp[TLK_HELLO] = "SC_HELLO";

	m_szGrp[TLK_PLHURT1] = "!SC_CUREA";
	m_szGrp[TLK_PLHURT2] = "!SC_CUREB";
	m_szGrp[TLK_PLHURT3] = "!SC_CUREC";

	m_szGrp[TLK_PHELLO] = "SC_PHELLO";
	m_szGrp[TLK_PIDLE] = "SC_PIDLE";
	m_szGrp[TLK_PQUESTION] = "SC_PQUEST";
	m_szGrp[TLK_SMELL] = "SC_SMELL";

	m_szGrp[TLK_WOUND] = "SC_WOUND";
	m_szGrp[TLK_MORTAL] = "SC_MORTAL";
#endif
	// get voice for head
	switch (m_nBody % 3)
	{
	default:
	case HEAD_GLASSES:	GetExpresser()->SetVoicePitch (105); break;	//glasses
	case HEAD_EINSTEIN: GetExpresser()->SetVoicePitch(100); break;	//einstein
	case HEAD_LUTHER:	GetExpresser()->SetVoicePitch(95);  break;	//luther
	case HEAD_SLICK:	GetExpresser()->SetVoicePitch(100);  break;//slick
	}
}

int CNPC_Scientist::OnTakeDamage_Alive(const CTakeDamageInfo& inputInfo)
{
	CBaseEntity* pevInflictor = inputInfo.GetInflictor();

	if (pevInflictor && pevInflictor->IsPlayer())
	{
		Remember(bits_MEMORY_PROVOKED);
		StopFollowing();
	}

	// make sure friends talk about it if player hurts scientist...
	return BaseClass::OnTakeDamage_Alive(inputInfo);
}


void CNPC_Scientist::ModifyOrAppendCriteria(AI_CriteriaSet& criteriaSet)
{
	bool IsitBeforeDisater;
	if (m_spawnflags & SF_BEFOREDISATER)
	{
		IsitBeforeDisater = true;
	}
	else
	{
		IsitBeforeDisater = false;
	}

	criteriaSet.AppendCriteria("disaster", IsitBeforeDisater ? "[disaster::pre]" : "[disaster::post]");

	BaseClass::ModifyOrAppendCriteria(criteriaSet);

}

//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
//=========================================================
int CNPC_Scientist::GetSoundInterests(void)
{
	return	SOUND_WORLD |
		SOUND_COMBAT |
		SOUND_DANGER |
		SOUND_PLAYER;
}

//=========================================================
// PainSound
//=========================================================
void CNPC_Scientist::PainSound(const CTakeDamageInfo& inputInfo)
{
	if (gpGlobals->curtime < m_painTime)
		return;

	m_painTime = gpGlobals->curtime + random->RandomFloat(0.5, 0.75);

	CSoundParameters param;
	CPASAttenuationFilter thisEntity(this);
	if (GetParametersForSound("npc_Scientist.Pain", param, NULL))
	{
		EmitSound_t snd(param);
		param.pitch = GetExpresser()->GetVoicePitch();
		EmitSound(thisEntity,entindex(), snd);
	}

	// not needed anymore but this use to be limited by 5 sounds for a pain sound, no more!
	/*switch (random->RandomInt(0, 4))
	{
	case 0: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain1.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 1: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain2.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 2: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain3.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 3: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain4.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 4: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain5.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	}*/
}

//=========================================================
// DeathSound 
//=========================================================
void CNPC_Scientist::DeathSound(const CTakeDamageInfo& inputInfo)
{
	PainSound(inputInfo);
}


void CNPC_Scientist::Event_Killed(const CTakeDamageInfo& inputInfo)
{
	SetUse(NULL);
	BaseClass::Event_Killed(inputInfo);
}

int CNPC_Scientist::TranslateSchedule(int scheduleType)
{
	switch (scheduleType)
	{
		// Hook these to make a looping schedule
	case SCHED_TARGET_FACE:
	{
		int psched;
		// call base class default so that scientist will talk
		// when 'used' 
		psched = BaseClass::TranslateSchedule(scheduleType);

		if (psched == SCHED_IDLE_STAND)
			return SCHED_TARGET_FACE;	// override this for different target face behavior
		else
			return psched;
	}
	break;

	case SCHED_TARGET_CHASE:
		return SCHED_SCIENTIST_FOLLOWTHETARGET;
		break;

	/*case SCHED_CANT_FOLLOW:
		return slStopFollowing;

	case SCHED_PANIC:
		return slSciPanic;

	case SCHED_TARGET_CHASE_SCARED:
		return slFollowScared;

	case SCHED_TARGET_FACE_SCARED:
		return slFaceTargetScared;*/

	case SCHED_IDLE_STAND:
	{
		int psched;
		// call base class default so that scientist will talk
		// when standing during idle
		psched = BaseClass::TranslateSchedule(scheduleType);

		if (psched == SCHED_IDLE_STAND)
			return SCHED_SCIENTIST_IDLE_STAND;
		else
			return psched;
	}
	break;
		/*case SCHED_HIDE:
		return slScientistHide;

	case SCHED_STARTLE:
		return slScientistStartle;

	case SCHED_FEAR:
		return slFear;*/
	}

	return BaseClass::TranslateSchedule(scheduleType);
}

float CNPC_Scientist::TargetDistance(void)
{
	CBaseEntity* pTargetWeareFollowing = GetFollowTarget();
	// If we lose the player, or he dies, return a really large distance
	if (pTargetWeareFollowing == NULL || !pTargetWeareFollowing->IsAlive())
		return 1e6;

	return (pTargetWeareFollowing->GetAbsOrigin() - GetAbsOrigin()).Length();
}

//-----------------------------------------------------------------------------
// Purpose:  This is a generic function (to be implemented by sub-classes) to
//			 handle specific interactions between different types of characters
//			 (For example the barnacle grabbing an NPC)
// Input  :  Constant for the type of interaction
// Output :	 true  - if sub-class has a response for the interaction
//			 false - if sub-class has no response
//-----------------------------------------------------------------------------
bool CNPC_Scientist::HandleInteraction(int interactionType, void* data, CBaseCombatCharacter* sourceEnt)
{
	if (interactionType == g_interactionBarnacleVictimDangle)
	{
		// Die instantly
		return false;
	}
	else if (interactionType == g_interactionVortigauntKick
		|| (interactionType == g_interactionBullsquidThrow)
		)
	{
		SetIdealState(NPC_STATE_PRONE);

		/*if (HasHeadroom())
		{
			MoveOrigin(Vector(0, 0, 1));
		}*/

		Vector vHitDir = GetLocalOrigin() - sourceEnt->GetLocalOrigin();
		VectorNormalize(vHitDir);

		CTakeDamageInfo info(sourceEnt, sourceEnt, m_iHealth + 1, DMG_CLUB);
		CalculateMeleeDamageForce(&info, vHitDir, GetAbsOrigin());

		TakeDamage(info);

		return true;
	}

	return BaseClass::HandleInteraction(interactionType, data, sourceEnt);
}

int CNPC_Scientist::SelectSchedule(void)
{
	// so we don't keep calling through the EHANDLE stuff
		CBaseEntity * pEnemy = GetEnemy();

		if (GetFollowTarget())
		{
			
			int relationship = D_NU;

			// Nothing scary, just me and the player
			if (pEnemy != NULL)
				relationship = IRelationType(pEnemy);

			// UNDONE: Model fear properly, fix R_FR and add multiple levels of fear
			if (relationship != D_DL && relationship != D_HT)
			{
				// If I'm already close enough to my target
				if (TargetDistance() <= 128)
				{
					if (CanHeal())	// Heal opportunistically
					{
						SetTarget(GetFollowTarget());
						return SCHED_SCIENTIST_HEAL;
					}

				}
				
			}
		}
		else if(HasCondition(COND_PLAYER_PUSHING))
		{
			return SCHED_MOVE_AWAY;
		}


		if (BehaviorSelectSchedule())
			return BaseClass::SelectSchedule();

		if (HasCondition(COND_HEAR_DANGER) && m_NPCState != NPC_STATE_PRONE)
		{
			CSound* pSound;
			pSound = GetBestSound();

			ASSERT(pSound != NULL);
			if (pSound && (pSound->IsSoundType(SOUND_DANGER)))
				return SCHED_TAKE_COVER_FROM_BEST_SOUND;

		}
	switch (m_NPCState)
	{
	case NPC_STATE_ALERT:
	case NPC_STATE_IDLE:
		if (pEnemy)
		{
			if (HasCondition(COND_SEE_ENEMY))
				m_fearTime = gpGlobals->curtime;
			else if (DisregardEnemy(pEnemy))		// After 15 seconds of being hidden, return to alert
			{
				SetEnemy( NULL);
				pEnemy = NULL;
			}
		}

		if (HasCondition(COND_LIGHT_DAMAGE) || HasCondition(COND_HEAVY_DAMAGE))
		{
			// flinch if hurt
			return SCHED_SMALL_FLINCH;
		}

		// Cower when you hear something scary
		if (HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_COMBAT))
		{
			CSound* pSound;
			pSound = GetBestSound();

			ASSERT(pSound != NULL);
			if (pSound)
			{
				if (pSound->IsSoundType(SOUND_DANGER | SOUND_COMBAT))
				{
					if (gpGlobals->curtime - m_fearTime > 3)	// Only cower every 3 seconds or so
					{
						m_fearTime = gpGlobals->curtime;		// Update last fear
						return SCHED_SCIENTIST_STARTLE;	// This will just duck for a second
					}
				}
			}
		}

		// Behavior for following the player
		if (GetFollowTarget())
		{
			if (!GetFollowTarget()->IsAlive())
			{
				// UNDONE: Comment about the recently dead player here?
				StopFollowing();
				break;
			}

			int relationship = D_NU;

			// Nothing scary, just me and the player
			if (pEnemy != NULL)
				relationship = IRelationType(pEnemy);

			// UNDONE: Model fear properly, fix R_FR and add multiple levels of fear
			if (relationship != D_DL && relationship != D_HT)
			{
				// If I'm already close enough to my target
				if (TargetDistance() <= 128)
				{
					if (CanHeal())	// Heal opportunistically
					{
						SetTarget(GetFollowTarget());
						return SCHED_SCIENTIST_HEAL;
					}
						
					if (HasCondition(COND_PLAYER_PUSHING))	// Player wants me to move
						return SCHED_MOVE_AWAY;//SCHED_MOVE_AWAY_FOLLOW;
				}
				return SCHED_TARGET_FACE;	// Just face and follow.
			}
			else	// UNDONE: When afraid, scientist won't move out of your way.  Keep This?  If not, write move away scared
			{
				if (HasCondition(COND_NEW_ENEMY)) // I just saw something new and scary, react
					return SCHED_SCIENTIST_FEAR;					// React to something scary
				return SCHED_SCIENTIST_TARGET_FACE_SCARED;	// face and follow, but I'm scared!
			}
		}

		if (HasCondition(COND_PLAYER_PUSHING))	// Player wants me to move
			return SCHED_MOVE_AWAY;

		// try to say something about smells
		TrySmellTalk();
		break;
	case NPC_STATE_COMBAT:
		if (HasCondition(COND_NEW_ENEMY))
			return SCHED_SCIENTIST_FEAR;					// Point and scream!
		if (HasCondition(COND_SEE_ENEMY))
			return SCHED_SCIENTIST_COVER;		// Take Cover

		if (HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_COMBAT))
			return SCHED_TAKE_COVER_FROM_BEST_SOUND;	// Cower and panic from the scary sound!

		return SCHED_SCIENTIST_COVER;			// Run & Cower
		break;
	}

	return BaseClass::SelectSchedule();
}



NPC_STATE CNPC_Scientist::SelectIdealState(void)
{
	CBaseEntity* pEnemy = GetEnemy();

	switch (m_NPCState)
	{
	case NPC_STATE_ALERT:
	case NPC_STATE_IDLE:
		if (HasCondition(COND_NEW_ENEMY))
		{
			if (GetFollowTarget()&& GetEnemy())
			{
				int relationship = IRelationType(pEnemy);
				if (relationship != D_FR || relationship != D_HT && !HasCondition(COND_LIGHT_DAMAGE | COND_HEAVY_DAMAGE))
				{
					// Don't go to combat if you're following the player
					
					return NPC_STATE_ALERT;
				}
				StopFollowing();
			}
		}
		else if (HasCondition(COND_LIGHT_DAMAGE | COND_HEAVY_DAMAGE))
		{
			// Stop following if you take damage
			if (GetFollowTarget())
				StopFollowing();
		}
		break;

	case NPC_STATE_COMBAT:
	{
		
		if (pEnemy != NULL)
		{
			if (DisregardEnemy(pEnemy))		// After 15 seconds of being hidden, return to alert
			{
				// Strip enemy when going to alert
				
				SetEnemy( NULL);
				return NPC_STATE_ALERT;
			}
			// Follow if only scared a little
			if (GetTarget() != NULL)
			{
				
				return  NPC_STATE_ALERT;
			}

			if (HasCondition(COND_SEE_ENEMY))
			{
				m_fearTime = gpGlobals->curtime;
				
				return NPC_STATE_COMBAT;
			}

		}
	}
	break;
	}

	return BaseClass::GetIdealState();
}


BOOL CNPC_Scientist::CanHeal(void)
{
	if ((m_healTime > gpGlobals->curtime) || (GetTarget() == NULL) || (GetTarget()->GetHealth() > (GetTarget()->GetMaxHealth() * 0.5)))
		return FALSE;

	return TRUE;
}

void CNPC_Scientist::Heal(void)
{
	if (!CanHeal())
		return;

	Vector target = GetTarget()->GetAbsOrigin() - GetAbsOrigin();
	if (target.Length() > 100)
		return;

	GetTarget()->TakeHealth(sk_scientist_heal.GetFloat(), DMG_GENERIC);
	// Don't heal again for 1 minute
	m_healTime = gpGlobals->curtime + 60;
}

// Do we actually need this?
#if 0
int CNPC_Scientist::FriendNumber(int arrayNumber)
{
	static int array[3] = { 1, 2, 0 };
	if (arrayNumber < 3)
		return array[arrayNumber];
	return arrayNumber;
}
#endif

//=========================================================
// Dead Scientist PROP
//=========================================================


class CNPC_DeadScientist : public CAI_BaseNPC
{
public:
	void Spawn(void);
	Class_T	Classify(void) { return	CLASS_HUMAN_PASSIVE; }

	virtual void Precache();

	bool KeyValue(const char* szKeyName, const char* szValue);
	int	m_iPose;// which sequence to display
	static const char* m_szPoses[7];
};
const char* CNPC_DeadScientist::m_szPoses[] = { "lying_on_back", "lying_on_stomach", "dead_sitting", "dead_hang", "dead_table1", "dead_table2", "dead_table3" };


bool CNPC_DeadScientist::KeyValue(const char* szKeyName, const char* szValue)
{
	if (FStrEq(szKeyName, "pose"))
	{
		m_iPose = atoi(szValue);
	}
	else
		return false;//return BaseClass::KeyValue(szKeyName, szValue);

	return true;
}

LINK_ENTITY_TO_CLASS(monster_scientist_dead, CNPC_DeadScientist);
LINK_ENTITY_TO_CLASS(npc_scientist_dead, CNPC_DeadScientist);


void CNPC_DeadScientist::Precache(void)
{
	PrecacheModel("models/scientist.mdl");
	PrecacheScriptSound("npc_Scientist.Pain");


	//TalkInit();
//	BaseClass::Precache();
}
//
// ********** DeadScientist SPAWN **********
//
void CNPC_DeadScientist::Spawn()
{
	Precache();
	
	m_nBody = -1;
	SetModel("models/scientist.mdl");

	//pev->effects = 0;
	//pev->sequence = 0;
	// Corpses have less health
	m_iHealth = 8;//gSkillData.scientistHealth;

	m_bloodColor = BLOOD_COLOR_RED;

	if (m_nBody == -1)
	{// -1 chooses a random head
		m_nBody = random->RandomInt(0, NUM_SCIENTIST_HEADS - 1);// pick a head, any head
		SetBodygroup(1, m_nBody);
	}
	// Luther is black, make his hands black
	if (m_nBody == HEAD_LUTHER)
		m_nSkin = 1;
	else
		m_nSkin = 0;

	SetSequence(LookupSequence(m_szPoses[m_iPose]));
	if (GetSequence() == -1)
	{
		DevWarning("Dead scientist with bad pose\n Setting it to a default pose! \n");
	}

	//	pev->skin += 2; // use bloody skin -- UNDONE: Turn this back on when we have a bloody skin again!

	//NPCInit();
	//MonsterInitDead();
}


//=========================================================
// Sitting Scientist PROP
//=========================================================


class CNPC_SittingScientist : public CNPC_Scientist // kdb: changed from public CBaseMonster so he can speak
{
	DECLARE_CLASS(CNPC_SittingScientist, CNPC_Scientist);
	DECLARE_DATADESC();
public:
	void Spawn(void);
	void  Precache(void);

	void EXPORT SittingThink(void);
	Class_T	Classify(void);


	virtual void SetAnswerQuestion(CNPCSimpleTalker* pSpeaker);
	int FriendNumber(int arrayNumber);

	int FIdleSpeak(void);
	int		m_baseSequence;
	int		m_headTurn;
	float	m_flResponseDelay;
};

LINK_ENTITY_TO_CLASS(monster_sitting_scientist, CNPC_SittingScientist);
//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC(CNPC_SittingScientist)

	// Don't need to save/restore m_baseSequence (recalced)
	DEFINE_FIELD( m_headTurn, FIELD_INTEGER),
	DEFINE_FIELD( m_flResponseDelay, FIELD_FLOAT),
END_DATADESC()

// animation sequence aliases 
typedef enum
{
	SITTING_ANIM_sitlookleft,
	SITTING_ANIM_sitlookright,
	SITTING_ANIM_sitscared,
	SITTING_ANIM_sitting2,
	SITTING_ANIM_sitting3
} SITTING_ANIM;


//
// ********** Scientist SPAWN **********
//
void CNPC_SittingScientist::Spawn()
{
	PrecacheModel("models/scientist.mdl");
	SetModel("models/scientist.mdl");
	Precache();
	InitBoneControllers();

	SetHullType(HULL_HUMAN);
	SetHullSizeNormal();
	//UTIL_SetSize(pev, HUMAN_HULL_MIN, HUMAN_HULL_MAX);

	SetSolid(SOLID_BBOX);
	SetMoveType(MOVETYPE_STEP);
	m_bloodColor = BLOOD_COLOR_RED;
	AddSolidFlags(FSOLID_NOT_STANDABLE);

	m_iHealth = 50;

	m_flFieldOfView = VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so scientists will notice player and say hello
	m_NPCState = NPC_STATE_NONE;

	//	m_flDistTooFar		= 256.0;

	CapabilitiesClear();
	CapabilitiesAdd(bits_CAP_MOVE_GROUND | bits_CAP_ANIMATEDFACE | bits_CAP_TURN_HEAD | bits_CAP_OPEN_DOORS | bits_CAP_AUTO_DOORS | bits_CAP_USE);

	//Do we actually care?
	//SetBits(pev->spawnflags, SF_MONSTER_PREDISASTER); // predisaster only!

	if (m_nBody == -1)
	{// -1 chooses a random head
		m_nBody = random->RandomInt(0, NUM_SCIENTIST_HEADS - 1);// pick a head, any head
	}
	// Luther is black, make his hands black
	if (m_nBody == HEAD_LUTHER)
		m_nSkin = 1;

	m_baseSequence = LookupSequence("sitlookleft");
	SetSequence(m_baseSequence + random->RandomInt(0, 4));
	ResetSequenceInfo();

	SetThink(&CNPC_SittingScientist::SittingThink);
	SetNextThink(gpGlobals->curtime + 0.1);

	
	UTIL_DropToFloor(this,NULL);
}

void CNPC_SittingScientist::Precache(void)
{
	m_baseSequence = LookupSequence("sitlookleft");
	TalkInit();
}

//=========================================================
// ID as a passive human
//=========================================================
Class_T CNPC_SittingScientist::Classify(void)
{

	return	CLASS_HUMAN_PASSIVE;
}


int CNPC_SittingScientist::FriendNumber(int arrayNumber)
{
	static int array[3] = { 2, 1, 0 };
	if (arrayNumber < 3)
		return array[arrayNumber];
	return arrayNumber;
}



//=========================================================
// sit, do stuff
//=========================================================
void CNPC_SittingScientist::SittingThink(void)
{
	CBaseEntity* pent;

	StudioFrameAdvance();

	// try to greet player
	if (FIdleHello())
	{
		pent = FindNearestFriend(TRUE);
		if (pent)
		{
			float yaw = VecToYaw(pent->GetAbsOrigin() - GetAbsOrigin()) - GetAbsAngles().y;

			if (yaw > 180) yaw -= 360;
			if (yaw < -180) yaw += 360;

			if (yaw > 0)
				SetSequence(m_baseSequence + SITTING_ANIM_sitlookleft);
			else
				SetSequence(m_baseSequence + SITTING_ANIM_sitlookright);

			ResetSequenceInfo();
			//pev->frame = 0;
			SetBoneController(0, 0);
		}
	}
	else if (IsSequenceFinished())
	{
		int i = random->RandomInt(0, 99);
		m_headTurn = 0;

		if (m_flResponseDelay && gpGlobals->curtime > m_flResponseDelay)
		{
			// respond to question
			IdleRespond();
			SetSequence(m_baseSequence + SITTING_ANIM_sitscared);
			m_flResponseDelay = 0;
		}
		else if (i < 30)
		{
			SetSequence(m_baseSequence + SITTING_ANIM_sitting3);

			// turn towards player or nearest friend and speak

			if (!CanSayHello())
				pent = FindNearestFriend(TRUE);
			else
				pent = FindNearestFriend(FALSE);

			if (!FIdleSpeak() || !pent)
			{
				m_headTurn = random->RandomInt(0, 8) * 10 - 40;
				SetSequence(m_baseSequence + SITTING_ANIM_sitting3);
			}
			else
			{
				// only turn head if we spoke
				float yaw = VecToYaw(pent->GetAbsOrigin() - GetAbsOrigin()) - GetAbsAngles().y;

				if (yaw > 180) yaw -= 360;
				if (yaw < -180) yaw += 360;

				if (yaw > 0)
					SetSequence(m_baseSequence + SITTING_ANIM_sitlookleft);
				else
					SetSequence(m_baseSequence + SITTING_ANIM_sitlookright);

				//ALERT(at_console, "sitting speak\n");
			}
		}
		else if (i < 60)
		{
			SetSequence(m_baseSequence + SITTING_ANIM_sitting3);
			m_headTurn = random->RandomInt(0, 8) * 10 - 40;
			if (random->RandomInt(0, 99) < 5)
			{
				//ALERT(at_console, "sitting speak2\n");
				FIdleSpeak();
			}
		}
		else if (i < 80)
		{
			SetSequence(m_baseSequence + SITTING_ANIM_sitting2);
		}
		else if (i < 100)
		{
			SetSequence(m_baseSequence + SITTING_ANIM_sitscared);
		}

		ResetSequenceInfo();
		//pev->frame = 0;
		SetBoneController(0, m_headTurn);
	}
	SetNextThink(gpGlobals->curtime + 0.1);
}

// prepare sitting scientist to answer a question
void CNPC_SittingScientist::SetAnswerQuestion(CNPCSimpleTalker* pSpeaker)
{
	m_flResponseDelay = gpGlobals->curtime + random->RandomFloat(3, 4);
	SetSpeechTarget((CNPCSimpleTalker*)pSpeaker);
}


//=========================================================
// FIdleSpeak
// ask question of nearby friend, or make statement
//=========================================================
int CNPC_SittingScientist::FIdleSpeak(void)
{
	return BaseClass::FIdleSpeak();
}

//------------------------------------------------------------------------------
//
// Schedules
//
//------------------------------------------------------------------------------

AI_BEGIN_CUSTOM_NPC(monster_scientist, CNPC_Scientist)

DECLARE_TASK(TASK_SAY_HEAL)
DECLARE_TASK(TASK_HEAL)
DECLARE_TASK(TASK_SAY_FEAR)
DECLARE_TASK(TASK_RUN_PATH_SCARED)
DECLARE_TASK(TASK_SCREAM)
DECLARE_TASK(TASK_RANDOM_SCREAM)
DECLARE_TASK(TASK_MOVE_TO_TARGET_RANGE_SCARED)

DECLARE_ACTIVITY(ACT_EXCITED)

//=========================================================
// > SCIENTIST SCHEDULES
//=========================================================
DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_HEAL,

	"	Tasks"
	"		TASK_GET_PATH_TO_TARGET				0"
	"		TASK_MOVE_TO_TARGET_RANGE			50"
	"		TASK_SET_FAIL_SCHEDULE				SCHEDULE:SCHED_SCIENTIST_FOLLOWTHETARGET"
	"		TASK_FACE_IDEAL						0"
	"		TASK_SAY_HEAL						0"
	"		TASK_PLAY_SEQUENCE_FACE_TARGET		ACTIVITY:ACT_ARM"
	"		TASK_HEAL							0"
	"		TASK_PLAY_SEQUENCE_FACE_TARGET		ACTIVITY:ACT_DISARM"
	"	"
	"	Interrupts"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_FOLLOWTHETARGET,

	"	Tasks"
	"		TASK_GET_PATH_TO_TARGET			0"
	"		TASK_MOVE_TO_TARGET_RANGE		128"
	"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_TARGET_FACE"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_LIGHT_DAMAGE"
	"		COND_HEAVY_DAMAGE"
	"		COND_HEAR_DANGER"
	"		COND_HEAR_COMBAT"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_FACETHETARGET,

	"	Tasks"
	"		TASK_STOP_MOVING			0"
	"		TASK_FACE_TARGET			0"
	"		TASK_SET_ACTIVITY			ACTIVITY:ACT_IDLE"
	"		TASK_SET_SCHEDULE			SCHEDULE:SCHED_SCIENTIST_FOLLOWTHETARGET"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_HEAR_DANGER"
	"		COND_HEAR_COMBAT"
	"		COND_GIVE_WAY"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_COVER,

	"	Tasks"
	"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_SCIENTIST_PANIC"
	"		TASK_STOP_MOVING				0"
	"		TASK_FIND_COVER_FROM_ENEMY		0"
	"		TASK_RUN_PATH_SCARED			0"
	"		TASK_TURN_LEFT					179"
	"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_SCIENTIST_HIDE"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_HIDE,

	"	Tasks"
	"		TASK_SET_FAIL_SCHEDULE		SCHEDULE:SCHED_SCIENTIST_PANIC"
	"		TASK_STOP_MOVING			0"
	"		TASK_PLAY_SEQUENCE			ACTIVITY:ACT_CROUCHIDLE"
	"		TASK_SET_ACTIVITY			ACTIVITY:ACT_CROUCHIDLE"
	"		TASK_WAIT_RANDOM			10"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_SEE_ENEMY"
	"		COND_SEE_HATE"
	"		COND_SEE_FEAR"
	"		COND_SEE_DISLIKE"
	"		COND_HEAR_DANGER"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_IDLE_STAND,

	"	Tasks"
	"		TASK_STOP_MOVING			0"
	"		TASK_SET_ACTIVITY			ACTIVITY:ACT_IDLE"
	"		TASK_WAIT					2"
	"		TASK_TALKER_HEADRESET		0"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_LIGHT_DAMAGE"
	"		COND_HEAVY_DAMAGE"
	"		COND_SMELL"
	"		COND_PROVOKED"
	"		COND_HEAR_COMBAT"
	"		COND_GIVE_WAY"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_PANIC,

	"	Tasks"
	"		TASK_STOP_MOVING					0"
	"		TASK_FACE_ENEMY						0"
	"		TASK_SCREAM							0"
	"		TASK_PLAY_SEQUENCE_FACE_ENEMY		ACTIVITY:ACT_EXCITED"
	"		TASK_SET_ACTIVITY					ACTIVITY:ACT_IDLE"
	"	"
	"	Interrupts"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_FOLLOWSCARED,

	"	Tasks"
	"		TASK_GET_PATH_TO_TARGET				0"
	"		TASK_SET_FAIL_SCHEDULE				SCHEDULE:SCHED_SCIENTIST_FOLLOWTHETARGET"
	"		TASK_MOVE_TO_TARGET_RANGE_SCARED	128"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_LIGHT_DAMAGE"
	"		COND_HEAVY_DAMAGE"
	"		COND_HEAR_DANGER"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_TARGET_FACE_SCARED,

	"	Tasks"
	"	TASK_FACE_TARGET				0"
	"		TASK_SET_ACTIVITY			ACTIVITY:ACT_CROUCHIDLE"
	"		TASK_SET_SCHEDULE			SCHEDULE:SCHED_SCIENTIST_FOLLOWSCARED"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_HEAR_DANGER"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_FEAR,

	"	Tasks"
	"		TASK_STOP_MOVING			0"
	"		TASK_FACE_ENEMY				0"
	"		TASK_SAY_FEAR				0"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
)

DEFINE_SCHEDULE
(
	SCHED_SCIENTIST_STARTLE,

	"	Tasks"
	"		TASK_SET_FAIL_SCHEDULE				SCHEDULE:SCHED_SCIENTIST_PANIC"
	"		TASK_RANDOM_SCREAM					0.3"
	"		TASK_STOP_MOVING					0"
	"		TASK_PLAY_SEQUENCE_FACE_ENEMY		ACTIVITY:ACT_CROUCH"
	"		TASK_RANDOM_SCREAM					0.1"
	"		TASK_PLAY_SEQUENCE_FACE_ENEMY		ACTIVITY:ACT_CROUCHIDLE"
	"		TASK_WAIT_RANDOM					1"
	"	"
	"	Interrupts"
	"		COND_NEW_ENEMY"
	"		COND_SEE_ENEMY"
	"		COND_SEE_HATE"
	"		COND_SEE_FEAR"
	"		COND_SEE_DISLIKE"
)

AI_END_CUSTOM_NPC()
