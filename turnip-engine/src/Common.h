#pragma once
#include <numeric>

#include "Core/Defines.h"
#include "Core/NonCopyable.h"

#include "Core/Memory/Memory.h"
#include "Core/Logger/Logger.h"
#include "Core/Assert/Assert.h"

#define BIND(function, argument) std::bind(function, argument, std::placeholders::_1)

#define EXPOSE_PROPERTY(ClassName, SetterName, PropertyName)						\
	const ClassName& SetterName() const { return PropertyName; }					\
	ClassName& SetterName() { return PropertyName; }
