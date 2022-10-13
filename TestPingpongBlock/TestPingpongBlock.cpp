#include "TestPingpongBlock.h"
#include <thread>
#include <queue>

using namespace std;

#define BLOCK_SIZE 1024

char* blockPing = nullptr;
char* blockPong = nullptr;

void funMgrData();
void funA(int v);

// 生产数据
void funCreateData() {
	cout << "enter funCreateData" << " tid:" << _threadid << endl;
	while (true) {
		static char bchar = 'a';
		bchar = bchar > 'a' + 25 ? 'a' : ++bchar;
		//cout << "loop funCreateData" << " tid:" << _threadid << endl;
		_sleep(100);

		// 是否可以填充ping，可以则填充ping
		if (blockPing == nullptr) {
			blockPing = new char[BLOCK_SIZE];
			memset(blockPing, 0, BLOCK_SIZE);
			blockPing[0] = bchar;
			cout << "loop fill ping " << " --- " << bchar << endl;

			// 送去处理
			funMgrData();
		}
	}
}

// 管理数据
void funMgrData() {
	cout << "[enter funMgrData]" << " tid:" << _threadid << endl;

	// 消费ping, ping pong内存互换
	if (blockPong == nullptr) {
		blockPong = blockPing;
		blockPing = nullptr;
		cout << "swap blockPing blockPong" << " tid:" << _threadid << endl;
	}
}

// 消费数据
void funA(int v) {
	cout << "enter funA" << " tid:" << _threadid << endl;
	while (true) {
		_sleep(3);

		if (blockPong != nullptr) {
			char t = blockPong[0];
			cout << " ---deal****** data-- " << t << " tid:" << _threadid << endl;
			_sleep(3000);
			delete blockPong;
			blockPong = nullptr;
			cout << " ---deal****** data end-- " << t << " tid:" << _threadid << endl;

			// 消费pong, ping pong内存互换
			blockPong = blockPing;
			blockPing = nullptr;
			cout << "funA swap blockPing blockPong" << " tid:" << _threadid << endl;
		}
	}
	cout << "leave funA" << " tid:" << _threadid << endl;
}

// ping pong内存块 示例
int main()
{
	// ping pong数据生产线程
	thread t0(funCreateData);
	t0.detach();

	// 消费线程
	static int v = 1;	
	thread t1(funA, v);
	t1.join();

	return 0;
}
