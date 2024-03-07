#pragma once
#include "../../Core/Engine.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int, int);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelsDelegate, void, int, int, float);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnResizeDelegate, void, int, int, int);

extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;
extern FCaptureOnMousesDelegate LMouseUpDelegate;
extern FCaptureOnMousesDelegate LMouseDownDelegate;
extern FCaptureOnMousesDelegate MouseDownDelegate;
extern FCaptureOnMousesDelegate MouseUpDelegate;
extern FCaptureOnMousesDelegate MouseMoveDelegate;
extern FCaptureOnMousesDelegate ActiveDelegate;
extern FCaptureOnResizeDelegate OnResetSizeDelegate;