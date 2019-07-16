#include "seh_guard.hpp"
#include "seh_exception.hpp"

seh_guard::seh_guard() :
	m_func(_set_se_translator(seh_translator))
{ }

seh_guard::~seh_guard() 
{
	_set_se_translator(m_func);
}