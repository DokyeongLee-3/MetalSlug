#pragma once

#include "GameInfo.h"

// 현재 사용자가 누른 키 상태를 계속해서 업데이트하는 구조체
struct KeyState
{
	int Key;
	bool State[KeyState_Max];

	KeyState() :
		Key(0),
		State{}
	{

	}
};

// 미리 KeyInfo(처음 등록하고 변경x)에 사용할 키를 등록해놓고 KeyState로 업데이트된
// 키 상태랑, 등록된 KeyInfo랑 비교해서 동작을 결정
struct KeyInfo
{
	std::string Name;
	KeyState	State;

	std::function<void(float)>	Callback[KeyState_Max];
	bool Ctrl;
	bool Alt;
	bool Shift;

	KeyInfo() :
		Ctrl(false),
		Alt(false),
		Shift(false)
	{

	}
};

class CInput
{

public:
	CInput();
	~CInput();

private:
	std::unordered_map<std::string, KeyInfo*>	m_mapInfo;
	std::vector<KeyState>	m_vecKeyState;
	// UpdateKeyState에서 m_vecAddKey에 등록된 키만
	// update하려고 만들어둔 멤버 변수
	std::vector<int>	m_vecAddKey;
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;
	Vector2 m_MousePos;
	Vector2 m_MouseMove;
	HWND m_hWnd;
	bool m_MouseDown;
	bool m_MousePush;
	bool m_MouseUp;

public:
	bool GetMouseDown()
	{
		return m_MouseDown;
	}

	bool GetMousePush()
	{
		return m_MousePush;
	}

	bool GetMouseUp()
	{
		return m_MouseUp;
	}


	Vector2 GetMousePos()
	{
		return m_MousePos;
	}

	Vector2 GetMouseMove()
	{
		return m_MouseMove;
	}

public:
	bool CreateKey(const std::string& Name, int Key);
	bool SetCtrlKey(const std::string& Name, bool State);
	bool SetAltKey(const std::string& Name, bool State);
	bool SetShiftKey(const std::string& Name, bool State);

private:
	KeyInfo* FindKeyInfo(const std::string& Name);

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float DeltaTime);

public:
	void ClearCallback();

public:
	template <typename T>
	void SetCallback(const std::string& Name, EKeyState State, T* Obj, void(T::* Func)(float))
	{
		KeyInfo* Info = FindKeyInfo(Name);

		Info->Callback[State] = std::bind(Func, Obj, std::placeholders::_1);
	}

private:
	static CInput* m_Inst;

public:
	static CInput* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CInput;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

