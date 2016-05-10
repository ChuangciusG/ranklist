#ifndef RANK_LIST_H
#define RANK_LIST_H

#include <vector>
#include <list>
#include <algorithm>
#include <set>
using namespace std;

template <
	typename T,					//��Ҫ���������
	typename KEY,				//���ݵ�����
	typename GET_KEY,			//�º���  ���ڻ�ȡ����  ������Ҫ֧�ֱȽϲ���
	typename RANK_T_LESS		//�º���  ���ڱȽ�����T����
>
class RankList
{
	vector<list<T> > m_sortvec;			//���ڴ洢����
	set<KEY> m_rankid;					//�ϰ������
	static GET_KEY m_fun_getkey;		//�º��� ��ȡkey
	static RANK_T_LESS m_fun_sort;		//�º��� ���ڱȽ�����T��������
	int m_rank_size;				//��Ҫ���������
	int m_rank_page_size;			//��Ҫ�����ҳ�� 
	vector<int> m_arr_everypagesize;

	RankList(){}
	
	// ͨ������ ��������
	int _find_rank(KEY aid);
	// ɾ��ָ������������
	void _find_erse(KEY aid);
	//��������
	void _insert(T& aidata);
	//�������ݽṹ
	void _add_finishing();
	void _erse_finishing();
	//���ҳ��Ϸ���
	bool _check_pagenum(int aipage);
	//��ȡ�������ڵ�ҳ������
	bool _rankpagepos(int airank, int& aipage, int& aipos);

public:
	RankList(int* aivec, int aisize);
	RankList(int arank_size, int arank_page_size);
	
