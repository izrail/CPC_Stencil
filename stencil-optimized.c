#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <athread.h>
#include "common.h"

extern void SLAVE_FUN(func_7)();
extern void SLAVE_FUN(func_27)();
const char* version_name = "Optimized version";
/* your implementation */
cptr_t a0;
ptr_t a1;
int x_end,y_end,z_end;
void create_dist_grid(dist_grid_info_t *grid_info, int stencil_type) {
	/*sovle 64process problem*/
	grid_info->local_size_x = grid_info->global_size_x;
	grid_info->local_size_y = grid_info->global_size_y;
	grid_info->local_size_z = grid_info->global_size_z/64;/*°´ÕÕzÖáÇÐ¸î64´Î*/

	grid_info->offset_x = 0;
	grid_info->offset_y = 0;
	grid_info->offset_z = (grid_info->global_size_z / 64)*grid_info->p_id;/*ÇÐ¸îºóµÄÆ«ÒÆÁ¿*/
	grid_info->halo_size_x = 1;
	grid_info->halo_size_y = 1;
	grid_info->halo_size_z = 1;
	athread_init();
	//athread_enter64();
}

void destroy_dist_grid(dist_grid_info_t *grid_info) {
	//athread_leave64();
	athread_halt();
}

ptr_t stencil_7(ptr_t grid, ptr_t aux, const dist_grid_info_t *grid_info, int nt) {
	MPI_Status status[4];
	MPI_Request request[4];
	ptr_t buffer[2] = { grid, aux };
	x_end = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	y_end = grid_info->local_size_y + 2 * grid_info->halo_size_y;
    z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;
    int x_y_dim=x_end*y_end;
    int z_end_2=(z_end -2)*x_y_dim; 
    int z_end_1=(z_end -1)*x_y_dim;
	for (int t = 0; t < nt; ++t) {
		a0 = buffer[t % 2];
		a1 = buffer[(t + 1) % 2];
	    z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;
        if (grid_info->p_id != 0 && grid_info->p_id != 63)
		{

			MPI_Isend(&a0[z_end_2], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[0]);//send to next process
	        MPI_Irecv(&a0[0], x_y_dim, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD, &request[2]);
			MPI_Isend(&a0[x_y_dim], x_y_dim, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD,&request[3]);//sent to front process
            MPI_Irecv(&a0[z_end_1], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[1]);
		}
		else if(grid_info->p_id==0)
		{
            MPI_Isend(&a0[z_end_2], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[0]);
			MPI_Irecv(&a0[z_end_1], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[1]);
		}
		else if(grid_info->p_id==63)
		{
			MPI_Irecv(&a0[0], x_y_dim, DATA_TYPE, grid_info->p_id-1, 0, MPI_COMM_WORLD, &request[2]);
			MPI_Isend(&a0[x_y_dim], x_y_dim, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD,&request[3]);
		}	
		athread_spawn(func_7, 0);
	    athread_join();
		for (int z = 2; z < z_end-2; ++z) {
			set_zero(a1 +  z*x_y_dim, x_end);
			for (int y = 1; y < y_end-1; ++y) {
                a1[y*x_end+z*x_y_dim]=0;
                a1[x_end-1+y*x_end+z*x_y_dim]=0;
			}
			set_zero(a1 + ( y_end-1)*x_end+z*x_y_dim, x_end);
		}

	        if(grid_info->p_id>0)
        {
            MPI_Wait(&request[2],status);
        }
		z_end=1;
		athread_spawn(func_7, 0);
        athread_join();

			set_zero(a1 +  x_y_dim, x_end);
			for (int y = 1; y < y_end-1; ++y) {
                a1[y*x_end+x_y_dim]=0;
                a1[x_end-1+y*x_end+x_y_dim]=0;
			}
			set_zero(a1 + ( y_end-1)*x_end+x_y_dim, x_end);

        if(grid_info->p_id<63)
        {
          MPI_Wait(&request[1],status); 
        }
		z_end = grid_info->local_size_z ;
			athread_spawn(func_7,0);
            athread_join();

			set_zero(a1 +  grid_info->local_size_z*x_y_dim, x_end);
			for (int y = 1; y < y_end-1; ++y) {
                a1[y*x_end+grid_info->local_size_z*x_y_dim]=0;
                a1[x_end-1+y*x_end+grid_info->local_size_z*x_y_dim]=0;
			}
			set_zero(a1 + ( y_end-1)*x_end+grid_info->local_size_z*x_y_dim, x_end);

	}

	return buffer[nt % 2];
}

ptr_t stencil_27(ptr_t grid, ptr_t aux, const dist_grid_info_t *grid_info, int nt) {
	MPI_Status status[4];
	MPI_Request request[4];
	ptr_t buffer[2] = { grid, aux };
x_end = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	y_end = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;
    int x_y_dim=x_end*y_end;
    int z_end_2=(z_end -2)*x_y_dim; 
    int z_end_1=(z_end -1)*x_y_dim;
	for (int t = 0; t < nt; ++t) {
		a0 = buffer[t % 2];
		a1 = buffer[(t + 1) % 2];
		if (grid_info->p_id != 0 && grid_info->p_id != 63)
		{

			MPI_Isend(&a0[z_end_2], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[0]);//send to next process
	        MPI_Irecv(&a0[0], x_y_dim, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD, &request[2]);
			MPI_Isend(&a0[x_y_dim], x_y_dim, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD,&request[3]);//sent to front process
            MPI_Irecv(&a0[z_end_1], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[1]);
		}
		else if(grid_info->p_id==0)
		{
            MPI_Isend(&a0[z_end_2], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[0]);
			MPI_Irecv(&a0[z_end_1], x_y_dim, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD,&request[1]);
		}
		else if(grid_info->p_id==63)
		{
			MPI_Irecv(&a0[0], x_y_dim, DATA_TYPE, grid_info->p_id-1, 0, MPI_COMM_WORLD, &request[2]);
			MPI_Isend(&a0[x_y_dim], x_y_dim, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD,&request[3]);
		}	
	//	athread_spawn(func_27, 0);
	//	athread_join();
       if(grid_info->p_id>0)
        {
            MPI_Wait(&request[2],status);
        }
		z_end=1;
	//	athread_spawn(func_27, 0);
	//	athread_join();
        if(grid_info->p_id<63)
        {
           MPI_Wait(&request[1],status); 
        }

		z_end = grid_info->local_size_z ;
	//	athread_spawn(func_27,0);
	//	athread_join();
		z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;
	for (int z = 1; z < z_end-1; ++z) {
            //memset(&a1[z*x_y_dim],0,x_end);
			set_zero(a1 +  z*x_y_dim, x_end);
			for (int y = 1; y < y_end-1; ++y) {
                a1[y*x_end+z*x_y_dim]=0;
                a1[x_end-1+y*x_end+z*x_y_dim]=0;
			}
			set_zero(a1 + ( y_end-1)*x_end+z*x_y_dim, x_end);
		}


	}
	return buffer[nt % 2];
}
