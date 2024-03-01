#pragma once

#ifndef RENZHAICOREOBJECT_EXPORTS
#define RENZHAICOREOBJECT_EXPORTS
#endif

#ifdef RENZHAICOREOBJECT_EXPORTS
#define RENZHAICOREOBJECT_API __declspec(dllexport)
#else
#define RENZHAICOREOBJECT_API __declspec(dllimport)
#endif // EEE