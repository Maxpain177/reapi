#pragma once

#define INDEXENT edictByIndex
#define ENTINDEX indexOfEdict
#define AMX_NULLENT -1

extern edict_t* g_pEdicts;

inline size_t indexOfEdict(edict_t* ed)
{
	return ed - g_pEdicts;
}

inline size_t indexOfEdict(entvars_t* pev)
{
	return indexOfEdict(pev->pContainingEntity);
}

// safe to nullptr
inline size_t indexOfEdictAmx(entvars_t* pev)
{
	size_t index = AMX_NULLENT;
	if (likely(pev != nullptr))
		index = indexOfEdict(pev);
	return index;
}

// fast
inline edict_t* edictByIndex(int index)
{
	return g_pEdicts + index;
}

// safe to index -1
inline edict_t* edictByIndexAmx(int index)
{
	auto ed = g_pEdicts + index;
	if (unlikely(index < 0)) // == AMX_NULLENT
		ed = nullptr;
	return ed;
}

template<typename T>
inline T* getPrivate(int index)
{
	T* pdata = nullptr;
	if (likely(index >= 0)) // != AMX_NULLENT
		pdata = (T *)g_pEdicts[index].pvPrivateData;
	return pdata;
}

inline entvars_t* PEV(int index)
{
	entvars_t* pvars = nullptr;
	if (likely(index >= 0)) // != AMX_NULLENT
		pvars = &g_pEdicts[index].v;
	return pvars;
}

template<typename T>
inline size_t indexOfPDataAmx(T* pdata)
{
	size_t index = AMX_NULLENT;
	if (likely(pdata != nullptr))
		index = indexOfEdict(pdata->pev);
	return index;
}

// HLTypeConversion.h -> AMXModX
template <typename T>
inline T &ref_member(void *ptr, int offset, int element = 0)
{
	return *reinterpret_cast<T *>((reinterpret_cast<int8 *>(ptr) + offset + (element * sizeof(T))));
}

template <typename T>
inline T &get_member(void *ptr, int offset, int element = 0)
{
	return ref_member<T>(ptr, offset, element);
}

template <typename T>
inline T &get_member(edict_t *pEntity, int offset, int element = 0)
{
	return get_member<T>(pEntity->pvPrivateData, offset, element);
}

template <typename T>
inline void set_member(void *ptr, int offset, T value, int element = 0)
{
	ref_member<T>(ptr, offset, element) = value;
}

template <typename T>
inline void set_member(edict_t *pEntity, int offset, T value, int element = 0)
{
	set_member<T>(pEntity->pvPrivateData, offset, value, element);
}

template <typename T>
inline T* get_member_direct(void *ptr, int offset, int element = 0, int size = sizeof(T))
{
	return reinterpret_cast<T *>(reinterpret_cast<int8 *>(ptr) + offset + (element * size));
}

template <typename T>
inline T get_member_direct(edict_t *pEntity, int offset, int element = 0, int size = 0)
{
	return get_member_direct<T>(pEntity->pvPrivateData, offset, element, size);
}

class CTempStrings
{
public:
	CTempStrings();
	char* push(struct tagAMX* amx);
	void pop(size_t count);

	enum
	{
		STRINGS_MAX = 16,
		STRING_SIZE = 1024,
		STRING_LEN = STRING_SIZE - 1
	};

private:
	size_t m_current;
	char m_strings[STRINGS_MAX][STRING_SIZE];
};
