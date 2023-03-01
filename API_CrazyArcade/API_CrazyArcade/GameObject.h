#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:
	void Set_Pos(float fX, float fY)
	{
		m_tInfo.fX = fX;
		m_tInfo.fY = fY;
	}
	POINT Get_Pos() { return { (LONG)m_tInfo.fX, (LONG)m_tInfo.fY }; }
	void Set_Dead() { m_bIsDead = true; }
	RECT Get_Rect() const { return m_tRect; }
	const INFO* Get_Info() const { return &m_tInfo; }
	void Set_Info(const INFO& rInfo) { m_tInfo = rInfo; }
	OBJECT::RENDER_LIST Get_RenderGroup() { return m_eRenderGroup; }
	OBJECT::ID Get_ObjectType() { return m_eObjectType; }
	void Set_ObjectType(OBJECT::ID eObjectType) { m_eObjectType = eObjectType; }
public:
	void Update_Rect_GameObject();
public:
	virtual int Ready_GameObject() = 0;
	virtual int Update_GameObject() = 0;
	virtual void Late_Update_GameObject() = 0;
	virtual void Render_GameObject(HDC hDC) = 0;
	virtual void Release_GameObject() = 0;
protected:
	INFO m_tInfo;
	RECT m_tRect;
	float m_fSpeed;
	bool m_bIsDead;
	OBJECT::RENDER_LIST m_eRenderGroup;
	OBJECT::ID m_eObjectType;
};

