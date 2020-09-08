#pragma once

#include "ResourceManager.h"

template <typename T>
struct ShaderTypeTraits;

template <>
struct ShaderTypeTraits<SVS>
{
    typedef CResourceManager::map_VS MapType;
    using HWShaderType = ID3DVertexShader*;

    static inline const char* GetShaderExt() { return ".vs"; }
    static inline const char* GetCompilationTarget()
    {
        return "vs_2_0";
    }

    static void GetCompilationTarget(const char*& target, const char*& entry, const char* data)
    {
        if (HW.Caps.geometry_major >= 2)
            target = "vs_2_0";
        else
            target = "vs_1_1";

        if (strstr(data, "main_vs_1_1"))
        {
            target = "vs_1_1";
            entry = "main_vs_1_1";
        }

        if (strstr(data, "main_vs_2_0"))
        {
            target = "vs_2_0";
            entry = "main_vs_2_0";
        }
    }

    static inline HWShaderType CreateHWShader(DWORD const* buffer, size_t size)
    {
         HWShaderType sh = 0;
#ifdef USE_DX11
        R_CHK(HW.pDevice->CreateVertexShader(buffer, size, 0, &sh));
#else
        R_CHK(HW.pDevice->CreateVertexShader(buffer, size, &sh));
#endif
        return sh;
    }

    static inline u32 GetShaderDest() { return RC_dest_vertex; }
};

template <>
struct ShaderTypeTraits<SPS>
{
    typedef CResourceManager::map_PS MapType;
    using HWShaderType = ID3DPixelShader*;

    static inline const char* GetShaderExt() { return ".ps"; }
    static inline const char* GetCompilationTarget()
    {
        return "ps_2_0";
    }

    static void GetCompilationTarget(const char*& target, const char*& entry, const char* data)
    {
        if (strstr(data, "main_ps_1_1"))
        {
            target = "ps_1_1";
            entry = "main_ps_1_1";
        }
        if (strstr(data, "main_ps_1_2"))
        {
            target = "ps_1_2";
            entry = "main_ps_1_2";
        }
        if (strstr(data, "main_ps_1_3"))
        {
            target = "ps_1_3";
            entry = "main_ps_1_3";
        }
        if (strstr(data, "main_ps_1_4"))
        {
            target = "ps_1_4";
            entry = "main_ps_1_4";
        }
        if (strstr(data, "main_ps_2_0"))
        {
            target = "ps_2_0";
            entry = "main_ps_2_0";
        }
    }

    static inline HWShaderType CreateHWShader(DWORD const* buffer, size_t size)
    {
        HWShaderType sh = 0;
#ifdef USE_DX11
        R_CHK(HW.pDevice->CreatePixelShader(buffer, size, 0, &sh));
#else
        R_CHK(HW.pDevice->CreatePixelShader(buffer, size, &sh));
#endif
        return sh;
    }

    static inline u32 GetShaderDest() { return RC_dest_pixel; }
};

template <>
struct ShaderTypeTraits<SGS>
{
    typedef CResourceManager::map_GS MapType;
    using HWShaderType = ID3DGeometryShader*;

    static inline const char* GetShaderExt() { return ".gs"; }
    static inline const char* GetCompilationTarget()
    {
#ifdef USE_DX10
        if (HW.pDevice1 == nullptr)
            return D3D10GetGeometryShaderProfile(HW.pDevice);
        else
            return "gs_4_1";
#endif
#ifdef USE_DX11
        if (HW.FeatureLevel == D3D_FEATURE_LEVEL_10_0)
            return "gs_4_0";
        else if (HW.FeatureLevel == D3D_FEATURE_LEVEL_10_1)
            return "gs_4_1";
        else if (HW.FeatureLevel == D3D_FEATURE_LEVEL_11_0)
            return "gs_5_0";
#endif
        NODEFAULT;
        return "gs_4_0";
    }
	
	static void GetCompilationTarget(const char*& target, const char*& entry, const char* /*data*/)
    {
        target = GetCompilationTarget();
        entry = "main";
    }

    static inline HWShaderType CreateHWShader(DWORD const* buffer, size_t size)
    {
        HWShaderType sh = 0;
#ifdef USE_DX11
        R_CHK(HW.pDevice->CreateGeometryShader(buffer, size, 0, &sh));
#else
        R_CHK(HW.pDevice->CreateGeometryShader(buffer, size, &sh));
#endif
        return sh;
    }

    static inline u32 GetShaderDest() { return RC_dest_geometry; }
};

#ifdef USE_DX11
template <>
struct ShaderTypeTraits<SHS>
{
    typedef CResourceManager::map_HS MapType;
    using HWShaderType = ID3D11HullShader*;

    static inline const char* GetShaderExt() { return ".hs"; }
    static inline const char* GetCompilationTarget() { return "hs_5_0"; }
	
	static void GetCompilationTarget(const char*& target, const char*& entry, const char* /*data*/)
    {
        target = GetCompilationTarget();
        entry = "main";
    }
	
    static inline HWShaderType CreateHWShader(DWORD const* buffer, size_t size)
    {
        HWShaderType sh = 0;
        R_CHK(HW.pDevice->CreateHullShader(buffer, size, NULL, &sh));

        return sh;
    }

    static inline u32 GetShaderDest() { return RC_dest_hull; }
};

template <>
struct ShaderTypeTraits<SDS>
{
    typedef CResourceManager::map_DS MapType;
    using HWShaderType = ID3D11DomainShader*;

    static inline const char* GetShaderExt() { return ".ds"; }
    static inline const char* GetCompilationTarget() { return "ds_5_0"; }
	
