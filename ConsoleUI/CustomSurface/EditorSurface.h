#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include "../FrameBase/Displayer.h"
#include "../FrameBase/Surface.h"

class EditorSurface : public Surface
{
public:
	/*输入字符时回调
	 * 返回值：是否通过测试
	 * 参数：输入的字符
	 */
	typedef std::function<bool(char)> OnInputTest;
	/*数据输入完成/切换输入数据选项回调
	 * 返回值：是否通过测试
	 * 第一个参数：输入的文本
	 * 第二个参数：错误提示文本，设置该值来设置错误信息，若设为空字符串则于返回true效果相同
	 */
	typedef std::function<bool(std::string&, std::string&)> OnFinishTest;

	/*数据已经存在，无法添加*/
	class DataExistedException :public std::runtime_error
	{
	public:
		DataExistedException() = default;
		DataExistedException(const std::string& msg) :
			runtime_error(msg) {}
	};
	/*数据不存在，无法获取*/
	class DataNotFoundException :public std::runtime_error
	{
	public:
		DataNotFoundException() = default;
		DataNotFoundException(const std::string& msg) :
			runtime_error(msg) {}
	};
private:

	/*数据提交时回调
	 * 返回值：是否关闭编辑界面
	 * 参数：unordered_map 保存数据的名称与内容的表
	 */
	typedef std::function<bool(const std::unordered_map<std::string, std::string>&)> OnPost;

	class EditorCom : public UIComponent
	{

		struct EditableData
		{
			std::string name; //数据名/ID，唯一
			std::string text; //显示的文本
			std::string tips; //输入提示信息
			OnInputTest onTestInput; //测试字符输入
			std::string inputText; //输入的文本
			OnFinishTest onFinishInput; //测试输入文本
			bool isMask;//是否设置文本遮盖/密码输入
			std::string errorText; //不符合要求的提示信息
		};

		struct EditorMenuItem
		{
			std::string name;
			std::function<void()> onSelected;
		};

	public:

		EditorCom(OnPost onPostData) :
			m_onPost(onPostData),
			m_menuItems({
				{"提交",std::bind(&EditorCom::PostData,this) },
				{"重置",std::bind(&EditorCom::Reset,this)},
				{"退出",[]() {SurfaceManager::GetInstance().CloseTop(); } } }) {}

		void AddEditableData(const std::string& name, const std::string& text, const std::string& tips, OnInputTest onTestInput, OnFinishTest onFinishTest, bool isMask, const std::string defaultInput)throw(DataExistedException)
		{
			for (auto& i : m_datas)
				if (i.name == name)
					throw DataExistedException("数据[" + name + "]已经存在，无法重复添加");
			m_datas.push_back({ name,text,tips,onTestInput,defaultInput ,onFinishTest,isMask });
		}

		void Reset()
		{
			for (auto& i : m_datas)
			{
				i.inputText.clear();
				i.errorText.clear();
			}
			m_inputIndex = 0;
			GetContext()->Refresh();
		}

		std::vector<std::string> GetDatasName()const
		{
			std::vector<std::string> datasName;
			datasName.reserve(m_datas.size());
			for (auto& i : m_datas)
				datasName.push_back(i.name);
			return datasName;
		}

		std::string& GetInput(const std::string& name)throw(DataNotFoundException)
		{
			for (auto& i : m_datas)
				if (i.name == name)
					return i.inputText;
			throw DataNotFoundException("数据[" + name + "]不存在，无法获取");
		}

		void SetErrorText(const std::string& name, const std::string& errMsg)throw(DataNotFoundException)
		{
			for (auto& i : m_datas)
				if (i.name == name)
				{
					i.errorText = errMsg;
					return;
				}
			throw DataNotFoundException("数据[" + name + "]不存在，无法设置错误信息");
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
				/*切换编辑项目*/
				if (m_inputIndex < m_datas.size()) //如果是编辑项目
				{
					/*回调输入文本测试*/
					if (m_datas[m_inputIndex].onFinishInput(m_datas[m_inputIndex].inputText, m_datas[m_inputIndex].errorText))
						m_datas[m_inputIndex].errorText.clear();
				}
				if (++m_inputIndex < m_datas.size()) //如果下一项还是编辑项目
					m_datas[m_inputIndex].errorText.clear();//清除该项的错误信息
				else if (m_inputIndex == m_datas.size() + m_menuItems.size()) //到底了
				{
					m_inputIndex = 0;
					if (!m_datas.empty())
						m_datas[m_inputIndex].errorText.clear();//清除该项的错误信息
				}
				break;
			case '\b':
				/*退格键*/
				if (m_inputIndex >= m_datas.size())
				{
					GetContext()->Refresh();
					return;
				}
				if (m_datas[m_inputIndex].inputText.empty())
					return;
				if (m_datas[m_inputIndex].inputText.back() > 0) //ASCII字符
					m_datas[m_inputIndex].inputText.pop_back();
				else //中文字符
				{
					m_datas[m_inputIndex].inputText.pop_back();
					m_datas[m_inputIndex].inputText.pop_back();
				}
				break;
			case '\r':
				/*提交数据*/
				if (m_inputIndex < m_datas.size())
					PostData();
				else
					m_menuItems[m_inputIndex - m_datas.size()].onSelected();
				return;
			default:

				if (m_inputIndex < m_datas.size() && m_datas[m_inputIndex].onTestInput(inputText))
					m_datas[m_inputIndex].inputText += inputText;
				break;
			}
			GetContext()->Refresh();
		}

