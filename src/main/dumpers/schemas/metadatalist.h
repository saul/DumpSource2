/**
 * =============================================================================
 * DumpSource2
 * Copyright (C) 2024 ValveResourceFormat Contributors
 *
 * Source2Gen
 * Copyright (C) 2024 neverlosecc
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
#pragma once
#include <map>

 // List of printable metadata entries that are not tied to any structures.
 // These are used to determine if we can include the corresponding metadata entry value in the dump.
 // Original list sourced from Source2Gen project.
namespace Dumpers::Schemas
{

enum class MetadataValueType {
	STRING,
	INLINE_STRING,
	INTEGER,
	FLOAT,
	VARNAME,
	KV3DEFAULTS
};

inline std::map<std::string, MetadataValueType> g_mapMetadataNameToValue
{
	// STRING
	{ "MAlternateSemanticName", MetadataValueType::STRING },
	{ "MCellForDomain", MetadataValueType::STRING },
	{ "MCustomFGDMetadata", MetadataValueType::STRING },
	{ "MEntitySubclassScopeFile", MetadataValueType::STRING },
	{ "MFgdHelper", MetadataValueType::STRING },
	{ "MFieldVerificationName", MetadataValueType::STRING },
	{ "MKV3TransferName", MetadataValueType::STRING },
	{ "MNetworkAlias", MetadataValueType::STRING },
	{ "MNetworkChangeCallback", MetadataValueType::STRING },
	{ "MNetworkChangePointerCallback", MetadataValueType::STRING },
	{ "MNetworkEncoder", MetadataValueType::STRING },
	{ "MNetworkExcludeByName", MetadataValueType::STRING },
	{ "MNetworkExcludeByUserGroup", MetadataValueType::STRING },
	{ "MNetworkIncludeByName", MetadataValueType::STRING },
	{ "MNetworkIncludeByUserGroup", MetadataValueType::STRING },
	{ "MNetworkReplayCompatField", MetadataValueType::STRING },
	{ "MNetworkSerializer", MetadataValueType::STRING },
	{ "MNetworkTypeAlias", MetadataValueType::STRING },
	{ "MNetworkUserGroup", MetadataValueType::STRING },
	{ "MNetworkUserGroupProxy", MetadataValueType::STRING },
	{ "MParticleReplacementOp", MetadataValueType::STRING },
	{ "MPropertyArrayElementNameKey", MetadataValueType::STRING },
	{ "MPropertyAttributeChoiceName", MetadataValueType::STRING },
	{ "MPropertyAttributeEditor", MetadataValueType::STRING },
	{ "MPropertyAttributeRange", MetadataValueType::STRING },
	{ "MPropertyAttributeSuggestionName", MetadataValueType::STRING },
	{ "MPropertyCustomEditor", MetadataValueType::STRING },
	{ "MPropertyCustomFGDType", MetadataValueType::STRING },
	{ "MPropertyDescription", MetadataValueType::STRING },
	{ "MPropertyExtendedEditor", MetadataValueType::STRING },
	{ "MPropertyFriendlyName", MetadataValueType::STRING },
	{ "MPropertyGroupName", MetadataValueType::STRING },
	{ "MPropertyIconName", MetadataValueType::STRING },
	{ "MPropertyProvidesEditContextString", MetadataValueType::STRING },
	{ "MPropertyStartGroup", MetadataValueType::STRING },
	{ "MPropertySuppressBaseClassField", MetadataValueType::STRING },
	{ "MPropertySuppressExpr", MetadataValueType::STRING },
	{ "MPulseEditorCanvasItemSpecKV3", MetadataValueType::STRING },
	{ "MPulseEditorHeaderIcon", MetadataValueType::STRING },
	{ "MPulseEditorHeaderText", MetadataValueType::STRING },
	{ "MPulseRequirementSummaryExpr", MetadataValueType::STRING },
	{ "MPulseSelectorAllowRequirementCriteria", MetadataValueType::STRING },
	{ "MResourceBlockType", MetadataValueType::STRING },
	{ "MScriptDescription", MetadataValueType::STRING },
	{ "MSrc1ImportAttributeName", MetadataValueType::STRING },
	{ "MSrc1ImportDmElementType", MetadataValueType::STRING },
	{ "MVDataAssociatedFile", MetadataValueType::STRING },
	{ "MVDataFileExtension", MetadataValueType::STRING },
	{ "MVDataOutlinerIcon", MetadataValueType::STRING },
	{ "MVDataOutlinerIconExpr", MetadataValueType::STRING },
	{ "MVDataUniqueMonotonicInt", MetadataValueType::STRING },
	{ "MVectorIsSometimesCoordinate", MetadataValueType::STRING },

	// INLINE STRING
	{ "MDiskDataForResourceType", MetadataValueType::INLINE_STRING },
	{ "MResourceTypeForInfoType", MetadataValueType::INLINE_STRING },

	// INTEGER
	{ "MAlignment", MetadataValueType::INTEGER },
	{ "MDiskDataForResourceType", MetadataValueType::INTEGER },
	{ "MGenerateArrayKeynamesFirstIndex", MetadataValueType::INTEGER },
	{ "MNetworkBitCount", MetadataValueType::INTEGER },
	{ "MNetworkEncodeFlags", MetadataValueType::INTEGER },
	{ "MNetworkPriority", MetadataValueType::INTEGER },
	{ "MNetworkVarEmbeddedFieldOffsetDelta", MetadataValueType::INTEGER },
	{ "MParticleMaxVersion", MetadataValueType::INTEGER },
	{ "MParticleMinVersion", MetadataValueType::INTEGER },
	{ "MParticleOperatorType", MetadataValueType::INTEGER },
	{ "MPropertySortPriority", MetadataValueType::INTEGER },
	{ "MResourceTypeForInfoType", MetadataValueType::INTEGER },
	{ "MResourceVersion", MetadataValueType::INTEGER },
	{ "MVDataNodeType", MetadataValueType::INTEGER },
	{ "MVDataOverlayType", MetadataValueType::INTEGER },

	// FLOAT
	{ "MNetworkMaxValue", MetadataValueType::FLOAT },
	{ "MNetworkMinValue", MetadataValueType::FLOAT },

	// VARNAME
	{ "MNetworkOverride", MetadataValueType::VARNAME },
	{ "MNetworkVarNames", MetadataValueType::VARNAME },
	{ "MNetworkVarTypeOverride", MetadataValueType::VARNAME },
	{ "MParticleDomainTag", MetadataValueType::VARNAME },
	{ "MScriptDescription", MetadataValueType::VARNAME },

	// KV3
	{ "MGetKV3ClassDefaults", MetadataValueType::KV3DEFAULTS }
};

} // namespace Dumpers::Schemas