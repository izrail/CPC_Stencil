#include<stdio.h>
#include"common.h"
#include<slave.h>
#include<dma.h>
#define myLength 380 
__thread_local int my_id;
__thread_local volatile unsigned long get_reply[2], put_reply[2];
__thread_local double a0_slave_0[2][myLength + 2] ,a0_slave_3[2][myLength +2], a0_slave_4[2][myLength + 2],a0_slave_5[2][myLength + 2],a0_slave_6[2][myLength + 2],a0_slave_15[2][myLength + 2],a0_slave_16[2][myLength + 2],a0_slave_17[2][myLength + 2],a0_slave_18[2][myLength + 2],a1_slave_0[2][myLength];
extern ptr_t  a1;
extern cptr_t a0;
extern int x_end,y_end,z_end;
void func_27()
{
	my_id = athread_get_id(-1);
        int x_y_dim=x_end*y_end;
        int Len,myDis,myStart,time;
        if(z_end==14){
         Len = x_y_dim *(z_end-4);
         myDis = Len / 64;
         myStart = my_id * (myDis)+2*x_y_dim;
        if (my_id < Len % 64)
        {
                myStart += my_id;
                myDis++;
        }
        else
        {
                myStart += Len % 64;
        }
        time = myDis / myLength;
        }
        else if(z_end==1){
        Len = x_y_dim ;
        myDis = Len / 64;
        myStart = my_id * (myDis)+x_y_dim;
        if (my_id < Len % 64)
        {
                myStart += my_id;
                myDis++;
        }
        else
        {
                myStart += Len % 64;
        }
        time = myDis / myLength;

        }
        else if(z_end==12){
        Len = x_y_dim ;
        myDis = Len / 64;
        myStart = my_id * (myDis)+12*x_y_dim;
        if (my_id < Len % 64)
        {
                myStart += my_id;
                myDis++;
        }
        else
        {
                myStart += Len % 64;
        }
        time = myDis / myLength;

        }

        int t, i, index, last, next;
        int ii;
        int ii_next;
	dma_desc get_desc;
        dma_descriptor_init(&get_desc,0);

        dma_desc put_desc;
        dma_descriptor_init(&put_desc,0);
        dma_set_op(&put_desc,DMA_PUT);

        for (t = 0; t < time; t++)
        {
                ii=t*myLength +myStart;
                ii_next=(t+1)*myLength +myStart;
                index = t % 2;
                next = (t + 1) % 2;
                last = (t - 1) % 2;

                if (t == 0)
                {
                        get_reply[index] = 0;
			dma_set_reply(&get_desc,&get_reply[index]);
			dma_set_size(&get_desc,(myLength+2)*8);
			dma(get_desc,&a0[ii - 1],&a0_slave_0[index][0]);
			dma(get_desc,&a0[ii - 1 - x_end],&a0_slave_3[index][0]);
			dma(get_desc,&a0[ii - 1 + x_end],&a0_slave_4[index][0]);
			dma(get_desc,&a0[ii - 1 - x_y_dim],&a0_slave_5[index][0]);
			dma(get_desc,&a0[ii - 1 + x_y_dim],&a0_slave_6[index][0]);
			dma(get_desc,&a0[ii - 1 - x_end - x_y_dim],&a0_slave_15[index][0]);
			dma(get_desc,&a0[ii - 1 + x_end - x_y_dim],&a0_slave_16[index][0]);
			dma(get_desc,&a0[ii - 1 - x_end + x_y_dim],&a0_slave_17[index][0]);
			dma(get_desc,&a0[ii - 1 + x_end + x_y_dim],&a0_slave_18[index][0]);
                    }
                if (t < time - 1)
                {
                        get_reply[next] = 0;
			dma_set_reply(&get_desc,&get_reply[next]);
                        dma_set_size(&get_desc,(myLength+2)*8);
                        dma(get_desc,&a0[ii_next - 1],&a0_slave_0[next][0]);
			dma(get_desc,&a0[ii_next - 1 - x_end],&a0_slave_3[next][0]);
			dma(get_desc,&a0[ii_next - 1 + x_end],&a0_slave_4[next][0]);
			dma(get_desc,&a0[ii_next - 1 - x_y_dim],&a0_slave_5[next][0]);
			dma(get_desc,&a0[ii_next - 1 + x_y_dim],&a0_slave_6[next][0]);
			dma(get_desc,&a0[ii_next - 1 - x_end - x_y_dim],&a0_slave_15[next][0]);
			dma(get_desc,&a0[ii_next - 1 + x_end - x_y_dim],&a0_slave_16[next][0]);
			dma(get_desc,&a0[ii_next - 1 - x_end + x_y_dim],&a0_slave_17[next][0]);
			dma(get_desc,&a0[ii_next - 1 + x_end + x_y_dim],&a0_slave_18[next][0]);
}
		dma_wait(&get_reply[index],9);
                for (i = 0; i < myLength; i++)
                {
               		a1_slave_0[index][i] = ALPHA_ZZZ * a0_slave_0[index][i + 1] + ALPHA_NZZ * a0_slave_0[index][i] + ALPHA_PZZ * a0_slave_0[index][i + 2] + ALPHA_ZNZ * a0_slave_3[index][i +1] + ALPHA_ZPZ * a0_slave_4[index][i + 1] + ALPHA_ZZN * a0_slave_5[index][i+1] + ALPHA_ZZP * a0_slave_6[index][i+1] + ALPHA_NNZ * a0_slave_3[index][i] + ALPHA_PNZ * a0_slave_3[index][i +2] + ALPHA_NPZ * a0_slave_4[index][i] + ALPHA_PPZ * a0_slave_4[index][i+2] + ALPHA_NZN * a0_slave_5[index][i] + ALPHA_PZN * a0_slave_5[index][i+2] + ALPHA_NZP * a0_slave_6[index][i] + ALPHA_PZP * a0_slave_6[index][i+2] + ALPHA_ZNN * a0_slave_15[index][i+1] + ALPHA_ZPN * a0_slave_16[index][i+1] + ALPHA_ZNP * a0_slave_17[index][i+1] + ALPHA_ZPP * a0_slave_18[index][i+1] + ALPHA_NNN * a0_slave_15[index][i] + ALPHA_PNN * a0_slave_15[index][i +2] + ALPHA_NPN * a0_slave_16[index][i] + ALPHA_PPN *a0_slave_16[index][i+2] + ALPHA_NNP * a0_slave_17[index][i] + ALPHA_PNP * a0_slave_17[index][i+2] + ALPHA_NPP * a0_slave_18[index][i] + ALPHA_PPP * a0_slave_18[index][i+2];
                }
                put_reply[index] = 0;
		dma_set_size(&put_desc,myLength*8);
                dma_set_reply(&put_desc,&put_reply[index]);
		dma(put_desc,&a1[ii],&a1_slave_0[index][0]);
                if (t > 0) {
			dma_wait(&put_reply[last],1);
                }

                if (t == time - 1)
                {
			dma_wait(&put_reply[index],1);
                }
        }

	      int sj=time*myLength+myStart;
        get_reply[0] = 0;
	dma_set_reply(&get_desc,&get_reply[0]);
        dma_set_size(&get_desc,(myDis%myLength+2)*8);
	dma(get_desc,&a0[sj - 1],&a0_slave_0[0][0]);
	dma(get_desc,&a0[sj - 1 - x_end],&a0_slave_3[0][0]);
	dma(get_desc,&a0[sj - 1 + x_end],&a0_slave_4[0][0]);
	dma(get_desc,&a0[sj - 1 - x_y_dim],&a0_slave_5[0][0]);
	dma(get_desc,&a0[sj - 1 + x_y_dim],&a0_slave_6[0][0]);
	dma(get_desc,&a0[sj - 1 - x_end - x_y_dim],&a0_slave_15[0][0]);
	dma(get_desc,&a0[sj - 1 + x_end - x_y_dim],&a0_slave_16[0][0]);
	dma(get_desc,&a0[sj - 1 - x_end + x_y_dim],&a0_slave_17[0][0]);
	dma(get_desc,&a0[sj - 1 + x_end + x_y_dim],&a0_slave_18[0][0]);
	
/* 	athread_get(PE_MODE, &a0[sj - 1], &a0_slave_0[0][0], (myDis%myLength +2) * 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE,&a0[sj - 1 - x_end], &a0_slave_3[0][0],(myDis%myLength +2) * 8,&get_reply[0],0,0,0);
       	athread_get(PE_MODE, &a0[sj - 1 + x_end], &a0_slave_4[0][0], (myDis%myLength +2) * 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE, &a0[sj - 1 - x_y_dim], &a0_slave_5[0][0], (myDis%myLength +2) * 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE, &a0[sj - 1 + x_y_dim], &a0_slave_6[0][0], (myDis%myLength +2) * 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE, &a0[sj - 1 - x_end - x_y_dim], &a0_slave_15[0][0], (myDis%myLength +2) * 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE, &a0[sj - 1 + x_end - x_y_dim], &a0_slave_16[0][0], (myDis%myLength +2)* 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE, &a0[sj - 1 - x_end + x_y_dim], &a0_slave_17[0][0], (myDis%myLength +2)* 8, &get_reply[0], 0, 0, 0);
       	athread_get(PE_MODE, &a0[sj - 1 + x_end + x_y_dim], &a0_slave_18[0][0], (myDis%myLength +2)* 8, &get_reply[0], 0, 0, 0);

        while (get_reply[0] != 9);
*/
	dma_wait(&get_reply[0],9);
        for (i = 0; i < (myDis%myLength); i++)
        {
		a1_slave_0[0][i] = ALPHA_ZZZ * a0_slave_0[0][i + 1] + ALPHA_NZZ * a0_slave_0[0][i] + ALPHA_PZZ * a0_slave_0[0][i + 2] + ALPHA_ZNZ * a0_slave_3[0][i +1] + ALPHA_ZPZ * a0_slave_4[0][i + 1] + ALPHA_ZZN * a0_slave_5[0][i+1] + ALPHA_ZZP * a0_slave_6[0][i+1] + ALPHA_NNZ * a0_slave_3[0][i] + ALPHA_PNZ * a0_slave_3[0][i +2] + ALPHA_NPZ * a0_slave_4[0][i] + ALPHA_PPZ * a0_slave_4[0][i+2] + ALPHA_NZN * a0_slave_5[0][i] + ALPHA_PZN * a0_slave_5[0][i+2] + ALPHA_NZP * a0_slave_6[0][i] + ALPHA_PZP * a0_slave_6[0][i+2] + ALPHA_ZNN * a0_slave_15[0][i+1] + ALPHA_ZPN * a0_slave_16[0][i+1] + ALPHA_ZNP * a0_slave_17[0][i+1] + ALPHA_ZPP * a0_slave_18[0][i+1] + ALPHA_NNN * a0_slave_15[0][i] + ALPHA_PNN * a0_slave_15[0][i +2] + ALPHA_NPN * a0_slave_16[0][i] + ALPHA_PPN *a0_slave_16[0][i+2] + ALPHA_NNP * a0_slave_17[0][i] + ALPHA_PNP * a0_slave_17[0][i+2] + ALPHA_NPP * a0_slave_18[0][i] + ALPHA_PPP * a0_slave_18[0][i+2];
        }
        put_reply[0] = 0;
	dma_set_size(&put_desc,(myDis%myLength)*8);
	dma_set_reply(&put_desc,&put_reply[0]);
	dma(put_desc,&a1[time *myLength +myStart],&a1_slave_0[0][0]);
	dma_wait(&put_reply[0],1);
       /* athread_put(PE_MODE, &a1_slave_0[0], &a1[time*myLength + myStart],(myDis%myLength) * 8, &put_reply[0], 0, 0);
        while (put_reply[0] != 1);
*/
}
