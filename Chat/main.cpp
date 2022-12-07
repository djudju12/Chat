#include "MultiClientChat.h"

void main()
{
	MultiClientChat mcc("192.168.0.126", 54000);
	if (mcc.init() != 0)
		return;

	mcc.run();

	system("pause");
}
