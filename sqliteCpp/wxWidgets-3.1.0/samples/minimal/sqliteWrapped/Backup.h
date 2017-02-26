#pragma once

#include "sqlite3.h"
#include "CommonMacros.h"
#include "Connection.h"
#include "Handle.h"

namespace sqliteWrapped
{
	class Backup
	{
		struct BackupHandleTraits : HandleTraits<sqlite3_backup *>
		{
			static void Close(Type value) noexcept
			{
				sqlite3_backup_finish(value);
			}
		};

		using BackupHandle = Handle<BackupHandleTraits>;
		BackupHandle m_handle;
		Connection const * m_destination = nullptr;
	public:
		Backup(Connection const & destination, Connection const & source,
			char const * const destinationName = "main", char const * const sourceName = "main") :
			m_handle(sqlite3_backup_init(destination.GetAbi(), destinationName, source.GetAbi(), sourceName)),
			m_destination(&destination)
		{
			if (!m_handle)
			{
				destination.ThrowLasterror();
			}
		}

		sqlite3_backup * GetAbi() const noexcept
		{
			return m_handle.Get();
		}

		bool Step(int const pages = -1)
		{
			int const result = sqlite3_backup_step(GetAbi(), pages);

			if (result == SQLITE_OK) return true;
			if (result == SQLITE_DONE) return false;

			m_handle.Reset();
			m_destination->ThrowLasterror();
		}
	};
}