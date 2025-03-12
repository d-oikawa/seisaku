#pragma once


#include <d3d12.h>
#include <dxgi.h>

#include "Lib/Graphics/MeshIB.h"


namespace Fwk{

class Shape
{
public:
	Shape();
	virtual ~Shape();
	virtual void Init(ID3D12Device* pDevice);
	virtual void Term();
	virtual void Draw(ID3D12GraphicsCommandList* pCmdList);
};

}//namespace Fwk