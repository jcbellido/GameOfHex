#pragma once

#include "CommonMacros.h"
#include "Types.h"

namespace sqliteWrapped
{
	template <typename T>
	struct Reader
	{
		int GetInt(int const column = 0) const noexcept
		{
			return sqlite3_column_int(static_cast<T const *>(this)->GetAbi(), column);
		}

		char const * GetString(int const column = 0) const noexcept
		{
			return reinterpret_cast<char const *>(sqlite3_column_text(static_cast<T const *>(this)->GetAbi(), column));
		}

		wchar_t const * GetWidestring(int const column = 0) const noexcept
		{
			return static_cast<wchar_t const *>(sqlite3_column_text16(static_cast<T const *>(this)->GetAbi(), column));
		}

		int GetStringLength(int const column = 0) const noexcept
		{
			return sqlite3_column_bytes(static_cast<T const *>(this)->GetAbi(), column);
		}

		int GetWideStringLength(int const column = 0) const noexcept
		{
			return sqlite3_column_bytes16(static_cast<T const *>(this)->GetAbi(), column) / sizeof(wchar_t);
		}

		Type GetType(int column = 0) const noexcept
		{
			return static_cast<Type>(sqlite3_column_type(static_cast<T const *>(this)->GetAbi(), column));
		}
	};
}