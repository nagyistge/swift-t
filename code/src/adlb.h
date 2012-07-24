
#ifndef ADLB_H
#define ADLB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <limits.h>

#include "mpi.h"
#include "adlb-defs.h"

#include "version.h"

#define XLB_VERSION

/*
   These are the functions available to ADLB application code
 */

adlb_code ADLBP_Init(int nservers, int ntypes, int type_vect[],
                     int *am_server, MPI_Comm *worker_comm);
adlb_code ADLB_Init(int nservers, int ntypes, int type_vect[],
                    int *am_server, MPI_Comm *worker_comm);

adlb_code ADLB_Server(long max_memory);

adlb_code ADLB_Version(version* output);

adlb_code ADLBP_Put(void* payload, int length, int target, int answer,
                    int type, int priority);
adlb_code ADLB_Put(void* payload, int length, int target, int answer,
                   int type, int priority);

adlb_code ADLBP_Get(int type_requested, void* payload, int* length,
                    int* answer, int* type_recvd);
adlb_code ADLB_Get(int type_requested, void* payload, int* length,
                   int* answer, int* type_recvd);

// Applications should not call these directly but
// should use the typed forms defined below
adlb_code ADLBP_Create(long id, adlb_data_type type,
                 const char* filename, adlb_data_type subscript_type);
adlb_code ADLB_Create(long id, adlb_data_type type,
                const char* filename, adlb_data_type subscript_type);

adlb_code ADLB_Create_integer(adlb_datum_id id);

adlb_code ADLB_Create_float(adlb_datum_id id);

adlb_code ADLB_Create_string(adlb_datum_id id);

adlb_code ADLB_Create_blob(adlb_datum_id id);

adlb_code ADLB_Create_file(adlb_datum_id id, const char* filename);

adlb_code ADLB_Create_container(adlb_datum_id id, adlb_data_type subscript_type);

adlb_code ADLBP_Exists(adlb_datum_id id, bool* result);
adlb_code ADLB_Exists(adlb_datum_id id, bool* result);

adlb_code ADLBP_Store(adlb_datum_id id, void *data, int length);
adlb_code ADLB_Store(adlb_datum_id id, void *data, int length);

adlb_code ADLBP_Retrieve(adlb_datum_id id, adlb_data_type* type,
		   void *data, int *length);
adlb_code ADLB_Retrieve(adlb_datum_id id, adlb_data_type* type,
		  void *data, int *length);

adlb_code ADLBP_Enumerate(adlb_datum_id container_id,
                   int count, int offset,
                   char** subscripts, int* subscripts_length,
                   char** members, int* members_length,
                   int* records);
adlb_code ADLB_Enumerate(adlb_datum_id container_id,
                   int count, int offset,
                   char** subscripts, int* subscripts_length,
                   char** members, int* members_length,
                   int* records);

adlb_code ADLBP_Slot_create(adlb_datum_id id);
adlb_code ADLB_Slot_create(adlb_datum_id id);

adlb_code ADLBP_Slot_drop(adlb_datum_id id);
adlb_code ADLB_Slot_drop(adlb_datum_id id);

adlb_code ADLBP_Insert(adlb_datum_id id, const char *subscript,
                 const char* member, int member_length, int drops);
adlb_code ADLB_Insert(adlb_datum_id id, const char *subscript,
                const char* member, int member_length, int drops);

adlb_code ADLBP_Insert_atomic(adlb_datum_id id, const char *subscript,
                        bool* result);
adlb_code ADLB_Insert_atomic(adlb_datum_id id, const char *subscript,
                       bool* result);

adlb_code ADLBP_Lookup(adlb_datum_id id, const char *subscript, char* member, int* found);
adlb_code ADLB_Lookup(adlb_datum_id id, const char *subscript, char* member, int* found);

adlb_code ADLBP_Subscribe(adlb_datum_id id, int* subscribed);
adlb_code ADLB_Subscribe(adlb_datum_id id, int* subscribed);

adlb_code ADLBP_Container_reference(adlb_datum_id id, const char *subscript,
                              adlb_datum_id reference);
adlb_code ADLB_Container_reference(adlb_datum_id id, const char *subscript,
                             adlb_datum_id reference);

adlb_code ADLBP_Close(adlb_datum_id id, int** ranks, int* count);
adlb_code ADLB_Close(adlb_datum_id id, int** ranks, int* count);

adlb_code ADLBP_Unique(adlb_datum_id *result);
adlb_code ADLB_Unique(adlb_datum_id *result);

adlb_code ADLBP_Typeof(adlb_datum_id id, adlb_data_type* type);
adlb_code ADLB_Typeof(adlb_datum_id id, adlb_data_type* type);

adlb_code ADLBP_Container_typeof(adlb_datum_id id, adlb_data_type* type);
adlb_code ADLB_Container_typeof(adlb_datum_id id, adlb_data_type* type);

adlb_code ADLBP_Container_size(adlb_datum_id container_id, int* size);
adlb_code ADLB_Container_size(adlb_datum_id container_id, int* size);

adlb_code ADLBP_Lock(adlb_datum_id id, bool* result);
adlb_code ADLB_Lock(adlb_datum_id id, bool* result);

adlb_code ADLBP_Unlock(adlb_datum_id id);
adlb_code ADLB_Unlock(adlb_datum_id id);

void ADLB_Data_string_totype(const char* type_string,
                             adlb_data_type* type);

adlb_code ADLB_Data_type_tostring(char* output, adlb_data_type type);

adlb_code ADLB_Server_idle(int rank, bool* result);
adlb_code ADLB_Server_shutdown(int rank);

adlb_code ADLBP_Finalize(void);
adlb_code ADLB_Finalize(void);

adlb_code ADLBP_Abort(int);
adlb_code ADLB_Abort(int);

#endif
