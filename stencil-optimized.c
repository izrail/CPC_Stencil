#include <stdio.h>
#include <stdlib.h>
#include<mpi.h>
#include "common.h"

const char* version_name = "Optimized version";

/* your implementation */

void create_dist_grid(dist_grid_info_t *grid_info, int stencil_type) {
	/*sovle 16process problem*/
	grid_info->local_size_x = grid_info->global_size_x;
	grid_info->local_size_y = grid_info->global_size_y;
	grid_info->local_size_z = grid_info->global_size_z/16;/*按照z轴切割16次*/

	grid_info->offset_x = 0;
	grid_info->offset_y = 0;
	grid_info->offset_z = (grid_info->global_size_z / 16)*grid_info->p_id;/*切割后的偏移量*/
	grid_info->halo_size_x = 1;
	grid_info->halo_size_y = 1;
	grid_info->halo_size_z = 1;
}

void destroy_dist_grid(dist_grid_info_t *grid_info) {

}

ptr_t stencil_7(ptr_t grid, ptr_t aux, const dist_grid_info_t *grid_info, int nt) {
	MPI_Status status;
	ptr_t buffer[2] = { grid, aux };
	int x_start = 0, x_end = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	int y_start = 0, y_end = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	int z_start = 0, z_end = grid_info->local_size_z + 2 * grid_info->halo_size_z;

	int ldx = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	int ldy = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	int ldz = grid_info->local_size_z + 2 * grid_info->halo_size_z;

	/*int x_start = grid_info->halo_size_x, x_end = grid_info->local_size_x + grid_info->halo_size_x;
	int y_start = grid_info->halo_size_y, y_end = grid_info->local_size_y + grid_info->halo_size_y;
	int z_start = grid_info->halo_size_z, z_end = grid_info->local_size_z + grid_info->halo_size_z;
	int ldx = grid_info->local_size_x + 2 * grid_info->halo_size_x;
	int ldy = grid_info->local_size_y + 2 * grid_info->halo_size_y;
	int ldz = grid_info->local_size_z + 2 * grid_info->halo_size_z;*/

	for (int t = 0; t < nt; ++t) {
		cptr_t a0 = buffer[t % 2];
		ptr_t a1 = buffer[(t + 1) % 2];
		printf("%d", grid_info->p_id);
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

		for (int z = z_start+1; z < z_end-1; ++z) {
			for (int y = y_start+1; y < y_end-1; ++y) {
				for (int x = x_start+1; x < x_end-1; ++x) {
					a1[INDEX(x, y, z, ldx, ldy)] \
						= ALPHA_ZZZ * a0[INDEX(x, y, z, ldx, ldy)] \
						+ ALPHA_NZZ * a0[INDEX(x - 1, y, z, ldx, ldy)] \
						+ ALPHA_PZZ * a0[INDEX(x + 1, y, z, ldx, ldy)] \
						+ ALPHA_ZNZ * a0[INDEX(x, y - 1, z, ldx, ldy)] \
						+ ALPHA_ZPZ * a0[INDEX(x, y + 1, z, ldx, ldy)] \
						+ ALPHA_ZZN * a0[INDEX(x, y, z - 1, ldx, ldy)] \
						+ ALPHA_ZZP * a0[INDEX(x, y, z + 1, ldx, ldy)];
				}
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
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

		for (int z = z_start+1; z < z_end-1; ++z) {
			for (int y = y_start+1; y < y_end-1; ++y) {
				for (int x = x_start+1; x < x_end-1; ++x) {
					a1[INDEX(x, y, z, ldx, ldy)] \
						= ALPHA_ZZZ * a0[INDEX(x, y, z, ldx, ldy)] \
						+ ALPHA_NZZ * a0[INDEX(x - 1, y, z, ldx, ldy)] \
						+ ALPHA_PZZ * a0[INDEX(x + 1, y, z, ldx, ldy)] \
						+ ALPHA_ZNZ * a0[INDEX(x, y - 1, z, ldx, ldy)] \
						+ ALPHA_ZPZ * a0[INDEX(x, y + 1, z, ldx, ldy)] \
						+ ALPHA_ZZN * a0[INDEX(x, y, z - 1, ldx, ldy)] \
						+ ALPHA_ZZP * a0[INDEX(x, y, z + 1, ldx, ldy)] \
						+ ALPHA_NNZ * a0[INDEX(x - 1, y - 1, z, ldx, ldy)] \
						+ ALPHA_PNZ * a0[INDEX(x + 1, y - 1, z, ldx, ldy)] \
						+ ALPHA_NPZ * a0[INDEX(x - 1, y + 1, z, ldx, ldy)] \
						+ ALPHA_PPZ * a0[INDEX(x + 1, y + 1, z, ldx, ldy)] \
						+ ALPHA_NZN * a0[INDEX(x - 1, y, z - 1, ldx, ldy)] \
						+ ALPHA_PZN * a0[INDEX(x + 1, y, z - 1, ldx, ldy)] \
						+ ALPHA_NZP * a0[INDEX(x - 1, y, z + 1, ldx, ldy)] \
						+ ALPHA_PZP * a0[INDEX(x + 1, y, z + 1, ldx, ldy)] \
						+ ALPHA_ZNN * a0[INDEX(x, y - 1, z - 1, ldx, ldy)] \
						+ ALPHA_ZPN * a0[INDEX(x, y + 1, z - 1, ldx, ldy)] \
						+ ALPHA_ZNP * a0[INDEX(x, y - 1, z + 1, ldx, ldy)] \
						+ ALPHA_ZPP * a0[INDEX(x, y + 1, z + 1, ldx, ldy)] \
						+ ALPHA_NNN * a0[INDEX(x - 1, y - 1, z - 1, ldx, ldy)] \
						+ ALPHA_PNN * a0[INDEX(x + 1, y - 1, z - 1, ldx, ldy)] \
						+ ALPHA_NPN * a0[INDEX(x - 1, y + 1, z - 1, ldx, ldy)] \
						+ ALPHA_PPN * a0[INDEX(x + 1, y + 1, z - 1, ldx, ldy)] \
						+ ALPHA_NNP * a0[INDEX(x - 1, y - 1, z + 1, ldx, ldy)] \
						+ ALPHA_PNP * a0[INDEX(x + 1, y - 1, z + 1, ldx, ldy)] \
						+ ALPHA_NPP * a0[INDEX(x - 1, y + 1, z + 1, ldx, ldy)] \
						+ ALPHA_PPP * a0[INDEX(x + 1, y + 1, z + 1, ldx, ldy)];
				}
			}
		}
	}
	return buffer[nt % 2];
}
