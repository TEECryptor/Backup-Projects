//----------------------------------------------------------------
// Copyright (C) 2006 Suntoward Inc.
// http://www.suntoward.com
// All rights reserved.
//
// Added by Edward D. Luo, 2006.11.03
//----------------------------------------------------------------
//
//  Logic for adding and manipulating VGS control
//
//----------------------------------------------------------------

// VGS插件的ID
var VgsId = "VGS2";
// VGS插件对象
var VGS2;

// 添加VGS对象
function AddVGS(sContainerId)
{
	var sHtml = '';
	var container = document.getElementById(sContainerId);
	if (container == null) return;
	
	removeChildNodes(container);
	
    if (window.ActiveXObject)
    {
		sHtml += '<object id="' + VgsId + '" classid="CLSID:046216A1-952E-486C-8598-4F2D0D0676F2" ';
		sHtml += 'width="100%" height="100%" style="width:100%;height:100%"></object>';

		container.innerHTML = sHtml;
		
		VGS2 = document.getElementById(VgsId);
    }
    else
    {
        sHtml += '<div style="text-align:center;color:#FFFF00">';
        sHtml += '<p>无法加载 <a style="color:#66FF00" href="http://www.suntoward.com/go/GetVgs/DownloadVgs.htm" target="_blank">VGS 三维网页引擎</a></p>';
        sHtml += '<p>您的浏览器不支持或已禁用 ActiveX 控件。<br />推荐使用 Microsoft Internet Explorer 浏览器，并启用 ActiveX 。</p>';
        sHtml += '</div>';
        
        container.style.backgroundColor = '34527A';
        container.innerHTML += sHtml;
    }
}

// 删除节点的所有子节点
function removeChildNodes(oNode)
{
	if (!oNode) return;
	
	while (oNode.hasChildNodes())
	{
		oNode.removeChild(oNode.firstChild);
	}
}

// 删除节点
function removeNode(sNodeId)
{
	var node = document.getElementById(sNodeId);
	if (!node || !node.parentNode) return;
	
	node.parentNode.removeChild(node);
}