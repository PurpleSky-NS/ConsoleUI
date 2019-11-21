#pragma once

#include <vector>
#include <functional>
#include "../FrameBase/Displayer.h"
#include "../FrameBase/Surface.h"
#include "../FrameBase/SurfaceManager.h"

class MenuSurface : public Surface
{
	class MenuCom : public UIComponent
	{
		struct MenuItem
		{
			std::string itemName;
			std::function<void()> onSelected;
		};

	public:

		void AddItem(const std::string& itemName, std::function<void()> onSelected)
		{
			m_items.push_back(std::move(m_items.back()));
			m_items[m_items.size() - 2] = { itemName,onSelected };
		}

		bool WillRegisterEvnet()const
		{
			return true;
		}

		void OnEvent(char inputText)
		{
			switch (inputText)
			{
			case '\t':
			case 's':
				/*向下选择*/
				m_selectIndex = (m_selectIndex + 1) % m_items.size();
				break;
			case 'w':
				/*想上选择*/
				m_selectIndex = (m_selectIndex + unsigned(m_items.size()) - 1) % m_items.size();
				break;
			case '\r':
				m_items[m_selectIndex].onSelected();
				return;
			}
			GetContext()->Refresh();
		}

		void OnPrint()
		{
			for (size_t i = 0; i < m_items.size(); ++i)
			{
				if (i != m_selectIndex)
					cp.DisplaySpace(4);
				else
					cp.Display(" -> ");
				cp.DisplayLine(m_items[i].itemName).DisplayLine();
			}
		}

	private:
		unsigned m_selectIndex = 0;
		std::vector<MenuItem> m_items = { {"退出该界面",[]() {SurfaceManager::GetInstance().CloseTop(); }} };
	};

	class TextCom : public UIComponent
	{
	public:

		TextCom(const std::string& title) :
			m_title(title) {}


		virtual void OnPrint()
		{
			cp.DisplayTab(2).DisplayLine(m_title)
				.DisplayLine()
				.DisplayLine("操作说明：")
				.DisplaySpace(4).DisplayLine("√ w/s键或者Tab键可以切换选项，回车键确认").DisplayLine().DisplayLine();
		}

	private:
		std::string m_title;
	};

protected:

	/*添加子项*/
	void AddItem(const std::string& itemName, std::function<void()> onSelected)
	{
		m_menu->AddItem(itemName, onSelected);
	}

	MenuSurface(const std::string& title)
	{
		Add(new TextCom(title));
		Add(m_menu = new MenuCom);
	}

private:
	MenuCom* m_menu;
};
