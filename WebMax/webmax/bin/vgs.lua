-- file: vgs.lua
-- brief:
--	1.本文档生成的标识符（变量名称、函数名称等）不可修改，否则将影响运行结果;
--	  甚至可能以使用应用程序崩溃.
--	2.提供的系统事件函数，请勿修改名称和返回值，否则将造成调用失败；
--	3.对于鼠标事件函数，如果修改其名称，请使用Webmax工具进行修改，否则将不被VGS引擎识别
-- date: 2008-12-10
-- author: webmax team

-- 测试是否可以执行Lua脚本
function TestLuaScripter()
	return "正确执行Lua"
end;

-- VGS初始化时调用的系统事件
function OnInitVgs()

end;

-- 退出VGS引擎时调用的系统事件
function OnExit()

end;

-- 窗体resize时调用的系统事件
function OnResize()

end;

-- 程序Active时调用的系统事件
function OnActive()

end;

-- 渲染一帧前的事件
function OnStartOneFrame()

end;

-- 结束一帧渲染时的事件
function OnEndOneFrame()

end;

-- 键盘事件采用全局:按键值+shift/ctrl/alt
-- Key	按键值
-- FuncKeys	功能键(shift,alt,ctrl)状态
function OnKeyPress(Key, FuncKeys)
	if Key == 0x77 or Key == 0x57 then -- w
		MovingModel(10, 10, 10)
	elseif  Key == 0x73 or Key == 0x53 then -- s
		MovingModel(-10, -10, -10)
	end

end;

-- Key	按键值
-- FuncKeys	功能键(shift,alt,ctrl)状态
function OnKeyDown(Key, FuncKeys)

end;

-- Key	按键值
-- FuncKeys	功能键(shift,alt,ctrl)状态
function OnKeyUp(Key, FuncKeys)
	--if 'c' == Key or 'C' == Key then
	if Key == 0x43 or Key == 0x63 then -- C
		SetNewModelposition()
	end
end;

-- 鼠标进入事件模板
-- xxxx	表示对象(model,camera,overlay,light)名称
-- FuncKeys 功能键(shift,alt,ctrl)状态
-- x,y 屏幕坐标
function OnMouseEnter_xxxx(FuncKeys, x, y)

end;

-- 鼠标离开事件
function OnMouseLeave_xxxx(FuncKeys, x, y)

end;

-- 鼠标按下事件
-- MouseID:0-左，1-右，2-中，其他值保留
-- FuncKeys 功能键(shift,alt,ctrl)状态
-- x,y 屏幕坐标
function OnMouseDown_xxxx(MouseID, FuncKeys, x, y)

end;

-- 鼠标弹起事件
function OnMouseUp_xxxx(MouseID, FuncKeys, x, y)

end;

-- 鼠标移动事件
function OnMouseMove_xxxx(MouseID, FuncKeys, x, y)

end;

-- 鼠标双击事件
function OnMouseDblClick_xxxx(MouseID, FuncKeys, x, y)

end;

-- 自动定义测试函数,停止所有动画
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
