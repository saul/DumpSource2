/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2024 Source2ZE
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
#include "oslink.h"
#include "dbg.h"
#include "interface.h"
#include "strtools.h"
#include "plat.h"
#include <filesystem>
#include "common.h"
#include <string>
#include <vector>
#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <Psapi.h>
#endif

class CModule
{
public:
	CModule(const char *path, const char *module) :
		m_pszModule(module), m_pszPath(path)
	{
		spdlog::debug("Loading \"{}\" module...", m_pszModule);
		auto szModule = (std::filesystem::current_path() / path / (MODULE_PREFIX + std::string(m_pszModule) + MODULE_EXT)).lexically_normal().generic_string();

		m_hModule = dlmount(szModule.c_str());

		if (!m_hModule)
		{
#ifdef _WIN32
			ExitError("Failed to load %s", szModule.c_str());
#else
			ExitError("Failed to load %s\n%s", szModule.c_str(), dlerror());
#endif
			return;
		}

		spdlog::trace("Initialized module {}", m_pszModule);
	}

	CreateInterfaceFn GetFactory()
	{
		CreateInterfaceFn fn = (CreateInterfaceFn)dlsym(m_hModule, "CreateInterface");

		if (!fn)
			ExitError("Could not find CreateInterface in %s", m_pszModule);
		
		return fn;
	}

	template <typename T>
	T FindInterface(const char *name)
	{
		CreateInterfaceFn fn = (CreateInterfaceFn)dlsym(m_hModule, "CreateInterface");
		
		if (!fn)
			ExitError("Could not find CreateInterface in %s", m_pszModule);
			
		void *pInterface = fn(name, nullptr);

		if (!pInterface)
		{
			ExitError("Could not find %s in %s", name, m_pszModule);
		}

		return (T)pInterface;
	}

	template <typename T>
	T GetSymbol(const char* name)
	{
		return (T)dlsym(m_hModule, name);
	}

public:
	const char *m_pszModule;
	const char* m_pszPath;
	HINSTANCE m_hModule;
};