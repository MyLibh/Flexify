#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <process.h>  

#include <type_traits>
 
template<typename T>
using cleanup_func_t = void WINAPI(T);

template<typename T, cleanup_func_t<T> CleanupFunc, T InvalidVal = T{ }>
class AutoCleanup
{
public:

	AutoCleanup() noexcept :
		m_val{ }
	{ }

	AutoCleanup(T&& val) noexcept : 
		m_val{ std::forward<T>(val) }
	{ }

	~AutoCleanup()
	{
		cleanup();
	}


	inline bool is_valid() const noexcept
	{
		return (m_val != InvalidVal);
	}

	T operator=(T val)
	{
		cleanup();

		m_val = val;

		return (*this);
	}

	operator T() const noexcept
	{
		return static_cast<T>(m_val);
	}

	void cleanup()
	{
		if (is_valid())
		{
			CleanupFunc(m_val);

			m_val = InvalidVal;
		}
	}

private:
	T m_val;
};

using AutoCloseHandle        = AutoCleanup<HANDLE,    reinterpret_cast<cleanup_func_t<HANDLE>*>(CloseHandle)>;
using AutoLocalFree			 = AutoCleanup<HLOCAL,    reinterpret_cast<cleanup_func_t<HLOCAL>*>(LocalFree)>;
using AutoGlobalFree	     = AutoCleanup<HGLOBAL,   reinterpret_cast<cleanup_func_t<HGLOBAL>*>(GlobalFree)>;
using AutoRegCloseKey		 = AutoCleanup<HKEY,	  reinterpret_cast<cleanup_func_t<HKEY>*>(RegCloseKey)>;
using AutoCloseServiceHandle = AutoCleanup<SC_HANDLE, reinterpret_cast<cleanup_func_t<SC_HANDLE>*>(CloseServiceHandle)>;
using AutoCloseWindowStation = AutoCleanup<HWINSTA,	  reinterpret_cast<cleanup_func_t<HWINSTA>*>(CloseWindowStation)>;
using AutoCloseDesktop		 = AutoCleanup<HDESK,     reinterpret_cast<cleanup_func_t<HDESK>*>(CloseDesktop)>;
using AutoUnmapViewOfFile	 = AutoCleanup<PVOID,     reinterpret_cast<cleanup_func_t<PVOID>*>(UnmapViewOfFile)>;
using AutoFreeLibrary		 = AutoCleanup<HMODULE,   reinterpret_cast<cleanup_func_t<HMODULE>*>(FreeLibrary)>;

class AutoReleaseRegionEx
{
public:
	AutoReleaseRegionEx(PVOID pv, HANDLE proc) noexcept: 
		m_pv{ pv },
		m_proc{ proc } 
	{ }

	~AutoReleaseRegionEx() 
	{
		cleanup(); 
	}

	PVOID operator=(PVOID pv)
	{
		cleanup();

		m_pv = pv;

		return(m_pv);
	}

	operator PVOID() 
	{ 
		return m_pv; 
	}

	void cleanup()
	{
		if (m_pv && m_proc)
		{
			VirtualFreeEx(m_proc, m_pv, 0LLU, MEM_RELEASE);

			m_pv = nullptr;
		}
	}

private:
	PVOID  m_pv;
	HANDLE m_proc;
};