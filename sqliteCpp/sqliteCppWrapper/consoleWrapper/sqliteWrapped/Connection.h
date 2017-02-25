#pragma once

#include "CommonMacros.h"
#include "Handle.h"
#include "sqlite3.h"

class Connection
{
	struct ConnectionHandleTraits : HandleTraits<sqlite3 *>
	{
		static void Close(Type value) noexcept
		{
			VERIFY_(SQLITE_OK, sqlite3_close(value));
		}
	};

	using ConnectionHandle = Handle<ConnectionHandleTraits>;

	ConnectionHandle m_handle;

	template <typename F, typename C>
	void InternalOpen(F open, C const * const filename)
	{
		Connection temp;
		if (SQLITE_OK != open(filename, temp.m_handle.Set()))
		{
			temp.ThrowLasterror();
		}
		swap(m_handle, temp.m_handle);
	}

public:
	Connection() noexcept = default;

	template <typename C>
	explicit Connection(C const * const filename)
	{
		Open(filename);
	}

	static Connection Memory()
	{
		return Connection(":memory:");
	}

	static Connection WideMemory()
	{
		return Connection(L":memory:");
	}

	explicit operator bool() const noexcept
	{
		return static_cast<bool>(m_handle);
	}

	sqlite3 * GetAbi() const noexcept
	{
		return m_handle.Get();
	}

	__declspec(noreturn) void ThrowLasterror() const
	{
		throw Exception(GetAbi());
	}

	void Open(char const * const filename)
	{
		InternalOpen(sqlite3_open, filename);
	}

	void Open(wchar_t const * const filename)
	{
		InternalOpen(sqlite3_open16, filename);
	}
};
