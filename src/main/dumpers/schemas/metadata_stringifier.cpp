/**
 * =============================================================================
 * DumpSource2
 * Copyright (C) 2024 ValveResourceFormat Contributors
 * 
 * source2gen
 * Copyright 2024 neverlosecc
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "schemas.h"
#include "globalvariables.h"
#include "interfaces.h"
#include "schemasystem/schemasystem.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <unordered_set>
#include <algorithm>
#include "metadatalist.h"
#include <optional>
#include <cstring>
#include <fmt/format.h>
#include "metadata_stringifier.h"


namespace Dumpers::Schemas
{

// Determine how and if to output metadata entry value based on it's type.
std::optional<std::string> GetMetadataValue(const SchemaMetadataEntryData_t& entry)
{
	if (g_mapMetadataNameToValue.find(entry.m_pszName) != g_mapMetadataNameToValue.end())
	{
		auto valueType = g_mapMetadataNameToValue.at(entry.m_pszName);
		switch (valueType)
		{
			case MetadataValueType::STRING:
			{
				auto value = *static_cast<const char**>(entry.m_pData);
				Globals::stringsIgnoreStream << value << "\n";
				return fmt::format("\"{}\"", value);
			}
			case MetadataValueType::INTEGER:
				return std::to_string(*static_cast<int*>(entry.m_pData));
			case MetadataValueType::FLOAT:
				return std::to_string(*static_cast<float*>(entry.m_pData));
			case MetadataValueType::INLINE_STRING:
			{
				// max 8 characters. Also check for null term.
				char* result = static_cast<char*>(entry.m_pData);
				for (uint8_t i = 0; i < 8; ++i) {
					if (result[i] == '\0') {
						return fmt::format("\"{}\"", std::string(result, i));
					}
				}
				return fmt::format("\"{}\"", std::string(result, 8));
			}
			case MetadataValueType::VARNAME:
			{
				auto value = static_cast<CSchemaVarName*>(entry.m_pData);

				const auto check_ptr = [](const char* ptr) -> bool {
					// Authored: source2gen
					// @note: hotfix for the deadlock 14/09/24 update,
					// where they filled some ptrs with -1 instead of nullptr
					return ptr != nullptr && ptr != reinterpret_cast<const char*>(-1);
				};

				std::stringstream stringStream;
				auto hasType = check_ptr(value->m_pszType);
				auto hasName = check_ptr(value->m_pszName);

				stringStream << "\"";

				if (hasType)
					stringStream << value->m_pszType;

				if (hasName)
				{
					if (hasType)
						stringStream << " ";
					stringStream << value->m_pszName;
				}

				stringStream << "\"";

				return stringStream.str();
			}
		}
	}

	return {};
}

} // namespace Dumpers::Schemas