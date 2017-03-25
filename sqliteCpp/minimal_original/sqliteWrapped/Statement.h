#pragma once

#include "CommonMacros.h"
#include "Handle.h"
#include "Reader.h"
#include "sqlite3.h"

namespace sqliteWrapped
{

	class Statement : public Reader<Statement>
	{
		struct StatementHandleTraits : HandleTraits<sqlite3_stmt *>
		{
			static void Close(Type value) noexcept
			{
				VERIFY_(SQLITE_OK, sqlite3_finalize(value));
			}
		};

		using StatementHandle = Handle<StatementHandleTraits>;
		StatementHandle m_handle;

		template <typename F, typename C, typename ...  Values>
		void InternalPrepare(Connection const & connection, F prepare, C const * const text, Values && ... values)
		{
			ASSERT(connection);
			if (SQLITE_OK != prepare(connection.GetAbi(), text, -1, m_handle.Set(), nullptr))
			{
				connection.ThrowLasterror();
			}

			BindAll(std::forward<Values>(values) ...);
		}

		/// Terminator for the variadic Binding
		void InternalBind(int) const noexcept
		{}

		template <typename First, typename ... Rest>
		void InternalBind(int const index, First && first, Rest && ... rest) const
		{
			Bind(index, std::forward<First>(first));
			InternalBind(index + 1, std::forward<Rest>(rest) ...);
		}

	public:

		Statement() noexcept = default;

		template <typename C, typename ... Values>
		Statement(Connection const & connection, C const * const text, Values && ... values)
		{
			Prepare(connection, text, std::forward<Values>(values) ...);
		}

		explicit operator bool() const noexcept
		{
			return static_cast<bool>(m_handle);
		}

		sqlite3_stmt * GetAbi() const noexcept
		{
			return m_handle.Get();
		}

		__declspec(noreturn) void ThrowLastError() const
		{
			throw Exception(sqlite3_db_handle(GetAbi()));
		}

		template <typename ... Values>
		void Prepare(Connection const & connection, char const * const text, Values && ... values)
		{
			InternalPrepare(connection, sqlite3_prepare_v2, text, std::forward<Values>(values) ...);
		}

		template <typename ... Values>
		void Prepare(Connection const & connection, wchar_t const * const text, Values && ... values)
		{
			InternalPrepare(connection, sqlite3_prepare16_v2, text, std::forward<Values>(values) ...);
		}

		bool Step() const
		{
			int const result = sqlite3_step(GetAbi());
			if (result == SQLITE_ROW) return true;
			if (result == SQLITE_DONE) return false;

			// There are avoidable errors: locked, synced, ... 

			ThrowLastError();
		}

		void Execute() const
		{
			VERIFY(!Step());
		}

		void Bind(int const index, int const value) const
		{
			if (SQLITE_OK != sqlite3_bind_int(GetAbi(), index, value))
			{
				ThrowLastError();
			}
		}

		void Bind(int const index, char const * const value, int const size = -1) const
		{
			if (SQLITE_OK != sqlite3_bind_text(GetAbi(), index, value, size, SQLITE_STATIC))
			{
				ThrowLastError();
			}
		}

		void Bind(int const index, wchar_t const * const value, int const size = -1) const
		{
			if (SQLITE_OK != sqlite3_bind_text16(GetAbi(), index, value, size, SQLITE_STATIC))
			{
				ThrowLastError();
			}
		}

		void Bind(int const index, std::string const & value) const
		{
			Bind(index, value.c_str(), value.size());
		}

		void Bind(int const index, std::wstring const & value) const
		{
			Bind(index, value.c_str(), value.size() * sizeof(wchar_t));
		}

		void Bind(int const index, std::string && value) const
		{
			if (SQLITE_OK != sqlite3_bind_text(GetAbi(), index, value.c_str(), value.size(), SQLITE_TRANSIENT))
			{
				ThrowLastError();
			}
		}

		void Bind(int const index, std::wstring && value) const
		{
			if (SQLITE_OK != sqlite3_bind_text16(GetAbi(), index, value.c_str(), value.size() * sizeof(wchar_t), SQLITE_TRANSIENT))
			{
				ThrowLastError();
			}
		}

		template <typename ... Values>
		void BindAll(Values && ... values) const
		{
			InternalBind(1, std::forward<Values>(values) ...);
		}

		template <typename ... Values>
		void Reset(Values && ... values) const
		{
			if (SQLITE_OK != sqlite3_reset(GetAbi()))
			{
				ThrowLastError();
			}

			BindAll(values ...);
		}
	};

	template <typename C, typename ... Values>
	void Execute(Connection const & connection, C const * const text, Values && ... values)
	{
		Statement(connection, text, std::forward<Values>(values) ...).Execute();
	}
}