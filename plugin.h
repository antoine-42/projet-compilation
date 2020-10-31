#ifndef CODE_PLUGIN_H
#define CODE_PLUGIN_H

#include <gcc-plugin.h>
#include <tree.h>
#include <tree-pass.h>
#include <context.h>
#include <function.h>
#include <basic-block.h>
#include <gimple.h>
#include <gimple-iterator.h>
#include <bitmap.h>
#include <c-family/c-pragma.h>
#include <stdexcept>

int plugin_is_GPL_compatible;

/* Enum to represent the collective operations */
enum mpi_collective_code {
#define DEFMPICOLLECTIVES( CODE, NAME ) CODE,
#include "MPI_collectives.def"
    LAST_AND_UNUSED_MPI_COLLECTIVE_CODE
#undef DEFMPICOLLECTIVES
} ;

/* Name of each MPI collective operations */
#define DEFMPICOLLECTIVES( CODE, NAME ) NAME,
const char *const mpi_collective_name[] = {
#include "MPI_collectives.def"
} ;

#undef DEFMPICOLLECTIVES

// 0 if the MPI collective is not monitored, 1 if it is. Order is from the MPI collective enum.
int MONITORED_MPI_COLLECTIVES[LAST_AND_UNUSED_MPI_COLLECTIVE_CODE] = {0};

#endif //CODE_PLUGIN_H
