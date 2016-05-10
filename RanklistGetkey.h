#ifndef RANKLIST_GETKEY_H
#define RANKLIST_GETKEY_H

//通过增加"operator()"
struct tempstruct
{
	int id;
	int time;
	int values;
};

class ranklist_getkey
{
public:
	int operator()(tempstruct* aidata)const
	{
		return aidata->id;
	}

};

#endif //RANKLIST_GETKEY_H