		void OnPrint()
		{
			for (size_t i = 0, size = m_datas.size() + m_menuItems.size(); i < size; ++i)
			{
				if (i < m_datas.size()) //编辑的数据
				{
					if (i == m_inputIndex) //正在编辑的文本
					{
						cp.DisplaySpace(4).Display("->").Display(m_datas[i].text).DisplayLine(":")
							.DisplayTab().Display((m_datas[i].isMask ? std::string(m_datas[i].inputText.size(), '*') : m_datas[i].inputText)).DisplayLine("_");
						if (!m_datas[i].tips.empty())
							cp.DisplayTab().Display("√ ").DisplayLine(m_datas[i].tips);
					}
					else
					{
						/*数据是否通过测试的标识符*/
						if (!m_datas[i].errorText.empty())
							cp.Display(" × ");
						else if (!m_datas[i].inputText.empty())
							cp.Display(" √ ");
						else
							cp.DisplaySpace(4);
						cp.Display("@ ").Display(m_datas[i].text).Display(":").DisplayLine((m_datas[i].isMask ? std::string(m_datas[i].inputText.size(), '*') : m_datas[i].inputText));
						if (!m_datas[i].errorText.empty())
							cp.DisplaySpace(4).Display("× ").DisplayLine(m_datas[i].errorText);
					}
				}
				else
				{
					size_t itemPos = i - m_datas.size();
					if (i != m_inputIndex) //不是正在选中的
						cp.DisplaySpace(4).Display("# ").Display(m_menuItems[itemPos].name).DisplayLine(" #");
					else
						cp.DisplaySpace(4).Display("->").Display(m_menuItems[itemPos].name).DisplayLine("<-");
				}
				cp.DisplayLine();
			}
		}

	private:
		unsigned m_inputIndex = 0;
		std::vector<EditableData> m_datas;
		std::vector<EditorMenuItem> m_menuItems;
		OnPost m_onPost;

		std::unordered_map<std::string, std::string> GetMappingData()const
		{
			std::unordered_map<std::string, std::string> map;
			for (auto& i : m_datas)
				map.insert({ i.name, i.inputText });
			return map;
		}

		void PostData()
		{
			/*测试当前选项*/
			if (m_inputIndex < m_datas.size() && m_datas[m_inputIndex].onFinishInput(m_datas[m_inputIndex].inputText, m_datas[m_inputIndex].errorText))
				m_datas[m_inputIndex].errorText.clear();
			/*检查是否有错误输入*/
			for (auto& i : m_datas)
				if (!i.errorText.empty())
				{
					cp.DisplayError("存在错误的输入项，请修改后提交~");
					GetContext()->Refresh();
					return;
				}
			if (m_onPost(GetMappingData()))
				SurfaceManager::GetInstance().CloseTop();
			else
				GetContext()->Refresh();
		}
	};

	class TextCom : public UIComponent
	{
	public:

		TextCom(const std::string& title, const std::string& describe) :
			m_title(title), m_describe(describe) {}


		virtual void OnPrint()
		{
			cp.DisplayTab(2).DisplayLine(m_title)
				.DisplayLine()
				.DisplayTab().DisplayLine(m_describe).DisplayLine()
				.DisplayLine("操作说明：")
				.DisplaySpace(4).DisplayLine("√ 使用Tab键切换输入框")
				.DisplaySpace(4).DisplayLine("√ 输入完成按下回车即可提交输入数据")
				.DisplaySpace(4).DisplayLine("√ 也可以选择最后几个选项进行相关操作").DisplayLine();
		}

	private:
		std::string m_title;
		std::string m_describe;
	};

protected:

	EditorSurface(const std::string& title, const std::string& describe)
	{
		m_editor = new EditorCom(std::bind(&EditorSurface::OnPostData, this, std::placeholders::_1));
		Add(new TextCom(title, describe));
		Add(m_editor);
	}

	/*如果tips为空则不显示提示，会抛出DataExistedException*/
	void AddEditableData(const std::string& name, const std::string& text, const std::string& tips, OnInputTest onTestInput, OnFinishTest onFinishTest, bool isMask = false, const std::string defaultInput = "")throw(DataExistedException)
	{
		m_editor->AddEditableData(name, text, tips, onTestInput, onFinishTest, isMask, defaultInput);
	}

	/*获取所有的数据名称*/
	std::vector<std::string> GetDatasName()const
	{
		return m_editor->GetDatasName();
	}

	/*根据数据的名称(name)获取输入的数据，会抛出DataNotFoundException*/
	std::string& GetInputData(const std::string& text)throw(DataNotFoundException)
	{
		return m_editor->GetInput(text);
	}

	/*设置某项数据的错误信息，使之错误，若errMsg为空字符串则清除错误*/
	void SetErrorText(const std::string& name, const std::string& errMsg)throw(DataNotFoundException)
	{
		m_editor->SetErrorText(name, errMsg);
	}

	/*用户提交数据回调，返回是否关闭该界面*/
	virtual bool OnPostData(const std::unordered_map<std::string, std::string>&) { return true; }

private:
	EditorCom* m_editor;
};
