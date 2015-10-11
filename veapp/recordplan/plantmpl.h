#ifndef PLAN_TMPL_STRUCT
#define PLAN_TMPL_STRUCT

typedef struct __PlanMod__{
	//char ModName[10];
	QString name;
	int ModId;
	int Week[7][2];
}PlanMod;

#endif //PLANMOD_STRUCT