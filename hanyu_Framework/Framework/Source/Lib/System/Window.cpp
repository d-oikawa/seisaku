﻿#include "Lib/System/Window.h"

namespace Lib {
namespace System {

Window::Window()
	: m_hInst()
	, m_hwnd(  )
	, m_Width( 0 )
	, m_Height( 0 )
	,m_ClassName(L"")
{

}

Window::~Window()
{

}

bool Window::Init(uint32_t _w, uint32_t _h, const  wchar_t* pClassName, const  wchar_t* pWndTitle, WNDPROC WndProc) {

	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
		return false;

	wcscpy_s(m_ClassName, 255, pClassName);
	m_Width = _w;
	m_Height = _h;

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_ClassName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
		return false;

	m_hInst = hInst;

	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);

	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, false);

	m_hwnd = CreateWindowEx(
		0,
		m_ClassName,
		pWndTitle,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		m_hInst,
		nullptr
	);

	if (m_hwnd == nullptr)
		return false;

	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;

}

void Window::Term()
{
	if (m_hInst != nullptr)
	{
		UnregisterClass(m_ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hwnd = nullptr;
}

HINSTANCE Window::GetInst()const {
	return m_hInst;
}
HWND Window::GetHWND()const {
	return m_hwnd;
}

Lib::Math::Vector2f Window::TransformToClient(const Lib::Math::Vector2f& screenPos)const
{
	POINT p = { (int)screenPos.x , (int)screenPos.y };
	ScreenToClient(m_hwnd, &p);

	RECT rect = {};
	GetClientRect(m_hwnd, &rect);

	long w = rect.right - rect.left;
	long h = rect.bottom - rect.top;

	p.x -= w / 2;
	p.y -= h / 2;

	Math::Vector2f ret((float)p.x, -(float)p.y);

	return ret;
}

}
}