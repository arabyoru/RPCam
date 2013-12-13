#pragma once
#include <iostream>

template <typename TVal, int theMax>
class RPCircularQueue
{
	typedef struct
	{
		TVal 	vData;
	}NodeData;

public:
	RPCircularQueue(void)
	{
		this->mnCapacity = theMax+1;
		mNode = new NodeData[this->mnCapacity];
		mnFront = 0;
		mnRear = 0;
	};

	~RPCircularQueue(void)
	{
		delete []mNode;
	};

	void EnQueue(TVal vData)
	{
		int nPos;
		if(mnRear == mnCapacity -1)
		{
			nPos = mnRear;
			mnRear = 0;
		}
		else
		{
			nPos = mnRear++;
		}
		mNode[nPos].vData = vData;
	};

	TVal DeQueue(void)
	{
		int nPos = mnFront;
		if(mnFront == mnCapacity-1) mnFront = 0;
		else mnFront++;
		return mNode[nPos].vData;
	};

	int GetSize(void)
	{
		if(mnFront <= mnRear) return mnRear - mnFront; // 전단 Index 가 후단 Index 보다 같거나 작을 경우
		else return mnCapacity - (mnFront + mnRear);
	};

	bool IsEmpty(void)
	{
		return bool(mnFront == mnRear);
	};

	bool IsFull(void)
	{
		if(mnFront<=mnRear) return bool((mnRear - mnFront) == (mnCapacity - 1));
		else return bool((mnRear + 1) == mnFront);
	};

	int GetRearPos(void) { return mnRear; };
	int GetFront(void) { return mnFront; };

private:
	NodeData* 	mNode;
	int 		mnCapacity;
	int 		mnFront;
	int 		mnRear;
};