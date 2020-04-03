#include<stdio.h>
#include"common.h"
#include<slave.h>
#include<dma.h>
#define myLength 650 
__thread_local int cpe_id;
__thread_local volatile unsigned long get_reply[2], put_reply[2];
__thread_local double a0_slave_0[2][myLength+2], a0_slave_3[2][myLength], a0_slave_4[2][myLength], a0_slave_5[2][myLength], a0_slave_6[2][myLength], a1_slave_0[2][myLength];
extern ptr_t  a1;
extern cptr_t a0;
extern int x_end,y_end,z_end;
void func_7()
{
	cpe_id = athread_get_id(-1);
        int x_y_dim=x_end*y_end;
        int Len,myDis,myStart,time;
        if(z_end==14){
         Len = x_y_dim *(z_end-4);
         myDis = Len / 64;
         myStart = cpe_id * (myDis)+2*x_y_dim;
        if (cpe_id < Len % 64)
        {
                myStart += cpe_id;
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
        myStart = cpe_id * (myDis)+x_y_dim;
        if (cpe_id < Len % 64)
        {
                myStart += cpe_id;
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
        myStart = cpe_id * (myDis)+12*x_y_dim;
        if (cpe_id < Len % 64)
        {
                myStart += cpe_id;
                myDis++;
        }
        else
        {
                myStart += Len % 64;
        }
        time = myDis / myLength;

        }
		
        int t, i, index, last, next;
        int ii,ii_next,sj;
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
		dma_descriptor_init(&get_desc, get_reply[index]);
                if (t == 0)
                {
                        get_reply[index] = 0;
			dma_set_reply(&get_desc,&get_reply[index]);
			dma_set_size(&get_desc,(myLength+2)*8);
			dma(get_desc,&a0[ii - 1],&a0_slave_0[index][0]);
			dma_set_size(&get_desc,myLength*8);
			dma(get_desc,&a0[ii - x_end],&a0_slave_3[index][0]);
			dma(get_desc,&a0[ii + x_end],&a0_slave_4[index][0]);
			dma(get_desc,&a0[ii - x_y_dim],&a0_slave_5[index][0]);
			dma(get_desc,&a0[ii + x_y_dim],&a0_slave_6[index][0]);
                }
                if (t < time - 1)
                {
                        get_reply[next] = 0;
			dma_set_reply(&get_desc,&get_reply[next]);
			dma_set_size(&get_desc,(myLength+2)*8);
			dma(get_desc,&a0[ii_next - 1],&a0_slave_0[next][0]);
			dma_set_size(&get_desc,myLength*8);
			dma(get_desc,&a0[ii_next - x_end],&a0_slave_3[next][0]);
			dma(get_desc,&a0[ii_next + x_end],&a0_slave_4[next][0]);
			dma(get_desc,&a0[ii_next - x_y_dim],&a0_slave_5[next][0]);
			dma(get_desc,&a0[ii_next + x_y_dim],&a0_slave_6[next][0]);
}
		dma_wait(&get_reply[index],5);
                for (i = 0; i < myLength; i++)
                {

                                a1_slave_0[index][i] = ALPHA_ZZZ * a0_slave_0[index][i+1] + ALPHA_NZZ * a0_slave_0[index][i] + ALPHA_PZZ * a0_slave_0[index][i+2] + ALPHA_ZNZ * a0_slave_3[index][i] + ALPHA_ZPZ * a0_slave_4[index][i] + ALPHA_ZZN * a0_slave_5[index][i] + ALPHA_ZZP * a0_slave_6[index][i];
 
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

	sj=time*myLength+myStart;
        get_reply[0] = 0;
	dma_set_reply(&get_desc,&get_reply[0]);
	dma_set_size(&get_desc,(myDis%myLength+2)*8);
	dma(get_desc,&a0[sj - 1],&a0_slave_0[0][0]);
	dma_set_size(&get_desc,(myDis%myLength)*8);
	dma(get_desc,&a0[sj - x_end],&a0_slave_3[0][0]);
	dma(get_desc,&a0[sj + x_end],&a0_slave_4[0][0]);
	dma(get_desc,&a0[sj - x_y_dim],&a0_slave_5[0][0]);
	dma(get_desc,&a0[sj + x_y_dim],&a0_slave_6[0][0]);
	dma_wait(&get_reply[0],5);
        for (i = 0; i < (myDis%myLength); i++)
        {
                        a1_slave_0[0][i] = ALPHA_ZZZ * a0_slave_0[0][i+1] + ALPHA_NZZ * a0_slave_0[0][i] + ALPHA_PZZ * a0_slave_0[0][i+2] + ALPHA_ZNZ * a0_slave_3[0][i] + ALPHA_ZPZ * a0_slave_4[0][i] + ALPHA_ZZN * a0_slave_5[0][i] + ALPHA_ZZP * a0_slave_6[0][i];
        }
        put_reply[0] = 0;
	dma_set_size(&put_desc,(myDis%myLength) * 8);
	dma_set_reply(&put_desc,&put_reply[0]);
	dma(put_desc,&a1[time * myLength + myStart],&a1_slave_0[0][0]);	
	dma_wait(&put_reply[0],1);


}

