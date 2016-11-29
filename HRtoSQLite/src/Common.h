/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _common_h_
#define _common_h_

#include <stdio.h>

#ifdef _WIN32
    #include <tchar.h>
#endif

#include <memory>
#include <functional>

template<typename F = std::function<void(bool bEnclosed)>> 
struct scope_t
{
	scope_t(F f) : f_(std::move(f)){
		}
	scope_t(scope_t&&) = delete;
	scope_t(const scope_t&) = delete;
	scope_t& operator=(scope_t&&) = delete;

	void enclose() {
		bEnclosed = true;
	}
	~scope_t() noexcept {
		f_(bEnclosed);
	}

	F f_;
	bool bEnclosed = false;
};








#endif //_common_h_




