#pragma once

#include <string>
#include <vector>

class Surface;
class UIComponent
{
	friend class Surface;
public:

	virtual ~UIComponent() = default;

	virtual void OnPrint() {}

	virtual void OnEvent(char inputText) {}

	virtual bool WillRegisterEvnet()const { return false; }

	inline Surface* GetContext()const;

private:
	Surface* m_context;
};
Surface* UIComponent::GetContext()const
{
	return m_context;
}
