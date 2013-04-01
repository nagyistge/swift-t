/*
 * Copyright 2013 University of Chicago and Argonne National Laboratory
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

/*
 * run.c
 *
 *  Created on: Feb 11, 2013
 *      Author: wozniak
 *
 *  Source for turbine_run()
 *  Not a core Turbine feature, thus, not in turbine.c
 */

#include <tcl.h>

// From c-utils
#include <tools.h>

#include "src/tcl/util.h"

// From turbine
#include "src/turbine/turbine.h"

turbine_code
turbine_run(MPI_Comm comm, char* script_file,
            int argc, char** argv, char* output)
{
  // Create Tcl interpreter:
  Tcl_Interp* interp = Tcl_CreateInterp();
  Tcl_Init(interp);
  return turbine_run_interp(comm, script_file, argc, argv, output,
                            interp);
}

turbine_code
turbine_run_interp(MPI_Comm comm, char* script_file,
                   int argc, char** argv, char* output,
                   Tcl_Interp* interp)
{
  // Store communicator pointer in Tcl variable for turbine::init
  MPI_Comm* comm_ptr = &comm;
  Tcl_Obj* TURBINE_ADLB_COMM =
      Tcl_NewStringObj("TURBINE_ADLB_COMM", -1);
  Tcl_Obj* adlb_comm_ptr = Tcl_NewLongObj((long) comm_ptr);
  Tcl_ObjSetVar2(interp, TURBINE_ADLB_COMM, NULL, adlb_comm_ptr, 0);

  // Render argc/argv for Tcl
  Tcl_Obj* argc_obj     = Tcl_NewStringObj("argc", -1);
  Tcl_Obj* argc_val_obj = Tcl_NewIntObj(argc);
  Tcl_ObjSetVar2(interp, argc_obj, NULL, argc_val_obj, 0);
  Tcl_Obj* argv_obj     = Tcl_NewStringObj("argv", -1);
  Tcl_Obj* argv_val_obj = tcl_list_new(argc, argv);
  Tcl_ObjSetVar2(interp, argv_obj, NULL, argv_val_obj, 0);

  // Read the user script
  char* script = slurp(script_file);

  // Run the user script
  int rc = Tcl_Eval(interp, script);

  // Check for errors
  if (rc != TCL_OK)
  {
    Tcl_Obj* error_dict = Tcl_GetReturnOptions(interp, rc);
    Tcl_Obj* error_info = Tcl_NewStringObj("-errorinfo", -1);
    Tcl_Obj* error_msg;
    Tcl_DictObjGet(interp, error_dict, error_info, &error_msg);
    char* msg_string = Tcl_GetString(error_msg);
    printf("Tcl error: %s\n", msg_string);
    return TURBINE_ERROR_UNKNOWN;
  }

  // Clean up
  Tcl_DeleteInterp(interp);
  free(script);

  return TURBINE_SUCCESS;
}
