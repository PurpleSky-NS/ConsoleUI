#pragma once

#include <functional>
#include "../FrameBase/SurfaceManager.h"

/*子项类型*/
template<class ItemType>
/*表单界面*/
class TableSurface : public Surface
{
public:
	/*子项的容器接口*/
	class ItemContainer
	{
	public:

		virtual const ItemType& Get(size_t pos) = 0;

		virtual size_t Size()const = 0;

		virtual size_t Empty()const = 0;

	};
private:
	class TitleCom : public UIComponent
	{
	public:

		TitleCom(const std::string& title) :
			m_title(title) {}


		virtual void OnPrint()
		{
			cp.DisplayTab(2).DisplayLine(m_title)
				.DisplayLine()
				.DisplayLine("操作说明：")
				.DisplaySpace(4).DisplayLine("√ w/s键或者Tab键可以切换页面，回车键退出该界面").DisplayLine().DisplayLine();
		}

	private:
		std::string m_title;
	};

	class TableCom : public UIComponent
	{
	public:

		TableCom(std::function<bool()> onTitlePrint, std::function<bool(const ItemType&, bool)> onItemPrint) :
			m_onTitlePrint(onTitlePrint), m_onItemPrint(onItemPrint) {}

		void SetPageItemsCount(unsigned count)
		{
			if (count > 0)
				m_pageItemsCount = count;
		}

		unsigned GetPageItemsCount()const
		{
			return m_pageItemsCount;
		}

		void SetContainer(ItemContainer* container)
		{
			m_container = container;
		}

		ItemContainer* GetContainer()const
		{
			return m_container;
		}

		virtual bool WillRegisterEvnet()const
		{
			return true;
		}

		void OnEvent(char input)
		{
			switch (input)
			{
			case '\t':
			case 's':
				if (m_itemIndex + m_pageItemsCount > m_container->Size()) //如果是最后一页
					m_itemIndex = 0;
				else
					m_itemIndex += m_pageItemsCount;
				break;
			case 'w':
				if (m_itemIndex == 0) //如果是第一页
					m_itemIndex = (m_container->Size() / m_pageItemsCount - (m_container->Size() % m_pageItemsCount == 0)) * m_pageItemsCount;
				else
					m_itemIndex -= m_pageItemsCount;
				break;
			case '\r':
				SurfaceManager::GetInstance().CloseTop();
				return;
			}
			GetContext()->Refresh();
		}

		void OnPrint()
		{
			if (!(m_container != nullptr && !m_container->Empty()))
			{
				cp.DisplayError("暂无更多数据...");
				return;
			}
			m_onTitlePrint();
			for (size_t i = 0; i < m_pageItemsCount && i + m_itemIndex < m_container->Size(); ++i)
				m_onItemPrint(m_container->Get(i + m_itemIndex), i + 1 < m_pageItemsCount && i + 1 + m_itemIndex < m_container->Size());
			cp.Display(" # 第 ").Display(m_itemIndex / m_pageItemsCount + 1).Display(" / ").Display((m_container->Size() / m_pageItemsCount - (m_container->Size() % m_pageItemsCount == 0)) + 1).Display(" 页 #");
		}

	private:
		size_t m_itemIndex = 0;
		unsigned m_pageItemsCount = 5;
		ItemContainer* m_container = nullptr;
		std::function<bool()> m_onTitlePrint;
		std::function<bool(const ItemType&, bool)> m_onItemPrint;
	};

protected:

	TableSurface(const std::string& title)
	{
		Add(new TitleCom(title));
		Add(m_table = new TableCom(std::bind(&TableSurface::OnItemTitlePrint, this), std::bind(&TableSurface::OnItemPrint, this, std::placeholders::_1, std::placeholders::_2)));
	}

	/*显示子项的时候调用，返回是否显示，是否是最后一项*/
	virtual bool OnItemPrint(const ItemType& item, bool isLastItem) { return false; }

	/*显示子项标题的时候调用*/
	virtual bool OnItemTitlePrint() { return false; }

	void SetPageItemsCount(unsigned count)
	{
		m_table->SetPageItemsCount(count);
	}

	unsigned GetPageItemsCount(unsigned count)const
	{
		return m_table->GetPageItemsCount();
	}

	void SetContainer(ItemContainer* container)
	{
		m_table->SetContainer(container);
	}

	ItemContainer* GetContainer()const
	{
		return m_table->GetContainer();
	}

private:
	TableCom* m_table;
};