	static void GetCompilationTarget(const char*& target, const char*& entry, const char* /*data*/)
    {
        target = GetCompilationTarget();
        entry = "main";
    }
	
    static inline HWShaderType CreateHWShader(DWORD const* buffer, size_t size)
    {
		HWShaderType sh = 0;
        R_CHK(HW.pDevice->CreateDomainShader(buffer, size, NULL, &sh));

        return sh;
    }

    static inline u32 GetShaderDest() { return RC_dest_domain; }
};

template <>
struct ShaderTypeTraits<SCS>
{
    typedef CResourceManager::map_CS MapType;
    using HWShaderType = ID3D11ComputeShader*;

    static inline const char* GetShaderExt() { return ".cs"; }
    static inline const char* GetCompilationTarget() { return "cs_5_0"; }
	
	static void GetCompilationTarget(const char*& target, const char*& entry, const char* /*data*/)
    {
        target = GetCompilationTarget();
        entry = "main";
    }
	
    static inline HWShaderType CreateHWShader(DWORD const* buffer, size_t size)
    {
		HWShaderType sh = 0;
        R_CHK(HW.pDevice->CreateComputeShader(buffer, size, NULL, &sh));

        return sh;
    }

    static inline u32 GetShaderDest() { return RC_dest_compute; }
};
#endif

template <>
inline CResourceManager::map_PS& CResourceManager::GetShaderMap()
{
    return m_ps;
}

template <>
inline CResourceManager::map_VS& CResourceManager::GetShaderMap()
{
    return m_vs;
}

template <>
inline CResourceManager::map_GS& CResourceManager::GetShaderMap()
{
    return m_gs;
}

#if defined(USE_DX11)
template <>
inline CResourceManager::map_DS& CResourceManager::GetShaderMap()
{
    return m_ds;
}

template <>
inline CResourceManager::map_HS& CResourceManager::GetShaderMap()
{
    return m_hs;
}

template <>
inline CResourceManager::map_CS& CResourceManager::GetShaderMap()
{
    return m_cs;
}
#endif

template <typename T>
inline T* CResourceManager::CreateShader(const char* name, const char* filename /*= nullptr*/, const bool searchForEntryAndTarget /*= false*/)
{
    ShaderTypeTraits<T>::MapType& sh_map = GetShaderMap<ShaderTypeTraits<T>::MapType>();
    LPSTR N = LPSTR(name);
    auto iterator = sh_map.find(N);

    if (iterator != sh_map.end())
        return iterator->second;
    else
    {
        T* sh = xr_new<T>();

        sh->dwFlags |= xr_resource_flagged::RF_REGISTERED;
        sh_map.insert(std::make_pair(sh->set_name(name), sh));
        if (0 == stricmp(name, "null"))
        {
            sh->sh = NULL;
            return sh;
        }

        // Remove ( and everything after it
        string_path shName;
        {
            if (filename == nullptr)
                filename = name;

            pcstr pchr = strchr(filename, '(');
            ptrdiff_t size = pchr ? pchr - filename : xr_strlen(filename);
            strncpy(shName, filename, size);
            shName[size] = 0;
        }

        // Open file
        string_path cname;
        strconcat(sizeof(cname), cname, ::Render->getShaderPath(), shName,
            ShaderTypeTraits<T>::GetShaderExt());
        FS.update_path(cname, "$game_shaders$", cname);

        // duplicate and zero-terminate
        IReader* file = FS.r_open(cname);
        bool fallback = strstr(Core.Params, "-lack_of_shaders");
        if (!file && fallback)
        {
			fallback:
				fallback = false;
            string1024 tmp;
            xr_sprintf(tmp, "CreateShader: %s is missing. Replacing it with stub_default%s", cname, ShaderTypeTraits<T>::GetShaderExt());
            Msg(tmp);
            strconcat(sizeof(cname), cname, ::Render->getShaderPath(), "stub_default", ShaderTypeTraits<T>::GetShaderExt());
            FS.update_path(cname, "$game_shaders$", cname);
            file = FS.r_open(cname);
        }
        R_ASSERT2(file, cname);
		
		const auto size = file->length();
        char* const data = (LPSTR)_alloca(size + 1);
        CopyMemory(data, file->pointer(), size);
        data[size] = 0;
        FS.r_close(file);
		
        // Select target
        LPCSTR c_target = ShaderTypeTraits<T>::GetCompilationTarget();
        LPCSTR c_entry = "main";
		
		if (searchForEntryAndTarget)
            ShaderTypeTraits<T>::GetCompilationTarget(c_target, c_entry, data);
		
        DWORD flags = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;

        // Compile
        HRESULT const _hr = ::Render->shader_compile(name, (DWORD const*)data, size,
            c_entry, c_target, flags, (void*&)sh);

        VERIFY(SUCCEEDED(_hr));
		
		if (FAILED(_hr) && fallback)
            goto fallback;
		
        CHECK_OR_EXIT(!FAILED(_hr), "An error occurred in the shaders, please write to the developers of the modification. The selected renderer: %i", renderer_value);

        return sh;
    }
}

template <typename T>
inline void CResourceManager::DestroyShader(const T* sh)
{
    if (0 == (sh->dwFlags & xr_resource_flagged::RF_REGISTERED))
        return;

    ShaderTypeTraits<T>::MapType& sh_map = GetShaderMap<ShaderTypeTraits<T>::MapType>();

    LPSTR N = LPSTR(*sh->cName);
    auto iterator = sh_map.find(N);

    if (iterator != sh_map.end())
    {
        sh_map.erase(iterator);
        return;
    }
    Msg("! ERROR: Failed to find compiled shader '%s'", sh->cName.c_str());
}
