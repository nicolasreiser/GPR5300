#include "MouseClass.h"

MouseClass::MouseClass()
{

}

void MouseClass::OnLeftPressed(int x, int y)
{
	_leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	_eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
	_leftIsDown = false;
	MouseEvent me(MouseEvent::EventType::LRelease, x, y);

}

void MouseClass::OnRightPressed(int x, int y)
{
	_RightIsDown = true;
	MouseEvent me(MouseEvent::EventType::RPress, x, y);
}

void MouseClass::OnRightReleased(int x, int y)
{
	_RightIsDown = false;
	MouseEvent me(MouseEvent::EventType::RRelease, x, y);
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	_mButtonIsDown = true;
	MouseEvent me(MouseEvent::EventType::MPress, x, y);

}

void MouseClass::OnMiddleReleased(int x, int y)
{
	_mButtonIsDown = false;
	MouseEvent me(MouseEvent::EventType::MRelease, x, y);

}

void MouseClass::OnWheelUp(int x, int y)
{
	_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));

}

void MouseClass::OnWheelDown(int x, int y)
{
	_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	_xValue = 1;
	_yValue = 1;
	_eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

bool MouseClass::IsLeftDown()
{
	return _leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return _mButtonIsDown;
}

bool MouseClass::IsRightDown()
{
	return _RightIsDown;
}

int MouseClass::GetPosX()
{
	return _xValue;
}

int MouseClass::GetPosY()
{
	return _yValue;
}

MousePoint MouseClass::GetPos()
{
	return {_xValue,_yValue};
}

bool MouseClass::EventBufferIsEmpty()
{
	return _eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (_eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = _eventBuffer.front(); // Get first Event
		_eventBuffer.pop(); // Remove first Event
		return e;
	}
}
