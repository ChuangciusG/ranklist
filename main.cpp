#include "RankList.h"

class less_temp
{
public:
	bool operator()(const tempstruct* al, const tempstruct* ar)const
	{
		if (al->values == ar->values)
		{
			if (al->time == ar->time)
			{
				return (al->id < ar->id);
			}
			else if (al->time < ar->time)
			{
				return true;
			}
		}
		else if (al->values > ar->values)
		{
			return true;
		}

		return false;
	}
};

class print
{
public:
	void operator()(tempstruct* aidata)
	{
		printf("%d", aidata->values);
	}
};

int main()
{
	vector<int> aipage = {3,4,4,4,4,2};
	RankList<tempstruct*, int, ranklist_getkey, less_temp> lrank(aipage.data(),6);
	list<tempstruct> ls;
	tempstruct ltemp;
	tempstruct* lp;
	for (int i = 0; i<100; i++)
	{
		ltemp.id = i;
		ltemp.time = rand()%100;
		ltemp.values = i;
		ls.push_back(ltemp);
		lp = &(*(ls.rbegin()));
		lrank.insert(lp);
	}
	ltemp.id = 99;
	ltemp.values =95;
	ltemp.time = 37;
	lp = &ltemp;
	lrank.insert(lp);

	for (auto itor = ls.begin(); itor != ls.end(); ++itor)
	{
		lp = &*itor;
		lp->values = rand() % 150;
		lp->time = rand() % 100;
		lrank.insert(lp);
	}

	const list<tempstruct*>& lret = lrank.get_page(3);

	lret.size();
	lret.begin();

	int temp = 0;
	for(int i = 0;i<10000;++i)
	{
		temp = rand() % 100+1;
		printf("[%d][%d]\n",temp,lrank.find_rank(temp));
	}

	int j = 1;
	for (unsigned int i = 1; i <= aipage.size(); ++i)
	{
		for_each(lrank.get_page(i).begin(), lrank.get_page(i).end(), [&j](const tempstruct* ap)
		{
			printf("[%02d,%3d,%3d,%3d] ",j++,ap->id,ap->time,ap->values);
		});
		printf("\n");
	}
	

	int beg = rand()%50;
	int end = beg+rand()%(50-beg);
	vector<tempstruct*> lsb;
	if (lrank.get_range(beg, end, lsb))
	{
		printf("ku");
	}

	tempstruct* lsb2;
	lrank.get(48, lsb2);


	return 0;
}