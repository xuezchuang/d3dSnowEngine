#pragma once
#include "simple_library/public/simple_library.h"
#include "imgui.h"

class FEditorLogSystem
{
	friend class FLogEditor;
public:
	FEditorLogSystem();

	static FEditorLogSystem* Get();
	static void Destory();
public:
	void Clear();

	void AddLog(const char* Fmt, ...);
	void AddError(const char* Fmt, ...);
	void AddWarning(const char* Fmt, ...);
	void AddSuccess(const char* Fmt, ...);

protected:
	virtual void Draw(float DeltaTime);

	//���ÿһ�е���ɫ״̬
	void AddLineColor(e_error InColor);

	//������ƫ��
	void ResetLineOffsets(e_error InColor,int InOldSize);

	//��ȡe_error��Ӧ����ɫ����
	ImVec4 GetColor(e_error InColorID);

	void HandleBackstageLog(e_error InColorID,int InOldSize);
protected:
	ImGuiTextBuffer TextBuff;//����buff ����洢���� ��/n������
	ImGuiTextFilter TextFikter;//��������� ���ڲ�ѯ
	ImVector<int>	LineOffsets;//ÿһ�������ƫ�ƣ��� /n������
	ImVector<e_error>	PreLineColor; //ÿһ�е���ɫ
	bool bAutoScroll;//�Զ����µ�������־

protected:
	static FEditorLogSystem* LogSystem;
};