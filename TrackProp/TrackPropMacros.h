/**
 * @file TrackPropMacros.h
 *
 * @brief Development helper
 * @author El Tom Jan Elzein
 * @date: 8.4.2022
 *
 * simplify TrackProp instantiation only if TRACK_PROP_ACTIVATED defined
 * should not be used it in production code (WIP)
 */
#ifdef __cplusplus

#ifdef TRACK_PROP_ACTIVATED
#define TRACK_PROP(typeId, varName) TrackProp<typeId> varName(#varName)
#define TRACK_PROP_NAME(typeId, varName, userLabel) TrackProp<typeId> varName(userLabel)
#define TRACK_PROP_CLASS(typeId, varName) TrackProp<typeId> varName
#define TRACK_PROP_EXTERN(typeId, varName) extern TrackProp<typeId> varName
#define TRACK_PROP_SET(typeId, varName, value) TrackProp<typeId> varName(#varName, value)
#define TRACK_PROP_ONLY(...) __VA_ARGS__
#define TRACK_PROP_RELEASE(...)
#define TRACK_PROP_SWITCH(debug, release) debug
#define TRACK_PROP_CONTENT(varName) varName._content
#define TRACK_PROP_INIT1(var, name, val) : var(name, val)
#define TRACK_PROP_INIT(var, name, val) , var(name, val)
#define TRACK_PROP_INIT_T1(ths, var, val) : var(ths, #var, val)
#define TRACK_PROP_INIT_T(ths, var, val) , var(ths, #var, val)
#else
// deactivated shold work same as original code
#define TRACK_PROP(typeId, varName) typeId varName
#define TRACK_PROP_NAME(typeId, varName, userLabel) typeId varName
#define TRACK_PROP_CLASS(typeId, varName) typeId varName
#define TRACK_PROP_EXTERN(typeId, varName) extern typeId varName
#define TRACK_PROP_SET(typeId, varName, userLabel) typeId varName = userLabel
#define TRACK_PROP_SWITCH(debug, release) release
#define TRACK_PROP_ONLY(...)
#define TRACK_PROP_RELEASE(...) __VA_ARGS__
#define TRACK_PROP_CONTENT(typeId) typeId
#define TRACK_PROP_INIT1(var, name, val)
#define TRACK_PROP_INIT(var, name, val)
#endif  // TRACK_PROP_ACTIVATED

#define TRACK_C(typeId, varName)                     \
	extern "C" typeId _get$##varName()               \
	{                                                \
		return varName;                              \
	};                                               \
	extern "C" void _set$##varName(typeId userLabel) \
	{                                                \
		##varName = userLabel;                       \
	};

#else  // __cplusplus

#define TRACK_C(typeId, varName) \
	typeId _get$##varName();     \
	void _set$##varName(typeId);
#define TRACK_GET(varName) _get$##varName()
#define TRACK_SET(varName, userLabel) _set$##varName(userLabel)

#endif  // __cplusplus
