#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef map < int, int >Table;

set < vector < int >> deadlock_result_sets;

bool DeadLock(Table lock2thread, Table waitedthread2lock)
{
	bool found_deadlock = false;
	int size = waitedthread2lock.size();
	Table::iterator it;
	// 从锁等待表中开始查找
	for (it = waitedthread2lock.begin(); it != waitedthread2lock.end(); it++) {
		int tid = it->first;	 // thread id
		int lockid = it->second; // lock_id
		int count = 0;
		cout << "tid= " << tid << " " << endl;
		// 如果 该锁lockid，没有被某个线程等待，则不会发生死锁
		if (lock2thread.find(lockid) == lock2thread.end())
			continue;

		// 说明该锁已经被占用，那么可能引起死锁
		int nexttid = 0;	//当前线程依赖的下一个线程
		int nextlock = lockid;
		vector < int > dlk_tids;	//deadlock thread ids;
		dlk_tids.clear();
		// 循环查找，同时避免无限循环
		while (count <= lock2thread.size()) {
			nexttid = lock2thread[nextlock];
			dlk_tids.push_back(nexttid);
			// 发生死锁  
			if (nexttid == tid) {
				sort(dlk_tids.begin(), dlk_tids.end());
				deadlock_result_sets.insert(move(dlk_tids));
				found_deadlock = true;
				break;
			}
			// 说明没有环路，不发生死锁。
			// 继续查看nexttid在等待哪个资源
			if (waitedthread2lock.find(nexttid) != waitedthread2lock.end()) {
				nextlock = waitedthread2lock[nexttid];
			}
			count++;
		}
	}
	if (found_deadlock)
		return true;
	return false;
}

int main()
{
	// 锁->线程，标识当前锁被哪个线程占用
	Table lock2thread = {
		{1, 1},
		{2, 2},
		{3, 3},
		{4, 4},
		{5, 5},
		{6, 6},
	};
	// 线程 -> 锁， 标识当前线程在等待哪个锁
	Table waitedthread2lock = {
		{1, 2},
		{2, 3},
		{3, 1},
		{4, 5},
		{5, 4},
	};

	if (false == DeadLock(lock2thread, waitedthread2lock)) {
		cout << "not found deadlock" << endl;
		return 0;
	}
	
	cout << "found deadlock" << endl;
	int set_id = 0;
	set < vector < int >>::iterator it;
	for (it = deadlock_result_sets.begin(); it != deadlock_result_sets.end(); it++) {
		cout << "deadlock thread set " << set_id++ << endl;
		vector < int >tmp = *it;
		vector < int >::iterator vit;
		for (vit = tmp.begin(); vit != tmp.end(); vit++)
			cout << *vit << " ";
		cout << endl;
	}

	return 0;
}
