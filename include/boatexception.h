/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
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
 * limitations under the License.
 *****************************************************************************/

/*!@brief Error catching macros

@file
boatexception.h defines error catching mechanism.
*/

#ifndef __BOATEXCEPTION_H__
#define __BOATEXCEPTION_H__

//!@{
//!@brief Declare variables used for try...throw...catch
#define boat_try_declare BOAT_RESULT _boat_result, boat_exception = BOAT_SUCCESS

/* boat_try is a limited implementation of try directive that only allows for
trying a single function call with return value of type BOAT_RESULT. If the
function returns BOAT_SUCCESS, it does nothing. Otherwise an exception is thrown.
The boat_catch directive must specify "cleanup" as its func_catch_point for use
with boat_try.

The boat_try directive can be used to simplify the codes that normally check
the function return value (type BOAT_RESULT) and omit following codes if any
of the tried function returns other than BOAT_SUCCESS.
*/
//!@brief A limited implementation of try directive
//!@param foo Any function calling, with return value of type BOAT_RESULT
#define boat_try(foo)\
{\
    _boat_result=(foo);\
    if( _boat_result != BOAT_SUCCESS ) boat_throw(_boat_result, cleanup)\
}


//!@brief Throw an exception
//!@param e Error code.
//!@param func_catch_point The catch block name to catch the exception.
#define boat_throw(e, func_catch_point) {boat_exception = e; goto func_catch_point;}

//!@brief Catch an exception
//!@param func_catch_point The catch block name matching the one passed to boat_throw().
#define boat_catch(func_catch_point) func_catch_point: _boat_result = 0; if( (boat_exception+_boat_result) != BOAT_SUCCESS )
//!@}





#endif
