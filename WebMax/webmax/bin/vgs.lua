-- file: vgs.lua
-- brief:
--	1.���ĵ����ɵı�ʶ�����������ơ��������Ƶȣ������޸ģ�����Ӱ�����н��;
--	  ����������ʹ��Ӧ�ó������.
--	2.�ṩ��ϵͳ�¼������������޸����ƺͷ���ֵ��������ɵ���ʧ�ܣ�
--	3.��������¼�����������޸������ƣ���ʹ��Webmax���߽����޸ģ����򽫲���VGS����ʶ��
-- date: 2008-12-10
-- author: webmax team

-- �����Ƿ����ִ��Lua�ű�
function TestLuaScripter()
	return "��ȷִ��Lua"
end;

-- VGS��ʼ��ʱ���õ�ϵͳ�¼�
function OnInitVgs()

end;

-- �˳�VGS����ʱ���õ�ϵͳ�¼�
function OnExit()

end;

-- ����resizeʱ���õ�ϵͳ�¼�
function OnResize()

end;

-- ����Activeʱ���õ�ϵͳ�¼�
function OnActive()

end;

-- ��Ⱦһ֡ǰ���¼�
function OnStartOneFrame()

end;

-- ����һ֡��Ⱦʱ���¼�
function OnEndOneFrame()

end;

-- �����¼�����ȫ��:����ֵ+shift/ctrl/alt
-- Key	����ֵ
-- FuncKeys	���ܼ�(shift,alt,ctrl)״̬
function OnKeyPress(Key, FuncKeys)
	if Key == 0x77 or Key == 0x57 then -- w
		MovingModel(10, 10, 10)
	elseif  Key == 0x73 or Key == 0x53 then -- s
		MovingModel(-10, -10, -10)
	end

end;

-- Key	����ֵ
-- FuncKeys	���ܼ�(shift,alt,ctrl)״̬
function OnKeyDown(Key, FuncKeys)

end;

-- Key	����ֵ
-- FuncKeys	���ܼ�(shift,alt,ctrl)״̬
function OnKeyUp(Key, FuncKeys)
	--if 'c' == Key or 'C' == Key then
	if Key == 0x43 or Key == 0x63 then -- C
		SetNewModelposition()
	end
end;

-- �������¼�ģ��
-- xxxx	��ʾ����(model,camera,overlay,light)����
-- FuncKeys ���ܼ�(shift,alt,ctrl)״̬
-- x,y ��Ļ����
function OnMouseEnter_xxxx(FuncKeys, x, y)

end;

-- ����뿪�¼�
function OnMouseLeave_xxxx(FuncKeys, x, y)

end;

-- ��갴���¼�
-- MouseID:0-��1-�ң�2-�У�����ֵ����
-- FuncKeys ���ܼ�(shift,alt,ctrl)״̬
-- x,y ��Ļ����
function OnMouseDown_xxxx(MouseID, FuncKeys, x, y)

end;

-- ��굯���¼�
function OnMouseUp_xxxx(MouseID, FuncKeys, x, y)

end;

-- ����ƶ��¼�
function OnMouseMove_xxxx(MouseID, FuncKeys, x, y)

end;

-- ���˫���¼�
function OnMouseDblClick_xxxx(MouseID, FuncKeys, x, y)

end;

-- �Զ�������Ժ���,ֹͣ���ж���
function StopAllSceneAnimations()

end;
function PlayAllSceneAnimations()

end;
function SwitchFillMode(camera, mode)
	-- local hMainScene = GetMainScene();
	SetSwapRenderWndFillMode(camera, mode);
end;

function SetNewModelposition()
	local hModel = GetModelHandleByIndex(0);
	local x,y,z = GetModelPosition(hModel);
	x = x +10;
	y = y + 10;
	z = z + 10;
	SetModelPosition(hModel, {x,y,z});
end;

function MovingModel(x, y, z)
	local hModel = GetModelHandleByIndex(0);
	MoveModel(hModel, {x, y, z}, 1);
end;
function OnMouseDown_KE(MouseID, FuncKeys, x, y)
	local hModel = GetModelHandleByName("KE");
	MoveModel(hModel, {10, 10, 10}, 1);
end;
