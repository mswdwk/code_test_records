#include<map>
#include<set>
#include<iostream>
#include<vector>
#include<stdio.h>
#include <algorithm>

using namespace std;

typedef map<int,int> table;

set<vector<int>> deadlock_result_set;

bool DeadLock(table lock2thread, table waitedthread2lock)
{
	bool find_deadlock = false; 
	int size = waitedthread2lock.size();
	map < int, int >::iterator it;
	for (it = waitedthread2lock.begin(); it != waitedthread2lock.end(); it++) {
		int tid = it->first;
		int lock = it->second;
		int count = 0;
		cout<<"tid= "<<tid<<" "<<endl;
		// 说明tid想要获取lock，那么这个操作是可能导致死锁的
		if (lock2thread.find(lock) != lock2thread.end()) {
			// 说明该锁已经被占用，那么可能引起死锁
			int nexttid = 0;	//当前线程依赖的下一个线程
			int nextlock = lock;
			vector<int> dlk_tids; //deadlock_thread_ids;
			dlk_tids.clear();
			while (count <= lock2thread.size() ) {
				nexttid = lock2thread[nextlock];
				dlk_tids.push_back(nexttid);
				if ( nexttid == tid ){
					sort(dlk_tids.begin(),dlk_tids.end());
					deadlock_result_set.insert(dlk_tids);
					dlk_tids.clear();
					find_deadlock = true;
					break;
					//return true;	//发生死锁	
				}
				//查看nexttid在等待哪个资源
				if (waitedthread2lock.find(nexttid) != waitedthread2lock.end()) {
					nextlock = waitedthread2lock[nexttid];
				}
				count++;
				//说明没有环路，不发生死锁
			}
		}
	}
			if(find_deadlock)
				return true;
			else
				return false;	
}

int main()
{
	// 锁->线程，标识当前锁被哪个线程占用
	map < int, int >lock2thread = {
		{1, 1},
		{2, 2},
		{3, 3},
		{4, 4},
		{5, 5},
		{6, 6},
	};
	// 线程 -> 锁， 标识当前线程在等待哪个锁
	map < int, int >waitedthread2lock = {
		{1, 2},
		{2, 3},
		{3, 1},
		{4, 5},
		{5, 4},
	};

	if (true == DeadLock(lock2thread, waitedthread2lock)){
		cout << "find deadlock" << endl;
		int set_id = 0;
		set<vector<int>>::iterator it;
		for(it = deadlock_result_set.begin(); it!= deadlock_result_set.end();it++){ 
			cout << "deadlock thread set "<<set_id++ << endl;
			vector<int>::iterator vit; 
			for(vit = it->begin(); vit!= it->end();vit++) 
				cout << *vit <<" ";
			cout<<endl;
		}
	}
	else
		cout << "not find deadlock" << endl;

	return 0;
}
