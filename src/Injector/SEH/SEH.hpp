#pragma once

///====================================================================================================================================
/// File:				SEH.hpp
/// Author:				MyLibh
/// Created:			16.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	16.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __SEH_HPP_INCLUDED__
#define __SEH_HPP_INCLUDED__

#include "seh_exception.hpp"
#include "seh_guard.hpp"

///====================================================================================================================================
/// <summary>   Generic unhandled exception filter. </summary>
///
/// <remarks>	MyLibh, 16.07.2019. </remarks>
///
/// <param name="exception"> Exception data pointer. </param>
///
/// <returns>	EXCEPTION_EXECUTE_HANDLER </returns>
///====================================================================================================================================
long __stdcall generic_unhandled_exception(EXCEPTION_POINTERS* exception);

#endif /* __SEH_HPP_INCLUDED__ */