/**
 * =============================================================================
 * DumpSource2
 * Copyright (C) 2024 ValveResourceFormat Contributors
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
#include <filesystem>
#include <fstream>
#include <map>
#include "metadatalist.h"
#include <optional>
#include <cstring>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <set>
#include "json_generator.h"

#define private public
#include "schemasystem/schemasystem.h"
#undef private

namespace Dumpers::Schemas {
    struct CSchemaVarName {
        const char* m_pszName;
        const char* m_pszType;
    };

    void WriteMetadataValue(const SchemaMetadataEntryData_t &entry, json::generator_t::self_ref builder) {
        if (g_mapMetadataNameToValue.find(entry.m_pszName) == g_mapMetadataNameToValue.end()) {
            return;
        }

        auto valueType = g_mapMetadataNameToValue.at(entry.m_pszName);
        switch (valueType) {
            case MetadataValueType::STRING: {
                auto value = *static_cast<const char **>(entry.m_pData);
                builder.json_string_value(value);
                return;
            }
            case MetadataValueType::INTEGER: {
                auto value = *static_cast<int *>(entry.m_pData);
                builder.json_literal_value(value);
                return;
            }
            case MetadataValueType::FLOAT: {
                auto value = *static_cast<float *>(entry.m_pData);
                builder.json_literal_value(value);
                return;
            }
            case MetadataValueType::INLINE_STRING: {
                // max 8 characters. Also check for null term.
                char *result = static_cast<char *>(entry.m_pData);
                for (uint8_t i = 0; i < 8; ++i) {
                    if (result[i] == '\0') {
                        builder.json_string_value(std::string(result, i));
                        return;
                    }
                }
                builder.json_string_value(std::string(result, 8));
                return;
            }
            case MetadataValueType::VARNAME: {
                auto value = static_cast<CSchemaVarName *>(entry.m_pData);

                const auto check_ptr = [](const char *ptr) -> bool {
                    // Authored: source2gen
                    // @note: hotfix for the deadlock 14/09/24 update,
                    // where they filled some ptrs with -1 instead of nullptr
                    return ptr != nullptr && ptr != reinterpret_cast<const char *>(-1);
                };

                auto hasType = check_ptr(value->m_pszType);
                auto hasName = check_ptr(value->m_pszName);

                builder.begin_json_object_value();

                if (hasType)
                    builder.json_property_name("type").json_string_value(value->m_pszType);
                if (hasName)
                    builder.json_property_name("name").json_string_value(value->m_pszName);

                builder.end_json_object();
                return;
            }
            case MetadataValueType::KV3DEFAULTS: {
                builder.json_string_value("<kv3>");
                return;
            }
        }
    }


    void WriteMetadataEntry(const SchemaMetadataEntryData_t &entry, json::generator_t::self_ref builder) {
        builder.begin_json_object_value();
        builder.json_property_name("name").json_string_value(entry.m_pszName);

        if (strcmp(entry.m_pszName, "MResourceTypeForInfoType") == 0) {
            builder.end_json_object();
            return;
        }

        if (entry.m_pData) {
            WriteMetadataValue(entry, builder);
        }
    }

    void WriteTypeJson(json::generator_t::self_ref builder, CSchemaType *current_type) {
        builder.begin_json_object_value().json_property_name("name").json_string_value(current_type->m_sTypeName.Get());

        builder.json_property_name("category").json_literal_value(static_cast<int>(current_type->m_eTypeCategory));

        if (current_type->m_eTypeCategory == SCHEMA_TYPE_ATOMIC) {
            builder.json_property_name("atomic").json_literal_value(static_cast<int>(current_type->m_eAtomicCategory));

            if (current_type->m_eAtomicCategory == SCHEMA_ATOMIC_T || current_type->m_eAtomicCategory ==
                SCHEMA_ATOMIC_COLLECTION_OF_T) {
                const auto atomic_t = static_cast<CSchemaType_Atomic_T *>(current_type);
                if (atomic_t->m_pAtomicInfo != nullptr) {
                    builder.json_property_name("outer").json_string_value(atomic_t->m_pAtomicInfo->m_pszName);
                }

                if (atomic_t->m_pTemplateType != nullptr) {
                    builder.json_property_name("inner");
                    WriteTypeJson(builder, atomic_t->m_pTemplateType);
                }
            }
        } else if (current_type->m_eTypeCategory == SCHEMA_TYPE_FIXED_ARRAY) {
            builder.json_property_name("arraySize").json_literal_value(
                static_cast<CSchemaType_FixedArray *>(current_type)->m_nElementCount);
            builder.json_property_name("inner");
            WriteTypeJson(builder, static_cast<CSchemaType_FixedArray *>(current_type)->m_pElementType);
        } else if (current_type->m_eTypeCategory == SCHEMA_TYPE_POINTER) {
            builder.json_property_name("inner");
            WriteTypeJson(builder, current_type->GetInnerType().Get());
        }

        builder.end_json_object();
    }

    void DumpClasses(CSchemaSystemTypeScope *typeScope, json::generator_t::self_ref builder) {
        const auto &classes = typeScope->m_ClassBindings;

        UtlTSHashHandle_t *handles = new UtlTSHashHandle_t[classes.Count()];
        classes.GetElements(0, classes.Count(), handles);

        builder.json_property_name("classes").begin_json_object_value();

        for (int j = 0; j < classes.Count(); ++j) {
            const auto classInfo = classes[handles[j]];

            spdlog::trace("Dumping class: '{}'", classInfo->m_pszName);

            builder.json_property_name(classInfo->m_pszName).begin_json_object_value();

            if (classInfo->m_nBaseClassCount >= 1) {
                builder.json_property_name("parent").json_string_value(classInfo->m_pBaseClasses[0].m_pClass->m_pszName);
            }

            builder.json_property_name("metadata").begin_json_array_value();

            bool isAtomic = false;
            std::set<std::string> networkVarNames;
            for (uint16_t k = 0; k < classInfo->m_nStaticMetadataCount; k++) {
                const auto &metadataEntry = classInfo->m_pStaticMetadata[k];

                // Keep track of all network vars
                if (strcmp(metadataEntry.m_pszName, "MNetworkVarNames") == 0) {
                    auto value = static_cast<CSchemaVarName *>(metadataEntry.m_pData);
                    networkVarNames.insert(value->m_pszName);

                    // don't write var names - too verbose
                    continue;
                }

                if (strcmp(metadataEntry.m_pszName, "MNetworkVarsAtomic") == 0) {
                    isAtomic = true;
                }

                WriteMetadataEntry(metadataEntry, builder);
            }
            builder.end_json_array();

            builder.json_property_name("fields").begin_json_array_value();

            for (uint16_t k = 0; k < classInfo->m_nFieldCount; k++) {
                const auto &field = classInfo->m_pFields[k];
                spdlog::trace("Dumping field: '{}' for class: '{}'", field.m_pszName, classInfo->m_pszName);

                if (!networkVarNames.contains(field.m_pszName) && !isAtomic) {
                    bool hasNetworkEnable = false;
                    for (auto j = 0; j < field.m_nStaticMetadataCount; j++) {
                        if (strcmp(field.m_pStaticMetadata[j].m_pszName, "MNetworkEnable") == 0) {
                            hasNetworkEnable = true;
                            break;
                        }
                    }

                    if (!hasNetworkEnable) {
                        continue;
                    }
                }

                builder.begin_json_object_value().json_property_name("name").json_string_value(field.m_pszName);

                builder.json_property_name("type");

                WriteTypeJson(builder, field.m_pType);

                builder.json_property_name("metadata").begin_json_array_value();

                // Output metadata entries as comments before the field definition
                for (uint16_t l = 0; l < field.m_nStaticMetadataCount; l++) {
                    const auto &metadataEntry = field.m_pStaticMetadata[l];
                    if (strcmp(metadataEntry.m_pszName, "MNetworkEnable")) {
                        WriteMetadataEntry(metadataEntry, builder);
                    }
                }

                builder.end_json_array();

                builder.end_json_object();
            }

            builder.end_json_array();

            builder.end_json_object();
        }

        builder.end_json_object();
    }

    void DumpEnums(CSchemaSystemTypeScope *typeScope, json::generator_t::self_ref builder) {
        builder.json_property_name("enums").begin_json_object_value();

        const auto &enums = typeScope->m_EnumBindings;

        UtlTSHashHandle_t *handles = new UtlTSHashHandle_t[enums.Count()];
        enums.GetElements(0, enums.Count(), handles);

        for (int j = 0; j < enums.Count(); ++j) {
            const auto enumInfo = enums[handles[j]];

            builder.json_property_name(enumInfo->m_pszName).begin_json_object_value();

            builder.json_property_name("align").json_literal_value(enumInfo->m_nAlignment);

            builder.json_property_name("items").begin_json_array_value();
            for (int enumIdx = 0; enumIdx < enumInfo->m_nEnumeratorCount; ++enumIdx) {
                const auto &field = enumInfo->m_pEnumerators[enumIdx];
                builder.begin_json_object_value()
                        .json_property_name("name")
                        .json_string_value(field.m_pszName)
                        .json_property_name("value")
                        .json_literal_value(field.m_nValue == std::numeric_limits<int64>::max() ? -1 : field.m_nValue)
                        .end_json_object();
            }
            builder.end_json_array();

            builder.end_json_object();
        }
    }


    void DumpTypeScope(CSchemaSystemTypeScope *typeScope) {
        const auto outPath = Globals::outputPath / fmt::format("{}.json", typeScope->m_szScopeName);
        spdlog::info("Writing {}...", typeScope->m_szScopeName, outPath.c_str());

        auto builder = json::get();

        builder.begin_json_object();

        DumpEnums(typeScope, builder);
        DumpClasses(typeScope, builder);

        builder.end_json_object();

        std::ofstream f(outPath, std::ios::out);
        f << builder.str();
        f.close();
    }

    void Dump() {
        if (!std::filesystem::exists(Globals::outputPath)) {
            spdlog::info("Creating output directory: {}", Globals::outputPath.c_str());
            std::filesystem::create_directories(Globals::outputPath);
        }

        spdlog::info("Dumping all schemas...");
        auto schemaSystem = Interfaces::schemaSystem;

        const auto &typeScopes = schemaSystem->m_TypeScopes;

        for (auto i = 0; i < typeScopes.m_Vector.Count(); ++i)
            DumpTypeScope(typeScopes[i]);

        DumpTypeScope(schemaSystem->GlobalTypeScope());
    }
} // namespace Dumpers::Schemas
