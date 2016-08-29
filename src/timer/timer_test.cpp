#include <iostream>
#include "timer.h"
using namespace std;
int main(){
	Timer timer1(0.0005);
	Timer timer2(1);
	while(1){
		timer2.start();

		timer1.start();

		for(int i = 0; i < 1000; ++i){}
		
		timer1.stop();

		timer1.sleep();
		//usleep(10000);

		timer2.stop();
		cout << timer2.get_interval() << endl;
	}
	return 0;
}
