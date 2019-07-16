// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "seh_exception.hpp"

seh_exception::seh_exception(unsigned code, EXCEPTION_POINTERS* exception) :
	m_code(code), 
	m_exception(exception)
{ }

unsigned seh_exception::get_code() const
{
	return m_code;
}

EXCEPTION_POINTERS* seh_exception::get_exception_pointers() const
{
	return m_exception;
}

void seh_translator(unsigned code, EXCEPTION_POINTERS* exception)
{
	throw seh_exception(code, exception);
}
