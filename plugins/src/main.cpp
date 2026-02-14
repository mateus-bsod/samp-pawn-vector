#include "main.h"
#include "vector.h"


void** ppPluginData;
extern void* pAMXFunctions;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	logprintf("[vector.inc]: plugin load.");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("[vector.inc]: plugin unload.");
}

AMX_NATIVE_INFO PluginNatives[] =
{
	// Vector Functions
	{ "vector_create", vector_create },
	{ "vector_add_int", vector_add_int },
	{ "vector_add_float", vector_add_float },
	{ "vector_add_string", vector_add_string },
	{ "vector_size", vector_size },
	{ "vector_get_int", vector_get_int },
	{ "vector_get_float", vector_get_float },
	{ "vector_get_string", vector_get_string },
	{ "vector_free", vector_free },
	{ "vector_get_index", vector_get_index },
	{ "vector_insert_str", vector_insert_str},
	{ "vector_set_index", vector_set_index},
	{ "vector_contains_index", vector_contains_index},
	{ "vector_get_index_type", vector_get_index_type},
	{ 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	return amx_Register(amx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	return AMX_ERR_NONE;
}