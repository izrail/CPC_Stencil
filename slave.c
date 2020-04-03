#include<stdio.h>
#include"common.h"
#include<slave.h>
#define myLength 250
__thread_local int my_id;
__thread_local volatile unsigned long get_reply[2], put_reply[2];
__thread_local volatile unsigned long get_reply1, put_reply1;
__thread_local double a0_slave_00[myLength], a0_slave_11[myLength], a0_slave_22[myLength], a0_slave_33[myLength], a0_slave_44[myLength], a0_slave_55[myLength], a0_slave_66[myLength], a1_slave_00[myLength];
__thread_local double a0_slave_0[2][myLength], a0_slave_1[2][myLength], a0_slave_2[2][myLength], a0_slave_3[2][myLength], a0_slave_4[2][myLength], a0_slave_5[2][myLength], a0_slave_6[2][myLength], a1_slave_0[2][myLength];
extern ptr_t  a1;
extern cptr_t a0;
extern int x_end,y_end,z_end;
void func_7()
{
	my_id = athread_get_id(-1);	
	int x_y_dim=x_end*y_end;
        int Len = x_y_dim *(z_end-2);/*7114800*/
	int myDis = Len / 64;/*111168*/
	int myStart = my_id * (myDis)+x_y_dim;
	if (my_id < Len % 64)/*余数为56*/
	{
		myStart += my_id;
		myDis++;
	}
	else
	{
		myStart += Len % 64;
	}
	/*myLength一次DMA的长度*/
	int time = myDis / myLength;/*循环次数*/
	int t, i, index, last, next;
	int ii;
	int ii_next;
	for (t = 0; t < time; t++)
	{
			int ii=t*myLength +myStart;
	int ii_next=(t+1)*myLength +myStart;

		index = t % 2;
		next = (t + 1) % 2;
		last = (t - 1) % 2;
		if (t == 0)
		{
			get_reply[index] = 0;
			athread_get(PE_MODE, &a0[ii], &a0_slave_0[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii - 1], &a0_slave_1[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii + 1], &a0_slave_2[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii - x_end], &a0_slave_3[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii + x_end], &a0_slave_4[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii - x_y_dim], &a0_slave_5[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii + x_y_dim], &a0_slave_6[index][0], myLength * 8, &get_reply[index], 0, 0, 0);
		}
		if (t < time - 1)
		{
			get_reply[next] = 0;
			athread_get(PE_MODE, &a0[ii_next], &a0_slave_0[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii_next - 1], &a0_slave_1[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii_next + 1], &a0_slave_2[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii_next - x_end], &a0_slave_3[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii_next + x_end], &a0_slave_4[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii_next - x_y_dim], &a0_slave_5[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
			athread_get(PE_MODE, &a0[ii_next + x_y_dim], &a0_slave_6[next][0], myLength * 8, &get_reply[next], 0, 0, 0);
}
		while (get_reply[index] != 7);
		for (i = 0; i < myLength; i++)
		{
				a1_slave_0[index][i] = ALPHA_ZZZ * a0_slave_0[index][i] + ALPHA_NZZ * a0_slave_1[index][i] + ALPHA_PZZ * a0_slave_2[index][i] + ALPHA_ZNZ * a0_slave_3[index][i] + ALPHA_ZPZ * a0_slave_4[index][i] + ALPHA_ZZN * a0_slave_5[index][i] + ALPHA_ZZP * a0_slave_6[index][i];
		}
		put_reply[index] = 0;
		athread_put(PE_MODE,  &a1_slave_0[index][0], &a1[ii], myLength * 8, &put_reply[index], 0, 0);
		if (t > 0) {
			while (put_reply[last] != 1);
		}

		if (t == time - 1)
		{
			while (put_reply[index] != 1);
		}
	}

	get_reply1 = 0;
	athread_get(PE_MODE, &a0[time*myLength + myStart], &a0_slave_00[0], (myDis%myLength)*8, &get_reply1, 0, 0,0);
		athread_get(PE_MODE, &a0[time*myLength + myStart - 1], &a0_slave_11[0], (myDis%myLength) * 8, &get_reply1, 0, 0, 0);
		athread_get(PE_MODE, &a0[time*myLength + myStart + 1], &a0_slave_22[0], (myDis%myLength) * 8, &get_reply1, 0, 0, 0);
		athread_get(PE_MODE, &a0[time*myLength + myStart - x_end], &a0_slave_33[0], (myDis%myLength) * 8, &get_reply1, 0, 0, 0);
		athread_get(PE_MODE, &a0[time*myLength + myStart + x_end], &a0_slave_44[0], (myDis%myLength) * 8, &get_reply1, 0, 0, 0);
		athread_get(PE_MODE, &a0[time*myLength + myStart - x_y_dim], &a0_slave_55[0], (myDis%myLength) * 8, &get_reply1, 0, 0, 0);
		athread_get(PE_MODE, &a0[time*myLength + myStart + x_y_dim], &a0_slave_66[0], (myDis%myLength) * 8, &get_reply1, 0, 0, 0);
	while (get_reply1 != 7);
	for (i = 0; i < (myDis%myLength); i++)
	{
			a1_slave_00[i] = ALPHA_ZZZ * a0_slave_00[i] + ALPHA_NZZ * a0_slave_11[i] + ALPHA_PZZ * a0_slave_22[i] + ALPHA_ZNZ * a0_slave_33[i] + ALPHA_ZPZ * a0_slave_44[i] + ALPHA_ZZN * a0_slave_55[i] + ALPHA_ZZP * a0_slave_66[i];
	}
	put_reply1 = 0;
	athread_put(PE_MODE, &a1_slave_00[0], &a1[time*myLength + myStart],(myDis%myLength) * 8, &put_reply1, 0, 0);
	while (put_reply1 != 1);
}

