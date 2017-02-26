#pragma once

#include "sqlite3.h"
#include "Reader.h"
#include "Statement.h"

namespace sqliteWrapped
{
	class Row : public Reader<Row>
	{
		sqlite3_stmt * m_statement = nullptr;

	public:

		sqlite3_stmt * GetAbi() const noexcept
		{
			return m_statement;
		}

		Row(sqlite3_stmt * const statement) noexcept:
		m_statement(statement)
		{}
	};

	class RowIterator
	{
		Statement const *m_statement = nullptr;

	public:

		RowIterator() noexcept = default;

		RowIterator(Statement const & statement) noexcept
		{
			if (statement.Step())
			{
				m_statement = &statement;
			}
		}

		RowIterator & operator++() noexcept
		{
			if (!m_statement->Step())
			{
				m_statement = nullptr;
			}

			return *this;
		}

		bool operator !=(RowIterator const & other) const noexcept
		{
			return m_statement != other.m_statement;
		}

		Row operator *() const noexcept
		{
			return Row(m_statement->GetAbi());
		}
	};

	inline RowIterator begin(Statement const & statement)
	{
		return RowIterator(statement);
	}

	inline RowIterator end(Statement const & statement)
	{
		return RowIterator();
	}

}