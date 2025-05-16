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
#include <array>
#include "utils/hash.h"

// List of printable metadata entries that are not tied to any structures.
// These are used to determine if we can include the corresponding metadata entry value in the dump.
// Original list sourced from Source2Gen project.

constinit std::array string_metadata_entries = {
	FNV32("MCellForDomain"),
	FNV32("MCustomFGDMetadata"),
	FNV32("MFieldVerificationName"),
	FNV32("MKV3TransferName"),
	FNV32("MNetworkAlias"),
	FNV32("MNetworkChangeCallback"),
	FNV32("MNetworkEncoder"),
	FNV32("MNetworkExcludeByName"),
	FNV32("MNetworkExcludeByUserGroup"),
	FNV32("MNetworkIncludeByName"),
	FNV32("MNetworkIncludeByUserGroup"),
	FNV32("MNetworkReplayCompatField"),
	FNV32("MNetworkSerializer"),
	FNV32("MNetworkTypeAlias"),
	FNV32("MNetworkUserGroup"),
	FNV32("MNetworkUserGroupProxy"),
	FNV32("MParticleReplacementOp"),
	FNV32("MPropertyArrayElementNameKey"),
	FNV32("MPropertyAttributeChoiceName"),
	FNV32("MPropertyAttributeEditor"),
	FNV32("MPropertyAttributeRange"),
	FNV32("MPropertyAttributeSuggestionName"),
	FNV32("MPropertyCustomEditor"),
	FNV32("MPropertyCustomFGDType"),
	FNV32("MPropertyDescription"),
	FNV32("MPropertyExtendedEditor"),
	FNV32("MPropertyFriendlyName"),
	FNV32("MPropertyFriendlyName"),
	FNV32("MPropertyGroupName"),
	FNV32("MPropertyIconName"),
	FNV32("MPropertyStartGroup"),
	FNV32("MPropertySuppressExpr"),
	FNV32("MPulseEditorHeaderIcon"),
	FNV32("MResourceBlockType"),
	FNV32("MScriptDescription"),
	FNV32("MSrc1ImportAttributeName"),
	FNV32("MSrc1ImportDmElementType"),
	FNV32("MVDataOutlinerIcon"),
	FNV32("MVDataOutlinerIconExpr"),
	FNV32("MVDataUniqueMonotonicInt"),
	FNV32("MVectorIsSometimesCoordinate"),
};

constinit std::array inline_string_metadata_entries = {
	FNV32("MResourceTypeForInfoType"),
	FNV32("MDiskDataForResourceType"),
};

constinit std::array integer_metadata_entries = {
	FNV32("MNetworkVarEmbeddedFieldOffsetDelta"),
	FNV32("MNetworkBitCount"),
	FNV32("MNetworkPriority"),
	FNV32("MParticleOperatorType"),
	FNV32("MPropertySortPriority"),
	FNV32("MParticleMinVersion"),
	FNV32("MParticleMaxVersion"),
	FNV32("MNetworkEncodeFlags"),
	FNV32("MResourceVersion"),
	FNV32("MVDataNodeType"),
	FNV32("MVDataOverlayType"),
	FNV32("MAlignment"),
	FNV32("MGenerateArrayKeynamesFirstIndex"),
};

constinit std::array float_metadata_entries = {
	FNV32("MNetworkMinValue"),
	FNV32("MNetworkMaxValue"),
};
