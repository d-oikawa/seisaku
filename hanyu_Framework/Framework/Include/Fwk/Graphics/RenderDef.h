#pragma once

enum class RENDER_MODE : int
{
	None = -1,

	Mesh_Default,		//標準3dメッシュ
	Sprite_Opaque,		//不透明2dスプライト
	Sprite_Transparent,	//透過2dスプライト
	Sprite_Blend_Add,	//加算合成2dスプライト
	Sprite_Blend_Sub,	//減算合成2dスプライト
	Sprite_Blend_Inverse,	//反転合成2dスプライト

	//ブレンド設定とか入ってきたらここ増える

};

//描画パス
enum class RENDER_PASS : int 
{
	None = -1,

	Mesh_Opaque,		//不透明メッシュ
	Mesh_Transparent,	//透過アリメッシュ
	Sprite,				//スプライト
	SpriteFont,			//スプライトフォント
};

enum class BLEND_MODE : int {
	Opaque,
	Transparent,
	Add,
	Sub,
	Inverse,
};

//コンスタントバッファのフォーマット
namespace Fwk
{

struct alignas(256) CB_Transform
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};

}