	// �������
	void insert(T& aidata);
	//����
	void updata(T& aidata);
	//���key �Ƿ����
	bool check(KEY aid);
	//����id���ڵ����� -1Ϊδ�ϰ�
	int find_rank(KEY aid);
	//��ȡָ��ҳ��������
	bool get_page(int aipage, vector<T>& aivec);
	//��ȡָ��ҳ��������
	const list<T>& get_page(int aipage);
	//��ȡָ�����������
	bool get_range(int abeg, int aend, vector<T>& airet);
	//��ȡĳ������������
	bool get(int arank, T& aidata);
	//�������
	void clear();
	//�������ݵ���Ŀ
	int size();
	//��ȡָ��ҳ�������
	int page_size(int aipage);
	//��ȡ�ܹ�ҳ������
	int page();
	//��ǰҳ��
	int current_page();
	//�ڼ�ҳ��һ����ҵ�����
	int page_begin_mingci(int aipage);
	//�Ƿ�Ϊ��
	bool empty();
};

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
GET_KEY RankList<T, KEY, GET_KEY, RANK_T_LESS>::m_fun_getkey;

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
RANK_T_LESS RankList<T, KEY, GET_KEY, RANK_T_LESS>::m_fun_sort;

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::_rankpagepos(int airank,int& aipage,int& aipos)
{
	aipage = 1;
	for (vector<int>::iterator itor = m_arr_everypagesize.begin(); 
		itor != m_arr_everypagesize.end(); ++itor, ++aipage)
	{
		airank -= *itor;
		if (airank == 0)
		{
			aipos = *itor;
			return true;
		}
	}
	return false;
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::get_range(int abeg, int aend, vector<T>& airet)
{
	--abeg;//������һ��
	--aend;//������һ��

	if (abeg < 0
		|| abeg >= m_rank_size
		|| aend < 0
		|| aend >= m_rank_size
		|| aend <= abeg
		)
	{
		return false;
	}

	int lbegpage;
	int lbegpos;
	_rankpagepos(abeg, lbegpage, lbegpos);
	int lsize = aend - (abeg-1);
	for (int i = lbegpage; i < m_rank_page_size; ++i)
	{
		list<T>& ltemp = m_sortvec[i];
		list<T>::iterator itor = ltemp.begin();
		for (int j = 0; itor != ltemp.end(); ++itor, ++j)
		{
			if (i != lbegpage || j >= lbegpos)
			{
				airet.push_back(*itor);
				if (--lsize == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}
//��ȡĳ������������
template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::get(int arank, T& aidata)
{
	--arank;//������һ��
	if (arank < 0
		|| arank >= m_rank_size
		)
	{
		return false;
	}
	int lbegpage;
	int lbegpos;
	_rankpagepos(arank, lbegpage, lbegpos);

	for (int i = lbegpage; i < m_rank_page_size; ++i)
	{
		list<T>& ltemp = m_sortvec[i];
		list<T>::iterator itor = ltemp.begin();
		for (int j = 0; itor != ltemp.end(); ++itor, ++j)
		{
			if (i != lbegpage || j >= lbegpos)
			{
				aidata = *itor;
				return true;
			}
		}
	}

	return false;

}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::empty()
{
	return m_rankid.empty();
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::_find_rank(KEY aid)
{
	int lrankpos = 0;
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		list<T>& ltemp = m_sortvec[i];
		list<T>::iterator itor = m_sortvec[i].begin();
		for (; itor != m_sortvec[i].end(); ++itor)
		{
			++lrankpos;
			if (m_fun_getkey(*itor) == aid)
			{
				return lrankpos;
			}
		}
	}

	//��������ﷵ��˵������
	return lrankpos;
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::_find_erse(KEY aid)
{
	if (m_rankid.find(aid) == m_rankid.end())
	{
		return;//δ�ϰ�
	}
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		list<T>& ltemp = m_sortvec[i];
		list<T>::iterator itor = m_sortvec[i].begin();
		for (; itor != m_sortvec[i].end(); ++itor)
		{
			if (m_fun_getkey(*itor) == aid)
			{
				ltemp.erase(itor);
				m_rankid.erase(aid);
				return;
			}
		}
	}
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::_insert(T& aidata)
{
	list<T>::iterator itor;
	bool linsert = false;
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		list<T>& ltemp = m_sortvec[i];

		if (ltemp.empty())
		{
			ltemp.push_back(aidata);
			m_rankid.insert(m_fun_getkey(aidata));
			return;
		}

		if (ltemp.size() >= (unsigned int)m_arr_everypagesize[i])
		{
			if (!m_fun_sort(aidata, *ltemp.rbegin()))//�߽��ж�
			{
				continue;
			}
		}
	
		itor = ltemp.begin();
		for (; itor != ltemp.end(); ++itor)
		{
			if (m_fun_sort(aidata, *itor))
			{
				break;
			}
		}

		ltemp.insert(itor, aidata);
		m_rankid.insert(m_fun_getkey(aidata));
		return;
	}
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::_add_finishing()
{
	list<T> ltemp;
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		if (!ltemp.empty())
		{
			m_sortvec[i].insert(m_sortvec[i].begin(), ltemp.begin(), ltemp.end());
			ltemp.clear();
		}
		while (m_sortvec[i].size() >(unsigned int)m_arr_everypagesize[i])
		{
			ltemp.push_back(*(m_sortvec[i].rbegin()));
			m_sortvec[i].pop_back();
		}
	}

	for (auto itor = ltemp.begin(); itor != ltemp.end(); ++itor)
	{
		m_rankid.erase(m_fun_getkey(*itor));
	}
}



template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::_erse_finishing()
{
	list<T> ltemp;
	int lisize = 0;
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		while (lisize-- > 0)
		{
			if (!m_sortvec[i].empty())
			{
				m_sortvec[i-1].push_back(*m_sortvec[i].begin());
				m_sortvec[i].pop_front();
			}
			
		}
		lisize = m_arr_everypagesize[i] - m_sortvec[i].size();
	}
}


template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::_check_pagenum(int aipage)
{
	return aipage <= m_rank_page_size && aipage > 0;
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
RankList<T, KEY, GET_KEY, RANK_T_LESS>::RankList(int arank_size, int arank_page_size):
	m_rank_size(arank_size),
	m_rank_page_size(arank_page_size)
{
	for (int i = 0; i < arank_page_size; ++i)
	{
		m_arr_everypagesize.push_back(arank_size / arank_page_size);
	}
	m_sortvec.resize(m_rank_page_size);
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
RankList<T, KEY, GET_KEY, RANK_T_LESS>::RankList(int* aivec, int aisize) :
m_rank_size(0),
m_rank_page_size(aisize)
{
	m_arr_everypagesize.resize(aisize);
	m_rank_size = 0;
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		m_arr_everypagesize[i] = aivec[i];
		m_rank_size += aivec[i];
	}
	m_sortvec.resize(m_rank_page_size);
}


template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::insert(T& aidata)
{
	//���� ɾ��
	_find_erse(m_fun_getkey(aidata));
	_erse_finishing();
	//����
	_insert(aidata);
	//����
	_add_finishing();
	
}
template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::updata(T& aidata)
{
	insert(aidata);
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::check(KEY aid)
{
	return m_rankid.find(aid) != m_rankid.end();
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::find_rank(KEY aid)
{
	if (!check(aid))
	{
		return -1;//δ�ϰ�
	}
	else
	{
		return _find_rank(aid);
	}
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
bool RankList<T, KEY, GET_KEY, RANK_T_LESS>::get_page(int aipage, vector<T>& aivec)
{
	if (aipage <= m_rank_page_size && aipage > 0)
	{
		aipage -= 1;//ҳ�� ��Ӧ��������
		aivec.clear();
		aivec.resize(m_sortvec[aipage].size());
		std::copy(m_sortvec[aipage].begin(), m_sortvec[aipage].end(), aivec.begin());
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
const list<T>& RankList<T, KEY, GET_KEY, RANK_T_LESS>::get_page(int aipage)
{
	static list<T> lnull;
	if (aipage <= m_rank_page_size && aipage > 0)
	{
		aipage -= 1;//ҳ�� ��Ӧ��������
		return m_sortvec[aipage];
	}
	else
	{
		return lnull;
	}
}
template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
void RankList<T, KEY, GET_KEY, RANK_T_LESS>::clear()
{
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		m_sortvec[i].clear();
	}
	m_rankid.clear();
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::size()
{
	return m_rankid.size();
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::page_size(int aipage)
{
	if (_check_pagenum())
	{
		aipage -= 1;//ҳ�� ��Ӧ��������
		return m_sortvec[aipage].size();
	}
	else
	{
		return false;
	}
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::page()
{
	return m_rank_page_size;
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::current_page()
{
	for (int i = 0; i < m_rank_page_size; ++i)
	{
		if (m_sortvec[i].empty())
		{
			return i;
		}
	}
	return m_rank_page_size;
}

template <typename T, typename KEY, typename GET_KEY, typename RANK_T_LESS>
int RankList<T, KEY, GET_KEY, RANK_T_LESS>::page_begin_mingci(int aipage)
{
	--aipage;
	if (aipage > m_rank_page_size)
	{
		aipage = m_rank_page_size;
	}
	if (aipage < 0)
	{
		aipage = 0;
	}
	int ret = 0;
	for (int i = 0; i < aipage; ++i)
	{
		ret += m_arr_everypagesize[i];
	}
	return ret;
}

//�����涨���ȡkey
#include "RanklistGetkey.h"

#endif //RANK_LIST_H