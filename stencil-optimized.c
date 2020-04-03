#include <stdio.h>
#include <stdlib.h>
#include<mpi.h>
#include<athread.h>
#include "common.h"

extern void SLAVE_FUN(func_7)();
/*void slave_stencil_27();
void slave_stencil_7();*/
const char* version_name = "Optimized version";

/* your implementation */
cptr_t a0;
ptr_t a1;
int x_end,y_end,z_end;
void create_dist_grid(dist_grid_info_t *grid_info, int stencil_type) {
	/*sovle 64process problem*/
	grid_info->local_size_x = grid_info->global_size_x;
	grid_info->local_size_y = grid_info->global_size_y;
	grid_info->local_size_z = grid_info->global_size_z/16;/*按照z轴切割64次*/

	grid_info->offset_x = 0;
	grid_info->offset_y = 0;
	grid_info->offset_z = (grid_info->global_size_z / 16)*grid_info->p_id;/*切割后的偏移量*/
	grid_info->halo_size_x = 1;
	grid_info->halo_size_y = 1;
	grid_info->halo_size_z = 1;
	athread_init();
	//athread_enter64();
}

void destroy_dist_grid(dist_grid_info_t *grid_info) {
	athread_halt();
}

ptr_t stencil_7(ptr_t grid, ptr_t aux, const dist_grid_info_t *grid_info, int nt) {
	MPI_Status status;
	ptr_t buffer[2] = { grid, aux };
	x_end = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	y_end = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;
	int ldx = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	int ldy = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	int ldz = grid_info->local_size_z + 2 * grid_info->halo_size_z;
	for (int t = 0; t < nt; ++t) {
		a0 = buffer[t % 2];
		a1 = buffer[(t + 1) % 2];
		/*每次迭代更新holo的数*/
		if (grid_info->p_id % 2 == 0)
		{
			MPI_Send(&a0[INDEX(0, 0, z_end - 2, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id+1, 0, MPI_COMM_WORLD);
			MPI_Recv(&a0[INDEX(0, 0, z_end - 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD, &status);
		}
		else
		{
			MPI_Recv(&a0[INDEX(0, 0, 0, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id-1, 0, MPI_COMM_WORLD, &status);
			MPI_Send(&a0[INDEX(0, 0, 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD);
		}
		if (grid_info->p_id != 0 && grid_info->p_id != 15)
		{
			if (grid_info->p_id % 2 == 1)
			{
				MPI_Send(&a0[INDEX(0, 0, z_end - 2, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&a0[INDEX(0, 0, z_end - 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD, &status);
			}
			else
			{
				MPI_Recv(&a0[INDEX(0, 0, 0, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD, &status);
				MPI_Send(&a0[INDEX(0, 0, 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD);
			}
		}
			athread_spawn(func_7, 0);
			athread_join();

			//set_zero(a1 + INDEX(0, 0, 0, ldx, ldy), x_end*y_end*grid_info->halo_size_z);
			for (int z = 1; z < z_end-1; ++z) {
				set_zero(a1 + INDEX(0, 0, z, ldx, ldy), x_end*grid_info->halo_size_y);
				for (int y = 1; y < y_end-1; ++y) {
					set_zero(a1 + INDEX(0, y, z, ldx,ldy), grid_info->halo_size_x);
					set_zero(a1 + INDEX(x_end-1, y, z, ldx, ldy), grid_info->halo_size_x);
				}
				set_zero(a1 + INDEX(0, y_end-1, z, ldx, ldy), x_end*grid_info->halo_size_y);
			}

			//set_zero(a1 + INDEX(0, 0, z_end-1, ldx, ldy), x_end*y_end*grid_info->halo_size_z);

	}
			
	
	return buffer[nt % 2];
}

/* your implementation */
ptr_t stencil_27(ptr_t grid, ptr_t aux, const dist_grid_info_t *grid_info, int nt) {
	MPI_Status status;
	ptr_t buffer[2] = { grid, aux };
	int x_start = 0, x_end = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	int y_start = 0, y_end = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	int z_start = 0, z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;

	int ldx = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	int ldy = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	int ldz = grid_info->local_size_z + 2 * grid_info->halo_size_z;
	for (int t = 0; t < nt; ++t) {
		cptr_t a0 = buffer[t % 2];
		ptr_t a1 = buffer[(t + 1) % 2];
		if (grid_info->p_id % 2 == 0)
		{
			MPI_Send(&a0[INDEX(0, 0, z_end - 2, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&a0[INDEX(0, 0, z_end - 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD, &status);
		}
		else
		{
			MPI_Recv(&a0[INDEX(0, 0, 0, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD, &status);
			MPI_Send(&a0[INDEX(0, 0, 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD);
		}
		if (grid_info->p_id != 0 && grid_info->p_id != 63)
		{
			if (grid_info->p_id % 2 == 1)
			{
				MPI_Send(&a0[INDEX(0, 0, z_end - 2, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&a0[INDEX(0, 0, z_end - 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id + 1, 0, MPI_COMM_WORLD, &status);
			}
			else
			{
				MPI_Recv(&a0[INDEX(0, 0, 0, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD, &status);
				MPI_Send(&a0[INDEX(0, 0, 1, ldx, ldy)], x_end*y_end, DATA_TYPE, grid_info->p_id - 1, 0, MPI_COMM_WORLD);
			}
		}
		/*athread_init();*/
		/*athread_spawn(slave_stencil_27,0);*/
		/*a1=slave_stencil_27(a0, a1, x_end, y_end, z_end);*/
		/*athread_join();*/
		
	}
	return buffer[nt % 2];
}
