#pragma once

#include "sqlite3.h"

enum class Type
{
	Integer = SQLITE_INTEGER,
	Float = SQLITE_FLOAT,
	Blob = SQLITE_BLOB,
	Null = SQLITE_NULL,
	Text = SQLITE_TEXT,
};

static char const * TypeName(Type const type)
{
	switch (type)
	{
	case Type::Integer: return "Integer";
	case Type::Float: return "Float";
	case Type::Blob: return "Blob";
	case Type::Null: return "Null";
	case Type::Text: return "Text";
	}
	ASSERT(false);
	return "Fucky Type";
}
