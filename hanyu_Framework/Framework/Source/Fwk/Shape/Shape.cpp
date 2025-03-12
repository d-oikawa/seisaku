#include "Fwk/Shape/Shape.h"


using namespace Lib::Graphics;

namespace Fwk {

Shape::Shape(){

}

Shape::~Shape() {
	Term();
}

void Shape::Init(ID3D12Device* pDevice) {

}

void Shape::Term() {

}

void Shape::Draw(ID3D12GraphicsCommandList* pCmdList) {

}

}//namespace Fwk