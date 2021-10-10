#pragma once
#ifndef NPC_BULLSQUID_H
#define NPC_BULLSQUID_H
#include "ai_basenpc.h"
#include "npc_talker.h"

#define SF_BEFOREDISATER 0x001

class CNPC_Scientist : public  CNPCSimpleTalker
{
	DECLARE_CLASS(CNPC_Scientist, CNPCSimpleTalker);
	DECLARE_DATADESC();
public:
	void Spawn(void);
	virtual void Precache(void);
	Class_T	Classify(void);

	//void IdleSound(void);
	void PainSound(const CTakeDamageInfo& info);
	//void AlertSound(void);
	void DeathSound(const CTakeDamageInfo& info);


	float MaxYawSpeed(void);

	void HandleAnimEvent(animevent_t* pEvent);

	
	int OnTakeDamage_Alive(const CTakeDamageInfo& inputInfo);

	int GetSoundInterests(void);
	//void RunAI(void);
	//virtual void OnListened(void);

	virtual int SelectSchedule(void);



	virtual void DeclineFollowing(void);

	virtual Activity GetStoppedActivity(void);

	virtual void Event_Killed(const CTakeDamageInfo& inputInfo);

	virtual NPC_STATE SelectIdealState(void);

	//virtual int FriendNumber(int arrayNumber);

	void StartTask(const Task_t* pTask);
	void RunTask(const Task_t* pTask);

	BOOL	CanHeal(void);
	void	Heal(void);
	void	Scream(void);

	void TalkInit(void);

	virtual int TranslateSchedule(int scheduleType);

	BOOL	DisregardEnemy(CBaseEntity* pEnemy) { return !pEnemy->IsAlive() || (gpGlobals->curtime - m_fearTime) > 15; }


	virtual Activity NPC_TranslateActivity(Activity newActivity);

	
	//Network Later! Better do it soon! I mean it! am typing this at 5:45AM, god damn it! --AnthonyP
	int m_nBody;
	//CNetworkVar(int, m_nBody);

	virtual void ModifyOrAppendCriteria(AI_CriteriaSet& criteriaSet);

	DEFINE_CUSTOM_AI;
	//Original HL1 code
	/*
	void SetYawSpeed(void);
	void HandleAnimEvent(MonsterEvent_t* pEvent);
	void RunTask(Task_t* pTask);
	void StartTask(Task_t* pTask);
	int	ObjectCaps(void) { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	virtual int FriendNumber(int arrayNumber);
	void SetActivity(Activity newActivity);
	Activity GetStoppedActivity(void);
	int ISoundMask(void);
	void DeclineFollowing(void);

	float	CoverRadius(void) { return 1200; }		// Need more room for cover because scientists want to get far away!
	BOOL	DisregardEnemy(CBaseEntity* pEnemy) { return !pEnemy->IsAlive() || (gpGlobals->time - m_fearTime) > 15; }

	BOOL	CanHeal(void);
	void	Heal(void);
	void	Scream(void);

	// Override these to set behavior
	Schedule_t* GetScheduleOfType(int Type);
	Schedule_t* GetSchedule(void);
	MONSTERSTATE GetIdealState(void);

	void DeathSound(void);
	void PainSound(void);

	void TalkInit(void);

	void			Killed(entvars_t* pevAttacker, int iGib);

	virtual int		Save(CSave& save);
	virtual int		Restore(CRestore& restore);
	static	TYPEDESCRIPTION m_SaveData[];

	CUSTOM_SCHEDULES;*/

	enum
	{
		SCHED_SCIENTIST_HIDE = BaseClass::NEXT_SCHEDULE,
		SCHED_SCIENTIST_FEAR,
		SCHED_SCIENTIST_PANIC,
		SCHED_SCIENTIST_STARTLE,
		SCHED_SCIENTIST_TARGET_CHASE_SCARED,
		SCHED_SCIENTIST_TARGET_FACE_SCARED,
		SCHED_SCIENTIST_CANT_FOLLOW,
		SCHED_SCIENTIST_HEAL,
		SCHED_SCIENTIST_COVER,
		SCHED_SCIENTIST_HIDEFROMBESTSOUND,
		SCHED_SCIENTIST_FOLLOWTHETARGET,
		SCHED_SCIENTIST_STAHPFOLLOWINGTHETARGET,
		SCHED_SCIENTIST_FACETHETARGET,
		SCHED_SCIENTIST_IDLE_STAND,
		SCHED_SCIENTIST_FOLLOWSCARED

	};

	enum
	{
		TASK_SAY_HEAL = BaseClass::NEXT_TASK,
		TASK_HEAL,
		TASK_SAY_FEAR,
		TASK_RUN_PATH_SCARED,
		TASK_SCREAM,
		TASK_RANDOM_SCREAM,
		TASK_MOVE_TO_TARGET_RANGE_SCARED,
		TASK_CANT_FOLLOW,
		TASK_TLK_HEADRESET,
	};


	float TargetDistance(void);

private:
	float m_painTime;
	float m_healTime;
	float m_fearTime;
};
#endif